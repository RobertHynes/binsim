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

#include <cctype>
#include <cstdio>
#include <cstring>

#include <fstream>
#include <string>

#include "keyword.h"
#include "stringutil.h"

using std::ifstream;

// The maximum size of a line is assumed to be RECORDSIZE
// RECORDSIZE defined like this because Visual C++ does not allow
// static const int members of a class
#define RECORDSIZE 1024

/*****************************************************************************/

/* 
   Constructor function.  Initialises pointers for an empty item 
*/
Key_item::Key_item()
{
  keyword = keyvalue = 0;
  next_item = 0;
}

/* 
   Destructor function.  Releases memory occupied by keyword and value
*/
Key_item::~Key_item()
{
  free_arrays();
}

/*
  Function to free keyword and value arrays
*/
void Key_item::free_arrays()
{
  delete keyword;
  delete keyvalue;
}

/*
  Store the line of text and process it to obtain the keyword and value
*/
int Key_item::new_key(char *text)
{
  // Buffer to hold the text that is being cut up
  char buffer[RECORDSIZE+2];

  // Pointers to the beginning and end of the line
  char *line_start, *line_end;

  // Pointers to the beginning and end of the keyword and the
  // beginning and end of the value
  char *key_start, *key_end, *val_start, *val_end;

  // Pointer to the delimiter, =, and comment
  char *delimit, *comment;

  // General pointer
  char *p;

  // Variables for the length of the text, the keyword and the value
  int text_len, key_len, val_len;

  // Reinitialise the keyword and value
  free_arrays();

  // Copy the received text to the buffer, making sure that it is null
  // terminated
  strncpy(buffer, text, RECORDSIZE+1);
  buffer[RECORDSIZE+1] = '\0';

  // Remove any comment that may be present, preceded by a #
  comment = strchr(buffer, '#');
  if (comment) *comment = '\0';

  // Remove whitespace from the beginning and end of the line
  line_start = buffer;
  line_end = buffer + strlen(buffer);
  while (isspace(*line_start) && line_start < line_end)   line_start++;
  while (isspace(*line_end)   && line_end   > line_start) line_end--;
  if (!isspace(*line_end)) line_end++;
  *line_end = '\0';

  // Find the length of the text line
  text_len = strlen(line_start);

  // Locate the delimiter, and exit if it is not present
  delimit = strchr(buffer, '=');
  
  if (delimit) {
    // Locate the keyword section
    key_start = line_start;
    key_end = delimit-1;

    // Remove whitespace from the end of the keyword
    while (isspace(*key_end)   && key_end   > key_start) key_end--;
    if (!isspace(*key_end)) key_end++;

    // Convert the keyword to upper case
    p = key_start;
    while (p <= key_end) {
      *p = toupper(*p);
      p++;
    }
    
    // Determine the length of the keyword and store it in the object
    key_len = key_end - key_start;
    keyword = new char[key_len+1];
    strncpy (keyword, key_start, key_len);
    keyword[key_len] = '\0';

    // Locate the value section
    val_start = delimit + 1;		
    val_end = line_end;

    // Remove whitespace from the start of the value
    while (isspace(*val_start) && val_start < val_end)   val_start++;

    // Determine the length of the value and store it in the object
    val_len = val_end - val_start;
    keyvalue = new char[val_len+1];
    strncpy(keyvalue, val_start, val_len);
    keyvalue[val_len] = '\0';
  } 
  else if (text_len > 0) 
    // Record is not in Keyword = Value format
    throw Key_list::File_format_exception(text);
  else 
    return 1;
  
  return 0;
}

/*
  Function to link to the target item
*/
void Key_item::link(Key_item* target)
{
  next_item = target;
}

/*
  Return a pointer to the next item in the list
*/
Key_item* Key_item::goto_next()
{
  return next_item;
}

/*****************************************************************************/

/*
  Constructor function.  Initialises pointers for an empty list
*/
Key_list::Key_list()
{
  first_item = last_item = 0;
}

/*
  Constructor function.  Read in list from a file
*/
Key_list::Key_list(string filename)
{
  first_item = last_item = 0;
  read_file(filename);
}

/*
  Destructor function.  Releases memory occupied by all Key_items
*/
Key_list::~Key_list()
{
  erase();
}

