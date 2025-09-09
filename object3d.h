/*
  Class to hold and render a grid defining the surface of an object
  defined by a rectangular mesh

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

#ifndef _OBJECT3D_H
#define _OBJECT3D_H

#include <string>
#include <vector>
#include <iostream>

#include <cmath>

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "mathvec.h"

#include "binsim_stdinc.h"

using std::cout;
using std::string;
using std::vector;

/*****************************************************************************/

class Object_3d {
protected:
  // Define the object name
  string object_name;

  // Define the phase gridded eye vector
  int n_phase;

  vector<float> phase;

  Vec3* eye_vec;

  // Define the surface grid
  int n_x, n_y, n_vert;

  GLfloat *red_grid, *green_grid, *blue_grid;
  GLfloat **coord_grid;

  // Issue OpenGL commands to draw a point
  virtual void draw_point(int coord_index, int color_index, int x, int y);
public:
  // Constructor and destructor
  Object_3d(const vector<float> phase1, const int n_x1, const int n_y1, 
	    const float inclination);

  virtual ~Object_3d();

  // Generate the OpenGL commands to draw the object
  virtual void draw(int phase_index);
};

/*****************************************************************************/

#endif
