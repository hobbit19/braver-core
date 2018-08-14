/* This Source Code Form is subject to the terms of the Mozilla Public
 * License. v. 2.0. If a copy of the MPL was not distributed with this file.
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled, { css } from 'styled-components'
import { Props, Type } from './index'

const getColors = (type?: Type) => {
  let color = '#4b4c5c'
  let boldColor = '#4b4c5c'

  switch (type) {
    case 'ads':
      color = '#ceb4e1'
      boldColor = '#b490cf'
      break
    case 'contribute':
      color = '#ce9ccf'
      boldColor = '#c16fc2'
      break
    case 'donation':
      color = '#AC9CCF'
      boldColor = '#696fdc'
      break
  }

  return css`
    color: ${color};

    b {
      color: ${boldColor};
    }
  `
}

export const StyledContent = styled<Props, 'div'>('div')`
  font-family: Poppins, sans-serif;
  font-size: 16px;
  font-weight: 500;
  line-height: 1.5;
  ${(p: Props) => getColors(p.type)};

  b,
  a {
    font-weight: 500;
  }

  h3 {
    margin: 0;
    display: inline-block;
    padding: 0;
    font-family: Poppins, sans-serif;
    font-size: 28px;
    font-weight: 500;
    line-height: 0.5;
    color: #ceb4e1;
  }
` as any
