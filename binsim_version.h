/*
  Header file defining version information
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

#ifndef _BINSIM_VERSION_H
#define _BINSIM_VERSION_H

#include <string>

/*****************************************************************************/

namespace Bin_sim_version {
  // Version number
  const string version = "0.9";
  
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
