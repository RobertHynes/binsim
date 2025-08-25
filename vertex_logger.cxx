/*
  Class to log vertex information to a log file

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

#include <iostream>

#include "vertex_logger.h"

using std::cout;

/*****************************************************************************/

/* 
   Constructor
*/
Vertex_logger::Vertex_logger()
{
  // Create input stream opened from filename
  output = new ofstream("vertices.log");
  if (!(*output)) {
    cout << "Unable to open vertex log file.  Vertex logging disabled\n";
    enabled = false;
  }
  else enabled = true;
}
  
/*
  Log a vertex from an opaque object
*/
void Vertex_logger::log_rgb(string objname, int i, int j, 
			    float x, float y, float z,
			    float r, float g, float b)
{
  if (enabled) (*output) 
    << objname << ", vertex[" << i << "," << j << "], "
    << "position: {" << x << ", " << y << ", " << z << "}, "
    << "colour: {" << r << ", " << g << ", " << b << "}\n";
}

/*
  Log a vertex from an opaque object
*/
void Vertex_logger::log_rgba(string objname, int i, int j, 
			     float x, float y, float z,
			     float r, float g, float b, float a)
{
  if (enabled) (*output) 
    << objname << ", vertex[" << i << "," << j << "], "
    << "position: {" << x << ", " << y << ", " << z << "}, "
    << "colour: {" << r << ", " << g << ", " << b << ", " << a << "}\n";
}

