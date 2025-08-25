/*
  Class to hold and render a grid defining the surface of a triangular
  object defined by a rectangular mesh

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

#ifndef _TRANSPARENT_OBJECT3D_H
#define _TRANSPARENT_OBJECT3D_H

#include <vector>

#include "object3d.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Transparent_object_3d : public Object_3d {
protected:
  // Add transparency grid
  GLfloat *alpha_grid;

  // Issue OpenGL commands to draw a point
  virtual void draw_point(int coord_index, int color_index, int x, int y);
public:
  // Constructor and destructor
  Transparent_object_3d(const vector<float> phase1, const int n_x1, 
			const int n_y1, const float inclination);

  ~Transparent_object_3d();

  // Generate the OpenGL commands to draw the object
  virtual void draw(const int phase_index);
};

/*****************************************************************************/

#endif
