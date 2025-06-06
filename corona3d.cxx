/*
  Class to hold and render a grid defining a corona
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

#include "bbcolormodel.h"
#include "constants.h"
#include "disc.h"
#include "corona3d.h"
#include "mathvec.h"
#include "roche.h"
#include "stream.h"

/*****************************************************************************/

Corona_3d::Corona_3d(const int n_steps1,
		     const vector<float> phase, const float q, 
		     const float inclination, const float radius,     
		     const float red, const float green, const float blue,
		     const float opacity, const float gradient,
		     const bool companion) 
  : Transparent_object_3d(phase, 4*n_steps1, 2*n_steps1+1, inclination)
{
  using namespace Sci_const;

  // Override the object name
  object_name = "Corona_3d";

  // Define grid size
  n_steps = n_steps1;
  n_theta = 2 * n_steps + 1;
  n_phi = 4 * n_steps;

  // Create model to describe lobe properties
  Roche_lobe lobe(1.0/q);
  const float c_of_m = lobe.get_c_of_m();

  for (int i = 0 ; i < n_theta ; i++) {
    // Calculate angle of longitude
    float theta = PI * i / (n_theta-1.0f);

    for (int j = 0 ; j < n_phi ; j++) {
      // Calculate angle of latitude
      float phi = (2.0f * PI * j) / n_phi;

      // Radial vector passing through point
      Vec3 r_vect(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

      // Determine index offset for this point
      int index = i*n_phi + j;

      for (int k = 0 ; k < n_phase ; k++) {
	// Calculate transparency
	float alpha = pow(eye_vec[k] * r_vect, gradient) * opacity; 

	// Determine phase dependent index for storing colour
	int index1 = k*n_vert + index;

	// Assign colours
	red_grid[index1] = red;      
	green_grid[index1] = green;      
	blue_grid[index1] = blue;
	alpha_grid[index1] = alpha * 0.5f;
      }

      // Assign coordinates; corona may be around companion or accretor
      if (companion) 
	*(coord_grid[index])   = radius * r_vect.x - (1.0f - c_of_m);
      else
	*(coord_grid[index])   = radius * r_vect.x + c_of_m;
      *(coord_grid[index]+1) = radius * r_vect.y;
      *(coord_grid[index]+2) = radius * r_vect.z;
    }
  }
}

