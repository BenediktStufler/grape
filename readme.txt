grape -- Generate RAndom PErmutations

* 
* 1. Introduction
* 

This project implements an efficient sampler for subcritical substitution-closed classes of permutations.

Currently, the following permutation classes are supported:
	- Separable permutations


*
* 2. Usage
*

Usage: grape [OPTION...] 
grape -- Generate RAndom PErmutations

  -i, --inputfile=INPUTFILE  Read a permutation from file INFILE instead of
                             generating it at random.
  -m, --maxincfile=MAXINCFILE   Output the length of the longest increasing
                             subsequence to MAXINCFILE.
  -N, --num=NUM              Simulate NUM many samples. Requires the use of the
                             % symbol in all specified output filenames. For
                             example, --num=100 --outfile=perm%.txt will create
                             the files perm001.graphml, perm002.graphml, ...,
                             perm100.graphml. If the filename does not contain
                             a % symbol then the file will be opend in append
                             mode.
  -o, --outfile=OUTFILE      Output simulated permutation to OUTFILE.
  -p, --permclass=PERMCLASS  Sample from PERMCLASS. Currently supported: 'sepa'
                             for the class of separable permutations
  -r, --randgen=RANDGEN      Use the pseudo random generator RANDGEN. Available
                             options are taus2, gfsr4, mt19937, ranlux,
                             ranlxs0, ranlxs1, ranlxs2, ranlxd1, ranlxd2, mrg,
                             cmrg, ranlux389. The default is taus2.
  -s, --size=SIZE            Generate a permutation of the integers from 1 to
                             SIZE.
  -S, --seed=SEED            Specify the seed of the random generator in the
                             first thread. Thread number k will receive SEED +
                             k - 1 as seed. The default is to use /dev/random.
                             If this fails, the systems timestamp (in seconds)
                             is used.
  -t, --threads=THREADS      Distribute the workload on THREADS many threads.
                             The default value is the number of CPU cores.
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version



*
* 3. Example
*

grape -s 10 -o /dev/stdout 
3, 6, 5, 4, 2, 7, 9, 8, 1, 10

grape -s 10000000 -m /dev/stdout 
368207



*
* 4. License
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

