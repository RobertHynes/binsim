/*
  Utility functions operating on strings
  Released as part of BinSim 0.9
  This release built on 18 January 2005

  Author: Robert I. Hynes (rih@astro.as.utexas.edu)
          The University of Texas at Austin
          Department of Astronomy
          Austin
          Texas, USA

  Copyright (C) 2005 Robert I. Hynes

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <cctype>
#include <cstdio>

#include "stringutil.h"

/*****************************************************************************/

/*
  Strip all whitespace from string
*/
string& String_util::strip_whitespace(string &s)
{
  strip_leading_whitespace(s);
  strip_trailing_whitespace(s);

  return s;
}

/*
  Strip leading whitespace from string
*/
string& String_util::strip_leading_whitespace(string &s)
{
  while (s.length() > 0 && isspace(s[0])) s.erase(0,1);

  return s;
}

/*
  Strip trailing whitespace from string
*/
string& String_util::strip_trailing_whitespace(string &s)
{
  int l = s.length();
  while (l > 0 && isspace(s[l-1])) s.erase(--l,1);

  return s;
}

/*
  Convert string to upper case
*/
string& String_util::string_toupper(string &s)
{
  for (unsigned int i = 0 ; i < s.length() ; i++)
    s[i] = toupper(s[i]);

  return s;
}

/*
  Convert string to lower case
*/
string& String_util::string_tolower(string &s)
{
  for (unsigned int i = 0 ; i < s.length() ; i++)
    s[i] = tolower(s[i]);

  return s;
}

/*
  Produce a string containing n consecutive characters ch
*/
string String_util::char_replicate(const int n, const char ch)
{
  string str;
  for (int i = 0 ; i < n ; i++) str += ch;
  return str;
}

/*
  Take an int and format it into a string.  If pad is true then pad
  with leading zeroes if less than num_pad characters.
*/
string String_util::int_to_string(const int i, const bool pad, 
				  const unsigned num_pad)
{
  char buffer[20];
  sprintf(buffer, "%i", i);
  string str(buffer);
  if (pad && str.length() < num_pad) 
    str = char_replicate(num_pad - str.length(), '0') + str;
  return str;
}


/*
  Take a string and split it into substrings using the given delimiter
  character.  If trim is set to true, strip leading and trailing
  whitespace from the substrings
*/
vector<string> String_util::split_string(const string s, const char delimit, 
					 const bool trim)
{
  // Empty vector to contain results
  vector<string> components;

  // Beginning of string
  string::size_type comp_start = 0;

  while (s.find(delimit, comp_start) != string::npos) {
    // Leading components which are terminated by delimiter
    string::size_type delim_pos = s.find(delimit, comp_start);
    string component = s.substr(comp_start, delim_pos-comp_start);
    comp_start = delim_pos + 1;

    // Trim if desired and add to vector
    if (trim) strip_whitespace(component);
    components.push_back(component);
  }
  
  // Last component
  string component = s.substr(comp_start, s.length()-comp_start);

  // Trim if desired and add to vector
  if (trim) strip_whitespace(component);
  components.push_back(component);

  return components;
}

