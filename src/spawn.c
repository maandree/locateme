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
#include "spawn.h"
#include "common.h"

#define _GNU_SOURCE
#include <string.h>
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


/**
 * Read location from the output of a command
 * 
 * @param   argc  The number of words in `args`
 * @param   args  The command line arguments (not parsed yes)
 * @return        Zero on success
 */
int guess_by_command(int argc, char** args)
{
  size_t len = (size_t)argc;
  size_t off = 0;
  size_t i;
  char* command;
  int read_write[2];
  pid_t pid;
  
  for (i = 0; i < (size_t)argc; i++)
    len += strlen(*(args + i));
  
  command = alloca(len * sizeof(char));
  
  for (i = 0; i < (size_t)argc; i++)
    {
      snprintf(command + off, len, "%s", *(args + i));
      off += strlen(*(args + i));
      *(command + off++) = ' ';
    }
  
  *(command + len - 1) = '\0';
  
  if (pipe(read_write))
    {
      perror("pipe");
      return 1;
    }
  
  pid = fork();
  if (pid == (pid_t)-1)
    {
      perror("fork");
      close(read_write[0]);
      close(read_write[1]);
      return 1;
    }
  
  if (pid)
    {
      int rc = 1;
      FILE* ch = NULL;
      int status;
      float latitude;
      float longitude;
      
      waitpid(pid, &status, WUNTRACED);
      if (WIFEXITED(status) && !WEXITSTATUS(status))
	if ((ch = fdopen(read_write[0], "r")) != NULL)
	  if ((rc = fscanf(ch, "%f %f", &latitude, &longitude) == 2))
	    report(latitude, longitude, "command", DO_CACHE, SYNC /* TODO : ASYNC, who should thread this */);
      
      /* I do not know if both of the two first are required, but it cannot hurt */
      if (ch)
	fclose(ch);
      close(read_write[0]);
      close(read_write[1]);
      
      return rc;
    }
  else
    {
      if (read_write[1] != STDOUT_FILENO)
	{
	  close(STDOUT_FILENO);
	  dup2(read_write[1], STDOUT_FILENO);
	}
      execlp("sh", "sh", "-c", command, NULL);
      perror("execlp");
      abort();
    }
  
  /* This really should not happen */
  return 1;
}

