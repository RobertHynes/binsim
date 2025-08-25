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

#include <vector>

#include <cmath>
#include <cstdlib>

#include "constants.h"
#include "mathvec.h"
#include "object3d.h"
#include "transparent_object3d.h"
#include "vertex_logger.h"

extern Vertex_logger *vertex_logger;

/*****************************************************************************/

/*
  Perform generic initialisation of this base class
*/
Transparent_object_3d::Transparent_object_3d(const vector<float> phase1,
					     const int n_x1, const int n_y1, 
					     const float inclination)
  : Object_3d(phase1, n_x1, n_y1, inclination)
{
  // Override the object name
  object_name = "Transparent_object_3d";

  // Allocate grid of transparencies
  alpha_grid = new GLfloat[n_phase*n_vert];
}

/*
  Release dynamically allocated memory
*/
Transparent_object_3d::~Transparent_object_3d()
{
  // Deallocate grid of transparencies
  delete[] alpha_grid;
}

/*
  Generate OpenGL drawing commands
*/
void Transparent_object_3d::draw(const int phase_index)
{
  // Index variables
  int index_ij, index, index_i;

  // More indices
  const int base_index = phase_index * n_vert;

  // Enable transparency
  glEnable (GL_BLEND);
  glDepthMask (GL_FALSE);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Define object as column of triangle strips
  for (int i = 0 ; i < n_y-1 ; i++) {
    glBegin(GL_TRIANGLE_STRIP);

    index_i = i * n_x;

    for (int j = 0 ; j < n_x ; j++) {
      // Upper point on strip
      index_ij = index_i + j;
      index = base_index + index_ij;
      draw_point(index_ij, index, i, j*2);

      // Lower point on strip
      index_ij += n_x;
      index += n_x;
      draw_point(index_ij, index, i, j*2+1);
    }

    // Join end of strip to beginning - upper point
    index_ij = index_i;
    index = base_index + index_ij;
    draw_point(index_ij, index, i, n_x*2);
    
    // Lower point
    index_ij += n_x;
    index += n_x;
    draw_point(index_ij, index, i, n_x*2+1); 

    glEnd();
  }

  // Disable transparency
  glDepthMask (GL_TRUE);
  glDisable (GL_BLEND);
}

/*
  Issue OpenGL commands to draw a point.  x and y are for output in
  vertex logging mode only to identify the point.
*/
void Transparent_object_3d::draw_point(int coord_index, int color_index, 
				       int x, int y)
{
  if (vertex_logger)
    vertex_logger->log_rgba(object_name, x, y,
			    *(coord_grid[coord_index]), 
			    *(coord_grid[coord_index]+1),
			    *(coord_grid[coord_index]+2),
			    red_grid[color_index], 
			    green_grid[color_index], 
			    blue_grid[color_index],
			    alpha_grid[color_index]);
  
  glColor4f(red_grid[color_index], green_grid[color_index],
	    blue_grid[color_index], alpha_grid[color_index]);
  glVertex3fv(coord_grid[coord_index]);
}

