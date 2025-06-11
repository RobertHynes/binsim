/*
  Class to describe 3D model of an interacting binary
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

#include <cmath>

#include "binary3d.h"
#include "constants.h"
#include "errmsg.h"

using std::cout;

/*****************************************************************************/

/*
  Constructor
*/
Binary_3d::Binary_3d(Key_list &params, vector<float> phase1) 
{ 
  cout << "Extracting binary options...\n";
  get_params(params);
 
  // Save pointer to phases
  phase = phase1;

  // Create color model
  BB_color_model cm(params);

  // Create Primary Roche lobe object
  if (show_lobe1) {
    cout << "Creating primary lobe object...\n";
    lobe1 = new Lobe_3d(lobe1_n_steps, phase, 1.0/q, inclination, period, m_prim,
			lobe1_t_pole, lobe1_t_min, luminosity2, disc_eff_thick,
		        lobe1_granulation, lobe1_granulation_period, 
		        lobe1_fill, cm, true);
  }
  
  // Create Companion Roche lobe object
  if (show_lobe2) {
    cout << "Creating companion lobe object...\n";
    lobe2 = new Lobe_3d(lobe2_n_steps, phase, q, inclination, period, m_prim,
		       lobe2_t_pole, lobe2_t_min, luminosity1, disc_eff_thick,
		       lobe2_granulation, lobe2_granulation_period, 
		       lobe2_fill, cm, false);
  }
  
  // Create disc object
  if (show_disc) {
    cout << "Creating disc object...\n";
    disc = new Disc_3d(disc_n_steps, phase, q, inclination, period, m_prim,
		       disc_geom_thick, disc_rad, disc_r_in, 
		       disc_tout, disc_temp_grad, disc_beta, 
		       hot_spot_temp, disc_n_flare, disc_flare_length, cm);
  }
  
  // Create optically thin disc object
  if (show_transparent_disc) {
    cout << "Creating optically thin disc object...\n";
    transparent_disc = new Transparent_disc_3d(transparent_disc_n_steps, 
			   phase, q, inclination, period, m_prim, 
                           transparent_disc_geom_thick, transparent_disc_rad, 
                           transparent_disc_r_in, transparent_disc_beta,
			   transparent_disc_red, transparent_disc_green, 
			   transparent_disc_blue, transparent_disc_opacity, 
			   transparent_disc_n_flare, 
			   transparent_disc_flare_length,
			   hot_spot_red, hot_spot_green, hot_spot_blue,
			   transparent_disc_hot_opacity);
  }

  // Create stream object
  if (show_stream) { 
    cout << "Creating stream object...\n";
#ifdef WIREFRAME
    stream = new Stream_3d(8, phase, q, inclination, m_prim, period, 
			   stream_disc_rad, lobe2_t_pole, stream_max_thick, 
			   stream_open_angle, 
			   stream_red, stream_green,
			   stream_blue, stream_opacity);
#else
    stream = new Stream_3d(20, phase, q, inclination, m_prim, period, 
			   stream_disc_rad, lobe2_t_pole, stream_max_thick, 
			   stream_open_angle, 
			   stream_red, stream_green,
			   stream_blue, stream_opacity);
#endif
  }

  // Create hot spot object
  if (show_hot_spot) {
    cout << "Creating hot spot object...\n";
#ifdef WIREFRAME
    hot_spot = new Hot_spot_3d(2, phase, q, inclination, m_prim, period,
			       hot_spot_disc_rad, hot_spot_size,
			       hot_spot_red, hot_spot_green,
			       hot_spot_blue, hot_spot_opacity,
			       hot_spot_timescale);

#else
    hot_spot = new Hot_spot_3d(20, phase, q, inclination, m_prim, period,
			       hot_spot_disc_rad, hot_spot_size,
			       hot_spot_red, hot_spot_green,
			       hot_spot_blue, hot_spot_opacity,
			       hot_spot_timescale);
#endif
  }

  // Create disc coronae
  if (show_corona1) {
    cout << "Creating corona object...\n";
    corona1 = new Corona_3d(50, phase, q, inclination, corona1_rad, 
			    corona1_red, corona1_green, corona1_blue,
			    corona1_opacity, corona1_exp);
  }

  if (show_corona2) {
    cout << "Creating corona object...\n";
    corona2 = new Corona_3d(50, phase, q, inclination, corona2_rad, 
			    corona2_red, corona2_green, corona2_blue,
			    corona2_opacity, corona2_exp);
  }

  // Create stellar wind
  if (show_stellar_wind) {
    cout << "Creating stellar wind object...\n";
    stellar_wind = new Corona_3d(120, phase, q, inclination, stellar_wind_rad, 
				 stellar_wind_red, stellar_wind_green, 
				 stellar_wind_blue, stellar_wind_opacity, 
				 stellar_wind_exp, true);
  }

  // Create jet
  if (show_jet) {
    cout << "Creating jet object...\n";
    jet = new Jet_3d(60, phase, q, inclination, jet_opening_angle, 
		     jet_red1, jet_green1, jet_blue1, 
		     jet_red2, jet_green2, jet_blue2, 
		     jet_opacity, jet_exp, 
		     jet_inc, jet_phi);
  }
}

/*****************************************************************************/

