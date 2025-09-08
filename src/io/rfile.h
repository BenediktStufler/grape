/*
 * parse a permutation from a file
 */

struct perm *parseperm(char * infile) {
	FILE *instream;
	struct perm *P;

	char ch;
	INT num;
	INT val;
	int parsing;
	int cap;
	INT *arr;
	INT dsize;

		// open input file 
		if(infile == NULL) { 
				instream = stdin;
		} else {
				instream = fopen(infile, "r");
				if(instream == NULL) { 
						fprintf(stderr, "Error opening input file.\n");
						exit(-1);
				}
		}

	// initialize dynamically growing array
	arr = malloc(sizeof(INT) * 1048576);
	if(arr == NULL) {
		fprintf(stderr, "Memory allocation error in function parseperm.\n");
		exit(-1);
	}
	dsize = 1048576; // current size of dynamically growing array
	
	
	cap = 0;	// how many digits of a number have we have read so far
	parsing = 0;	// are we currently parsing a number
	val = 0; 	// value of current number we are parsing
	num = 0; 	// how many numbers we have parsed
		while ((ch = fgetc(instream)) != EOF) {
		if( isdigit( (unsigned char) ch ) ) {
			parsing = 1;	
			cap++;
			if(cap > 16) {
							fprintf(stderr, "Error trying to read a too large number from input file.\n");
				exit(-1);
			}
			val = val * 10 + (ch - '0');
		} else {
			if( parsing ) {
				// we successfully parsed a new number

				// grow dynamic array if necessary
				if( num >= dsize ) {
					dsize = dsize*2;
					arr = realloc(arr, sizeof(INT) * dsize);
					if(arr == NULL) {
						fprintf(stderr, "Memory allocation error in function parseperm.\n");
						exit(-1);
					}
				}	

				arr[num] = val;
				val = 0;
				parsing = 0;
				cap = 0;

				num++;
			}
		}
	}
	
	// if the file ended with a digit
	if( parsing ) {
		// we successfully parsed a new number

		// grow dynamic array if necessary
		if( num >= dsize ) {
			dsize = dsize*2;
			arr = realloc(arr, sizeof(INT) * dsize);
			if(arr == NULL) {
				fprintf(stderr, "Memory allocation error in function parseperm.\n");
				exit(-1);
			}
		}	

		arr[num] = val;
		num++;
	}

	// sanity check
	if(num == 0) {
		fprintf(stderr, "Error: no numbers in input file.\n");
		exit(-1);
	}

	// shrink output array
	arr = realloc(arr, sizeof(INT) * num);

	// create permutation
	P = (struct perm *) malloc(sizeof(struct perm));
	if(P == NULL) {
		fprintf(stderr, "Memory allocation error in function parseperm.\n");
		exit(-1);
	}
	P->arr = arr;
	P->len = num;

	// close input file if necessary
	if(infile != NULL) fclose(instream);

	return P;
}


/*
 * Read permutation from file instead of generating it at random
 * Perform all operations specified by user
 */
int rpermfile(struct cmdarg *comarg) {
	struct perm *P;
	INT mis;

	/* read permutation from file */
	P = parseperm(comarg->infile);

	/* output permutation if requested */
	if( comarg->Toutfile ) {
		outperm(P,comarg->outfile,'w');
	}

	/* output length of longest increasing subsequence if requested */
	if( comarg->Tmisfile ) {
		mis = calcmis(P);
		outmisfile(mis, comarg->misfile, 'w');
	}

	/* clean up */
	free_perm(P);

	return 0;
}
