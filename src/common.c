/**
 * locateme – really simple universal continuous location tracking
 * 
 * Copyright © 2014  Mattias Andrée (m@maandree.se)
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
#include "common.h"
#include "coordinator.h"

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>


/**
 * Get the user's home directory
 * 
 * @return  The user's home directory
 */
const char* home(void)
{
  static char* home = NULL;
  
  if (home == NULL)
    {
      struct passwd* pwd = getpwuid(getuid());
      home = pwd->pw_dir;
    }
  
  return (const char*)home;
}


/**
 * Report a found location, and cache it if preferable
 * 
 * @param  latitude   The user's guessed latitude location
 * @param  longitude  The user's guessed longitude location
 * @param  method     Location guessing method
 * @param  cacheable  Whether to cache the found location
 * @param  async      Whether the information was fetched asynchronously
 */
void report(float latitude, float longitude, const char* method, int cacheable, int async)
{
  if (!may_i_report(async))
    return;
  
  printf("%f %f %s\n", (double)latitude, (double)longitude, method);
  fflush(stdout);
  
  if (cacheable)
    {
      char pathname[4096];
      FILE* f;
      
      snprintf(pathname, 4096, "%s/.cache", home());
      mkdir(pathname, 0777);
      
      snprintf(pathname, 4096, "%s/.cache/locateme", home());
      f = fopen(pathname, "w");
      if (f != NULL)
	{
	  fprintf(f, "%f %f %s\n", (double)latitude, (double)longitude, method);
	  fflush(f);
	  fclose(f);
	}
    }
}

