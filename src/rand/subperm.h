/*
 * Simulate a random permutation from a substitution-closed class
 */
int grape(struct cmdarg *comarg, gsl_rng **rgens) {
	struct stree *T = NULL;
	struct perm *P = NULL;

	INT *degprofile = NULL;		// outdeg profile of tree
	INT *D = NULL;			// degree sequence of tree
	INT *E = NULL;			// expanded degree sequence of tree
	INT lenE = 0;			// length of expanded degree sequence
	INT counter;
	char *cname = NULL;
	INT mis = 0;			// length of maximal increasing subpermutation
	char rootdec;	

	//INT ***cdeco = NULL;
	//INT **cbeta = NULL;


	for(counter=1; counter <= comarg->num; counter++) {	
		if(comarg->method == 1) {	// class of separable permutations

			/* initialize precomputed values */
			sepainit();

			/* generate degree profile */
			degprofile = tbinb_sepa(comarg->size, comarg->size-1, sepaq, sepamax, comarg->threads, rgens);

			/* generate degree sequence */
			D = gendegsequence(degprofile, comarg->size, rgens[0]);

			/* we no longer need degree profile degprofile */
			free(degprofile);

			/* cyclically shift sequence */
			cycshift(D, comarg->size);

			/* expand sequence */
			sepaexpand(D, comarg->size, &E, &lenE, rgens[0]);

			/* we no longer need first outdegree sequence D */
			free(D);

			/* fair conflip on whether the root is decorated with a + or - */
			/* nozero value means +, zero value means - */
			rootdec = (char) gsl_rng_uniform_int(rgens[0],2);

			/* generate permutation if required */
			if( comarg->Toutfile ) {
				/*
			 	* create tree from expanded outdegree sequence E according to bfs order
			 	* (leaves distribution invariant)
			 	* propagate +/- decorations with root deco chosen by fair coin flip
			 	*/
				T = streedfs(E, lenE, rootdec, comarg->size);
			
				/* transform tree to permutation */
				P = tree2perm(T);
			}

			/* calculate maximal increasing subsequence */
			/* this does not require us to generate the permutation */
			if( comarg->Tmisfile ) {
				mis = sepacalcmis(E, lenE, comarg->size, rootdec);
			}


			// DEBUG
			/*
			T = streedfs(E, lenE, rootdec, comarg->size);
			P = tree2perm(T);
			mis = sepacalcmis(E, lenE, comarg->size, rootdec);
			INT mis2 = calcmis(P);
			
			printf("Parity bit: %c\n", '0' + rootdec);
			printf("Degree Sequence: \n");
			for(INT i=0; i<lenE; i++) printf("%ld, ", E[i]);
			printf("\n");
			printf("Permutation: \n");
			for(INT i=0; i<comarg->size; i++) printf("%ld, ", P->arr[i]);
			printf("\n");
			printf("Sepamis: %ld, Calcmis: %ld\n", mis, mis2);
			
			if(mis != mis2) {
				printf("ERROR - mismatch of mis\n");
			} else {
				printf("MATCH\n");
			}
			printf("\n");
			*/	


		} else {
			// method not supported yet
			fprintf(stderr, "Chosen permutation class is not supported yet.\n");
			exit(-1);
		}


		// output stuff
		if( comarg->Tmisfile || comarg->Toutfile ) {

			/* output permutation if requested */
			if( comarg->Toutfile ) {
				cname = convname(comarg->outfile, counter, comarg->num, comarg->Aoutfile);
				outperm(P,cname,comarg->Woutfile);
				free(cname);
			}

			/* output length of longest increasing subsequence if requested */
			if( comarg->Tmisfile ) {
				cname = convname(comarg->misfile, counter, comarg->num, comarg->Amisfile);
				outmisfile(mis, cname, comarg->Wmisfile);
				free(cname);
			}

		}


		// clean up
		if(E != NULL) free(E);
		if(P != NULL) free_perm(P);
		if(T != NULL) free_stree(T);
	}


	// free precomputed values... (add function)

	return 0;
}


