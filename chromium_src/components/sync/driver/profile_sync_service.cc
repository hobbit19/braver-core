#include "brave/components/brave_sync/brave_sync_prefs.h"
#include "components/prefs/pref_service.h"

namespace syncer {
const int64_t kBraveDefaultPollIntervalSeconds = 60;
}  // namespace syncer

#define BRAVE_SET_POLL_INTERVAL \
  sync_prefs_.SetPollInterval(  \
      base::TimeDelta::FromSeconds(syncer::kBraveDefaultPollIntervalSeconds));

#define BRAVE_PROFILE_SYNC_SERVICE                                         \
  brave_sync_prefs_change_registrar_.Init(sync_client_->GetPrefService()); \
  brave_sync_prefs_change_registrar_.Add(                                  \
      brave_sync::Prefs::GetSeedPath(),                                    \
      base::Bind(&ProfileSyncService::OnBraveSyncPrefsChanged,             \
                 base::Unretained(this)));                                 \
  if (!init_params.access_token_fetcher_for_test)                          \
    auth_manager_->CreateAccessTokenFetcher(url_loader_factory_,           \
                                            sync_service_url_);            \
  else                                                                     \
    auth_manager_->SetAccessTokenFetcherForTest(                           \
        std::move(init_params.access_token_fetcher_for_test));             \
  brave_sync::Prefs brave_sync_prefs(sync_client_->GetPrefService());      \
  auth_manager_->DeriveSigningKeys(brave_sync_prefs.GetSeed());            \
  if (!brave_sync_prefs.IsSyncV1Migrated()) {                              \
    StopImpl(CLEAR_DATA);                                                  \
    brave_sync_prefs.SetSyncV1Migrated(true);                              \
  }

#define BRAVE_D_PROFILE_SYNC_SERVICE \
  brave_sync_prefs_change_registrar_.RemoveAll();

#include "../../../../../components/sync/driver/profile_sync_service.cc"
#undef BRAVE_SET_POLL_INTERVAL
#undef BRAVE_PROFILE_SYNC_SERVICE
#undef BRAVE_D_PROFILE_SYNC_SERVICE
#undef BRAVE_ON_FIRST_SETUP_COMPLETE_PREF_CHANGE

namespace syncer {
void ProfileSyncService::OnBraveSyncPrefsChanged(const std::string& path) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (path == brave_sync::Prefs::GetSeedPath()) {
    brave_sync::Prefs brave_sync_prefs(sync_client_->GetPrefService());
    const std::string seed = brave_sync_prefs.GetSeed();
    if (!seed.empty()) {
      auth_manager_->DeriveSigningKeys(seed);
    } else {
      VLOG(1) << "Brave sync seed cleared";
      auth_manager_->ResetKeys();
    }
  }
}
}  // namespace syncer
