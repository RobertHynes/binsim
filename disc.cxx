/*
  Class to calculate properties of an accretion disc

  Released as part of BinSim 1.0
  This release built on August 25 2025

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


#include "disc.h"
#include "roche.h"

/*****************************************************************************/

/*
  Initialise the disc from given parameters
*/
Disc::Disc(const float q1, const float p_orb1, const float mass1,
	   const float disc_thick1, const float r_disc1, 
	   const float tout1, const float temp_grad1, const float beta1) 
{ 
  // Assign calculation accuracy
  TOL = 1e-4f;

  // Assign binary parameters
  q = q1; 
  p_orb = p_orb1;
  mass = mass1;
  
  // Create Roche lobe enclosing disc
  Roche_lobe lobe(q, p_orb, mass);

  // Calculate binary separation, 
  sep = lobe.get_separation();
  c_of_m = lobe.get_c_of_m();
  r_disc = r_disc1 * lobe.get_eggleton();

  // Determine disc radial profile parameters
  disc_thick = disc_thick1 * r_disc;
  beta = beta1;

  // Assign disc temperature distribution parameters
  tout = tout1;
  temp_grad = temp_grad1;

  // Determine where to put transition between concave disc and convex
  // rim - this is certainly within the outer 90% of disc as first guess
  float r1 = 0.1f * r_disc;
  float r2 = r_disc;

  // Precision needed for transition radius
  const float r_tol = TOL * r_disc;

  // Calculation parameters
  float r, h, hr, l1, l2;

  // Iterate to find transition radius which will give a smooth transition
  while (r2 - r1 > r_tol) {
    r = (r1 + r2) / 2.0f;
    h = disc_thick * pow(r/r_disc, beta);
    
    hr = h / r;
    
    l1 = r_disc - r - beta * h * hr;
    l2 = h * sqrt(1 + beta * beta * hr * hr);

    if (l1 > l2) r1 = r;
    else r2 = r;
  }

  // Assign radius of torus used to model rim and transition between
  // rim and concave disc
  r_torus = (l1 + l2) / 2.0;
  r_trans = r;
}

/*
  Calculate properties of the disc surface
*/
Surface_properties Disc::get_surface_properties(const float r, 
						const float phi)
{
  // First determine height of surface above the disc plane
  float h;

  if (r > r_trans) {
    float dr = r + r_torus - r_disc;
    float sinangle = dr / r_torus;
    // Check for rounding errors at disc edge
    if (fabs(sinangle) > 1.0f)
      h = 0.0f;
    else
      h =  r_torus * cos(asin(sinangle));
  } else {
    h = disc_thick * pow(r / r_disc, beta);
  }

  // Calculate Cartesian coordinates of disc surface
  float x = r * cos(phi);
  float y = r * sin(phi);
  float z = h;

  // Calculate unit radial vector in disc plane
  Vec3 r_vec;
  r_vec.x = x;
  r_vec.y = y;
  r_vec.z = 0;
  r_vec.normalize();

  // Calculate normal vector
  Vec3 n_vec;

  if (r > r_trans) {
    float dr = r - r_trans;
    n_vec = r_vec * dr;
    n_vec.z = h;
  } else {
    float dh = beta * disc_thick / r_disc * pow(r/r_disc, beta-1.0f);
    n_vec = dh * r_vec;
    n_vec.z = 1.0;
  }
  n_vec.normalize();
  
  // Calculate disc temperature
  float temp; 
  if (r/r_disc < 0.01) temp = tout * pow(0.01f, temp_grad);
  else temp = tout * pow(r/r_disc, temp_grad);
  
  if (r > r_trans) temp *= (1.0f - 0.2f*(r - r_trans) / r_torus);

  // Return properties of disc surface
  Surface_properties properties(Vec3(x,y,z), n_vec, temp, 0.0f);
  return properties;
}

