/*
  Main program for off-screen OpenGL based binary visualisation
  Released as part of BinSim 0.9

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
#include <GL/osmesa.h>

#include "binsim.h"
#include "errmsg.h"
#include "keyword.h"
#include "keyword_translator.h"

#include "binsim_stdinc.h"

using std::cout;

/*****************************************************************************/

// Print an error message and quit
void terminate(string msg)
{
  cout << msg << "\nTerminating program\n";
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
  // Object encapsulating information about the model
  Bin_sim *bin_sim;

  // Parameter file
  string filename;

  // Offscreen dimensions
  int width, height;

  // RGBA-mode context
  OSMesaContext ctx;

  // Image buffer
  void *buffer; 

  // Image writer
  Image_writer *writer;

  // Animation flag
  bool anim;

  // Basic OpenGL initialisation
  glutInit(&argc, argv);

  // Get input file names
  if (argc == 2)
    filename = argv[1];
  else {
    cout << "Usage: osbinsim paramfile\n";
    exit(1);
  }

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
    width = params.get_int("WIDTH"); 
    if (width < 1) 
      throw Key_list::Value_out_of_range_exception("WIDTH", ">= 1");

    // Image height - must be positive
    height = params.get_int("HEIGHT");
    if (height < 1) 
      throw Key_list::Value_out_of_range_exception("HEIGHT", ">= 1");

    // OSMesa16 interface
#ifdef BIGBUFFER
    // Allocate the image buffer
    buffer = malloc(width * height * 4 * sizeof(GLushort));
    
    // Create image writer
    writer = new OS16_image_writer(width, height, 
				   static_cast<GLushort *> (buffer));
#else
    // Allocate the image buffer
    buffer = malloc(width * height * 4 * sizeof(GLubyte));
    
    // Create image writer
    writer = new OS_image_writer(width, height, 
				 static_cast<GLubyte *> (buffer));
#endif

   // Create renderer
    bin_sim = new Bin_sim(params, writer);
  } 
  catch (Key_list::File_access_exception e) {
    terminate("File access error: " + e);
  } 
  catch (Key_list::File_format_exception e) {
    terminate("File format error: " + e);
  } 
  catch (Key_list::Key_not_found_exception e) {
    terminate("Key not found: " + e + " - No default value!");
  } 
  catch (Key_list::Value_out_of_range_exception e) {
    terminate("   Value out of range: " + e.keyword + 
              " - Must be " + e.value);
  }

  // Main OpenGL initialisation
  cout << "Initialising renderer...\n";

  // OSMesa16 interface
#ifdef BIGBUFFER
  // Create an RGBA-mode context
  ctx = OSMesaCreateContextExt(GL_RGBA, 16, 0, 16, NULL);
  
  // Bind the buffer to the context and make it current
  OSMesaMakeCurrent(ctx, buffer, GL_UNSIGNED_SHORT, width, height);
#else    
  // Create an RGBA-mode context
  ctx = OSMesaCreateContext(GL_RGBA, NULL);
  
  // Bind the buffer to the context and make it current
  OSMesaMakeCurrent(ctx, buffer, GL_UNSIGNED_BYTE, width, height);
#endif

  bin_sim->gl_setup(false);
  
  // Begin rendering
  cout << "Rendering...\n";
  if (anim) { 
    while (!bin_sim->last_frame()) {
      bin_sim->next_frame();
      bin_sim->draw(false);
    } 
  } else bin_sim->draw(false);
  
  // Free the image buffer
  free(buffer);
  
  // Destroy the context
  OSMesaDestroyContext(ctx);
  
  // End normally
  return 0; 
}
                                                    
