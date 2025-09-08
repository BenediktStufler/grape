/*
 *
 * We define a data structure that holds a permutation
 *
 */

struct perm {
	INT *arr;	// array with permuted list of INT values from 1 to len
	INT len;	// length of permutation
	INT mis;	// length of longest increasing subsequence
};

void free_perm(struct perm *P) {
	// free permuted list
	free(P->arr);

	// free permutation
	free(P);
}

struct perm *new_perm(INT len) {
	struct perm *P;
	INT *arr;

	P = (struct perm *) malloc(sizeof(struct perm));
	arr = (INT *) malloc(len * sizeof(INT));

	if( P == NULL || arr == 0) {
		fprintf(stderr, "Error allocating memory for permutation.\n");
		exit(-1);
	}

	P->arr = arr;
	P->len = len;

	return P;
}
