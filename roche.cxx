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

#include "roche.h"

/*****************************************************************************/

/*
  Initialise the lobe from given parameters
*/
Roche_lobe::Roche_lobe(const float q1, const float p_orb1, 
		       const float mass1, const float fill1) 
{ 
  TOL = 1e-6f;
  DXYZ = 1e-4f;

  q = q1; 
  q_inv = 1.0f / q;
  fill = fill1;
  p_orb = p_orb1;
  mass = mass1;

  // Initialise undefined derived parameters
  c_of_m = l1 = eggleton = inv_eggleton = sep = -1.0f;
  surf_pot = r_pole = g_pole = -1.0f;
}

/*
  Return the binary separation
*/
float Roche_lobe::get_separation()
{
  using Sci_const::PI;
  using Sci_const::G;

  if (sep < 0.0f) 
    sep = pow((G*mass * (1.0f+1.0f/q)) * p_orb*p_orb / 4.0f/PI/PI, 1.0f/3.0f);

  return sep;
}

/*
  Return the distance of the centre of mass from this star
*/
float Roche_lobe::get_c_of_m() 
{
  if (c_of_m < 0.0f) c_of_m = 1.0f / (1.0f+q);
	
  return c_of_m;
}

/*
  Return the distance of the L1 point from the centre of this star
*/
float Roche_lobe::get_l1() 
{
  if (l1 < 0.0f) {
    // Bracket L1 point with the two Eggelton radii
    float x0 = get_eggleton();
    float x2 = 1.0f - get_inverse_eggleton();
	
    double x1 = (x0 + x2) / 2.0f;
    double dx = x2 - x0;

    // Use binary search to locate point where potential gradient is
    // zero
    while (dx > TOL) {
      if (get_pot_deriv_loc(x1) < 0.0f)
	x0 = x1;
      else
	x2 = x1;

      x1 = (x0 + x2) / 2.0f;
      dx = x2 - x0;
    }

    // Cache best value
    l1 = x1;
  }

  return l1;
}

/*
  Return the Eggelton effective radius of this star
*/
float Roche_lobe::get_eggleton() 
{ 
  if (eggleton < 0.0f) {
    // Calculate q^1/3 and q^2/3 for convenience
    const float q13 = pow(q, 1.0f/3.0f);
    const float q23 = q13 * q13;

    eggleton = 0.49f*q23 / (0.6f*q23 + log(1.0f+q13));
  }

  return eggleton; 
}

/*
  Return the Eggleton effective radius of the companion star
*/
float Roche_lobe::get_inverse_eggleton() 
{ 
  if (inv_eggleton < 0.0f) {
    // Calculate q^1/3 and q^2/3 for convenience
    const float q13 = pow(1.0f/q, 1.0f/3.0f);
    const float q23 = q13 * q13;

    inv_eggleton = 0.49f * q23 / (0.6f*q23 + log(1.0f+q13));
  }

  return inv_eggleton; 
}
 
/*
  Return the Kopak's Omega potential at an arbitary point
*/
float Roche_lobe::get_pot(const float r, const float l, const float nu) 
{ 
  // Compute the three terms given by Kopal
  const float term1 = 1.0f / r;
  const float term2 = q_inv * (1.0f / sqrt(1.0f - 2.0f*l*r + r*r) - l*r);
  const float term3 = (q_inv + 1.0f) / 2.0f * r*r * (1.0f - nu*nu);

  return (term1 + term2 + term3);
}

/*
  Return the potential at an arbitary point
*/
float Roche_lobe::get_pot_cartesian(const float x, const float y,
				    const float z) 
{ 
  // Convert to standard coordinate system
  const float r = sqrt(x*x + y*y + z*z);
  const float l = x / r;
  const float nu = z / r;

  // Calculate potential
  return get_pot(r, l, nu);
}

/*
  Return the potential at the lobe surface
*/
float Roche_lobe::get_surf_pot() 
{
  if (surf_pot < 0.0f) 
    // Calculate the potential at the L1 point
    surf_pot = get_pot(get_l1(), 1.0f, 0.0f);

  return surf_pot / fill;
}

/*
  Return the polar radius of the lobe
*/
float Roche_lobe::get_polar_rad() 
{
  if (r_pole < 0.0f) {
    // Bracket radius point with the Eggelton radius and a center of star
    float r0 = DXYZ;
    float r2 = get_eggleton();
	
    float r1 = (r0 + r2) / 2.0f;
    float dr = r2 - r0;

    // Use binary search to locate point where potential equals the
    // surface potential
    while (dr > TOL) {
      if (get_pot(r1, 0.0f, 1.0f) > get_surf_pot())
	r0 = r1;
      else
	r2 = r1;
      
      r1 = (r0 + r2) / 2.0f;
      dr = r2 - r0;
    }

    r_pole = r1;
  }

  return r_pole;
}

