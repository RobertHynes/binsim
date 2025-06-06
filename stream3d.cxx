/*
  Class to hold and render a grid defining an accretion stream
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

#include <iostream>

#include <GL/glut.h>

#include "mathvec.h"
#include "roche.h"
#include "stream.h"
#include "stream3d.h"

/*****************************************************************************/

Stream_3d::Stream_3d(const int n_phi1, vector<float> phase, const float q, 
		     const float inclination, const float m_prim, 
		     const float period, const float disc_rad, 
		     const float t_pole, const float max_stream_thick,
		     const float open_angle, 
		     const float red, const float green, 
		     const float blue, const float opacity)
  : Transparent_object_3d(phase, n_phi1, 1, inclination)
{
  using Sci_const::PI;

  // Override the object name
  object_name = "Stream_3d";

  n_phi = n_phi1;

  // Index variable
  int i;

  // Create model to describe accretor lobe properties
  Roche_lobe lobe(1.0f / q, period, m_prim);

  // Create model to describe donor lobe properties
  Roche_lobe donor_lobe(q, period, m_prim * q);

  // Create stream model
  Stream stream_model(q, m_prim, period);
  stream = stream_model.stream_calc(0.01f, 0.9f * disc_rad);
  float stream_speed = stream_model.get_speed();
  float stream_period = (stream[stream.size()-1] - stream[0]).mod() / 
    stream_speed;

  // Create extension of stream back to overlap L1 point
  Vec3 ext_point;

  // Extrapolate stream backwards by one point
  stream.insert(stream.begin(), ext_point);

  // Delete existing arrays (created by superclass) so they can be
  // replaced with correct size versions
  delete[] red_grid;
  delete[] green_grid;
  delete[] blue_grid;
  delete[] alpha_grid;

  for (i = 0; i < n_vert; i++) delete[] coord_grid[i];
  delete[] coord_grid;

  // Calculate size of arrays needed. Leave one spare point at the end
  n_y = stream.size()-1;
  n_vert = n_x * n_y;

  // Create array of distances travelled along stream
  float *x = new float[n_y];
  x[0] = 0.0f;
  float l = 0.0f;

  for (i = 1 ; i < n_y ; i++) {
    l += (stream[i] - stream[i-1]).mod();
    x[i] = l;
  }

  // Create stream surface grids
  red_grid = new GLfloat[n_phase*n_vert];
  green_grid = new GLfloat[n_phase*n_vert];
  blue_grid = new GLfloat[n_phase*n_vert];
  alpha_grid = new GLfloat[n_phase*n_vert];

  coord_grid = new GLfloat*[n_vert];
  for (i = 0 ; i < n_vert ; i++) coord_grid[i] = new GLfloat[3]; 

  // Declare vectors to be used in calculations
  Vec3 basis_vert(0.0f, 0.0f, 1.0f);
  Vec3 basis_parallel, basis_perp;
  Vec3 rad_vect, pos_vect;

  // Opacity
  float alpha;

  // Estimate sound speed at L1 point
  float c_s = 10000.0f * sqrt(0.75f * t_pole / 10000.0f);

  // Calculate stream thickness
  float stream_rad = 0.5f * c_s * period / 2.0f / PI / lobe.get_separation();
  stream_rad = (stream_rad < max_stream_thick) ? stream_rad : max_stream_thick;

  // Variability on the stream
  float *stream_density = new float[n_y*n_phi];

  // Initialise density distribution
  for (i = 0 ; i < n_y ; i++)
    for (int j = 0 ; j < n_phi ; j++)
      stream_density[i*n_phi + j] = pow(((float) rand()) / RAND_MAX, 4.0f);

  for (i = 0 ; i < n_y ; i++) {
    // Calculate vectors parallel and normal to the stream
    basis_parallel = stream[i+1] - stream[i];
    basis_parallel.normalize();

    basis_perp = basis_parallel % basis_vert;
    basis_perp.normalize();

    for (int j = 0 ; j < n_phi ; j++) {
      // Calculate azimuthal angle
      float phi = (2.0f * PI * j) / n_phi;

      // Determine index offset for this point
      int index = i * n_phi + j;

      // Calculate unit vector to stream surface point
      rad_vect = basis_perp * cos(phi) + basis_vert * sin(phi);

      // Calculate opacity - complicated fudge to look good - no physics
      if (i < 4) alpha = 0.15f * i; 
      else alpha = 0.5f;

      for (int k = 0 ; k < n_phase ; k++) {
	// Add randomness to opacity
	int density_index = static_cast<int>(i - phase[k] / stream_period * n_y);
	while (density_index < 0) density_index += n_y;
	while (density_index >= n_y) density_index -= n_y;

	float alpha1 = alpha * stream_density[density_index * n_phi + j];
	
	// Determine phase dependent index for storing colour
	int index1 = k * n_vert + index;
	
	// Calculate observer's angle to surface to fudge opacity
	float mu = fabs(rad_vect * eye_vec[k]);
	float nu = fabs(basis_parallel * eye_vec[k]);
	nu = 1.0f / sqrt(1.0f - nu*nu);

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
	alpha_grid[index1] = alpha1 * mu * mu * nu * opacity;
#endif
      }

      // Calculate position of stream surface point
      pos_vect = stream[i] + rad_vect * stream_rad;

      if (i < 10) {
	float open_height = tan(open_angle / 360.0f * 2.0f * PI);
	Vec3 contact_point;
	contact_point.x = 1.0f;
	contact_point.y = -open_height * cos(phi);
	contact_point.z = open_height * sin(phi);
	contact_point.normalize();
	
	contact_point *= donor_lobe.get_rad(contact_point.x, contact_point.z);
	contact_point.x = stream[1].x - donor_lobe.get_l1() + contact_point.x;

	pos_vect = sqrt(sqrt(i/10.0f)) * pos_vect + 
	           (1.0f - sqrt(sqrt(i/10.0f))) * contact_point; 
      }

      // Assign coordinates
      *(coord_grid[index])   = pos_vect.x;
      *(coord_grid[index]+1) = pos_vect.y;
      *(coord_grid[index]+2) = pos_vect.z;
    }
  }

  // Clean up
  delete[] stream_density;
}