/*
  Draw binary components
*/
void Binary_3d::draw(int phase_index)
{
  using Sci_const::PI;

  // Define angle corresponding to orbital phase
  float angle = phase[phase_index] * 360.0f + 90.0f;
  if (angle >= 360.0f) angle -= 360.0f;

  // Orient binary according to inclination and phase
  glRotatef(-inclination, 1.0f, 0.0f, 0.0f);
  glRotatef(angle, 0.0f, 0.0f, 1.0f);

  // Draw selected components
  if (show_lobe1) lobe1->draw(phase_index);
  if (show_lobe2) lobe2->draw(phase_index);
  if (show_disc) disc->draw(phase_index);
  if (show_transparent_disc) transparent_disc->draw(phase_index);
  if (show_stream) stream->draw(phase_index);
  if (show_hot_spot) hot_spot->draw(phase_index);
  if (show_corona1) corona1->draw(phase_index);
  if (show_corona2) corona2->draw(phase_index);
  if (show_stellar_wind) stellar_wind->draw(phase_index);

  // Draw jet fixed in inertial fram
  if (show_jet) {
    // Define offsets and angles
    float offset = 1.0f - 1.0f / (1.0f+q);
    float phase_angle = phase[phase_index] * 2.0f * PI;

    // Remove orbital motion
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);

    // Shift jet to position of compact object
    glTranslatef(-offset * sin(phase_angle), offset*cos(phase_angle), 0.0f);

    // Apply jet rotations
    glRotatef(jet_phi, 0.0f, 0.0f, 1.0f);
    glRotatef(jet_inc, 1.0f, 0.0f, 0.0f);

    // Draw jet
    jet->draw(phase_index);

    // Reverse transformations applied
    glRotatef(-jet_inc, 1.0f, 0.0f, 0.0f);
    glRotatef(-jet_phi, 0.0f, 0.0f, 1.0f);
    glTranslatef(offset * sin(phase_angle), -offset*cos(phase_angle), 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
  }
}

/*****************************************************************************/

