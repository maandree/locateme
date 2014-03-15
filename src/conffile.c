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

#include "common.h"


/**
 * Open the configuration file and store its pathname
 * 
 * @param   conffile_pathname  Storage location for the file's pathname
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
  
  if (f != NULL)
    memcpy(conffile_pathname, pathname, (strlen(pathname) + 1) * sizeof(char));
  return f;
}

