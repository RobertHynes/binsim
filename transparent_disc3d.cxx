/*
  Class to hold and render a grid defining the surface of an optically
  thin accretion disc
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
#include "transparent_disc3d.h"
#include "mathvec.h"
#include "roche.h"
#include "stream.h"

/*****************************************************************************/

Transparent_disc_3d::Transparent_disc_3d(const int n_steps1, 
		     const vector<float> phase, const float q, 
		     const float inclination, const float period, 
		     const float mass, const float disc_thick, 
		     const float r_disc, const float r_in,
		     const float beta, const float red, 
		     const float green, const float blue, 
		     const float opacity, const int n_flare, 
		     const int flare_length, 
		     const float hot_red, const float hot_green,
		     const float hot_blue, const float hot_opacity)
  : Transparent_object_3d(phase, n_steps1*4, n_steps1*2, inclination)
{
  using namespace Sci_const;

  // Override the object name
  object_name = "Transparent_disc_3d";

  // Define grid size
  n_steps = n_steps1;
  n_rad = n_steps * 2;
  n_phi = n_steps * 4;

  // Create model to describe lobe properties
  Roche_lobe lobe(1.0/q);

  // Create disc model - dummy temperature distribution
  Disc disc(1.0/q, period, mass, disc_thick, r_disc, 1000.0f, 0.0f, beta);
  
  // Create stream model
  Stream stream_model(q, mass, period);
  vector<Vec3> stream = stream_model.stream_calc(0.005, r_disc);

  // Determine location of hotspot
  Vec3 hotspot_centre = stream[stream.size()-1];
  hotspot_centre.x = lobe.get_c_of_m() - hotspot_centre.x;

  // Coordinates on disc surface
  float r, phi;

  // Properties of point on the surface
  Surface_properties surf;
  
  // Local opacity
  float alpha;

  // Index variables
  int i, index;

  // Positions of flares on the disc
  float *flares = new float[n_rad*n_phi];

  // Initialise flare distribution
  for (i = 0 ; i < n_rad ; i++)
    for (int j = 0 ; j < n_phi ; j++)
      flares[i*n_phi + j] = 1.0f;

  // Add flares
  for (i = 0 ; i < n_flare ; i++) {
    // Determine random location of flare
    int index_r = (int) (((float) rand()) / RAND_MAX * (n_rad-1));
    int index_phi = (int) (((float) rand()) / RAND_MAX * (n_phi-1));

    // Smear flare in azimuth
    if (abs(index_r-n_rad/2) > 3) {
      for (int j = 0 ; j < flare_length ; j++) {
	flares[index_r*n_phi + index_phi] *= 1.0f + 
	  (1.0f - 2.0f * fabs(j-flare_length/2.0f) / flare_length) * 0.2f;
	if (index_phi == n_phi-1) index_phi = 0;
	else index_phi++;
      }
    }
  }

  // Parameters for extension of hotspot on disc
  float rDiff, phiDiff, phi0 = PI + atan(-hotspot_centre.y / hotspot_centre.x);
  float fPhi, fR;

  for (i = 0 ; i < n_rad ; i++) {
    // Calculate radius
    if (i < n_rad/2)
      r =  ((r_disc - r_in) * i / (n_rad/2-1.0f) + r_in) * 
	lobe.get_eggleton();
    else
      r = ((r_disc - r_in) * (n_rad-1-i) / (n_rad/2-1.0f) + r_in) * 
	lobe.get_eggleton();

    // Calculate Keplerian period as fraction of Porb
    float p_kep = sqrt(r*r*r*(1.0f + q));

    for (int j = 0 ; j < n_phi ; j++) {
      // Calculate azimuthal angle
      phi = (2.0f * PI * j) / n_phi;

      // Determine index offset for this point
      index = i * n_phi + j;

      // Get properties of the point on the surface
      surf = disc.get_surface_properties(r, phi);
      if (i >= n_rad/2) surf.normal.z = -surf.normal.z;

      // Code to add hot spot 
      phiDiff = phi - phi0;
      rDiff = fabs(r - r_disc * lobe.get_eggleton());
      if (phiDiff > 0.0f)
	fPhi = exp(-1.0f * phiDiff * phiDiff);
      else 
	fPhi = exp(-50.0f * phiDiff * phiDiff);
      fR = exp(-500.0f * rDiff * rDiff);

      for (int k = 0 ; k < n_phase ; k++) {
	// Determine index for flares allowing for Keplerian rotation
	int j1;
	if (i != 0 && i != n_rad-1)
	  j1 = j - static_cast<int> 
	    (phase[k] / p_kep * n_phi + 0.5f);
	else j1 = j;
	while (j1 < 0) j1 += n_phi;

	// Apply flares to disc
	alpha = opacity * flares[i*n_phi + j1];

	// Apply heating downstream of hotspot
	alpha *= (1.0f + hot_opacity * fR * fPhi);

	// If disc is significantly truncated apply fade-out effect
	if (r_in > 0.01f) {
	  float frac_r = (r / lobe.get_eggleton() - r_in) / 0.2f;
	  if (frac_r < 1.0f) alpha *= frac_r;
	}

	// Determine phase dependent index for storing colour
	int index1 = k * n_vert + index;

	// Assign colours
#ifdef WIREFRAME
	red_grid[index1] = 0.4f;
	green_grid[index1] = 0.7f;
	blue_grid[index1] = 1.0f;
	alpha_grid[index1] = 1.0f;
#else
	if (i == n_rad/2) {
	  int index2 = index1 - n_phi;
	  red_grid[index1] = red_grid[index2];
	  green_grid[index1] = green_grid[index2];
	  blue_grid[index1] = blue_grid[index2];
	  alpha_grid[index1] = alpha_grid[index2];
	} else {
	  red_grid[index1] = (red + hot_red * hot_opacity * fR * fPhi) / 
	    (1.0f + hot_opacity * fR * fPhi);
	  green_grid[index1] = (green + hot_green * hot_opacity * fR * fPhi) / 
	    (1.0f + hot_opacity * fR * fPhi);
	  blue_grid[index1] = (blue + hot_blue * hot_opacity * fR * fPhi) / 
	    (1.0f + hot_opacity * fR * fPhi);
	  alpha_grid[index1] = alpha;
	}
#endif
      }
      if (r_in > 0.0f && (i == 0 || i == n_rad-1))
	surf.coords.z = 0.0f;
      if (i > n_rad/2) surf.coords.z = -surf.coords.z;
      
      // Assign coordinates
      *(coord_grid[index])   = surf.coords.x + lobe.get_c_of_m();
      *(coord_grid[index]+1) = surf.coords.y;
      *(coord_grid[index]+2) = surf.coords.z;
    }
  }

  // Clean up
  delete[] flares;
}

