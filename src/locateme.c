/**
 * locateme – really simple universal continuous location tracking
 * 
 * Copyright © 2014  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "conffile.h"


/**
 * This is the main entry point of the program
 * 
 * @param   argc  The number of elements in `argv`
 * @param   argv  Command line arguments
 * @return        Zero on success
 */
int main(int argc, char** argv)
{
  int continuous = 0;
  int quickly = 0;
  int i;
  FILE* f;
  conffile_t* data;
  conffile_t* data_;
  
  for (i = 0; i < argc; i++)
    {
      char* arg = *(argv + i);
      
      #define __(opt)  (!strcmp(arg, opt))
      
      if (__("-c") || __("-cq") || __("-qc") || __("--continuous"))
	continuous = 1;
      if (__("-q") || __("-cq") || __("-qc") || __("--quickly"))
	quickly = 1;
      
      #undef __
    }
  
  f = get_conffile(NULL);
  if (f != NULL)
    {
      data_ = data = read_conffile(f);
      fclose(f);
    }
  else
    {
      /* If there are not configurion use use: */
      
      data_ = data = malloc(3 * sizeof(conffile_t));
      
      #define __use(INDEX, METHOD)							\
	({										\
	  data[INDEX].argc = 1;								\
	  data[INDEX].argv = malloc(sizeof(char*));					\
	  data[INDEX].argv[0] = malloc(strlen(METHOD ".") * sizeof(char));		\
	  memcpy(data[INDEX].argv[0], METHOD, strlen(METHOD ".") * sizeof(char));	\
	})
      
      __use(0, "cache");
      __use(1, "timezone-offset");
      
      #undef __use
      
      data[2].argc = -1;
      data[2].argv = NULL;
    }
  for (; data->argc > 0; data++)
    {
      printf("%s + %i\n", data->argv[0], data->argc - 1);
      free(data->argv[0]);
      free(data->argv);
    }
  free(data_);
  
  return 0;
}

