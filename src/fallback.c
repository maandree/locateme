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
#include "fallback.h"
#include "common.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <alloca.h>


/**
 * Uses the cache to determine last location
 * and guess that that one is still accurate.
 * 
 * @return  Zero on success
 */
int guess_by_cache(void)
{
  char* method_ = alloca(128 * sizeof(char));
  char method[128];
  char pathname[4096];
  FILE* f;
  int matched;
  float latitude;
  float longitude;
  
  snprintf(pathname, 4096, "%s/.cache/locateme", home());
  f = fopen(pathname, "r");
  if (f == NULL)
    return errno;
  
  matched = fscanf(f, "%f %f", &latitude, &longitude);
  if (matched < 2)
    {
      fclose(f);
      return -1;
    }
  
  (void) fgetc(f);
  *method_ = '\0';
  fgets(method_, 128, f);
  if (*method_ == '\0')
    method_ = "unknown";
  else
    {
      size_t n = strlen(method_);
      if ((n > 0) && (*(method_ + n - 1) == '\n'))
	*(method_ + n - 1) = '\0';
      if (*method_ == '\0')
	method_ = "unknown";
    }
  fclose(f);
  
  snprintf(method, 128, "%s cache", method_);
  report(latitude, longitude, method, DO_NOT_CACHE, SYNC);
  fflush(stdout);
  
  return 0;
}


/**
 * Perform an approximate guess on longitude
 * based on timezone offset, during standard
 * time and assume double summer time is never
 * used and that summer time adjustment is
 * always +1 hour, and default latitude to 0°.
 * 
 * @return  Zero on success
 */
int guess_by_timezone_offset(void)
{
  time_t time_utc;
  time_t time_local;
  time_t time_diff;
  struct tm* time_local_;
  float longitude;
  
  time_utc = time(NULL);
  time_local_ = localtime(&time_utc);
  time_local = time_local_->tm_hour;
  time_local = time_local * 60 + time_local_->tm_min;
  time_local = time_local * 60 + time_local_->tm_sec;
  if (time_local_->tm_isdst > 0)
    time_local -= 60 * 60; /* nullify summer time adjustment */
  
  time_utc %= 24 * 60 * 60;
  time_diff = time_local - time_utc;
  time_diff = (time_diff + 30) / 60; /* minutes */
  time_diff = (time_diff + 15) / 30; /* half hours */
  
  longitude = (float)time_diff * 180.f / 12.f / 2.f;
  if (longitude > 180.f)
    longitude -= 360.f;
  if (longitude < -180.f)
    longitude += 360.f;
  
  report(0.f, longitude, "timezone offset", DO_NOT_CACHE, SYNC);
  
  return 0;
}


/**
 * Use location been provided manually
 * 
 * @param   args  Configurations
 * @return        Zero on success
 */
int guess_by_manual(char** args)
{
  size_t len = strlen(args[0]) + strlen(args[1]);
  char* text = alloca((len + 2) * sizeof(char));
  float latitude;
  float longitude;
  
  snprintf(text, len + 2, "%s %s", args[0], args[1]);
  
  if (sscanf(text, "%f %f", &latitude, &longitude) < 2)
    return 1;
  
  report(latitude, longitude, "manual", DO_NOT_CACHE, SYNC);
  
  return 0;
}


/**
 * Read location from a file
 * 
 * @param   argc  The number of words in `args`
 * @param   args  The file
 * @return        Zero on success
 */
int guess_by_file(int argc, char** args)
{
  size_t len = (size_t)argc;
  size_t off = 0;
  size_t i;
  char* pathname;
  FILE* f;
  float latitude;
  float longitude;
  
  for (i = 0; i < (size_t)argc; i++)
    len += strlen(*(args + i));
  
  pathname = alloca(len * sizeof(char));
  
  for (i = 0; i < (size_t)argc; i++)
    {
      snprintf(pathname + off, len, "%s", *(args + i));
      off += strlen(*(args + i));
      *(pathname + off++) = ' ';
    }
  
  *(pathname + len - 1) = '\0';
  
  f = fopen(pathname, "r");
  if (f == NULL)
    return 1;
  
  if (fscanf(f, "%f %f", &latitude, &longitude) < 2)
    {
      fclose(f);
      return 1;
    }
  
  report(latitude, longitude, "file", DO_NOT_CACHE, SYNC);
  
  fclose(f);
  return 0;
}

