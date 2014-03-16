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
#include <pthread.h>
#include <string.h>

#include "fallback.h"
#include "spawn.h"


/**
 * Synchronisation mutex
 */
static pthread_mutex_t coordinator_mutex;

/**
 * Synchronisation condition
 */
static pthread_cond_t coordinator_cond;


/**
 * Whether program should run continuously
 */
static int continuous_mode;

/**
 * Whether program should provide a reasonable location as fast a possible
 */
static int quickly_mode;

/**
 * Whether we have found a location
 */
static int got_a_location;



/**
 * Initialise coordination
 * 
 * @param  continuous  Whether program should run continuously
 * @param  quickly     Whether program should provide a reasonable location as fast a possible
 */
void init_coordination(int continuous, int quickly)
{
  continuous_mode = continuous;
  quickly_mode = quickly;
  got_a_location = 0;
  
  pthread_mutex_init(&coordinator_mutex, NULL);
  pthread_cond_init(&coordinator_cond, NULL);
}


/**
 * Abort coordination
 */
void abort_coordination(void)
{
}


/**
 * Terminate coordination
 * 
 * @return  Whether we have found a location
 */
int term_coordination(void)
{
  pthread_mutex_destroy(&coordinator_mutex);
  pthread_cond_destroy(&coordinator_cond);
  
  return got_a_location;
}


/**
 * Check whether we are already
 * 
 * @return  Whether we are already
 */
int already_done(void)
{
  int rc = continuous_mode;
  pthread_mutex_lock(&coordinator_mutex);
  
  rc &= got_a_location;
  
  pthread_mutex_unlock(&coordinator_mutex);
  return rc;
}



/**
 * Coordinate for running a method for getting the user's location
 * 
 * @param  argc  The number of arguments
 * @param  argv  The arguments, it and its first element will be free:d
 */
void run(int argc, char** argv)
{
  #define __case(CASE)   if (!strcmp(*argv, CASE))
  #define ___case(CASE)  else __case(CASE)
  
  __case("cache")
    guess_by_cache();
  
  ___case("timezone-offset")
    guess_by_timezone_offset();
  
  ___case("manual")
    guess_by_manual(argv + 1);
  
  ___case("read")
    guess_by_file(argc - 1, argv + 1);
  
  ___case("spawn")
    guess_by_command(argc - 1, argv + 1);
  
  #undef ___case
  #undef __case
  
  free(*argv);
  free(argv);
}


/**
 * Report that you have a location to report
 * 
 * @param   async  Did you retrieve location asynchronously?
 * @return         Whether you may report your findings
 */
int may_i_report(int async)
{
  int rc = async;
  pthread_mutex_lock(&coordinator_mutex);
  
  if ((rc |= quickly_mode))
    got_a_location = 1;
  quickly_mode = 0;
  
  pthread_mutex_unlock(&coordinator_mutex);
  return rc;
}

