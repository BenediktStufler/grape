#!/bin/bash

gcc src/grape.c -O2 -Wall -o bin/grape -pthread -lm -lgsl -lmpfr -lgmp -lgslcblas 
