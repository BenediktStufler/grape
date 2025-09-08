/*
 * slim tree data structure
 */ 

struct svert;

struct svert {
	INT deg;
	INT label;
	char dec;
	struct svert *son;
};

struct stree {
	struct svert* arr;
	INT len;
	INT numleaves;
};

struct stree *new_stree(INT len) {
	struct stree *T;
	struct svert *arr;

	T = (struct stree *) malloc(sizeof(struct stree));
	arr = (struct svert *) malloc(len * sizeof(struct svert));

        if(arr == NULL || T == NULL) {
                fprintf(stderr, "Error allocating memory for stree.\n");
                exit(-1);
        }

	T->arr = arr;
	T->len = len;

	return T;
}

void free_stree(struct stree *T) {
	if(T != NULL) {
		free(T->arr);
		free(T);
	}
}


/*
 * create slim tree from vertex degree sequence - bfs order
*/

struct stree *streebfs(INT *E, INT lenE, char rdec, INT numleaves) {
	struct svert *arr = NULL;
	struct svert *pos;
	char inv;
	INT i, j;
	struct stree *T;

	if(lenE <= 0) return NULL;


	// initialize slim tree structure
	T = new_stree(lenE);
	T->numleaves = numleaves;

	// add edges such that the i th entry of the outdegree list corresponds
	// to the outdegree of the i th vertex in bfs order 
	// the son points to the first child, with its siblings following right after

	arr = T->arr;

	arr[0].dec = rdec;
	for(i=0, pos=arr+1; i<lenE; i++) {
		arr[i].deg = E[i];
		arr[i].son = pos;
		if(E[i]>0) {
			inv = arr[i].dec ?0 :1;
			for(j=0; j<E[i]; j++) {
				pos[j].dec = inv;
				/*
				// debug
				printf("%ld - %ld\n", i, ( (INT) (pos + j) - (INT) arr) / sizeof(struct svert));
				*/
			}
		}
		pos += E[i];	
	}	

	return T;
}


/*
 * create slim tree from vertex degree sequence - dfs order
*/

struct stree *streedfs(INT *E, INT lenE, char rdec, INT numleaves) {
	struct svert *pos;
	struct svert *qp;
	struct svert **queue;
	char inv;
	INT i, j, qpos;
	struct stree *T;

	if(lenE <= 0) return NULL;


	// initialize slim tree structure
	T = new_stree(lenE);
	T->numleaves = numleaves;

	// add edges such that the i th entry of the outdegree list corresponds
	// to the outdegree of the i th vertex in dfs order 
	// the son points to the first child, with its siblings following right after
	
	queue = (struct svert **) malloc(lenE * sizeof(struct svert *));
        if(queue == NULL) {
                fprintf(stderr, "Memory allocation error in function streedfs.\n");
                exit(-1);
        }


	queue[0] = T->arr;
	queue[0]->dec = rdec;

	// points to next free space in array
	pos=T->arr+1;

	for(i=0, qpos=0; qpos != -1; qpos--, i++) {
		// pop vertex from queue
		qp = queue[qpos];

		// assign outdegree to vertex
		qp->deg = E[i];

		if(E[i] > 0) {
			// vertex has positive outdegree
			
			// it's first son is located at the beginning of the next free chunk
			qp->son = pos;

			// set decoration of children and add them to queue
			inv = qp->dec ?0 :1;
			for(j=0; j < qp->deg; j++) {
				queue[qpos+j] = qp->son + (qp->deg - j - 1);
				queue[qpos+j]->dec = inv;
			}

			// update pointer to end of queue
			qpos += E[i];

			// update pointer to next free chunk
			pos += E[i];
		}
	}

	// clean up
	free(queue);

	return T;
}



/*
 * transform decorated tree into permutation
 */

struct perm *tree2perm(struct stree *T) {
	// position in queue
	INT pos;
	INT lcnt;
	INT label;
	INT i;

	struct perm *P;
	struct svert *qp;
	struct svert **queue;

	queue = (struct svert **) malloc(T->len * sizeof(struct svert *));
        if(queue == NULL) {
                fprintf(stderr, "Memory allocation error in function tree2perm.\n");
                exit(-1);
        }

	/*
	// debug;
	INT jj=0;
	for(i=0;i<T->len;i++) {
		printf("%ld, ", T->arr[i].deg);
	}
	printf("\n");
	*/


	// assign labels of leaves according to DFS search
	label=1;
	pos=0;
	queue[0] = T->arr;

	for(; pos != -1; pos--) {
		qp = queue[pos];

		if(qp->deg == 0) {
			qp->label = label;
			label++;
		} else {
			for(i=0; i < qp->deg; i++) queue[pos+i] = qp->son + (qp->deg - i - 1);
			pos += qp->deg;
		}
	}


	// initialize output permutation
	P = new_perm(T->numleaves);

	// transform tree into permutation
	pos=0;
	queue[0] = T->arr;
	lcnt=0;

