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
#ifndef LOCATEME_FALLBACK_H
#define LOCATEME_FALLBACK_H


/**
 * Uses the cache to determine last location
 * and guess that that one is still accurate.
 * 
 * @return  Zero on success
 */
int guess_by_cache(void);

/**
 * Perform an approximate guess on longitude
 * based on timezone offset, during standard
 * time and assume double summer time is never
 * used and that summer time adjustment is
 * always +1 hour, and default latitude to 0°.
 * 
 * @return  Zero on success
 */
int guess_by_timezone_offset(void);


#endif

