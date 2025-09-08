/*
 *
 * Calculate the length of the longest/maximal increasing subsequence of a permutation
 *
 */

// optimized function used for calculating mis of separable permutations

INT sepacalcmis(INT *E, INT lenE, INT size, char rootdec) {
	struct qentry {
			INT deg;
			INT val;
	};

	struct qentry *Q;

	INT i,p;
	INT mis;
	char inv;

	// The worst case length of this queue is the height of the tree plus 1.
	// A tree with n leaves and no internal vertex with outdegree 1 has height at most n-1.
	// We add one parent to the root
	Q = (struct qentry *) malloc((size + 1) * sizeof(struct qentry));
	if(Q == NULL) {
		// memory allocation error
		fprintf(stderr, "Memory allocation error in function sepacalcmis.\n");
		exit(-1);
	}


	// debug


	// initialize queue with parent of root vertex
	Q[0].deg = 1;
	Q[0].val = 0;

	// initialize parity bit
	// parity bit will correspond to decoration of parent of current node
	inv = rootdec;

	i=0;
	p=0;
	do {
		if( Q[p].deg == 0 ) {
			// pop queue entry and perform a "join"
			if( inv ) {
				// sum join
				Q[p-1].val += Q[p].val;
			} else {
				// max join
				if( Q[p-1].val < Q[p].val ) Q[p-1].val = Q[p].val;
			}

			p--;
		} else {
			// push queue entry
			Q[p].deg--;
			
			// push vertex to queue
			Q[p+1].deg = E[i];
			Q[p+1].val = E[i] ?0 :1;

			p++;
			i++;
		}


		// flip decoration of current level
		inv = inv ?0 :1;
	} while(p != 0);

	// value of parent of root is now the length of the maximal increasing subsequence
	mis = Q[0].val;

	// clean up
	free(Q);
	
	return mis;
}



// general purpose function for calculating mis of permutations

INT calcmis(struct perm *P) {
	INT *arr;
	INT n;
	INT i;
	INT lo, hi, mis;
	INT *tail;

	arr = P->arr;
	n = P->len;

	// tail[i] = index in arr of the end element of an increasing subseq of length i+1
	tail = (INT *) malloc(n * sizeof(INT));

	/*
	//prev[k] = index of the predecessor of arr[k] in the maximal increasing subsequence ending at k
       	INT *prev;
	prev = (INT *) malloc(n * sizeof(INT));
	if(prev == NULL) {
		fprintf(stderr, "Memory allocation error in function calcmis.\n");
		exit(-1);
	}


	*/

	if(tail == NULL) {
		fprintf(stderr, "Memory allocation error in function calcmis.\n");
		exit(-1);
	}

	for (i=0, mis=0; i < n; i++) {
		// binary search for the insertion point of arr[i] in tail[0..mis)
		lo = 0;
		hi = mis;
		while (lo < hi) {
			INT mid = (lo + hi) / 2;
			if (arr[tail[mid]] < arr[i])
				lo = mid + 1;
			else
				hi = mid;
		}
		// lo is the length-1 position where arr[i] will go

		/*
		if (lo > 0)
			prev[i] = tail[lo - 1];
		else
			prev[i] = -1;
		*/
		tail[lo] = i;
		if (lo == mis) mis++;
	}

	/*
	// reconstruct maximal increasing subsequence by walking back from tail[mis-1]
	INT *seq = malloc(mis * sizeof(int));
	INT k = tail[mis - 1];
	for (INT idx = mis - 1; idx >= 0; idx--) {
		seq[idx] = arr[k];
		k = prev[k];
	}
	
	free(prev);
	*/

	free(tail);


	return mis;
}


