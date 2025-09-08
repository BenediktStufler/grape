/*
 * GRAPE -- Generate RAndom PErmutations
 *
 *
	Copyright (C) 2023 Benedikt Stufler

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


// needed to retrieve the number of cpu cores
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <unistd.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>

#include <argp.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
//#include <gsl/gsl_sf_gamma.h>
//#include <expat.h>
#include <mpfr.h>



/*
 * define data types and format strings for floats and large integers
 * keep in mind that the data type int could occupy only 32bit on 64bit machines
 *
 * DOUBLE: data type used for storing probabilities
 * PREC:   precision (in number of bits) used for intermediate floating point
 *         calculations
 * INT:    data type that needs to be able to store SIZE + 1
 * INTD:   the data type that needs to be able to store the sum of distances
 *	   from a single vertex to the rest
 */

#define DOUBLE long double
#define FDOUBLE 17.17Lf

#define INT long int
#define FINT ld

#define INTD unsigned long long
#define FINTD llu

#define PREC 1024

/* in order to use the format strings we need some macros 
 *
 * example code: 
 *
 * INTD largenum = 1099511627776;
 * printf("%" STR(FINTD) "\n", largenum);
 *
 */

// put quotes around argument
#define STR_(X) #X
// make sure argument is expanded
#define STR(X) STR_(X)





/*######### permutation data structures and algorithms #######*/

/*
 * provides a data structure that holds a permutation
*/
#include "perm/permstructure.h"

/*
 * an algorithm that calculates the length of the longest increasing subseqence
 */
#include "perm/permmis.h"



/*######### graph data structures and algorithms #######*/

/*
 * provides a data structure that holds a graph
*/
#include "graph/graphstructure.h"


/*
 * a multi-threaded bfs based algorithm that calculates the closeness 
 * centrality of a list of vertices in a graph
 */
#include "graph/bfscentrality.h"



/*######### io-functions #######*/

/*
 * parses command line options using the argp library
 * sets default values (number of threads to use, random generator seed, ...)
 * performs some sanity checks on the arguments
 */
#include "io/cmdparse.h"

/*
 * output functions 
 */
#include "io/output.h"

/*
 * read a previously simulated permutation from input
 */
#include "io/rfile.h"


/*######### functions for simulating random objects #######*/

/*
 * a multi-threaded algorithm to simulate the weighted balls in boxes model
 *
 */
#include "rand/ballsinboxes.h"

/*
 * simulate size-constrained Galton-Watson trees
 */
#include "rand/sgwtree.h"

/*
 * precomputed constants for permutation classes
 */
#include "prec/sepa.h"

/*
 * simulate random permutations from substitution-closed classes
 */
#include "rand/sepa.h"
#include "rand/subperm.h"




int main(int argc, char *argv[]) {
	struct cmdarg comarg;	// holds command line options
	gsl_rng **rgens;	// array of random number generators
	int i;

	/* read command line options and perform some sanity checks*/
	if(getcmdargs(&comarg, argc, argv)) {
		fprintf(stderr, "Error reading command line options.\n");
		exit(-1);
	}

	if( comarg.method == 0 ) {
		// read from input file instead of random generation
		rpermfile(&comarg);
	} else {
		/* initialize random number generators; one for each thread */
		rgens = (gsl_rng **) calloc(comarg.threads, sizeof(gsl_rng *));
		if(rgens == NULL) {
			// memory allocation error
			fprintf(stderr, "Memory allocation error in main function.\n");
			exit(-1);
		}
		for(i=0; i<comarg.threads; i++) {
			rgens[i] = gsl_rng_alloc(comarg.randgen);
			// each random generator gets initialized with a unique seed
			gsl_rng_set(rgens[i], comarg.seed + i);
		}

		// simulate a random permutation
		grape(&comarg, rgens);	
	
		/* clean up */
		for(i=0; i<comarg.threads; i++)
			gsl_rng_free(rgens[i]);
		free(rgens);
	}	

	return 0;
}


