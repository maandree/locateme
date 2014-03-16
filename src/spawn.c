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
#include <string.h>
#include <alloca.h>
#include <stdio.h>

#include "common.h"


/**
 * Read location from the output of a command
 * 
 * @param   argc  The number of words in `args`
 * @param   args  The command line arguments (not parsed yes)
 * @return        Zero on success
 */
int guess_by_command(int argc, char** args)
{
  int len = argc;
  int off = 0;
  int i;
  char* command;
  
  for (i = 0; i < argc; i++)
    len += strlen(*(args + i));
  
  command = alloca(len * sizeof(char));
  
  for (i = 0; i < argc; i++)
    {
      snprintf(command + off, len, "%s", *(args + i));
      off += strlen(*(args + i));
      *(command + off++) = ' ';
    }
  
  *(command + len - 1) = '\0';

  return 1;
}

