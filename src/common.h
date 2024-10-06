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
#ifndef LOCATEME_COMMON_H
#define LOCATEME_COMMON_H


/**
 * Please cache the location for me
 */
#define DO_CACHE  1

/**
 * Whatever you do, do not cache this
 */
#define DO_NOT_CACHE  0


/**
 * The information was fetched asynchronously
 */
#define ASYNC  1

/**
 * The information was fetched synchronously
 */
#define SYNC  0



/**
 * Get the user's home directory
 * 
 * @return  The user's home directory
 */
const char* home(void);

/**
 * Report a found location, and cache it if preferable
 * 
 * @param  latitude   The user's guessed latitude location
 * @param  longitude  The user's guessed longitude location
 * @param  method     Location guessing method
 * @param  cacheable  Whether to cache the found location
 * @param  async      Whether the information was fetched asynchronously
 */
void report(float latitude, float longitude, const char* method, int cacheable, int async);


#endif

