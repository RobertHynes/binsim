/*
  Class to hold and render a grid defining the surface of a Roche lobe
  filling star
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

#include <cstdlib>

#include "bbcolormodel.h"
#include "constants.h"
#include "lobe3d.h"
#include "mathvec.h"
#include "roche.h"

/*****************************************************************************/

Lobe_3d::Lobe_3d(const int n_steps1, const vector<float> phase, const float q, 
		 const float inclination, const float period, 
		 const float mass, const float t_pole, 
		 const float t_min,
		 const float l_irrad, const float disc_thick,
		 const float granulation_amplitude, 
		 const float granulation_period,
		 const float fill, BB_color_model &cm, const bool primary)
  : Object_3d(phase, 4*n_steps1, 2*n_steps1+1, inclination)
{
  using namespace Sci_const;

  // Override the object name
  object_name = "Lobe_3d";

  // Define grid size
  n_steps = n_steps1;
  n_lat = 2 * n_steps + 1;
  n_long = 4 * n_steps;

  // Create Roche lobe model to describe object
  Roche_star lobe(q, t_pole, period, mass, fill);
  lobe.enable_irradiation(l_irrad, disc_thick, 0.5f, true);
 
  // Angles specifying a point on the surface
  float theta, phi;

  // Properties of a point on the surface
  Surface_properties surf;
  float temp, tirr, temp1;
  Vec3 rgb;

  // Index variables
  int j, index;

  for (int i = 0 ; i < n_lat ; i++) {
    // Calculate angle of latitude
    theta = PI * i / n_lat;    

    // Calculate granulation model with constant size granulations
    int n_granules = static_cast<int> (n_long * sin(theta) + 0.5f);
    n_granules = (n_granules > 1) ? n_granules : 1; 

    //float granulation[n_granules];
    float *gran_phase = new float[n_granules];
    for (j = 0 ; j < n_granules ; j++)
      gran_phase[j] = ((float) rand()) / RAND_MAX * 2.0f * PI;

    for (j = 0 ; j < n_long ; j++) {
      // Calculate angle of longitude
      phi = (2.0f * PI * j) / n_long;

      // Determine index offset for this point
      index = i * n_long + j;

      // Get properties of the point on the surface
      surf = lobe.get_surface_properties(theta, phi);
      temp = surf.temp;
      
      // Invert coordinates and normals for companion
      if (primary) {
	surf.coords.x = -surf.coords.x;
	surf.coords.y = -surf.coords.y;
	surf.normal.x = -surf.normal.x;
	surf.normal.y = -surf.normal.y;
      }

      // Enforce minimum temperature
      temp = (temp > t_pole * t_min) ? temp : t_pole * t_min;

      // Get irradiation heating
      tirr = surf.t_irr;

      for (int k = 0 ; k < n_phase ; k++) {
	// Apply granulation
	float base_ind = phi / 2.0f / PI * n_granules; 
	int ind1 = static_cast<int> (base_ind);
	int ind2 = (ind1 == n_granules-1) ? 0 : ind1+1;
	
	float int_frac = base_ind - floor(base_ind);
	
	float gran1 = 1.0f + granulation_amplitude * 
	  sin(2.0f * PI * phase[k] * 10.0f + gran_phase[ind1]);
	
	float gran2 = 1.0f + granulation_amplitude * 
	  sin(2.0f * PI * phase[k] / granulation_period + gran_phase[ind2]);
	
	temp1 = temp * ((1.0f - int_frac) * gran1 + int_frac * gran2);
	
	// Combine intrinsic and irradiation temperatures
	temp1 = sqrt(sqrt(temp1*temp1*temp1*temp1 + tirr*tirr*tirr*tirr));

	// Determine phase dependent index for storing colour
	int index1 = k * n_vert + index;

	// Calculate limb darkened colour on surface
	float mu = fabs(surf.normal * eye_vec[k]);
 	rgb = cm.get_rgb(temp1, mu);

	// Assign colours
#ifdef WIREFRAME
	red_grid[index1] = 1.0f;
	green_grid[index1] = 0.0f;
	blue_grid[index1] = 0.0f;
#else
	red_grid[index1] = rgb.x;
	green_grid[index1] = rgb.y;
	blue_grid[index1] = rgb.z;
#endif
      }

      // Assign coordinates
      if (primary) {
	*(coord_grid[index])   = surf.coords.x + lobe.get_c_of_m();
	*(coord_grid[index]+1) = surf.coords.y;
	*(coord_grid[index]+2) = surf.coords.z;
      } else {
	*(coord_grid[index])   = surf.coords.x - lobe.get_c_of_m();
	*(coord_grid[index]+1) = surf.coords.y;
	*(coord_grid[index]+2) = surf.coords.z;
      }
    }
    // Clean up
    delete[] gran_phase;
  }
}
