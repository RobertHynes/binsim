/*
  Functions for outputting error messages.  This header defines the
  allowed interfaces; main application must provide implementation.

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

#ifndef _ERRMSG_H
#define _ERRMSG_H

#include <string>

#include "binsim_stdinc.h"

using std::string;

/*****************************************************************************/

/*
  Print an Keyword default message and continue
*/
void print_default_key_msg(const string key, const string def);

/*****************************************************************************/

#endif
