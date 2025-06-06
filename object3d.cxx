/*
  Class to hold and render a grid defining the surface of an object
  defined by a rectangular mesh
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

#include <iostream>

#include <cmath>
#include <cstdlib>

#include "constants.h"
#include "object3d.h"
#include "mathvec.h"
#include "vertex_logger.h"

extern Vertex_logger *vertex_logger;

/*****************************************************************************/

/*
  Perform generic initialisation of this base class
*/
Object_3d::Object_3d(const vector<float> phase1, const int n_x1, 
		     const int n_y1, const float inclination)
{
  using Sci_const::PI;

  // Set the object name
  object_name = "Object_3d";

  // Allocate and define array of eye vectors
  phase = phase1;
  n_phase = phase.size();

  eye_vec = new Vec3[n_phase];

  float inc_angle = -2.0f * PI * inclination / 360.f;
  for (int k = 0 ; k < n_phase ; k++) {
    float phase_angle = 2.0f * PI * phase[k];
    eye_vec[k].x = cos(phase_angle) * sin(inc_angle);
    eye_vec[k].y = -sin(phase_angle) * sin(inc_angle);
    eye_vec[k].z = cos(inc_angle);
  }

  // Define grid size
  n_x = n_x1;
  n_y = n_y1;
  n_vert = n_x * n_y;

  // Allocate grids of colours
  red_grid = new GLfloat[n_phase*n_vert];
  green_grid = new GLfloat[n_phase*n_vert];
  blue_grid = new GLfloat[n_phase*n_vert];

  // Allocate grid of coordinates
  coord_grid = new GLfloat*[n_vert];
  for (int i = 0 ; i < n_vert ; i++) coord_grid[i] = new GLfloat[3]; 
}

/*
  Release dynamically allocated memory
*/
Object_3d::~Object_3d()
{
  // Deallocate array of eye vectors
  delete[] eye_vec;

  // Deallocate grids of colours
  delete[] red_grid;
  delete[] green_grid;
  delete[] blue_grid;

  // Deallocate grids of coordinates
  for (int i = 0; i < n_vert; i++) delete[] coord_grid[i];
  delete[] coord_grid;
}

/*
  Generate OpenGL drawing commands
*/
void Object_3d::draw(const int phase_index)
{
  // Index variables
  int index_i, index_ij, index;
  
  // More indices
  const int base_index = phase_index * n_vert;

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
}

/*
  Issue OpenGL commands to draw a point.  x and y are for output in
  vertex logging mode only to identify the point.
*/
void Object_3d::draw_point(int coord_index, int color_index, int x, int y)
{
  if (vertex_logger)
    vertex_logger->log_rgb(object_name, x, y,
			   *(coord_grid[coord_index]), 
			   *(coord_grid[coord_index]+1),
			   *(coord_grid[coord_index]+2),
			   red_grid[color_index], 
			   green_grid[color_index], 
			   blue_grid[color_index]);
  
  glColor3f(red_grid[color_index], green_grid[color_index],
	    blue_grid[color_index]);
  glVertex3fv(coord_grid[coord_index]);
}