/*
  Calculate the gravity at the pole 
*/
float Roche_lobe::get_polar_grav() 
{
  if (g_pole < 0.0f) 
    g_pole = get_grav(get_polar_rad(), 0.0f, 0.0f, 1.0f);

  return g_pole;
}

/*
  Return the radius at in an arbitary direction
*/
float Roche_lobe::get_rad(const float l, const float nu) 
{
  // Bracket radius point with the polar radius and the L1 point
  float r0 = get_polar_rad();
  float r2 = get_l1();
	
  float r1 = (r0 + r2) / 2.0f;
  float dr = r2 - r0;
	
  // Use binary search to locate point where potential equals the
  // surface potential
  while (dr > TOL) {
    if (get_pot(r1, l, nu) > get_surf_pot())
      r0 = r1;
    else
      r2 = r1;
	    
    r1 = (r0 + r2) / 2.0f;
    dr = r2 - r0;
  }
	
  return r1;
}

/*
  Calculate the gravity at an arbitary point
*/
float Roche_lobe::get_grav(const float r, const float l, 
			   const float mu, const float nu) 
{
  // Convert the point to Cartesian coordinates
  const float x = r * l;
  const float y = r * mu;
  const float z = r * nu;

  // Evaluate the Cartesian partial derivatives of the potential
  const float gx = (get_pot_cartesian(x + DXYZ, y, z) - 
	      get_pot_cartesian(x - DXYZ, y, z)) / DXYZ;
  const float gy = (get_pot_cartesian(x, y + DXYZ, z) - 
	      get_pot_cartesian(x, y - DXYZ, z)) / DXYZ;
  const float gz = (get_pot_cartesian(x, y, z + DXYZ) - 
	      get_pot_cartesian(x, y, z - DXYZ)) / DXYZ;

  // Return the gravity
  return sqrt(gx*gx + gy*gy + gz*gz);
}

/*
  Calculate the normal vector to an arbitary point
*/
Vec3 Roche_lobe::get_normal(const float r, const float l, 
			    const float mu, const float nu) 
{
  // Convert the point to Cartesian coordinates
  const float x = r * l;
  const float y = r * mu;
  const float z = r * nu;

  // Evaluate the Cartesian partial derivatives of the potential
  const float gx = (get_pot_cartesian(x + DXYZ, y, z) - 
	      get_pot_cartesian(x - DXYZ, y, z)) / DXYZ;
  const float gy = (get_pot_cartesian(x, y + DXYZ, z) - 
	      get_pot_cartesian(x, y - DXYZ, z)) / DXYZ;
  const float gz = (get_pot_cartesian(x, y, z + DXYZ) - 
	      get_pot_cartesian(x, y, z - DXYZ)) / DXYZ;

  Vec3 grad(-gx,-gz,-gy);
  grad.normalize();

  // The normalised negative gradient is equivalent to a unit normal
  return grad;
}

/*
  Return the potential gradient at a point along the line of centres
*/
float Roche_lobe::get_pot_deriv_loc(const float x) 
{
  return -1.0f / x / x + q_inv / (1.0f-x) / (1.0f-x) - q_inv + 
    (1.0f+q_inv) * x;
}

/*****************************************************************************/

/*
  Constructor
*/
Roche_star::Roche_star(const float q1, const float t_pole1, 
		       const float p_orb1, const float mass1, 
		       const float fill1) : 
  Roche_lobe(q1, p_orb1, mass1, fill1)
{ 
  t_pole = t_pole1;

  // Determine if star has a convective envelope
  convective = (t_pole < 7000.0f) ? true : false;
	
  // Determine gravity darkening exponent
  beta = (convective) ? 0.08 : 0.25;

  // Set irradiation off by default
  irradiation = false;
}

/*
  Turn on irradiation and set parameters
*/
void Roche_star::enable_irradiation(const double l_irr1, 
				    const float disk_thick1, 
				    const float albedo1, 
				    const bool black_hole1)
{
  irradiation = true;
  l_irr = l_irr1;
  disk_thick = disk_thick1;
  disk_blur = 0.2F * disk_thick;
  albedo = albedo1;
  black_hole = black_hole1;
}

/*
  Turn off irradiation
*/
void Roche_star::disable_irradiation() { irradiation = false; }

/*
  Return the polar temperature
*/
float Roche_star::get_polar_temp() { return t_pole; }

