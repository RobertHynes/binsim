/*
  Class to hold and render a background of stars
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

#include <cmath>
#include <cstdlib>

#include <iostream>

#include "constants.h"
#include "errmsg.h"
#include "starsky.h"

using std::cout;

/*****************************************************************************/

/*
  Create ensemble of stars
*/
Star_sky::Star_sky(const float x0, const float x1, 
		   const float y0, const float y1,
		   Key_list &params)
{
  cout << "Creating background object...\n";

  // Calculate width and height of field
  const float dx = x1 - x0;
  const float dy = y1 - y0;

  // Get scale silently - error messages will already have been logged
  // by Bin_sim
  float scale;
  try { scale = 1.0f / params.get_float("SCALE"); }
  catch (Key_list::Key_not_found_exception) {
    scale = 1.0f;
  }

  // Get number of stars - no default
  n_star = static_cast<int> (params.get_float("N_STAR") * scale*scale);
  if (n_star < 0)
    throw Key_list::Value_out_of_range_exception("N_STAR", ">= 0");

  // Get colour range
  try { colour_range = params.get_float("STAR_COLOURS"); }
  catch (Key_list::Key_not_found_exception) {
    colour_range = 0.0f;
    print_default_key_msg("STAR_COLOURS", "0.0");
  }
  if (colour_range < 0.0)
    throw Key_list::Value_out_of_range_exception("STAR_COLOURS", ">= 0.0");

  // Image quality - default is low quality.  Extract silently - 
  // error messages will already have been logged by Bin_sim
  try { star_quality = params.get_bool("HIGHQUALITY");}   
  catch (Key_list::Key_not_found_exception) {
    star_quality = false;
  }

  // Get star size, but only if high quality
  if (star_quality) {
    try { star_size = params.get_float("STAR_SIZE"); }
    catch (Key_list::Key_not_found_exception) {
      star_size = 0.5f;
      print_default_key_msg("STAR_SIZE", "0.5");
    }
    if (star_size <= 0.0) 
      throw Key_list::Value_out_of_range_exception("STAR_SIZE", ">= 0.0");
    star_size *= 0.0015f * dx;
  }

  // Initialise arrays
  red_grid = new GLfloat[n_star];
  green_grid = new GLfloat[n_star];
  blue_grid = new GLfloat[n_star];
  coord_grid = new GLfloat*[n_star];

  // Generate each star in turn
  for (int i = 0 ; i < n_star ; i++) {
    coord_grid[i] = new GLfloat[3];

    // Calculate random position within desired window
    float x = (((float) rand()) / RAND_MAX) * dx - 0.5f * dx;
    float y = (((float) rand()) / RAND_MAX) * dy - 0.5f * dy;
    
    *(coord_grid[i]) = x0 + x + 0.5f * dx;
    *(coord_grid[i]+1) = y0 + y + 0.5f * dy; 

    // Place stars in the background
    *(coord_grid[i]+2) = -8.0f;

    // Determine random colour of stars
    float max_colour = ((float) rand()) / RAND_MAX;
    float colour_bias = (((float) rand()) / RAND_MAX - 0.5f) * colour_range;

    // Star is blue
    if (colour_bias < 0.0f) {
      red_grid[i] = max_colour;
      green_grid[i] = max_colour + colour_bias * 0.5f;
      blue_grid[i] = max_colour + colour_bias * 1.0f;
    }
    // Star is red
    else {
      red_grid[i] = max_colour - colour_bias * 1.0f;
      green_grid[i] = max_colour - colour_bias * 0.5f;
      blue_grid[i] = max_colour;
    }
  }
}

/*
  Release dynamically allocated memory
*/
Star_sky::~Star_sky()
{
  // Deallocate grids of colours
  delete[] red_grid;
  delete[] green_grid;
  delete[] blue_grid;

  // Deallocate grids of coordinates
  for (int i = 0; i < n_star; i++) delete[] coord_grid[i];
  delete[] coord_grid;
}

/*
  Draw stars
*/
void Star_sky::draw()
{
  using Sci_const::PI;

  if (star_quality) {
    // Enable transparency
    glEnable (GL_BLEND);
    glDepthMask (GL_FALSE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0 ; i < n_star ; i++) {
      glBegin(GL_TRIANGLE_FAN);

      float rad = star_size * (red_grid[i] + green_grid[i] + blue_grid[i]);

      glColor4f(red_grid[i], green_grid[i], blue_grid[i], 1.0f);
      glVertex3fv(coord_grid[i]);
      
      for (int j = 0 ; j <= 20 ; j++) {
	float angle = j / 20.0f * 2.0f * PI;
	glColor4f(red_grid[i], green_grid[i], blue_grid[i], 0.0f);
	glVertex3f(*(coord_grid[i]) + 
		   static_cast<GLfloat> (rad * cos(angle)), 
		   *(coord_grid[i]+1) + 
		   static_cast<GLfloat> (rad * sin(angle)), 
		   *(coord_grid[i]+2));
      }

      glEnd();
    }

    // Disable transparency
    glDepthMask (GL_TRUE);
    glDisable (GL_BLEND);
  }
  else {
    glPointSize(1);
  
    glBegin(GL_POINTS);

    for (int i = 0 ; i < n_star ; i++) {
      glColor3f(red_grid[i], green_grid[i], blue_grid[i]);
      glVertex3fv(coord_grid[i]);
    }
    
    glEnd();
  }
}
