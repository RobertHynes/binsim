/*
  Class to hold and render a grid defining a jet

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

#include <cmath>
#include <cstdlib>

#include "bbcolormodel.h"
#include "constants.h"
#include "disc.h"
#include "jet3d.h"
#include "mathvec.h"
#include "stream.h"

/*****************************************************************************/

Jet_3d::Jet_3d(const int n_phi1, const vector<float> phase, const float q, 
	       const float inclination, const float opening_angle, 
	       const float red1, const float green1, const float blue1,
	       const float red2, const float green2, const float blue2,
	       const float opacity, const float gradient,
	       const float jet_inc, const float jet_phi)
  : Transparent_object_3d(phase, n_phi1, 4, inclination)
{
  using namespace Sci_const;

  // Override the object name
  object_name = "Jet_3d";

  n_phi = n_phi1;

  // Calculate radius of ends of jet
  float radius = 10.0f * tan(opening_angle / 360.0f * 2.0f * PI);

  // Calculate non-rotating inertial frame
  Vec3 fixed_eye_vec;
  float inc_angle = -2.0f * PI * inclination / 360.f;
  float phase_angle = 2.0f * PI * (jet_phi+270.0f) / 360.0f;

  // This is not the correct eye vector; the jet inclination has not
  // been taken into account.  It does produce the desired effect, though.
  fixed_eye_vec.x = cos(phase_angle) * sin(inc_angle);
  fixed_eye_vec.y = -sin(phase_angle) * sin(inc_angle);
  fixed_eye_vec.z = cos(inc_angle);

  for (int i = 0 ; i < 4 ; i++) {
    for (int j = 0 ; j < n_phi ; j++) {
      // Calculate angle of latitude
      float phi = (2.0f * PI * j) / n_phi;

      // Radial vector passing through point
      Vec3 r_vect(cos(phi), sin(phi), 0.0f);

      // Determine index offset for this point
      int index = i*n_phi + j;

      for (int k = 0 ; k < n_phase ; k++) {
	// Calculate transparency
	float alpha = pow(fixed_eye_vec * r_vect, gradient) * opacity; 

	// Determine phase dependent index for storing colour
	int index1 = k*n_vert + index;

	// Assign colours
	if (i <= 1) {
	  red_grid[index1] = red1;      
	  green_grid[index1] = green1;      
	  blue_grid[index1] = blue1;
	} else {
	  red_grid[index1] = red2;      
	  green_grid[index1] = green2;      
	  blue_grid[index1] = blue2;
	}
	alpha_grid[index1] = alpha * 0.5f;
      }

      // Assign coordinates
      if (i == 0) {
	// End of upper jet
	*(coord_grid[index])   = radius * r_vect.x; // +1.0 
	*(coord_grid[index]+1) = radius * r_vect.y; // +0.5
	*(coord_grid[index]+2) = 10.0f;
      } else if (i == 3) {
	// End of lower jet
	*(coord_grid[index])   = radius * r_vect.x; // -1.0f
	*(coord_grid[index]+1) = radius * r_vect.y; // -0.5f
	*(coord_grid[index]+2) = -10.0f;
      } else {
	// Base of jet
	*(coord_grid[index])   = 0.0f;
	*(coord_grid[index]+1) = 0.0f;
	*(coord_grid[index]+2) = 0.0f;
      }
    }
  }
}

