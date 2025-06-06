/*
  Class to assemble a group of images into an MPEG movie.
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

#include "errmsg.h"
#include "movie_maker.h"

using std::cout;

/*****************************************************************************/

/*
  Constructor
*/
Movie_maker::Movie_maker(Key_list &params)
{
  // Get output directory silently - Bin_sim will provide output
  try { root_dir = params.get_value("ANIM_ROOT"); }
  catch (Key_list::Key_not_found_exception) {
    root_dir = "";
  }

  // Ensure root_dir has the correct trailing slash
#ifdef WIN32
  char delimiter = '\\';
#else
  char delimiter = '/';
#endif

  if (root_dir.length() == 0) {
    root_dir = ".";
    root_dir += delimiter;
  } else {
    if (root_dir[root_dir.length()-1] == '\\' ||
	root_dir[root_dir.length()-1] == '/')
      root_dir[root_dir.length()-1] = delimiter;
    else root_dir += delimiter;
  }

  // Get filename
  try { mpeg_filename = params.get_value("MPEG_FILE"); }
  catch (Key_list::Key_not_found_exception) {
    mpeg_filename = "binsim.mpg";
    print_default_key_msg("MPEG_FILE", "binsim.mpg");
  }

  // Get pattern
  try { mpeg_pattern = params.get_value("MPEG_PATTERN"); }
  catch (Key_list::Key_not_found_exception) {
    mpeg_pattern = "I";
    print_default_key_msg("MPEG_PATTERN", "I");
  }

  // Get IQscale
  try { mpeg_iqscale = params.get_value("MPEG_IQSCALE"); }
  catch (Key_list::Key_not_found_exception) {
    mpeg_iqscale = "1";
    print_default_key_msg("MPEG_IQSCALE", "1");
  }

  // Get PQscale
  try { mpeg_pqscale = params.get_value("MPEG_PQSCALE"); }
  catch (Key_list::Key_not_found_exception) {
    mpeg_pqscale = "1";
    print_default_key_msg("MPEG_PQSCALE", "1");
  }

  // Get BQscale
  try { mpeg_bqscale = params.get_value("MPEG_BQSCALE"); }
  catch (Key_list::Key_not_found_exception) {
    mpeg_bqscale = "1";
    print_default_key_msg("MPEG_BQSCALE", "1");
  }

  // Open MPEG parameter file
  param_file = new ofstream("binsim_mpeg.param");

  // Write MPEG paramet file header
  write_mpeg_header();
}

/*
  Add an image to the parameter file
*/
void Movie_maker::add_image_to_mpeg(const string filename)
{
  // Add image filename to the MPEG parameter file
  (*param_file) << filename << "\n";
}

/*
  Create the MPEG from the parameter file
*/
void Movie_maker::make_mpeg()
{
  // End the MPEG parameter file
  write_mpeg_trailer();
  delete param_file;

  // Generate the MPEG movie
#ifdef WIN32
  cout << "MPEG creation disabled on Windows systems\n";
#else
  cout << "MPEG creation disabled on temporarily\n";
  //system("mpeg_encode binsim_mpeg.param");
#endif
}

/*****************************************************************************/

/*
  Write the beginning of the MPEG parameter file
*/
void Movie_maker::write_mpeg_header()
{
  (*param_file) << "OUTPUT " << mpeg_filename << "\n";
  (*param_file) << "IQSCALE " << mpeg_iqscale << "\n";
  (*param_file) << "PQSCALE " << mpeg_pqscale << "\n";
  (*param_file) << "BQSCALE " << mpeg_bqscale << "\n";
  (*param_file) << "PSEARCH_ALG LOGARITHMIC\n";
  (*param_file) << "BSEARCH_ALG SIMPLE\n";
  (*param_file) << "GOP_SIZE 30\n";
  (*param_file) << "SLICES_PER_FRAME 1\n";
  (*param_file) << "PIXEL FULL\n";
  (*param_file) << "RANGE 10\n";
  (*param_file) << "PATTERN " << mpeg_pattern << "\n";
  (*param_file) << "FORCE_ENCODE_LAST_FRAME\n";
  (*param_file) << "REFERENCE_FRAME ORIGINAL\n";
  (*param_file) << "BASE_FILE_FORMAT PPM\n";
  (*param_file) << "INPUT_CONVERT *\n";
  (*param_file) << "INPUT_DIR " << root_dir << "\n";
  (*param_file) << "INPUT\n";
}

/*
  Write the end of the MPEG parameter file
*/
void Movie_maker::write_mpeg_trailer()
{
  (*param_file) << "END_INPUT\n";
}
