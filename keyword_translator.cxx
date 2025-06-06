/*
  Function to translate keywords.  Takes care of deprecated keywords,
  variations, internationalisation and personal preferences.

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

#include <string>

#include "keyword_translator.h"

using std::string;

/*****************************************************************************/

/*
  Function to translate (remap) keywords in a keylist from one string
  to another.  Separated from keyword.h and keyword.cxx to facilitate
  easy switching of translation files, e.g. to allow
  internationalisation. 
*/
void apply_keyword_translation(Key_list *list)
{
  // Apply a series of translations via a sequence of
  // translate_keyword calls.  The order is important.  Later
  // translations will override earlier ones.

  ////////////////////////////////////////////////////////////////////////////
  // Translations for internationalised version of BinSim - translate
  // non-English keywords into internal English forms.  

  /////////////////////////////////////////////////////////////////////////////
  // Translations for deprecated old keywords - do not modify this section

  // Translations added in v0.9
  list->translate_keyword("SHOW_LOBE", "SHOW_LOBE2");
  list->translate_keyword("LOBE_NSTEPS", "LOBE2_NSTEPS");
  list->translate_keyword("LOBE_FILL", "LOBE2_FILL");
  list->translate_keyword("LOBE_T_POLE", "LOBE2_T_POLE");
  list->translate_keyword("LOBE_T_MIN", "LOBE2_T_MIN");
  list->translate_keyword("LOBE_GRANULATION", "LOBE2_GRANULATION");
  list->translate_keyword("LOBE_GRANULATION_PERIOD", "LOBE2_GRANULATION_PERIOD");
  list->translate_keyword("LUMINOSITY", "LUMINOSITY1");

  /////////////////////////////////////////////////////////////////////////////
  // Allow for some alternate variations of keywords - add to this if
  // you wish
  list->translate_keyword("MASS_RATIO", "Q");
  list->translate_keyword("LX", "LUMINOSITY1");

  ////////////////////////////////////////////////////////////////////////////
  // Translations for personalisation - any other changes you wish to
  // make, will override all previous settings
}









