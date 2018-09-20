#!/usr/bin/python2

from __future__ import print_function
import argparse
from subprocess import *
import os
import re
import time
###############################################################################
# This script launches several runs in parallel in a SGE Cluster, and
# each run is parallelized using MPI.  Execute without parameters to see usage.
###############################################################################

########################### CONFIGURATION POINTS ##############################

QUEUE='short'
#MACHINE='opteron2216' #rack 1
#MACHINE='xeon5410' #rack 2
MACHINE='opteron6128' #rack 3
#MACHINE='opteron6272' #rack 4
#MACHINE='xeon2680' #rack 5

LIST_MISSIONS_AUTOMODE = ["mate_spd11.xml"]

LIST_NAMES = ["resultados_SPD11_assessment_50k"]

LIST_CONTROLLERS = ["resultados_SPD11_50k.txt"]

numExperimentos = 10

###############################################################################

p = argparse.ArgumentParser(description='')
p.add_argument('-r', '--run', help='the execdir', default="all")

#############
# Mate #
#############

def run_automode(mission, controllers, index, name):
    mission_dir = "/home/fmendiburu/AutoMoDe-Mate-SPD11/optimization/SPD11/experiments-folder/"
    controllers_dir = "/home/fmendiburu/AutoMoDe-Mate-SPD11/optimization/SPD11/resultados/"

    data = {"jobname": "mate-%s-%i-%i" % (name, os.getpid(), index),
            "runname": "%s-%i" % (name, index),
            "execdir": ("resultados" + ("/%s-%i" % (name, index))),
            "nbjob": 1,
            "machine": MACHINE,
            "queue": QUEUE,
            "mission": mission_dir + mission,
            "controllers": controllers_dir + controllers,
            "index": index,
    }
    script = """#!/bin/bash
#$ -N %(jobname)s
#$ -l %(machine)s
#$ -l %(queue)s
#$ -m a
#      b     Mail is sent at the beginning of the job.
#      e     Mail is sent at the end of the job.
#      a     Mail is sent when the job is aborted or rescheduled.
#      s     Mail is sent when the job is suspended.
#$ -o %(execdir)s/argos3-auto.stdout
#$ -e %(execdir)s/argos3-auto.stderr
#$ -cwd
#$ -pe mpi %(nbjob)s
#$ -binding linear:256
USERNAME=`whoami`
COMMAND="/home/fmendiburu/AutoMoDe-Mate-SPD11/optimization/SPD11/run_automode_cluster.py -m %(mission)s -c %(controllers)s -i %(index)s -s True -n %(runname)s "
cd %(execdir)s
echo "$COMMAND"
eval $COMMAND
if [ $? -eq 0 ]
then
  echo "Success!"
  exit 0
else
  echo "Fail!"
  exit 1
fi
"""
    p = Popen("qsub -v PATH", shell=True, stdin=PIPE, stdout=PIPE, close_fds=True)
    (child_stdout, child_stdin) = (p.stdout, p.stdin)
    child_stdin.write(script % data)
    #print(script % data)
    child_stdin.close()
    print('Job sended')
    print(child_stdout.read())

########
# Main #
########

if __name__ == "__main__":
    args = p.parse_args()

    def create_mate_jobs():
        for m in range(0, len(LIST_MISSIONS_AUTOMODE)):
            for i in range(0, numExperimentos):
                directory = "resultados/%s-%i" % (LIST_NAMES[m], i)
		if not os.path.exists(directory):
                    os.makedirs(directory)
                run_automode(LIST_MISSIONS_AUTOMODE[0], LIST_CONTROLLERS[m], i, LIST_NAMES[m])

    if (args.run == "mate"):
        create_mate_jobs()
    else:
        print("ERROR: Unknown design method {0}. Possible values are 'mate'.".format(args.run))
