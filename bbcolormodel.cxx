/*
  Class to calculate RGB colors corresponding to a black body spectrum

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


#include <cmath>

#include "bbcolormodel.h"
#include "constants.h"

#include "errmsg.h"

/*****************************************************************************/

/*
  Create a color model
*/
BB_color_model::BB_color_model(const float brightness1, const float contrast1)
{
  // Set brightness and contrast
  brightness = brightness1;
  contrast = contrast1;

  // Define red, green and blue wavelengths; RGB is really NIR/opt/NUV
  // to increase colour contrast
  ref_red = 12500.0f;
  ref_green = 5000.0f;
  ref_blue = 2000.0f;

  // Define temperature of a 'white' object
  ref_temp = 7000.0f;

  // Define limb darkening coefficients
  u_red = 0.8f;
  u_green = 0.8f;
  u_blue = 0.8f;

  // Calculate reference fluxes
  f0_red = get_flux(ref_temp, ref_red);
  f0_green = get_flux(ref_temp, ref_green);
  f0_blue = get_flux(ref_temp, ref_blue);
}

BB_color_model::BB_color_model(Key_list &params)
{
  // Set brightness and contrast
  try { brightness = params.get_float("BRIGHTNESS"); }
  catch (Key_list::Key_not_found_exception) {
    brightness = 0.6f;
    print_default_key_msg("BRIGHTNESS", "0.6");
  }
  try { contrast = params.get_float("CONTRAST"); }
  catch (Key_list::Key_not_found_exception) {
    contrast = 0.25f;
    print_default_key_msg("CONTRAST", "0.25");
  }

  if (contrast <= 0.0f) 
    throw Key_list::Value_out_of_range_exception("CONTRAST", "> 0.0");

  // Define red, green and blue wavelengths; RGB is really NIR/opt/NUV
  // to increase colour contrast
  ref_red = 12500.0f;
  ref_green = 5000.0f;
  ref_blue = 2000.0f;

  // Define temperature of a 'white' object
  ref_temp = 7000.0f;

  // Define limb darkening coefficients
  u_red = 0.8f;
  u_green = 0.8f;
  u_blue = 0.8f;

  // Calculate reference fluxes
  f0_red = get_flux(ref_temp, ref_red);
  f0_green = get_flux(ref_temp, ref_green);
  f0_blue = get_flux(ref_temp, ref_blue);
}

/*
  Get RGB values between 0 and 1 with no limb darkening
*/
Vec3 BB_color_model::get_rgb(const float temp)
{
  // Calculate fluxes
  const float f_red = get_flux(temp, ref_red);
  const float f_green = get_flux(temp, ref_green);
  const float f_blue = get_flux(temp, ref_blue);

  // Divide by reference flux and calculate colour components
  float red = brightness + log(f_red / f0_red) * contrast;
  if (red < 0.0f) red = 0.0f;
  else if (red > 1.0f) red = 1.0f;

  float green = brightness + log(f_green / f0_green) * contrast;
  if (green < 0.0f) green = 0.0f;
  else if (green > 1.0f) green = 1.0f;

  float blue = brightness + log(f_blue / f0_blue) * contrast;
  if (blue < 0.0f) blue = 0.0f;
  else if (blue > 1.0f) blue = 1.0f;

  return Vec3(red, green, blue);
}

/*
  Get RGB values between 0 and 1 allowing for limb darkening
*/
Vec3 BB_color_model::get_rgb(const float temp, const float mu)
{
  // Calculate fluxes
  float f_red = get_flux(temp, ref_red);
  float f_green = get_flux(temp, ref_green);
  float f_blue = get_flux(temp, ref_blue);

  // Apply linear limb darkening
  f_red *= 1.0f - (1.0f - mu) * u_red;
  f_green *= 1.0f - (1.0f - mu) * u_green;
  f_blue *= 1.0f - (1.0f - mu) * u_blue;

  // Divide by reference flux and calculate colour components
  float red = brightness + log(f_red / f0_red) * contrast;
  if (red < 0.0f) red = 0.0f;
  else if (red > 1.0f) red = 1.0f;

  float green = brightness + log(f_green / f0_green) * contrast;
  if (green < 0.0f) green = 0.0f;
  else if (green > 1.0f) green = 1.0f;

  float blue = brightness + log(f_blue / f0_blue) * contrast;
  if (blue < 0.0f) blue = 0.0f;
  else if (blue > 1.0f) blue = 1.0f;

  return Vec3(red, green, blue);
}

/*
  Calculate the black body flux
*/
float BB_color_model::get_flux(const float temp, float lam) 
{
  using namespace Sci_const;

  // Convert wavelength to microns
  lam *= 1e-4f;  

  // Return flux (arbitary normalisation)
  return 1.0f / (exp(1.0e6 * H * C / K / lam / temp) - 1.0f);
}
