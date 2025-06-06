/*
  Class to calculate RGB colors corresponding to a black body spectrum
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

#ifndef _BBCOLORMODEL_H
#define _BBCOLORMODEL_H

#include "keyword.h"
#include "mathvec.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class BB_color_model {
  // Flux from black body of reference temperature
  float f0_red, f0_green, f0_blue;

  // Reference wavelengths
  float ref_red, ref_green, ref_blue;

  // Reference temperature
  float ref_temp;

  // Limb darkening coefficients
  float u_red, u_green, u_blue;

  // Brightness and contrast factors
  float brightness, contrast;
public:
  // Constructors
  BB_color_model(const float brightness1, const float contrast1);
  BB_color_model(Key_list &params);

  // Get RGB values between 0 and 1
  Vec3 get_rgb(const float temp);
  Vec3 get_rgb(const float temp, const float mu);
  
  // Calculate a black body flux
  float get_flux(const float temp, float wavelength);
};
/*****************************************************************************/

#endif
