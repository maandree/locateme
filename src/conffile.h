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
#ifndef LOCATEME_CONFFILE_H
#define LOCATEME_CONFFILE_H

#include <stdio.h>


/**
 * Open the configuration file and store its pathname
 * 
 * @param   conffile_pathname  Storage location for the file's pathname
 * @return                     A file stream of the file, `NULL` if not found
 */
FILE* get_conffile(char* conffile_pathname);


#endif

