/*
  Class to calculate properties of an accretion disc

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

#ifndef _DISC_H
#define _DISC_H

#include <cmath>

#include "constants.h"
#include "mathvec.h"
#include "surface.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

/*
  Class defining properties of an accretion disc
*/
class Disc { 
  // Calculation accuracy
  // Now defined by constructor as Visual C++ does not allow static
  // const members
  float TOL;

  // Mass ratio
  float q;
  
  // Orbital period and separation
  float p_orb, sep;

  // Distance of compact object from centre of mass
  float c_of_m;

  // Mass of compact star
  float mass;

  // Disk thickness, radius and radius of outer torus
  float disc_thick, r_disc, r_torus;  

  // Disc height exponent
  float beta;

  // Temperature profile
  float tout, temp_grad;

  // Transition radius from concave to convex
  float r_trans;
public:
  // Constructor
  Disc(const float q1, const float p_orb1, const float mass1,
       const float disc_thick1, const float r_disc1,
       const float tout1, const float temp_grad1, const float beta1);

  // Function to calculate all properties
  Surface_properties get_surface_properties(const float r, 
					    const float phi);

};

/*****************************************************************************/

#endif

