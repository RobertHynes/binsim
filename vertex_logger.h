/*
  Class to log vertex information to a log file
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

#ifndef _VERTEX_LOGGER_H
#define _VERTEX_LOGGER_H

#include <fstream>
#include <string>

#include "binsim_stdinc.h"

using std::ofstream;
using std::string;

/*****************************************************************************/

class Vertex_logger {
  // Pointer to output stream
  ofstream *output;

  // Flag controlling whether logging is enabled
  bool enabled;
public:
  // Constructor
  Vertex_logger();

  // Log a vertex
  void log_rgb(string objname, int i, int j, float x, float y, float z,
	       float r, float g, float b);
  void log_rgba(string objname, int i, int j, float x, float y, float z,
		float r, float g, float b, float a);
};

/*****************************************************************************/

#endif
