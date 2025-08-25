/*
  Header file defining version information

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

#ifndef _BINSIM_VERSION_H
#define _BINSIM_VERSION_H

#include <string>

/*****************************************************************************/

namespace Bin_sim_version {
  // Version number
  const string version = "1/0";
  
  // Program name
#ifdef WIN32
  const string program_name = "BinSim for Windows";
#else
  const string program_name = "BinSim";
#endif

  // Combine elements into full name
  const string full_name = program_name + " " + version;

  // Underlining string of same length as full_name
#ifdef WIN32
  const string underline = "----------------------";
#else
  const string underline = "----------";
#endif
}

/*****************************************************************************/

#endif
