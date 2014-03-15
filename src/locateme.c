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
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <alloca.h>


#define DO_CACHE  1
#define DO_NOT_CACHE  0


int guess_by_timezone_offset(void);
int guess_by_cache(void);
void report(float latitude, float longitude, const char* method, int cacheable);


int main(int argc, char** argv)
{
  if (guess_by_cache())
    if (guess_by_timezone_offset())
      return 1;
  
  return 0;
}


/**
 * Uses the cache to determine last location
 * and guess that that one is still accurate.
 * 
 * @return  Zero on success
 */
int guess_by_cache(void)
{
  struct passwd* pwd = getpwuid(getuid());
  char* home = pwd->pw_dir;
  char* pathname = alloca(4096 * sizeof(char));
  FILE* f;
  int matched;
  char* method_ = alloca(128 * sizeof(char));
  char* method = alloca(128 * sizeof(char));
  float latitude;
  float longitude;
  
  snprintf(pathname, 4096, "%s/.cache/locateme", home);
  f = fopen(pathname, "r");
  if (f == NULL)
    return errno;
  
  matched = fscanf(f, "%f %f %s\n", &latitude, &longitude, method_);
  fclose(f);
  if (matched < 2)
    return -1;
  
  if (matched < 3)
    method_ = "unknown";
  
  snprintf(method, 128, "%s cache", method_);
  report(latitude, longitude, method, DO_NOT_CACHE);
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
  
  report(0.f, longitude, "timezone offset", DO_NOT_CACHE);
  
  return 0;
}


/**
 * Report a found location, and cache it if preferable
 * 
 * @param  latitude   The user's guessed latitude location
 * @param  longitude  The user's guessed longitude location
 * @param  method     Location guessing method
 * @param  cacheable  Whether to cache the found location
 */
void report(float latitude, float longitude, const char* method, int cacheable)
{
  printf("%f %f %s\n", latitude, longitude, method);
  fflush(stdout);
  
  if (cacheable)
    ; /* TODO */
}

