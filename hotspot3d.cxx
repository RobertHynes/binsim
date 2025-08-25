/*
  Class to hold and render a grid defining a hot spot

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
#include "hotspot3d.h"
#include "mathvec.h"
#include "stream.h"

/*****************************************************************************/

Hot_spot_3d::Hot_spot_3d(const int n_steps1,
			 vector<float> phase, const float q, 
			 const float inclination, const float mass, 
			 const float period,  const float r, 
			 const float size, const float red, 
			 const float green, const float blue, 
			 const float opacity, const float timescale) 
  : Transparent_object_3d(phase, 4*n_steps1, 2*n_steps1+1, inclination)
{
  using namespace Sci_const;

  // Override the object name
  object_name = "Hot_spot_3d";

  // Index variable
  int j;

  // Define grid size
  n_steps = n_steps1;
  n_theta = 2 * n_steps + 1;
  n_phi = 4 * n_steps;

  // Create stream model and calculate stream-disc impact point
  Stream stream_model(q, mass, period);
  vector<Vec3> stream = stream_model.stream_calc(0.005, r);
  Vec3 centre = stream[stream.size()-1];

  for (int i = 0 ; i < n_theta ; i++) {
    // Calculate angle of longitude
    float theta = PI * i / (n_theta-1.0f);

    // Calculate variability model with constant size variations
    int n_granules = static_cast<int> (n_phi * sin(theta) + 0.5f);
    float *gran_phase = new float[n_granules];
    for (j = 0 ; j < n_granules ; j++)
      gran_phase[j] = ((float) rand()) / RAND_MAX * 2.0f * PI;
    float polar_phase = ((float) rand()) / RAND_MAX * 2.0f * PI;
    
    for (j = 0 ; j < n_phi ; j++) {
      // Calculate angle of latitude
      float phi = (2.0f * PI * j) / n_phi;

      // Radial vector passing through point
      Vec3 r_vect(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

      // Determine index offset for this point
      int index = i * n_phi + j;

      // Define how to interpolate variability
      float base_ind = phi / 2.0f / PI * n_granules; 
      float int_frac = base_ind - floor(base_ind);
      int ind1 = static_cast<int> (base_ind);
      int ind2 = (ind1 == n_granules-1) ? 0 : ind1+1;

      // Variability 
      float var_phase, var;

      for (int k = 0 ; k < n_phase ; k++) {
	// interpolate random structure in hot spot
	if (i != 0 && i != n_theta-1)
	  var_phase = (1.0f - int_frac) * gran_phase[ind1] + 
	    int_frac * gran_phase[ind2];
	else var_phase = polar_phase;

	var = 2.0f * pow(sin(2.0f * phase[k] / timescale + var_phase), 4.0f);

	// Calculate transparency
	float alpha = pow(eye_vec[k] * r_vect, 10) * 1.8f * var; 

	// Determine phase dependent index for storing colour
	int index1 = k*n_vert + index;

	// Assign colours
#ifdef WIREFRAME
	red_grid[index1] = 0.0f;      
	green_grid[index1] = 1.0f;      
	blue_grid[index1] = 0.0f;
	alpha_grid[index1] = 1.0f;
#else
	red_grid[index1] = red;      
	green_grid[index1] = green;      
	blue_grid[index1] = blue;
	alpha_grid[index1] = alpha * opacity;
#endif
      }

      // Assign coordinates
      *(coord_grid[index])   = centre.x + size * r_vect.x;
      *(coord_grid[index]+1) = centre.y + size * r_vect.y;
      *(coord_grid[index]+2) = centre.z + size * r_vect.z;
    }

    // Clean up
    delete[] gran_phase;
  }
}

