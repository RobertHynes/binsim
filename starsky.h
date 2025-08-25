/*
  Class to hold and render a background of stars

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

#ifndef _STARSKY_H
#define _STARSKY_H

#include <GL/glut.h>

#include "keyword.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Star_sky {
  // Define the number of stars and their size and colour
  int n_star;
  float star_size, colour_range;

  // Flag for high quality stars
  bool star_quality;

  // Define colour and position arrays
  GLfloat *red_grid, *green_grid, *blue_grid;
  GLfloat **coord_grid;
public:
  // Constructor and destructor
  Star_sky(const float x0, const float x1, const float y0, const float y1,
	   Key_list &params);
  ~Star_sky();

  // Generate the OpenGL commands to draw the stars
  void draw();
};

/*****************************************************************************/

#endif
