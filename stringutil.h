/*
  Utility functions operating on strings

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

#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <string>
#include <vector>

#include "binsim_stdinc.h"

using std::string;
using std::vector;

/*****************************************************************************/

namespace String_util {
  // Strip whitespace from a string
  string& strip_whitespace(string &s);
  string& strip_leading_whitespace(string &s);
  string& strip_trailing_whitespace(string &s);

  // Convert a string to all uppercase or all lower case
  string& string_toupper(string &s);
  string& string_tolower(string &s);

  // Replicate character ch n times
  string char_replicate(const int n, const char ch);

  // Format an integer into a string
  string int_to_string(const int i, const bool pad = false, 
		       const unsigned num_pad = 0);

  // Split a string into a vector of delimited substrings
  vector<string> split_string(const string s, const char delimit, 
			      const bool trim = false);
}

/*****************************************************************************/

#endif
