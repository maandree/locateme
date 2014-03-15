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
#include <stdlib.h>


/**
 * Coordinate for running a method for getting the user's location
 * 
 * @param  argc        The number of arguments
 * @param  argv        The arguments, it and its first element will be free:d
 * @param  continuous  Whether program should run continuously
 * @param  quickly     Whether program should provide a reasonable location as fast a possible
 */
void run(int argc, char** argv, int continuous, int quickly)
{
  free(*argv);
  free(argv);
}

