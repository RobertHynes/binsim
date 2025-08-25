/*
  Function to translate keywords.  Takes care of deprecated keywords,
  variations, internationalisation and personal preferences.

  Released as part of BinSim 1.0
  This release built on August 25 2025

  Author: Robert I. Hynes (rhynes@lsu.edu)
          Louisiana State University
          Department of Physics and Astronomy
          Baton Rouge
          Louisiana, USA

  Copyright (C) 2025 Robert I. Hynes

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. 
*/

#ifndef _KEYWORD_TRANSLATOR_H
#define _KEYWORD_TRANSLATOR_H

#include <string>

#include "binsim_stdinc.h"
#include "keyword.h"

using std::string;

/*****************************************************************************/

/*
  Function to translate (remap) keywords in a keylist from one string
  to another.  Separated from keyword.h and keyword.cxx to facilitate
  easy switching of translation files, e.g. to allow
  internationalisation. 
*/
void apply_keyword_translation(Key_list *list);

/*****************************************************************************/

#endif








