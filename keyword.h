/*
  Class to hold keyword=value pairs and process them into their
  components.  Comments should be specified with a #
  
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

#ifndef _KEYWORD_H
#define _KEYWORD_H

#include <string>

#include "binsim_stdinc.h"

using std::string;

/*****************************************************************************/

/*
  Class defining linked list item to hold a keyword and its associated
  value
*/
class Key_item {
    // Pointer to the next item
    Key_item *next_item;

public:
  // The maximum size of a line is assumed to be RECORDSIZE
  // RECORDSIZE is now defined by a local macro in keyword.cxx as
  // Visual C++ does not recognise static const int members
  //static const int RECORDSIZE = 1024;

  // Character arrays for the keyword and the value
  char *keyword;
  char *keyvalue;
  
  // Constructor and destructor functions
  Key_item();
  ~Key_item();
  
  // Function to free keyword and value arrays
  void free_arrays();
  
  // Function for receiving a line of text and cutting it into keyword
  // and value
  int new_key(char *text);
  
  // Function to link the item to another
  void link(Key_item *target);
  
  // Function to return a pointer to the next item in the list
  Key_item* goto_next();
};

/*****************************************************************************/

/*
  Class defining linked list to hold a set of key items
*/
class Key_list {
  // Pointers to the first and last items in the list
  Key_item *first_item, *last_item;
  
  // Function to add an item to the list
  void add_item(Key_item *new_item);

public:
  // Constructor and destructor functions
  Key_list();
  Key_list(string filename);
    
  ~Key_list();
    
  // Function to add an item to the list
  void add_item(string key, string value);

  // Function to translate a keyword
  void translate_keyword(string old_key, string new_key);
    
  // Functions to return the value associated with a specified keyword
  string get_value(string key);
  int get_int(string key);
  float get_float(string key);
  bool get_bool(string key);
  
  // Function to read in a list from a file
  void read_file(string filename);

  // Function to reinitialise to an empty list
  void erase();

  // Exceptions
  class Key_not_found_exception: public string { 
  public: 
    Key_not_found_exception(string s) : string(s) {} 
  };
  
  class File_format_exception: public string { 
  public:
    File_format_exception(string s) : string(s) {} 
  };
  
  class File_access_exception: public string { 
  public:
    File_access_exception(string s) : string(s) {} 
  };

  // New style, more versatile exception
  struct Value_out_of_range_exception {
    string keyword, value;
    
    Value_out_of_range_exception(const string &k, const string &v)
      : keyword(k), value(v) {}
  };    
};

/*****************************************************************************/

#endif