/*
  Read in parameters from file
*/
void Binary_3d::get_params(Key_list &params)
{
  // Show the primary star?  Default false
  try { show_lobe1 = params.get_bool("SHOW_LOBE1");}   
  catch (Key_list::Key_not_found_exception) {
    show_lobe1 = false;
    print_default_key_msg("SHOW_LOBE1", "False");
  } 

  // Show the companion star?  Default false
  try { show_lobe2 = params.get_bool("SHOW_LOBE2");}   
  catch (Key_list::Key_not_found_exception) {
    show_lobe2 = false;
    print_default_key_msg("SHOW_LOBE2", "False");
  } 

  // Show the accretion disc?  Default false
  try { show_disc = params.get_bool("SHOW_DISC");}   
  catch (Key_list::Key_not_found_exception) {
    show_disc = false;
    print_default_key_msg("SHOW_DISC", "False");
  } 

  // Show the optically thin accretion disc?  Default false
  try { show_transparent_disc = params.get_bool("SHOW_THIN_DISC");}   
  catch (Key_list::Key_not_found_exception) {
    show_transparent_disc = false;
    print_default_key_msg("SHOW_THIN_DISC", "False");
  } 

  // Show the accretion stream?  Default false
  try { show_stream = params.get_bool("SHOW_STREAM");}   
  catch (Key_list::Key_not_found_exception) {
    show_stream = false;
    print_default_key_msg("SHOW_STREAM", "False");
  } 

  // Show the stream impact point?  Default false
  try { show_hot_spot = params.get_bool("SHOW_HOT_SPOT");}   
  catch (Key_list::Key_not_found_exception) {
    show_hot_spot = false;
    print_default_key_msg("SHOW_HOT_SPOT", "False");
  } 
  
  // Show the first corona?  Default false
  try { show_corona1 = params.get_bool("SHOW_CORONA1");}   
  catch (Key_list::Key_not_found_exception) {
    show_corona1 = false;
    print_default_key_msg("SHOW_CORONA1", "False");
  } 

  // Show the second corona?  Default false
  try { show_corona2 = params.get_bool("SHOW_CORONA2");}   
  catch (Key_list::Key_not_found_exception) {
    show_corona2 = false;
    print_default_key_msg("SHOW_CORONA2", "False");
  } 
  
  // Show the stellar wind?  Default false
  try { show_stellar_wind = params.get_bool("SHOW_STELLAR_WIND");}   
  catch (Key_list::Key_not_found_exception) {
    show_stellar_wind = false;
    print_default_key_msg("SHOW_STELLAR_WIND", "False");
  } 
  
  // Show the jet?  Default false
  try { show_jet = params.get_bool("SHOW_JET");}   
  catch (Key_list::Key_not_found_exception) {
    show_jet = false;
    print_default_key_msg("SHOW_JET", "False");
  } 
  
  /***************************************************************************/

  // Determine orbital period - no default, must be > 0.0
  period = params.get_float("PERIOD") * Sci_const::HOUR;
  if (period <= 0.0f) 
    throw Key_list::Value_out_of_range_exception("PERIOD", "> 0.0");

  // Determine mass ratio - no default, must be > 0.0
  q = params.get_float("Q");
  if (q <= 0.0f) throw Key_list::Value_out_of_range_exception("Q", "> 0.0");

  // Determine primary mass - no default, must be > 0.0
  m_prim = params.get_float("M1") * Sci_const::MSUN;
  if (m_prim <= 0.0f) 
    throw Key_list::Value_out_of_range_exception("M1", "> 0.0");

  // Determine inclination - no default, must be > -90.0, < 90.0
  inclination = params.get_float("INCLINATION");  
  if (inclination < 0.0f || inclination > 90.0f)
    throw Key_list::Value_out_of_range_exception("INCLINATION", 
						 "0.0-90.0");

  /***************************************************************************/

  // Determine primary lobe parameters
  if (show_lobe1) {
    // Determine lobe grid step - default 60, must be > 2
    try { lobe1_n_steps = params.get_int("LOBE1_NSTEPS"); }
    catch (Key_list::Key_not_found_exception) {
      lobe1_n_steps = 60;
      print_default_key_msg("LOBE1_NSTEPS", "60");
    }
    if (lobe1_n_steps < 2)
      throw Key_list::Value_out_of_range_exception("LOBE1_NSTEPS", "> 2");
     
    // Determine lobe filling factor - default 1.0, must be 0-1
    try { lobe1_fill = params.get_float("LOBE1_FILL"); }
    catch (Key_list::Key_not_found_exception) {
      lobe1_fill = 1.0f;
      print_default_key_msg("LOBE1_FILL", "1.0");
    }
    if (lobe1_fill < 0.0f || lobe1_fill > 1.0f) 
      throw Key_list::Value_out_of_range_exception("LOBE1_FILL", "0.0-1.0");

    // Determine lobe polar temperature - no default, must be > 0.0
    lobe1_t_pole = params.get_float("LOBE1_T_POLE");
    if (lobe1_t_pole <= 0.0f)
      throw Key_list::Value_out_of_range_exception("LOBE1_T_POLE", "> 0.0");

    // Determine minimum lobe temperature as fraction of t_pole -
    // default 1.0, must be 0.0-1.0
    try { lobe1_t_min = params.get_float("LOBE1_T_MIN"); }
    catch (Key_list::Key_not_found_exception) {
      lobe1_t_min = 0.0f;
      print_default_key_msg("LOBE1_T_MIN", "0.0");
    }
    if (lobe1_t_min < 0.0f || lobe1_t_min > 1.0f)
      throw Key_list::Value_out_of_range_exception("LOBE1_T_MIN", "0.0-1.0");

    // Determine amount of granulation on the lobe - default 0.0, must
    // be > 0.0
    try { lobe1_granulation = params.get_float("LOBE1_GRANULATION"); }
    catch (Key_list::Key_not_found_exception) {
      lobe1_granulation = 0.0f;
      print_default_key_msg("LOBE1_GRANULATION", "0.0");
    }
    if (lobe1_granulation < 0.0f)
      throw Key_list::Value_out_of_range_exception("LOBE1_GRANULATION", 
						   ">= 0.0");

    // Determine bubbling period for granulation as fraction of
    // orbital period - default 1e6, must be > 0.0
    try { lobe1_granulation_period = 
	    params.get_float("LOBE1_GRANULATION_PERIOD"); }
    catch (Key_list::Key_not_found_exception) {
      lobe1_granulation_period = 1e6;
      print_default_key_msg("LOBE1_GRANULATION_PERIOD", "1e6");
    }
    if (lobe1_granulation_period <= 0.0f)
      throw Key_list::Value_out_of_range_exception("LOBE1_GRANULATION_PERIOD", 
						   "> 0.0");

    // X-ray luminosity - default 0.0, must be >= 0.0
    try { luminosity2 = params.get_float("LUMINOSITY2")
	    * Sci_const::ERG/Sci_const::SECOND; }
    catch (Key_list::Key_not_found_exception) {
      luminosity2 = 0.0f;
      print_default_key_msg("LUMINOSITY2", "0.0");
    }
    if (luminosity2 < 0.0f)
      throw Key_list::Value_out_of_range_exception("LUMINOSITY2", ">= 0.0");
  }

  /***************************************************************************/

  // Determine companion lobe parameters
  if (show_lobe2) {
    // Determine lobe grid step - default 60, must be > 2
    try { lobe2_n_steps = params.get_int("LOBE2_NSTEPS"); }
    catch (Key_list::Key_not_found_exception) {
      lobe2_n_steps = 60;
      print_default_key_msg("LOBE2_NSTEPS", "60");
    }
    if (lobe2_n_steps < 2)
      throw Key_list::Value_out_of_range_exception("LOBE2_NSTEPS", "> 2");
     
    // Determine lobe filling factor - default 1.0, must be 0-1
    try { lobe2_fill = params.get_float("LOBE2_FILL"); }
    catch (Key_list::Key_not_found_exception) {
      lobe2_fill = 1.0f;
      print_default_key_msg("LOBE2_FILL", "1.0");
    }
    if (lobe2_fill < 0.0f || lobe2_fill > 1.0f) 
      throw Key_list::Value_out_of_range_exception("LOBE2_FILL", "0.0-1.0");

    // Determine lobe polar temperature - no default, must be > 0.0
    lobe2_t_pole = params.get_float("LOBE2_T_POLE");
    if (lobe2_t_pole <= 0.0f)
      throw Key_list::Value_out_of_range_exception("LOBE2_T_POLE", "> 0.0");

    // Determine minimum lobe temperature as fraction of t_pole -
    // default 1.0, must be 0.0-1.0
    try { lobe2_t_min = params.get_float("LOBE2_T_MIN"); }
    catch (Key_list::Key_not_found_exception) {
      lobe2_t_min = 0.0f;
      print_default_key_msg("LOBE2_T_MIN", "0.0");
    }
    if (lobe2_t_min < 0.0f || lobe2_t_min > 1.0f)
      throw Key_list::Value_out_of_range_exception("LOBE2_T_MIN", "0.0-1.0");

    // Determine amount of granulation on the lobe - default 0.0, must
    // be > 0.0
    try { lobe2_granulation = params.get_float("LOBE2_GRANULATION"); }
    catch (Key_list::Key_not_found_exception) {
      lobe2_granulation = 0.0f;
      print_default_key_msg("LOBE2_GRANULATION", "0.0");
    }
    if (lobe2_granulation < 0.0f)
      throw Key_list::Value_out_of_range_exception("LOBE2_GRANULATION", 
						   ">= 0.0");

    // Determine bubbling period for granulation as fraction of
    // orbital period - default 1e6, must be > 0.0
    try { lobe2_granulation_period = 
	    params.get_float("LOBE2_GRANULATION_PERIOD"); }
    catch (Key_list::Key_not_found_exception) {
      lobe2_granulation_period = 1e6;
      print_default_key_msg("LOBE2_GRANULATION_PERIOD", "1e6");
    }
    if (lobe2_granulation_period <= 0.0f)
      throw Key_list::Value_out_of_range_exception("LOBE2_GRANULATION_PERIOD", 
						   "> 0.0");

    // X-ray luminosity - default 0.0, must be >= 0.0
    try { luminosity1 = params.get_float("LUMINOSITY1") 
	    * Sci_const::ERG/Sci_const::SECOND; }
    catch (Key_list::Key_not_found_exception) {
      luminosity1 = 0.0f;
      print_default_key_msg("LUMINOSITY1", "0.0");
    }
    if (luminosity1 < 0.0f)
      throw Key_list::Value_out_of_range_exception("LUMINOSITY1", ">= 0.0");

    // Effective thickness of disc, for shielding - default -1.0, no limits
    try { disc_eff_thick = params.get_float("DISC_EFF_THICK"); }
    catch (Key_list::Key_not_found_exception) {
      disc_eff_thick = -1.0f;
      print_default_key_msg("DISC_EFF_THICK", "-1.0 (no shielding)");
    }
  }

  /***************************************************************************/

  // Determine disc parameters
  if (show_disc) {
    // Determine disc grid step - default 60, must be > 2
    try { disc_n_steps = params.get_int("DISC_NSTEPS"); }
    catch (Key_list::Key_not_found_exception) {
      disc_n_steps = 60;
      print_default_key_msg("DISC_NSTEPS", "60");
    }
    if (disc_n_steps < 2)
      throw Key_list::Value_out_of_range_exception("DISC_NSTEPS", ">= 2");

    // Determine disc radius - no default, must be 0-1
    disc_rad = params.get_float("DISC_RAD");
    if (disc_rad > 1.0f || disc_rad < 0.0f)
      throw Key_list::Value_out_of_range_exception("DISC_RAD", 
						   "0.0-1.0");

    // Determine disc inner radius - default 0.0, must be 0-Disc_Rad
    try { disc_r_in = params.get_float("DISC_R_IN"); }
    catch (Key_list::Key_not_found_exception) {
      disc_r_in = 0.0;
      print_default_key_msg("DISC_R_IN", "0.0");
    }
    if (disc_r_in >= disc_rad || disc_r_in < 0.0f)
      throw Key_list::Value_out_of_range_exception("DISC_R_IN", 
						   "0.0-DISC_RAD");

    // Determine disc thickness - no default, must be > 0.0
    disc_geom_thick = params.get_float("DISC_GEOM_THICK");
    if (disc_geom_thick <= 0.0f) 
      throw Key_list::Value_out_of_range_exception("DISC_GEOM_THICK", "> 0.0");

    // Disc edge temperature - no default, must be > 0.0
    disc_tout = params.get_float("DISC_TOUT");
    if (disc_tout <= 0.0f) 
      throw Key_list::Value_out_of_range_exception("DISC_TOUT", "> 0.0");

    // Disc temperature gradient - no default, no limits
    disc_temp_grad = params.get_float("DISC_TEMP_GRAD");

    // Disc profile exponent - no default, must be > 0.0
    disc_beta = params.get_float("DISC_BETA");
    if (disc_beta <= 0.0f) 
      throw Key_list::Value_out_of_range_exception("DISC_BETA", "> 0.0");
    
    // Number of flares on the disc - default 1500, must be >=0
    try { disc_n_flare = params.get_int("DISC_N_FLARE"); }
    catch (Key_list::Key_not_found_exception) {
      disc_n_flare = 1500;
      print_default_key_msg("DISC_N_FLARE", "1500");
    }
    if (disc_n_flare < 0)
      throw Key_list::Value_out_of_range_exception("DISC_N_FLARE", ">= 0");

    // Length of flares - default 25, must be > 0
    try { disc_flare_length = params.get_int("DISC_FLARE_LENGTH"); }
    catch (Key_list::Key_not_found_exception) {
      disc_flare_length = 25;
      print_default_key_msg("DISC_FLARE_LENGTH", "25");
    }
    if (disc_flare_length <= 0)
      throw Key_list::Value_out_of_range_exception("DISC_FLARE_LENGTH", "> 0");

    // Extra heating at stream impact point, default 0, must be >= 0.0
    try { hot_spot_temp = params.get_float("HOT_TEMP"); }
    catch (Key_list::Key_not_found_exception) {
      hot_spot_temp = 0.0f;
      print_default_key_msg("HOT_TEMP", "0.0");
    }
    if (hot_spot_temp < 0.0f)
      throw Key_list::Value_out_of_range_exception("HOT_TEMP", ">= 0.0");
  }

  /***************************************************************************/

  // Determine optically thin disc parameters
  if (show_transparent_disc) {
    // Determine disc grid step - default 60, must be > 2
    try { transparent_disc_n_steps = params.get_int("THIN_DISC_NSTEPS"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_n_steps = 60;
      print_default_key_msg("THIN_DISC_NSTEPS", "60");
    }
    if (transparent_disc_n_steps < 2)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_NSTEPS", 
						   ">= 2");

    // Determine disc radius - no default, must be 0-1
    transparent_disc_rad = params.get_float("THIN_DISC_RAD");
    if (transparent_disc_rad > 1.0f || transparent_disc_rad < 0.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_RAD", 
						   "0.0-1.0");

    // Determine disc inner radius - default 0.0, must be 0-Disc_Rad
    try { transparent_disc_r_in = params.get_float("THIN_DISC_R_IN"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_r_in = 0.0;
      print_default_key_msg("THIN_DISC_R_IN", "0.0");
    }
    if (transparent_disc_r_in >= transparent_disc_rad || 
	transparent_disc_r_in < 0.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_R_IN", 
						   "0.0-THIN_DISC_RAD");

    // Determine disc thickness - no default, must be > 0.0
    transparent_disc_geom_thick = params.get_float("THIN_DISC_GEOM_THICK");
    if (transparent_disc_geom_thick <= 0.0f) 
      throw Key_list::Value_out_of_range_exception("THIN_DISC_GEOM_THICK", 
						   "> 0.0");

    // Disc profile exponent - no default, must be > 0.0
    transparent_disc_beta = params.get_float("THIN_DISC_BETA");
    if (transparent_disc_beta <= 0.0f) 
      throw Key_list::Value_out_of_range_exception("THIN_DISC_BETA", "> 0.0");

    // Red component of disc colour - default 1.0, must be 0-1
    try { transparent_disc_red = params.get_float("THIN_DISC_RED"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_red = 1.0f;
      print_default_key_msg("THIN_DISC_RED", "1.0");
    }
    if (transparent_disc_red < 0.0f || transparent_disc_red > 1.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_RED", "0.0-1.0");

    // Green component of disc colour - default 1.0, must be 0-1
    try { transparent_disc_green = params.get_float("THIN_DISC_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_green = 1.0f;
      print_default_key_msg("THIN_DISC_GREEN", "1.0");
    }
    if (transparent_disc_green < 0.0f || transparent_disc_green > 1.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_GREEN", 
						   "0.0-1.0");

    // Blue component of disc colour - default 1.0, must be 0-1
    try { transparent_disc_blue = params.get_float("THIN_DISC_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_blue = 1.0f;
      print_default_key_msg("THIN_DISC_BLUE", "1.0");
    }
    if (transparent_disc_blue < 0.0f || transparent_disc_blue > 1.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_BLUE", 
						   "0.0-1.0");

    // Disc opacity - default 1.0, must be >= 0.0
    try { transparent_disc_opacity = params.get_float("THIN_DISC_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_opacity = 1.0f;
      print_default_key_msg("THIN_DISC_OPACITY", "1.0");
    }
    if (transparent_disc_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_OPACITY",
						   ">= 0.0");

    // Number of flares on the disc - default 1500, must be >=0
    try { transparent_disc_n_flare = params.get_int("THIN_DISC_N_FLARE"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_n_flare = 1500;
      print_default_key_msg("THIN_DISC_N_FLARE", "1500");
    }
    if (transparent_disc_n_flare < 0)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_N_FLARE", 
						   ">= 0");

    // Length of flares - default 25, must be > 0
    try { transparent_disc_flare_length = 
	    params.get_int("THIN_DISC_FLARE_LENGTH"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_flare_length = 25;
      print_default_key_msg("THIN_DISC_FLARE_LENGTH", "25");
    }
    if (transparent_disc_flare_length <= 0)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_FLARE_LENGTH", 
						   "> 0");

    if (!show_hot_spot) {
      // Red component of hot spot colour - default 1.0, must be 0-1
      try { hot_spot_red = params.get_float("HOT_SPOT_RED"); }
      catch (Key_list::Key_not_found_exception) {
	hot_spot_red = 1.0f;
	print_default_key_msg("HOT_SPOT_RED", "1.0");
      }
      if (hot_spot_red < 0.0f || hot_spot_red > 1.0f)
	throw Key_list::Value_out_of_range_exception("HOT_SPOT_RED", 
						     "0.0-1.0");
      
      // Green component of hot spot colour - default 1.0, must be 0-1
      try { hot_spot_green = params.get_float("HOT_SPOT_GREEN"); }
      catch (Key_list::Key_not_found_exception) {
	hot_spot_green = 1.0f;
	print_default_key_msg("HOT_SPOT_GREEN", "1.0");
      }
      if (hot_spot_green < 0.0f || hot_spot_green > 1.0f)
	throw Key_list::Value_out_of_range_exception("HOT_SPOT_GREEN", 
						     "0.0-1.0");

      // Blue component of hot spot colour - default 1.0, must be 0-1
      try { hot_spot_blue = params.get_float("HOT_SPOT_BLUE"); }
      catch (Key_list::Key_not_found_exception) {
	hot_spot_blue = 1.0f;
	print_default_key_msg("HOT_SPOT_BLUE", "1.0");
      }
      if (hot_spot_blue < 0.0f || hot_spot_blue > 1.0f)
	throw Key_list::Value_out_of_range_exception("HOT_SPOT_BLUE", 
						     "0.0-1.0");
    }

    // Hot spot trail opacity - default 1.0, must be >= 0.0
    try { transparent_disc_hot_opacity = 
	    params.get_float("THIN_DISC_HOT_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      transparent_disc_hot_opacity = 0.0f;
      print_default_key_msg("THIN_DISC_HOT_OPACITY", "0.0");
    }
    if (transparent_disc_hot_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("THIN_DISC_HOT_OPACITY",
						   ">= 0.0");
  }

  /***************************************************************************/

  // Determine stream parameters
  if (show_stream) {
    // Max limit on stream thickness, default 1.0, , must be > 0.0
    try { stream_max_thick = params.get_float("STREAM_MAX_THICK"); }
    catch (Key_list::Key_not_found_exception) {
      stream_max_thick = 1.0f;
      print_default_key_msg("STREAM_MAX_THICK", "1.0");
    }
    if (stream_max_thick <= 0.0f)
      throw Key_list::Value_out_of_range_exception("STREAM_MAX_THICK", 
						   "> 0.0");

    // Stream opening angle, default 10.0, must be 0.0-45.0
    try { stream_open_angle = params.get_float("STREAM_OPEN_ANGLE"); }
    catch (Key_list::Key_not_found_exception) {
      stream_open_angle = 10.0f;
      print_default_key_msg("STREAM_OPEN_ANGLE", "10.0");
    }
    if (stream_open_angle < 0.0f || stream_open_angle > 45.0f)
      throw Key_list::Value_out_of_range_exception("STREAM_OPEN_ANGLE", 
						   "0.0-45.0");

    // Red component of stream colour - default 1.0, must be 0-1
    try { stream_red = params.get_float("STREAM_RED"); }
    catch (Key_list::Key_not_found_exception) {
      stream_red = 1.0f;
      print_default_key_msg("STREAM_RED", "1.0");
    }
    if (stream_red < 0.0f || stream_red > 1.0f)
      throw Key_list::Value_out_of_range_exception("STREAM_RED", "0.0-1.0");

    // Green component of stream colour - default 1.0, must be 0-1
    try { stream_green = params.get_float("STREAM_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      stream_green = 1.0f;
      print_default_key_msg("STREAM_GREEN", "1.0");
    }
    if (stream_green < 0.0f || stream_green > 1.0f)
      throw Key_list::Value_out_of_range_exception("STREAM_GREEN", "0.0-1.0");

    // Blue component of stream colour - default 1.0, must be 0-1
    try { stream_blue = params.get_float("STREAM_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      stream_blue = 1.0f;
      print_default_key_msg("STREAM_BLUE", "1.0");
    }
    if (stream_blue < 0.0f || stream_blue > 1.0f)
      throw Key_list::Value_out_of_range_exception("STREAM_BLUE", "0.0-1.0");

    // Stream opacity - default 1.0, must be >= 0.0
    try { stream_opacity = params.get_float("STREAM_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      stream_opacity = 1.0f;
      print_default_key_msg("STREAM_OPACITY", "1.0");
    }
    if (stream_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("STREAM_OPACITY",
						   ">= 0.0");

    // Determine lobe polar temperature - no default, must be > 0.0
    if (!show_lobe2) {
      lobe2_t_pole = params.get_float("LOBE2_T_POLE");
      if (lobe2_t_pole <= 0.0f)
	throw Key_list::Value_out_of_range_exception("LOBE2_T_POLE", "> 0.0");
    }

    // Determine 'disc' radius if no disc present- no default, must be 0-1
    if (!show_disc && !show_transparent_disc) {
      stream_disc_rad = params.get_float("DISC_RAD");
      if (stream_disc_rad > 1.0f || stream_disc_rad < 0.0f)
	throw Key_list::Value_out_of_range_exception("DISC_RAD", "0.0-1.0");
    }
    // Only normal disc is present
    else if (show_disc && !show_transparent_disc)
      stream_disc_rad = disc_rad;
    // Only thin disc is present
    else if (!show_disc && show_transparent_disc)
      stream_disc_rad = transparent_disc_rad;
    // Both discs present - use large radius
    else {
      if (disc_rad > transparent_disc_rad)
	stream_disc_rad = disc_rad;
      else 
	stream_disc_rad = transparent_disc_rad;
    } 
  }

  /***************************************************************************/

  // Determine hot spot parameters
  if (show_hot_spot) {
    // Hot spot size - no default, must be > 0.0
    hot_spot_size = params.get_float("HOT_SPOT_SIZE");
    if (hot_spot_size <= 0.0f)
      throw Key_list::Value_out_of_range_exception("HOT_SPOT_SIZE", "> 0.0");

    // Red component of hot spot colour - default 1.0, must be 0-1
    try { hot_spot_red = params.get_float("HOT_SPOT_RED"); }
    catch (Key_list::Key_not_found_exception) {
      hot_spot_red = 1.0f;
      print_default_key_msg("HOT_SPOT_RED", "1.0");
    }
    if (hot_spot_red < 0.0f || hot_spot_red > 1.0f)
      throw Key_list::Value_out_of_range_exception("HOT_SPOT_RED", "0.0-1.0");

    // Green component of hot spot colour - default 1.0, must be 0-1
    try { hot_spot_green = params.get_float("HOT_SPOT_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      hot_spot_green = 1.0f;
      print_default_key_msg("HOT_SPOT_GREEN", "1.0");
    }
    if (hot_spot_green < 0.0f || hot_spot_green > 1.0f)
      throw Key_list::Value_out_of_range_exception("HOT_SPOT_GREEN", 
						   "0.0-1.0");

    // Blue component of hot spot colour - default 1.0, must be 0-1
    try { hot_spot_blue = params.get_float("HOT_SPOT_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      hot_spot_blue = 1.0f;
      print_default_key_msg("HOT_SPOT_BLUE", "1.0");
    }
    if (hot_spot_blue < 0.0f || hot_spot_blue > 1.0f)
      throw Key_list::Value_out_of_range_exception("HOT_SPOT_BLUE", "0.0-1.0");

    // Hot Spot opacity - default 1.0, must be >= 0.0
    try { hot_spot_opacity = params.get_float("HOT_SPOT_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      hot_spot_opacity = 1.0f;
      print_default_key_msg("HOT_SPOT_OPACITY", "1.0");
    }
    if (hot_spot_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("HOT_SPOT_OPACITY",
						   ">= 0.0");

    // Determine timescale for hot spot flickering - default 1e6, must be > 0.0
    try { hot_spot_timescale = params.get_float("HOT_SPOT_TIMESCALE"); }
    catch (Key_list::Key_not_found_exception) {
      hot_spot_timescale = 1e6;
      print_default_key_msg("HOT_SPOT_TIMESCALE", "1e6");
    }
    if (hot_spot_timescale <= 0.0f)
      throw Key_list::Value_out_of_range_exception("HOT_SPOT_TIMESCALE", 
						   "> 0.0");

    // Determine disc radius if no disc present - no default, must be 0-1
    if (!show_disc && !show_stream) {
      hot_spot_disc_rad = params.get_float("DISC_RAD");
      if (hot_spot_disc_rad > 1.0f || hot_spot_disc_rad < 0.0f)
	throw Key_list::Value_out_of_range_exception("DISC_RAD", 
						   "0.0-1.0");
    }
    // Only normal disc is present
    else if (show_disc && !show_transparent_disc)
      hot_spot_disc_rad = disc_rad;
    // Only thin disc is present
    else if (!show_disc && show_transparent_disc)
      hot_spot_disc_rad = transparent_disc_rad;
    // Both discs present - use large radius
    else {
      if (disc_rad > transparent_disc_rad)
	hot_spot_disc_rad = disc_rad;
      else 
	hot_spot_disc_rad = transparent_disc_rad;
    }
  }

  /***************************************************************************/

  // Determine parameters of first corona
  if (show_corona1) {
    // Corona #1 radius - no default, must be >= 0.0
    corona1_rad = params.get_float("CORONA1_RAD");
    if (corona1_rad < 0.0f)
      throw Key_list::Value_out_of_range_exception("CORONA1_RAD",
						   ">= 0.0");

    // Red component of corona #1 colour - default 1.0, must be 0-1
    try { corona1_red = params.get_float("CORONA1_RED"); }
    catch (Key_list::Key_not_found_exception) {
      corona1_red = 1.0f;
      print_default_key_msg("CORONA1_RED", "1.0");
    }
    if (corona1_red < 0.0f || corona1_red > 1.0f)
      throw Key_list::Value_out_of_range_exception("CORONA1_RED", "0.0-1.0");

    // Green component of corona #1 colour - default 1.0, must be 0-1
    try { corona1_green = params.get_float("CORONA1_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      corona1_green = 1.0f;
      print_default_key_msg("CORONA1_GREEN", "1.0");
    }
    if (corona1_green < 0.0f || corona1_green > 1.0f)
      throw Key_list::Value_out_of_range_exception("CORONA1_GREEN", 
						   "0.0-1.0");

    // Blue component of corona #1 colour - default 1.0, must be 0-1
    try { corona1_blue = params.get_float("CORONA1_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      corona1_blue = 1.0f;
      print_default_key_msg("CORONA1_BLUE", "1.0");
    }
    if (corona1_blue < 0.0f || corona1_blue > 1.0f)
      throw Key_list::Value_out_of_range_exception("CORONA1_BLUE", "0.0-1.0");

    // Corona #1 opacity - default 1.0, must be >= 0.0
    try { corona1_opacity = params.get_float("CORONA1_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      corona1_opacity = 1.0f;
      print_default_key_msg("CORONA1_OPACITY", "1.0");
    }
    if (corona1_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("CORONA1_OPACITY",
						   ">= 0.0");

    // Corona #1 density exponent - default 5.0, no limits
    try { corona1_exp = params.get_float("CORONA1_EXPONENT"); }
    catch (Key_list::Key_not_found_exception) {
      corona1_exp = 5.0f;
      print_default_key_msg("CORONA1_EXPONENT", "5.0");
    }
  }

  /***************************************************************************/

  // Determine parameters of second corona
  if (show_corona2) {
    // Corona #2 radius - no default, must be >= 0.0
    corona2_rad = params.get_float("CORONA2_RAD");
    if (corona2_rad < 0.0f)
      throw Key_list::Value_out_of_range_exception("CORONA2_RAD",
						   ">= 0.0");

    // Red component of corona #2 colour - default 1.0, must be 0-1
    try { corona2_red = params.get_float("CORONA2_RED"); }
    catch (Key_list::Key_not_found_exception) {
      corona2_red = 1.0f;
      print_default_key_msg("CORONA2_RED", "1.0");
    }
    if (corona2_red < 0.0f || corona2_red > 1.0f)
      throw Key_list::Value_out_of_range_exception("CORONA2_RED", "0.0-1.0");

    // Green component of corona #2 colour - default 1.0, must be 0-1
    try { corona2_green = params.get_float("CORONA2_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      corona2_green = 1.0f;
      print_default_key_msg("CORONA2_GREEN", "1.0");
    }
    if (corona2_green < 0.0f || corona2_green > 1.0f)
      throw Key_list::Value_out_of_range_exception("CORONA2_GREEN", 
						   "0.0-1.0");

    // Blue component of corona #2 colour - default 1.0, must be 0-1
    try { corona2_blue = params.get_float("CORONA2_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      corona2_blue = 1.0f;
      print_default_key_msg("CORONA2_BLUE", "1.0");
    }
    if (corona2_blue < 0.0f || corona2_blue > 1.0f)
      throw Key_list::Value_out_of_range_exception("CORONA2_BLUE", "0.0-1.0");

    // Corona #2 opacity - default 1.0, must be >= 0.0
    try { corona2_opacity = params.get_float("CORONA2_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      corona2_opacity = 1.0f;
      print_default_key_msg("CORONA2_OPACITY", "1.0");
    }
    if (corona2_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("CORONA2_OPACITY",
						   ">= 0.0");

    // Corona #2 density exponent - default 5.0, no limits
    try { corona2_exp = params.get_float("CORONA2_EXPONENT"); }
    catch (Key_list::Key_not_found_exception) {
      corona2_exp = 5.0f;
      print_default_key_msg("CORONA2_EXPONENT", "5.0");
    }
  }

  /***************************************************************************/

  // Determine parameters of stellar wind
  if (show_stellar_wind) {
    // Stellar wind radius - no default, must be >= 0.0
    stellar_wind_rad = params.get_float("STELLAR_WIND_RAD");
    if (stellar_wind_rad < 0.0f)
      throw Key_list::Value_out_of_range_exception("STELLAR_WIND_RAD",
						   ">= 0.0");

    // Red component of stellar wind colour - default 1.0, must be 0-1
    try { stellar_wind_red = params.get_float("STELLAR_WIND_RED"); }
    catch (Key_list::Key_not_found_exception) {
      stellar_wind_red = 1.0f;
      print_default_key_msg("STELLAR_WIND_RED", "1.0");
    }
    if (stellar_wind_red < 0.0f || stellar_wind_red > 1.0f)
      throw Key_list::Value_out_of_range_exception("STELLAR_WIND_RED", 
						   "0.0-1.0");

    // Green component of stellar wind colour - default 1.0, must be 0-1
    try { stellar_wind_green = params.get_float("STELLAR_WIND_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      stellar_wind_green = 1.0f;
      print_default_key_msg("STELLAR_WIND_GREEN", "1.0");
    }
    if (stellar_wind_green < 0.0f || stellar_wind_green > 1.0f)
      throw Key_list::Value_out_of_range_exception("STELLAR_WIND_GREEN", 
						   "0.0-1.0");

    // Blue component of stellar wind colour - default 1.0, must be 0-1
    try { stellar_wind_blue = params.get_float("STELLAR_WIND_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      stellar_wind_blue = 1.0f;
      print_default_key_msg("STELLAR_WIND_BLUE", "1.0");
    }
    if (stellar_wind_blue < 0.0f || stellar_wind_blue > 1.0f)
      throw Key_list::Value_out_of_range_exception("STELLAR_WIND_BLUE", 
						   "0.0-1.0");

    // Stellar wind opacity - default 1.0, must be >= 0.0
    try { stellar_wind_opacity = params.get_float("STELLAR_WIND_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      stellar_wind_opacity = 1.0f;
      print_default_key_msg("STELLAR_WIND_OPACITY", "1.0");
    }
    if (stellar_wind_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("STELLAR_WIND_OPACITY",
						   ">= 0.0");

    // Stellar wind density exponent - default 5.0, no limits
    try { stellar_wind_exp = params.get_float("STELLAR_WIND_EXPONENT"); }
    catch (Key_list::Key_not_found_exception) {
      stellar_wind_exp = 5.0f;
      print_default_key_msg("STELLAR_WIND_EXPONENT", "5.0");
    }
  }

  /***************************************************************************/

  // Determine parameters of jet
  if (show_jet) {
    // Jet opening angle - no default, must be 0-90
    jet_opening_angle = params.get_float("JET_OPENING_ANGLE");
    if (jet_opening_angle <= 0.0f || jet_opening_angle >= 90.0f)
      throw Key_list::Value_out_of_range_exception("JET_OPENING_ANGLE",
						   "0.0-90.0");

    // Red component of upper jet colour - default 1.0, must be 0-1
    try { jet_red1 = params.get_float("JET_UPPER_RED"); }
    catch (Key_list::Key_not_found_exception) {
      jet_red1 = 1.0f;
      print_default_key_msg("JET_UPPER_RED", "1.0");
    }
    if (jet_red1 < 0.0f || jet_red1 > 1.0f)
      throw Key_list::Value_out_of_range_exception("JET_UPPER_RED", 
						   "0.0-1.0");

    // Green component of upper jet colour - default 1.0, must be 0-1
    try { jet_green1 = params.get_float("JET_UPPER_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      jet_green1 = 1.0f;
      print_default_key_msg("JET_UPPER_GREEN", "1.0");
    }
    if (jet_green1 < 0.0f || jet_green1 > 1.0f)
      throw Key_list::Value_out_of_range_exception("JET_UPPER_GREEN", 
						   "0.0-1.0");

    // Blue component of upper jet colour - default 1.0, must be 0-1
    try { jet_blue1 = params.get_float("JET_UPPER_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      jet_blue1 = 1.0f;
      print_default_key_msg("JET_UPPER_BLUE", "1.0");
    }
    if (jet_blue1 < 0.0f || jet_blue1 > 1.0f)
      throw Key_list::Value_out_of_range_exception("JET_UPPER_BLUE", 
						   "0.0-1.0");

    // Red component of lower jet colour - default 1.0, must be 0-1
    try { jet_red2 = params.get_float("JET_LOWER_RED"); }
    catch (Key_list::Key_not_found_exception) {
      jet_red2 = 1.0f;
      print_default_key_msg("JET_LOWER_RED", "1.0");
    }
    if (jet_red2 < 0.0f || jet_red2 > 1.0f)
      throw Key_list::Value_out_of_range_exception("JET_LOWER_RED", 
						   "0.0-1.0");

    // Green component of lower jet colour - default 1.0, must be 0-1
    try { jet_green2 = params.get_float("JET_LOWER_GREEN"); }
    catch (Key_list::Key_not_found_exception) {
      jet_green2 = 1.0f;
      print_default_key_msg("JET_LOWER_GREEN", "1.0");
    }
    if (jet_green2 < 0.0f || jet_green2 > 1.0f)
      throw Key_list::Value_out_of_range_exception("JET_LOWER_GREEN", 
						   "0.0-1.0");

    // Blue component of lower jet colour - default 1.0, must be 0-1
    try { jet_blue2 = params.get_float("JET_LOWER_BLUE"); }
    catch (Key_list::Key_not_found_exception) {
      jet_blue2 = 1.0f;
      print_default_key_msg("JET_LOWER_BLUE", "1.0");
    }
    if (jet_blue2 < 0.0f || jet_blue2 > 1.0f)
      throw Key_list::Value_out_of_range_exception("JET_LOWER_BLUE", 
						   "0.0-1.0");

    // Jet opacity - default 1.0, must be >= 0.0
    try { jet_opacity = params.get_float("JET_OPACITY"); }
    catch (Key_list::Key_not_found_exception) {
      jet_opacity = 1.0f;
      print_default_key_msg("JET_OPACITY", "1.0");
    }
    if (jet_opacity < 0.0f)
      throw Key_list::Value_out_of_range_exception("JET_OPACITY",
						   ">= 0.0");

    // Jet density exponent - default 5.0, no limits
    try { jet_exp = params.get_float("JET_EXPONENT"); }
    catch (Key_list::Key_not_found_exception) {
      jet_exp = 5.0f;
      print_default_key_msg("JET_EXPONENT", "5.0");
    }

    // Jet inclination - default 0.0, must be 0.0-90.0
    try { jet_inc = params.get_float("JET_INCLINATION"); }
    catch (Key_list::Key_not_found_exception) {
      jet_inc = 0.0f;
      print_default_key_msg("JET_INCLINATION", "0.0");
    }
    if (jet_inc < 0.0f || jet_inc > 90.0f)
      throw Key_list::Value_out_of_range_exception("JET_INCLINATION",
						   "0.0-90.0");

    // Jet rotation - default 0.0, must be 0.0-360.0
    try { jet_phi = params.get_float("JET_ROTATION"); }
    catch (Key_list::Key_not_found_exception) {
      jet_phi = 0.0f;
      print_default_key_msg("JET_ROTATION", "0.0");
    }
    if (jet_phi < 0.0f || jet_phi > 360.0f)
      throw Key_list::Value_out_of_range_exception("JET_ROTATION",
						   "0.0-360.0");


  }
}