/*
  Function to add an item to the end of the list
*/
void Key_list::add_item(Key_item *new_item)
{
  // Link the new item into the list
  if (first_item) {
    last_item->link(new_item);
    last_item = new_item;
  }
  else 
    first_item = last_item = new_item;

  // Set the pointer of the new item to 0.
  new_item->link(0);
}

/*
  Function to add an item to the end of the list
*/
void Key_list::add_item(string key, string value)
{
  Key_item *new_item = new Key_item;

  new_item->keyword = new char[key.length() + 1]; 
  strcpy(new_item->keyword, key.c_str());

  new_item->keyvalue = new char[value.length() + 1]; 
  strcpy(new_item->keyvalue, value.c_str());

  add_item(new_item);
}

/*
  Function to translate a keyword
*/
void Key_list::translate_keyword(string old_key, string new_key)
{
  // Pointer to current item in search
  Key_item *this_item = first_item;

  // Check each item in turn to see if it matches the required
  // keyword. All items in the list will be checked and translated if matched
  while (this_item) {
    if (! strcmp(old_key.c_str(), this_item->keyword)) {
      delete this_item->keyword;
      this_item->keyword = new char[strlen(new_key.c_str())+1];
      strcpy(this_item->keyword, new_key.c_str());
    }

    // Move to next item in the list
    this_item = this_item->goto_next();
  }
}

/*
  Function to return the value associated with a specified keyword.
  Throws an exception if keyword is undefined
*/
string Key_list::get_value(string key)
{
  // Pointer to current item in search
  Key_item *this_item = first_item;

  // Pointer to value to be returned.
  char *keyvalue1 = 0;

  // Check each item in turn to see if it matches the required
  // keyword. All items in the list will be checked and the last value
  // in the list returned
  while (this_item) {
    if (! strcmp(key.c_str(), this_item->keyword)) 
      keyvalue1 = this_item->keyvalue;

    // Move to next item in the list
    this_item = this_item->goto_next();
  }

  if (keyvalue1) 
    return *(new string(keyvalue1));
  else 
    throw Key_not_found_exception(key);
}

/*
  Function to return the value associated with a specified keyword as
  an integer.
*/
int Key_list::get_int(string key)
{
  // Integer value
  int value;

  // Dummy character to test for trailing junk
  char tail;

  // Parse string
  int code = sscanf(get_value(key).c_str(), "%d%c", &value, &tail);

  if (code == 1) return value;
  else throw File_format_exception(key + " = " + get_value(key));
}

/*
  Function to return the value associated with a specified keyword as
  a float.
*/
float Key_list::get_float(string key)
{
  // Float value
  float value;

  // Dummy character to test for trailing junk
  char tail;

  // Parse string
  int code = sscanf(get_value(key).c_str(), "%f%c", &value, &tail);

  if (code == 1) return value;
  else throw File_format_exception(key + " = " + get_value(key));
}

/*
  Function to return a boolean value associated with a specified
  keyword.
*/
bool Key_list::get_bool(string key)
{
  using String_util::string_toupper;
  using String_util::strip_whitespace;

  // Get value as trimmed upper case string
  string value = get_value(key);
  string_toupper(value);
  strip_whitespace(value);

  // Determine boolean type or throw exception
  if (value == "TRUE" || value == "ON" || value == "YES") 
    return true;
  else if (value == "FALSE" || value == "OFF" || value == "NO")
    return false;
  else throw File_format_exception(key + " = " + value);
}

/*
  Function to read a list of keywords from a file
*/
void Key_list::read_file(string filename)
{
  // Create input stream opened from filename
  ifstream input(filename.c_str());
  if (!input) throw File_access_exception(filename);
  
  // Create buffer for storing lines as they are read
  char buffer[RECORDSIZE+2];

  // Pointer to new item
  Key_item *new_item;

  // Read lines from file one by one
  while (!input.eof () && input.good ()) {
    // Read next line from input stream
    input.getline(buffer, RECORDSIZE+1);
    
    // Convert string to Key_item and add to list if valid
    new_item = new Key_item;
    if (!new_item->new_key(buffer)) 
      add_item(new_item);
    else
      delete new_item;
  }
}

/*
  Function to delete all Key_items and reinitialise the list
*/
void Key_list::erase()
{
  // Pointers to Key_items
  Key_item *p = first_item;
  Key_item *q;

  // Delete each Key_item in turn
  while (p) {
    q = p->goto_next();
    delete p;
    p = q;
  }

  first_item = last_item = 0;
} 
