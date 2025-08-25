/*
  Class to calculate properties of a Roche lobe filling star

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

#ifndef _ROCHE_H
#define _ROCHE_H

#include <cmath>

#include "constants.h"
#include "mathvec.h"
#include "surface.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

/*
  Class defining properties of a Roche lobe  
*/
class Roche_lobe { 
protected:
  // Calculation accuracy
  // Now defined by constructor as Visual C++ does not allow static
  // const members
  float TOL, DXYZ;

  // Mass ratio and its inverse 
  float q, q_inv;
  
  // Lobe filling factor
  float fill;

  // Position of centre of mass and L1 point
  float c_of_m;
  float l1;
    
  // Eggleton radii
  float eggleton, inv_eggleton;
  
  // Surface potential
  float surf_pot;
  
  // Polar radius and gravity
  float r_pole, g_pole;
  
  // Orbital period and separation
  float p_orb, sep;

  // Mass of THIS star
  float mass;

  float get_pot_deriv_loc(const float x);

public:
  // Constructor.  Default parameters are used if explicit dimensions
  // are not necessary
  Roche_lobe(const float q1, const float p_orb1 = 1.0 * Sci_const::DAY, 
	     const float mass1 = 1.0 * Sci_const::MSUN, 
	     const float fill1 = 1.0);

  // Binary separation
  float get_separation();

  // Centre of mass and L1 point
  float get_c_of_m();
  float get_l1();

  // Effective radii of this and counterpart lobe
  float get_eggleton(); 
  float get_inverse_eggleton();

  // Potential
  float get_pot(const float r, const float l, const float nu);
  float get_pot_cartesian(const float x, const float y, const float z);
  float get_surf_pot();

  // Polar radius and gravity
  float get_polar_rad();
  float get_polar_grav();

  // General radius and normal vector
  float get_rad(const float l, const float nu);
  float get_grav(const float r, const float l, const float mu, const float nu);
  Vec3  get_normal(const float r, const float l, const float nu, 
		   const float mu);
};

/*****************************************************************************/

/*
  Class defining properties of a Roche lobe filling star  
*/
class Roche_star: public Roche_lobe { 
  // Convective flag and related gravity darkening exponent
  bool convective;
  float beta;

  // Polar temperature
  float t_pole;

  // Irradiation flag and irradiating luminosity
  bool irradiation;
  double l_irr;
  
  // Albedo
  float albedo;

  // Disk thickness
  float disk_thick;

  // Blurring range for disk (use disk_thick +/- disk_blur)
  float disk_blur;

  // Flag for black hole irradiation
  bool black_hole;
public:
  // Constructors
  Roche_star(const float q1, const float t_pole1, 
	     const float p_orb1 = 1.0 * Sci_const::DAY, 
	     const float mass1 = 1.0 * Sci_const::MSUN, 	     
	     const float fill1 = 1.0);

  // Control irradiation
  void enable_irradiation(const double l_irr1, const float disk_thick1,
			  const float albedo1 = 0.5, 
			  const bool black_hole1 = false);
  void disable_irradiation();

  // Polar temperature
  float get_polar_temp();

  // General temperature
  float get_temp(const float r, const float l, const float mu, 
		 const float nu);

  // Irradiation temperature
  float get_t_irr(const float r, const float l, const float mu, 
		  const float nu);

  // Optimised convenience function to calculate all properties
  Surface_properties get_surface_properties(const float theta, 
					    const float phi);
};

/*****************************************************************************/

#endif

