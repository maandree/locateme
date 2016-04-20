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
#include <unistd.h>
#include <pwd.h>

#include "coordinator.h"
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
  char* env;
  struct passwd* pw;
  
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
  
  if ((env = getenv("HOME")) && *env && !chdir(env));
  else if ((pw = getpwuid(getuid())) && pw->pw_dir)
    if (chdir(pw->pw_dir))
      fprintf(stderr, "%s: cannot change directory to your home directory", *argv);
  
retry:
  init_coordination(continuous, quickly);
  
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
      
      #define __use1(INDEX, METHOD)							\
	({										\
	  data[INDEX].argc = 1;								\
	  data[INDEX].argv = malloc(sizeof(char*));					\
	  data[INDEX].argv[0] = malloc(strlen(METHOD ".") * sizeof(char));		\
	  memcpy(data[INDEX].argv[0], METHOD, strlen(METHOD ".") * sizeof(char));	\
	})
      #define __use2(INDEX, METHOD, ARGV1)						\
	({										\
	  data[INDEX].argc = 2;								\
	  data[INDEX].argv = malloc(2 * sizeof(char*));					\
	  data[INDEX].argv[0] = malloc(strlen(METHOD ".") * sizeof(char));		\
	  memcpy(data[INDEX].argv[0], METHOD, strlen(METHOD ".") * sizeof(char));	\
	  data[INDEX].argv[1] = malloc(strlen(METHOD ".") * sizeof(char));		\
	  memcpy(data[INDEX].argv[1], ARGV1, strlen(ARGV1 ".") * sizeof(char));		\
	})
      
      __use1(0, "cache");
      __use2(1, "read", ".config/geolocation");
      __use2(2, "read", "/etc/geolocation");
      __use1(3, "timezone-offset");
      
      #undef __use1
      #undef __use2
      
      data[4].argc = -1;
      data[4].argv = NULL;
    }
  for (; data->argc > 0; data++)
    run(data->argc, data->argv);
  free(data_);
  
  if (already_done())
    abort_coordination();
  
  if (!term_coordination())
    {
      if (quickly && !continuous)
	return 1;
      quickly = 1;
      continuous = 0;
      goto retry;
    }
  return 0;
}

