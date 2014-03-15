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


void guess_by_timezone_offset(void);


int main(int argc, char** argv)
{
  guess_by_timezone_offset();
  return 0;
}


/**
 * Perform an approximate guess on longitude
 * based on timezone offset, during standard
 * time and assume double summer time is never
 * used and that summer time adjustment is
 * always +1 hour, and default latitude to 0°.
 */
void guess_by_timezone_offset(void)
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
  
  printf("0.0 %f timezone offset\n", longitude);
  fflush(stdout);
}

