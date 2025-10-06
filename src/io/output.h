/*
 * Provides functions that output the simulated data
 */


/*
 * counts the number of digits of an integer
 */
unsigned int count_digits(int arg) {
	return snprintf(NULL, 0, "%d", arg) - (arg < 0);
}

/*
 * make string out of integer
 */
char *uint2string(unsigned int p) {
	unsigned int digits;
	char *res;
	digits = count_digits(p);

	res = (char *) calloc(digits + 1, sizeof(char));
	if(res == NULL) {
		fprintf(stderr, "Memory allocation error in function unit2string.\n");
		exit(-1);
	}

	snprintf(res, digits + 1, "%u", p);

	return res;
}

/*
 * make string out of integer and fill with zeros
 */
char *uint2zstring(unsigned int p, unsigned int digits) {
	char *res;
	char *format;
	char *tmp;

	if(digits < count_digits(p)) {
		// undefined behaviour
		return NULL;
	}

	format = (char *) calloc(4 + count_digits(digits) , sizeof(char));
	res = (char *) calloc(digits + 1, sizeof(char));
	if(format == NULL || res == NULL) {
		fprintf(stderr, "Memory allocation error in function uint2zstring.\n");
		exit(-1);
	}

	// build format string
	// example: format = "%u0555" if digits = 555
	strcpy(format, "%0");
	tmp = uint2string(digits);
	strcat(format, tmp);
	free(tmp);
	strcat(format, "u");

	// build string representation
	snprintf(res, digits+1, format, p);

	free(format);

	return res;
}

/*
 * returns a dynamically allocated string with the correct filename
 */
char *convname(char *outfile, unsigned int counter, unsigned int num, int Tnum) {
	unsigned int digispace, loc;
	char *paddednumber, *fname;

	
	if( Tnum ) {
		// find location of % symbol
		for(loc=0; loc<strlen(outfile); loc++) {
			if( outfile[loc] == '%' ) break;
		}
		
		// safety check - check if % is contained in filename
		if(loc == strlen(outfile)) {
			fprintf(stderr, "Error in function convname. Received invalid data. Filename lacks %% symbol.\n");
			exit(-1);
		}

		// number of digits required to display num
		digispace = count_digits(num);

		// save to unique filenames
		fname = (char *) calloc(strlen(outfile) + digispace + 1, sizeof(char)); 
		if(fname == NULL) {
			fprintf(stderr, "Memory allocation error in function convname.\n");
			exit(-1);
		}

		strncpy(fname, outfile, loc);
		strcpy(fname + loc + digispace, outfile + loc + 1);
		paddednumber = uint2zstring(counter, digispace);
		strncpy(fname + loc, paddednumber, digispace);
		free(paddednumber);
	} else {
		fname = (char *) calloc(strlen(outfile) + 1, sizeof(char)); 
		if(fname == NULL) {
			fprintf(stderr, "Memory allocation error in function convname.\n");
			exit(-1);
		}
		strcpy(fname, outfile);
	}
	
	return fname;
}



/*
 * Output permutation
 */
int outperm(struct perm *P, char *outfile, char *mode) {
	FILE *outstream = NULL;	
	INT i;

	// open output file if necessary
	if(outfile == NULL || strlen(outfile) == 0) {
		outstream = stdout;
	} else {
		outstream = fopen(outfile, mode);
		if(outstream == NULL) {
			fprintf(stderr, "Error opening output file %s.\n", outfile);
			exit(-1);
		}
	}

	// output permutation
        for(i=0; i<P->len-1; i++) {
                fprintf(outstream, "%"STR(FINT)", ", P->arr[i]);   
        }
        fprintf(outstream, "%"STR(FINT), P->arr[P->len-1]);   
	fprintf(outstream, "\n");
	
	// close file if necessary
	if(outfile != NULL) fclose(outstream);

	return 0;
}


/*
 * Output length of maximal increasing subsequence
 */
int outmisfile(INT mis, char *outfile, char *mode) {
	FILE *outstream = NULL;	

	// open output file if necessary
	if(outfile == NULL || strlen(outfile) == 0) {
		outstream = stdout;
	} else {
		outstream = fopen(outfile, mode);
		if(outstream == NULL) {
			fprintf(stderr, "Error opening output file %s.\n", outfile);
			exit(-1);
		}
	}

	// output data
	fprintf(outstream, "%"STR(FINT), mis);	
	fprintf(outstream, "\n");
	
	// close file if necessary
	if(outfile != NULL) fclose(outstream);

	return 0;
}






/*
 * Write a sequence of values to a file or stdout
 */
int outseq(void *seq, INT size, char *outfile, int format, char *mode) {
	FILE *outstream = NULL;	
	INT i;

	// open output file if necessary
	if(outfile == NULL || strlen(outfile) == 0) {
		outstream = stdout;
	} else {
		outstream = fopen(outfile, mode);
		if(outstream == NULL) {
			fprintf(stderr, "Error opening output file %s.\n", outfile);
			exit(-1);
		}
	}


	// output values
	fprintf(outstream, "{");
	for(i=0; i<size-1; i++) {
		switch(format) {
			case 1:
				// INT
				fprintf(outstream, "%" STR(FINT) ", ", ((INT *) seq)[i]);
				break;
			case 2:
				// INTD
				fprintf(outstream, "%" STR(FINTD) ", ", ((INTD *) seq)[i]);
				break;
			case 3:
				// DOUBLE
				fprintf(outstream, "%" STR(FDOUBLE) ", ", ((DOUBLE *) seq)[i]);
				break;
			default:
				fprintf(stderr, "Error, unknown format in function outseq.\n");
				exit(-1);
		}
	}
	if(size>0) {
		// print last element without comma
		switch(format) {
			case 1:
				// INT
				fprintf(outstream, "%" STR(FINT), ((INT *) seq)[i]);
				break;
			case 2:
				// INTD
				fprintf(outstream, "%" STR(FINTD), ((INTD *) seq)[i]);
				break;
			case 3:
				// DOUBLE
				fprintf(outstream, "%" STR(FDOUBLE), ((DOUBLE *) seq)[i]);
				break;
			default:
				fprintf(stderr, "Error, unknown format in function outseq.\n");
				exit(-1);
		}
	}
	fprintf(outstream, "}\n");

	// close file if necessary
	if(outfile != NULL) fclose(outstream);

	return 0;
}

