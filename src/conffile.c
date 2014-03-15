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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>

#include "common.h"

#include "conffile.h"


/**
 * Open the configuration file and store its pathname
 * 
 * @param   conffile_pathname  Storage location for the file's pathname, may be `NULL`
 * @return                     A file stream of the file, `NULL` if not found
 */
FILE* get_conffile(char* conffile_pathname)
{
  FILE* f = NULL;
  char pathname[4096];
  char* superpath;
  
  int env(const char* variable, char** value)
  {
    *value = getenv(variable);
    return (value != NULL) && (*value != '\0');
  }
  
  #define openfile(PATTERN, ARGS...)				\
    ({								\
      snprintf(pathname, sizeof(pathname), PATTERN, ##ARGS);	\
      fopen(pathname, "r");					\
    })
  
  if ((f == NULL) && env("XDG_CONFIG_HOME", &superpath))
    f = openfile("%s/locateme.conf", superpath);
  
  if ((f == NULL) && env("HOME", &superpath))
    f = openfile("%s/.config/locateme.conf", superpath);
  
  if (f == NULL)
    f = openfile("%s/.config/locateme.conf", home());
  
  if ((f == NULL) && env("XDG_CONFIG_DIRS", &superpath))
    {
      char* end = " ";
      char* begin;
      for (begin = superpath; (f == NULL) && *end; begin = end + 1)
	{
	  int len;
	  end = strchr(begin, ':') ?: strchr(begin, '\0');
	  if ((len = end - begin))
	    f = openfile("%.*s/locateme.conf", len, begin);
	}
    }
  
  if (f == NULL)
    f = fopen("/etc/locateme.conf", "r");
  
  printf("%i\n", (f != NULL) && (conffile_pathname != NULL));
  printf("%s\n", pathname);
  if ((f != NULL) && (conffile_pathname != NULL))
    memcpy(conffile_pathname, pathname, (strlen(pathname) + 1) * sizeof(char));
  return f;
}


/**
 * Read the content of the configuration file
 * 
 * @param   f  Stream of the configuration file
 * @return     A list of line content, it and some of its content need to be free:d
 */
conffile_t* read_conffile(FILE* f)
{
  char* line = alloca(4096 * sizeof(char));
  conffile_t* cont = malloc(1 * sizeof(conffile_t));
  int i = 0;
  
  while (fgets(line, 4096, f))
    {
      int count = 0;
      int len = strlen(line);
      if (len > 0)
	{
	  char* args_cont = malloc(len * sizeof(char));
	  char** args = malloc(1 * sizeof(char*));
	  
	  *(args + count++) = args_cont;
	  while (*line != '\n')
	    if (*line != ' ')
	      *args_cont++ = *line++;
	    else
	      {
		line++;
		*args_cont++ = '\0';
		if ((count & -count) == count)
		  args = realloc(args, (count << 1) * sizeof(char*));
		*(args + count++) = args_cont;
	      }
	  *args_cont = '\0';
	  
	  (cont + i)->argc = count;
	  (cont + i)->argv = args;
	  i++;
	  if ((i & -i) == i)
	    cont = realloc(cont, (i << 1) * sizeof(conffile_t));
	}
    }
  
  (cont + i)->argc = -1;
  (cont + i)->argv = NULL;
  
  return cont;
}