/*
  Return the gravity darkened temperature at an arbitary point
*/
float Roche_star::get_temp(const float r, const float l, 
			   const float mu, const float nu) 
{
  return (t_pole * pow(get_grav(r,l,mu,nu) / get_polar_grav(), beta));
}

/*
  Return the irradiation temperature
*/
float Roche_star::get_t_irr(const float r, const float l, 
			    const float mu, const float nu) 
{
  // Stefain-Boltzmann constant
  using namespace Sci_const;

  // Convert the point to Cartesian coordinates
  const float x = r * l;
  const float y = r * mu;
  const float z = r * nu;

  // Get the normal vector
  Vec3 norm = get_normal(r, l, mu, nu);

  if (irradiation) {
    // Calculate unit displacement vector and distance from
    // irradiation source
    Vec3 r_vec = Vec3(1.0f,0.0f,0.0f) - Vec3(x,y,z);
    float r_abs = r_vec.mod() * get_separation();
    r_vec.normalize();
    
    // Irradiation flux with appropriate irradiation geometry
    float f_irr;
    if (black_hole)
      f_irr = l_irr / r_abs / r_abs / 2.0f / PI * 
	fabs(r_vec.z / sqrt(r_vec.x*r_vec.x + r_vec.y*r_vec.y));
    else
      f_irr = l_irr / r_abs / r_abs / 4.0f / PI;

    // Projection factor
    float dot_product = norm * r_vec;

    // Check for shielding by disc and return irradiation temperature
    float h = fabs(r_vec.z) / sqrt(r_vec.x * r_vec.x + r_vec.y * r_vec.y);
    if (dot_product > 0.0f && h > disk_thick - disk_blur) {
      float t_irr = sqrt(sqrt(f_irr * dot_product * albedo / S));
      if (h < disk_thick + disk_blur)
	t_irr *= (h - (disk_thick - disk_blur)) / 2.0F / disk_blur;
      return t_irr;
    }
    else
      return 0.0f;
  } else return 0.0f;
}

/*
  Return all the properties at the lobe surface
*/
Surface_properties Roche_star::get_surface_properties(const float theta, 
						      const float phi)
{
  // Stefan-Boltzmann constant
  using Sci_const::S;

  // Convert to standard coordinate system
  const float l = sin(theta) * cos(phi);
  const float mu = sin(theta) * sin(phi);
  const float nu = cos(theta);

  // Calculate the surface radius
  const float r = get_rad(l, nu);

  // Convert the point to Cartesian coordinates
  const float x = r * l;
  const float y = r * mu;
  const float z = r * nu;

  // Evaluate the Cartesian partial derivatives of the potential
  const float gx = (get_pot_cartesian(x + DXYZ, y, z) - 
	      get_pot_cartesian(x - DXYZ, y, z)) / DXYZ;
  const float gy = (get_pot_cartesian(x, y + DXYZ, z) - 
	      get_pot_cartesian(x, y - DXYZ, z)) / DXYZ;
  const float gz = (get_pot_cartesian(x, y, z + DXYZ) - 
	      get_pot_cartesian(x, y, z - DXYZ)) / DXYZ;

  // Calculate the surface gravity
  const float g = sqrt(gx*gx + gy*gy + gz*gz);

  // Calculate the gravity darkened temperature
  const float temp = t_pole * pow(g / get_polar_grav(), beta);

  // Determine the normal (= the normalized negative potential gradient)
  Vec3 grad(-gx, -gy, -gz);
  grad.normalize();

  // Determine the irradiation temperature
  float t_irr;
  if (irradiation) {
    // Calculate unit displacement vector and distance from
    // irradiation source
    Vec3 r_vec = Vec3(1.0f,0.0f,0.0f) - Vec3(x,y,z);
    float r_abs = r_vec.mod() * get_separation();
    r_vec.normalize();

    // Irradiation flux
    float f_irr = l_irr / r_abs / r_abs;

    // Projection factor
    float dot_product = grad * r_vec;

    // Check for shielding by disc and evaluate irradiation temperature
    float h = fabs(r_vec.z) / sqrt(r_vec.x * r_vec.x + r_vec.y * r_vec.y);
    if (dot_product > 0.0f && h > disk_thick - disk_blur) {
      t_irr = sqrt(sqrt(f_irr * dot_product * albedo / S));
      if (h < disk_thick + disk_blur)
	t_irr *= (h - (disk_thick - disk_blur)) / 2.0F / disk_blur;
    }
    else
      t_irr = 0.0f;
  } else t_irr = 0.0f;

  Surface_properties properties(Vec3(x,y,z), grad, temp, t_irr);
  return properties;
}
