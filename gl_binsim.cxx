/*
  Main program for OpenGL based binary visualisation

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

#include <iostream>
#include <string>

#include <GL/glut.h>

#include "binsim.h"
#include "binsim_version.h"
#include "errmsg.h"
#include "image_writer.h"
#include "keyword.h"
#include "keyword_translator.h"

#include "binsim_stdinc.h"

using std::cout;

/*****************************************************************************/

// Object encapsulating information about the model
Bin_sim *bin_sim;

// OpenGL friendly wrapper for draw function of bin_sim
void draw(void) { bin_sim->draw(true); }

// Handle keyboard events
void key(unsigned char k, int x, int y)
{
  // Exit normally
  if (k == 27) exit(0);
}

// OpenGL friendly wrapper for next frame function of bin_sim
void next_frame()
{
  if (!bin_sim->last_frame()) {
    bin_sim->next_frame();
    glutPostRedisplay();
  }
}

// Print an error message and quit
void terminate(string msg)
{
  cout << msg << "\n\nTerminating program!\n";
  exit(1);
}

// Print an Keyword default message and continue
void print_default_key_msg(const string key, const string def)
{
  cout << "   Key not found: " << key << " - Assuming " << def << "\n";
}

/*****************************************************************************/

int main(int argc, char** argv)
{
  // Parameter file
  string filename;

  // Animation flag
  bool anim;

  // Basic OpenGL initialisation
  glutInit(&argc, argv);

  // Get input file names
  if (argc == 2)
    filename = argv[1];
  else {
    //cout << "Usage: binsim paramfile\n";
    //exit(1);

	filename = "sample.par";	
  }

  // Welcome message
  cout << Bin_sim_version::full_name << "\n";
  cout << Bin_sim_version::underline << "\n\n";

  try {
    // Read parameter file
    cout << "Parsing parameter file...\n";
    Key_list params(filename);

    // Translate parameter file
    apply_keyword_translation(&params);

    // Get animation switch silently - default message will be
    // triggered by Bin_sim
    try { anim = params.get_bool("ANIM"); }
    catch (Key_list::Key_not_found_exception) {
      anim = false;
    }

    // Image width - must be positive
    int width = params.get_int("WIDTH"); 
    if (width < 1) 
      throw Key_list::Value_out_of_range_exception("WIDTH", ">= 1");

    // Image height - must be positive
    int height = params.get_int("HEIGHT");
    if (height < 1) 
      throw Key_list::Value_out_of_range_exception("HEIGHT", ">= 1");

    // Create image writer
    FB_image_writer writer(width, height);

    // Create renderer
    bin_sim = new Bin_sim(params, &writer);
  } 
  catch (Key_list::File_access_exception e) {
    terminate("File access error: " + e);
  } 
  catch (Key_list::File_format_exception e) {
    terminate("File format error: " + e);
  } 
  catch (Key_list::Key_not_found_exception e) {
    terminate("   Key not found: " + e + " - No default value!");
  } 
  catch (Key_list::Value_out_of_range_exception e) {
    terminate("   Value out of range: " + e.keyword + 
	      " - Must be " + e.value);
  }

  // Main OpenGL initialisation
  cout << "Initialising renderer...\n";
  bin_sim->gl_setup(true);
  
  // Define GLUT functions
  glutDisplayFunc(draw);
  glutKeyboardFunc(key);
  if (anim) glutIdleFunc(next_frame);

  // Begin rendering
  cout << "Rendering...\n";
  glutMainLoop();

  // End normally
  return 0; 
}
                                                    
