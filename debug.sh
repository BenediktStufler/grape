#!/bin/bash

gcc src/grape.c -Wall -ggdb3 -o bin/grape -pthread -lm -lgsl -lmpfr -lgmp -lgslcblas 
