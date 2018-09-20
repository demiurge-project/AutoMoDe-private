#!/usr/bin/env python

#from __future__ import print_function
import argparse
from subprocess import *
import os
import re
import time


########################### CONFIGURATION  ####################################



FILES = "resultados/resultados_SPD11_assessment_50k"
BRANCH = "mate"
numExperimentos = 10

###############################################################################


#############
#   fcns    #
#############

def write(name, index):



	fileInput = "%s-%i.txt" % (name, index)
	fileOutput = "%s_overall_%s.txt" % (name,BRANCH)

	txtFileInput = open(fileInput, "r")
	

	if (index):	
		txtFileOutput = open(fileOutput, "a")
		
	else:
		txtFileOutput = open(fileOutput, "w")


	with txtFileInput as infile:
	    for line in infile:
		print >> txtFileOutput, line.split()[1],
		print >> txtFileOutput


		


########
# Main #
########

if __name__ == "__main__":


            for i in range(0, numExperimentos):
                write(FILES, i)





