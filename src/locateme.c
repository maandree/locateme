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

#include "common.h"
#include "fallback.h"
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
  
  if (guess_by_cache())
    if (guess_by_timezone_offset())
      return 1;
  
  FILE* f = get_conffile(NULL);
  conffile_t* data = read_conffile(f);
  conffile_t* data_ = data;
  fclose(f);
  
  for (; data->argc > 0; data++)
    {
      printf("%s + %i\n", data->argv[0], data->argc - 1);
      free(data->argv[0]);
      free(data->argv);
    }
  
  free(data_);
  
  return 0;
}

