/*
  Class to hold information about a binary visualisation
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

#include <GL/glut.h>

#include "binsim.h"
#include "binsim_version.h"
#include "errmsg.h"
#include "stringutil.h"
#include "vertex_logger.h"

using std::cout;

Vertex_logger *vertex_logger;

/*****************************************************************************/

/*
  Constructor
*/
Bin_sim::Bin_sim(Key_list &params, Image_writer *writer1)
{
  cout << "Extracting rendering options...\n";
    
  // Image width - must be positive
  width = params.get_int("WIDTH"); 
  if (width < 1) 
    throw Key_list::Value_out_of_range_exception("WIDTH", ">= 1");

  // Image height - must be positive
  height = params.get_int("HEIGHT");
  if (height < 1) 
    throw Key_list::Value_out_of_range_exception("HEIGHT", ">= 1");

  // Derive aspect ratio of image
  aspect_ratio = ((float) width) / ((float) height);

  // Image quality - default is low quality
  try { high_quality = params.get_bool("HIGHQUALITY");}   
  catch (Key_list::Key_not_found_exception) {
    high_quality = false;
    print_default_key_msg("HIGHQUALITY", "False");
  }
  
  if (high_quality) {
    try { hq_antialias = params.get_bool("HIGHQUALITY_AA");}   
    catch (Key_list::Key_not_found_exception) {
      hq_antialias = true;
    }
    
    antialias = hq_antialias;
  } else {
    antialias = hq_antialias = false;
  }
  
  if (antialias) {
    n_samples = params.get_int("SAMPLES");
    if (n_samples != 2 && n_samples != 4)
      throw Key_list::Value_out_of_range_exception("SAMPLES", 
						   "2 (2x2) or 4 (4x4)");
    n_samples *= n_samples;
  }

  // Animation switch
  try { anim = params.get_bool("ANIM"); }
  catch (Key_list::Key_not_found_exception) {
    anim = false;
    print_default_key_msg("ANIM", "False");
  }

  // Binary phase - keywords depend on whether animation is enabled
  if (anim) {
    // Initialise phase counter - use -1 as it will be incremented
    // before displaying the first frame
    phase_index = -1;

    // Starting phase - no default, no limits
    float low_phase = params.get_float("LOW_PHASE");

    // Final phase - no default, must be > LOW_PHASE
    float high_phase = params.get_float("HIGH_PHASE");
    if (high_phase <= low_phase)
      throw Key_list::Value_out_of_range_exception("HIGH_PHASE", 
						   "> LOW_PHASE");
          
    // Increment in phase - no default, must be > 0.0
    float d_phase = params.get_float("DELTA_PHASE");
    if (d_phase <= 0)
      throw Key_list::Value_out_of_range_exception("DELTA_PHASE", 
						   "> 0.0");
       
    // Determine number of phases to display
    n_phase = static_cast<int> 
      ((high_phase - low_phase) / d_phase + 0.5f) + 1;

    // Create phase array
    for (int i = 0 ; i < n_phase ; i++) 
      phase.push_back(low_phase + i * d_phase);
  } else {
    phase.push_back(params.get_float("PHASE"));
    phase_index = 0;
  }

  // Get image scale - must be positive
  try { scale = 1.0f / params.get_float("SCALE"); }
  catch (Key_list::Key_not_found_exception) {
    scale = 1.0f;
    print_default_key_msg("SCALE", "1.0");
  }
  if (scale <= 0.0f)
    throw Key_list::Value_out_of_range_exception("SCALE", "> 0.0");
   
  // Derive size of viewport
  world_width = scale * sqrt(aspect_ratio);
  world_height = scale / sqrt(aspect_ratio);    
  world_pixsize = world_width / width;
  
  // Get x centre of viewport
  try { world_centre_x = -params.get_float("XOFFSET"); }   
  catch (Key_list::Key_not_found_exception) {
    world_centre_x = 0.0f;
    print_default_key_msg("XOFFSET", "0.0");
  } 
  
  // Get y centre of viewport
  try { world_centre_y = -params.get_float("YOFFSET"); }   
  catch (Key_list::Key_not_found_exception) {
    world_centre_y = 0.0f;
    print_default_key_msg("YOFFSET", "0.0");
  } 

  // Derive boundaries of viewport
  world_min_x = world_centre_x - world_width / 2.0f;
  world_max_x = world_centre_x + world_width / 2.0f;
  world_min_y = world_centre_y - world_height / 2.0f;
  world_max_y = world_centre_y + world_height / 2.0f;
  
  // Should an image be saved?
  try { save = params.get_bool("SAVE"); }
  catch (Key_list::Key_not_found_exception) {
    save = false;
    print_default_key_msg("SAVE", "False");
  }
  
  if (save) {
    // Get output filename
    try { imagefile = params.get_value("IMAGE_FILE"); }
    catch (Key_list::Key_not_found_exception) {
#ifndef NOJPEG
      imagefile = "binsim.jpg";
      print_default_key_msg("IMAGE_FILE", "binsim.jpg");
#else
      imagefile = "binsim.ppm";
      print_default_key_msg("IMAGE_FILE", "binsim.ppm");
#endif
    }

    // Extract 3 and 4 character extensions
    string ext3 = imagefile.substr(imagefile.length()-3,3);
    string ext4 = imagefile.substr(imagefile.length()-4,4);

    // Identify extensions
#ifndef NOJPEG
    if (ext3 == "jpg" || ext3 == "JPG" || ext4 == "jpeg" || ext4 == "JPEG")
      output_format = Image_writer::JPEG;
    else 
#endif
    if (ext3 == "ppm" || ext3 == "PPM")
      output_format = Image_writer::PPM;
    else {
#ifndef NOJPEG
      cout << "Only JPEG and PPM support is available\n";
#else
      cout << "Only PPM support is available\n";
#endif
      throw Key_list::File_format_exception(imagefile);
    }

#ifndef NOJPEG
    // Get output JPEG quality
    if (output_format == Image_writer::JPEG) {
      try { jpeg_quality = params.get_int("JPEG_QUALITY"); }
      catch (Key_list::Key_not_found_exception) {
	jpeg_quality = 100;
	print_default_key_msg("JPEG_QUALITY", "100");
      }
      if (jpeg_quality < 0 || jpeg_quality > 100) 
	throw Key_list::Value_out_of_range_exception("JPEG_QUALITY", "0-100");
    }
#endif    

    if (anim) {
      // Get directory for animation images
      try { anim_root = params.get_value("ANIM_ROOT"); }
      catch (Key_list::Key_not_found_exception) {
	anim_root = "";
	print_default_key_msg("ANIM_ROOT", "");
      }

      // Ensure anim_root has the correct trailing slash
#ifdef WIN32
      char delimiter = '\\';
#else
      char delimiter = '/';
#endif

      if (anim_root.length() == 0) {
	anim_root = ".";
	anim_root += delimiter;
      } else {
	if (anim_root[anim_root.length()-1] == '\\' ||
	    anim_root[anim_root.length()-1] == '/')
	  anim_root[anim_root.length()-1] = delimiter;
	else anim_root += delimiter;
      }

      // Create movie maker object
      animator = new Movie_maker(params);
    }
  }

  // Should a vertex log file be created?
  try { vertex_log = params.get_bool("VERTEX_LOG"); }
  catch (Key_list::Key_not_found_exception) {
    vertex_log = false;
  }
  if (vertex_log) vertex_logger = new Vertex_logger();
  else vertex_logger = 0;
  
  // Determine if background stars should be seen
  try { show_stars = params.get_bool("SHOW_STARS"); }
  catch (Key_list::Key_not_found_exception) {
    show_stars = false;
    print_default_key_msg("SHOW_STARS", "False");
  }

  // Create starfield object
  if (show_stars)
    sky = new Star_sky(world_min_x, world_max_x, world_min_y, world_max_y, 
		       params);

  // Create binary object
  binary = new Binary_3d(params, phase);

  // Save pointer to image writer
  writer = writer1;
}