	for(; pos != -1; pos--) {
		qp = queue[pos];

		/*
		// debug
		printf("pos = %ld, qpdeg = %ld, lnct = %ld\n", pos, qp->deg,lcnt);
		*/

		if(qp->deg == 0) {
			(P->arr)[lcnt] = qp->label;
			lcnt++;
		} else {
			if( qp->dec ) {
				// increasing permutation
				for(i=0; i < qp->deg; i++) queue[pos+i] = qp->son + (qp->deg - i - 1);
			} else {
				// decreasing permutation
				for(i=0; i < qp->deg; i++) queue[pos+i] = qp->son + i;
			}
			pos += qp->deg;
		}
	}

	// clean up
	free(queue);

	return P;
}



/*
 * expand degree sequence of a tree with n vertices into 
 * a degree sequence of a tree with n leaves
 */

void sepaexpand(INT *D, INT size, INT **E, INT *lenE, gsl_rng *rgen) {

	uint32_t buf;
	INT i,j;
	int bitcnt, bitnum;
	INT *F;
	INT d, a;
	INT pnt;


	// safety checks
	if(gsl_rng_min(rgen) != 0) {
		fprintf(stderr, "Error. For the class of separable permutations we use an optimization that requires a random number generator with range from 0 to 2^32-1 (or 2^24 -1). The one you selected has range %lu to %lu. Please select a suitable generator instead, for example taus2 or gfsr4 or mt19937.\n", gsl_rng_min(rgen), gsl_rng_max(rgen));
		exit(-1);
	}


	// the only random number generators of gsl that do not
	// satisfy these requirements are mrg and cmrg.
	// these have gsl_rng_max = 2^31-2
	if(gsl_rng_max(rgen) == 4294967295) {
		bitnum = 32;
	} else if(gsl_rng_max(rgen) == 16777215) {
		bitnum = 24;
	} else {
		fprintf(stderr, "Error. For the class of separable permutations we use an optimization that requires a random number generator with range from 0 to 2^32-1 (or 2^24 - 1). The one you selected has range %lu to %lu. Please select a suitable generator instead, for example taus2 or gfsr4 or mt19937.\n", gsl_rng_min(rgen), gsl_rng_max(rgen));
		exit(-1);

	}


	// the output array, where we store the new outdegree sequence
	// created by the blow-up. a tree with n vertices gets blown up
	// into a tree with n leaves and no inner vertex having outdegree 1.
	// hence the new tree has at most 2n vertices.
	//
	// 2n could be somewhat more space than we need, but this is faster
	// than reserving a new byte for each write
	F = (INT *) calloc(2*size, sizeof(INT));
	if(F == NULL) {
                // memory allocation error
                fprintf(stderr, "Memory allocation error in function sepaexpand.\n");
                exit(-1);
        }

	// we always get bitnum fresh bits
	buf = gsl_rng_get(rgen);
	// bitcnt keeps track how many we have used
	bitcnt = 0;

	// pnt tells us where to write the next value in the output sequence
	pnt=0;
	
	// we have size many outdegrees in the input sequence
	// each needs to be blown up
	for(i=0; i<size; i++) {
		d = D[i];

		/*
		 * Here is what the following code does:
		 *
		 * For d > 0 we generate a sequence of outdegrees d_1, ..., d_K \ge 2 
		 * with sum d_1 + ... + d_K = d + K and 1 \le K \le D summands.
		 *
		 * This is because in the Ehrenborg-M\'endez transform we have 
		 * phi(z) = SEQ(SEQ_{\ge 1})
		 *
		 * Any such sequence corresponds bijectively to a list b_1, ..., b_{D-1}
		 * of bits with the bit 1 occurring precisely K-1 many times.
		 *
		 * The correspondence is by inserting two zero bits into the K spaces
		 * before/between/after one bits and taking the length of consecutive
		 * occurrences of zero bits.
		 *
		 * Example:
		 * 	D = 10
		 *	(b_1, ..., b_{D-1}) = 001010010
		 *	after blow-up: 00OO10OO100OO10OO
		 *	(d_1, ..., d_K) = (4,3,4,3)
		 *	K = 4
		 *
		 * */


		// perform blow-up if d>0
		if( d != 0 ) {
			a = 1;

			// we need d-1 random bits in order to blow up d
			for(j=1; j<d; j++) {
				if( buf&1 ) {
					// we got a new value of the blow-up
					
					// write new value to output array
					F[pnt] = a+1;

					// update counter for next writing location
					pnt++;

					// reset for calculating next value of blow-up
					a = 1;
				} else {
					a++;
				}

				// prepare next random bit
				bitcnt++;	// we have used one bit
				if(bitcnt < bitnum) {
					// shift buf one step to the right, 
					// discarding the used bit
					buf>>=1;
				} else {
					// get bitnum fresh random bits
					buf = gsl_rng_get(rgen);
					// reset counter for used bits
					bitcnt = 0;
				}
			}

			// perform the last write
			// if the last bit did caused a write, then we need to add push
			// the number 2 ot the output list; in this case we have a=1, so 
			// we push a+1;
			// if the last bit did not cause a write, then we need to push
			// a+1 as well
			F[pnt] = a+1;
			pnt++;
		}
		// push final zero
		F[pnt] = 0;
		pnt++;


	}

	/*
	// debug
	for(buf = gsl_rng_get(rgen), i=0; i<bitnum; buf>>=1, i++) {
		printf("%d ", buf&1);
	}
	*/

	// return array produced by the blow-up
	*E = F;

	// return length of output array
	*lenE = pnt;
}