/*****************************************************************************/

/*
  Perform option dependent OpenGL initialisation
*/
void Bin_sim::gl_setup(const bool onscreen)
{
  // Set display mode
  if (onscreen) {
    if (antialias) 
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB | GLUT_ACCUM);
    else 
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  } else {
    if (antialias) 
      glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_ACCUM);
    else 
      glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
  }  

  // Create display window
  if (onscreen) {
    glutInitWindowSize(width, height); 
    glutInitWindowPosition (0, 0);
    glutCreateWindow(Bin_sim_version::full_name.c_str());
  }
  
  // Clear buffers
  glClearColor(0.0,0.0,0.0,0.0);
  if (antialias) glClearAccum(0.0,0.0,0.0,0.0);

  // Set rendering options
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDisable(GL_DITHER);

#ifdef WIREFRAME
  glPolygonMode(GL_FRONT, GL_LINE);
  glLineWidth(2.0f);
#endif
 }

/*****************************************************************************/

/*
  Commands to be executed whenever objects are drawn.  Convenience
  function to avoid duplication in draw function.
*/
void Bin_sim::gl_commands(void)
{
  // Draw starry background
  if (show_stars) sky->draw();

  // Draw binary
  binary->draw(phase_index);
}

/*
  Main function to draw image
*/
void Bin_sim::draw(const bool onscreen)
{
  if (anim)
    cout << "Frame " << phase_index << "\n";

  using String_util::int_to_string;

  // Ensure phase_index >= 0
  if (phase_index < 0) phase_index = 0;
  
    int bits;
  glGetIntegerv(GL_ACCUM_BLUE_BITS, & bits);
  cout << bits << '\n';
  
  // Draw with antialiasing enabled
  if (antialias) {
    glClear(GL_ACCUM_BUFFER_BIT);

    // Define jittering stepsize for antialiasing
    float dx = world_pixsize * 0.25;
    float dy = world_pixsize * 0.25;
    
    // Perform 4x4 antialiasing
    if (n_samples == 4) {
      // Define arrays of offsets
      dx = 0.75f * dx;
      dy = 0.75f * dy;
      float x_shift[16] = {-3*dx, -dx, dx, 3*dx, 
			   -3*dx, -dx, dx, 3*dx,
			   -3*dx, -dx, dx, 3*dx,
			   -3*dx, -dx, dx, 3*dx};
      float y_shift[16] = {-3*dy, -3*dy, -3*dy, -3*dy,
			   -dy, -dy, -dy, -dy, 
			   dy, dy, dy, dy,
			   3*dy, 3*dy, 3*dy, 3*dy};

      // Define weighting policy
      float weight[16]  = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
      float weight_total = 16.0;

      // Render each offset positino into the accumulation buffer
      for (int i = 0 ; i < 16 ; i++) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	glOrtho(world_min_x + x_shift[i], world_max_x + x_shift[i], 
	        world_min_y + y_shift[i], world_max_y + y_shift[i], 
		-10.0f, 10.0f);
	gl_commands();
	glAccum(GL_ACCUM, weight[i]/ weight_total);
      }
    }
    // Perform 2x2 antialiasing
    else {
      // Define arrays of offsets
      float x_shift[4] = {-dx, dx, -dx, dx};
      float y_shift[4] = {-dy, -dy, dy, dy};

      // Define weighting policy
      float weight[4]  = {1,1,1,1};
      float weight_total = 4.0;

      // Render each offset position into the accumulation buffer
      for (int i = 0 ; i < 4 ; i++) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	glOrtho(world_min_x + x_shift[i], world_max_x + x_shift[i], 
		world_min_y + y_shift[i], world_max_y + y_shift[i], 
		-10.0f, 10.0f);
	gl_commands();
	glAccum(GL_ACCUM, weight[i]/ weight_total);
      }
    }

    // Transfer image from accumulation buffer
    glAccum(GL_RETURN, 1.0);
  }
  // Draw with antialiasing disabled
  else {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(world_min_x, world_max_x, world_min_y, world_max_y, -10.0f, 10.0f);
    gl_commands();
  }
  
  // Transfer current image to front buffer
  if (onscreen) glutSwapBuffers();

  // Save current frame if desired.  Only save once on first draw.
  if (save) {
    if (anim) {
      // Construct image filenames
      string stripped_filename = "binsim_tmp."  + 
        int_to_string(phase_index, true, 4) + ".ppm";
      imagefile = anim_root + stripped_filename;
      // Write animation frame
      writer->write_ppm(imagefile);
      //writer->write_jpeg(imagefile, jpeg_quality);
      animator->add_image_to_mpeg(stripped_filename);

      // When last frame is written make movie and stop saving
      if (last_frame()) {
	animator->make_mpeg();
	save = false;
      }
    } else {
      // Write still image
      if (output_format == Image_writer::PPM) {
	writer->write_ppm(imagefile);
#ifndef NOJPEG
      } else if (output_format == Image_writer::JPEG) {
	writer->write_jpeg(imagefile, jpeg_quality);
#endif
      }
      save = false;
    }
  }
}

