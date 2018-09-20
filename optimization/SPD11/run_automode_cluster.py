#!/usr/bin/python2

from __future__ import print_function
import sys, getopt
from subprocess import *
import os
import re
import time

###############################################################################
# This script launches ...
###############################################################################

########################### CONFIGURATION POINTS ##############################

AUTOMODE_BIN_DIR = "/home/fmendiburu/AutoMoDe-Mate-SPD11/bin/"

def run_automode_simu(name, index, controllers, mission):
    print("SIMULATION")
    output_file = open("/home/fmendiburu/AutoMoDe-Mate-SPD11/optimization/SPD11/resultados/" + name + ".txt", 'w')
    seeds = open("/home/fmendiburu/AutoMoDe-Mate-SPD11/optimization/SPD11/seeds.txt", 'r')
    controllers = open(controllers, 'r').readlines()
    controller = controllers[int(index)]
    for seed in seeds.readlines():
        seed = int(seed)
        output_file.write(str(seed) + '\t')
        full_cmd = AUTOMODE_BIN_DIR + '/automode_main' + ' -s ' + str(seed) + ' -c ' + mission + ' --fsm-config' + controller
        print(full_cmd)
	p = Popen(full_cmd.split(" "), stdin=PIPE, stdout=PIPE, stderr=PIPE)
        output, err = p.communicate(b"input data that is passed to subprocess' stdin")
	print(output)
	print(err)
        vec_output = output.split("\n")
        score = vec_output[len(vec_output)-2].split(" ")[1]
        output_file.write(str(score) + '\t')
        output_file.write('\n')
    output_file.close()

if __name__ == "__main__":
    mission_file = ""
    controller_file = ""
    controller_index = 0
    simulation_flag = True
    name = ""
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hm:c:i:s:n:", ["mission=", "controllers=", "index=", "simulation=", "name="])
    except getopt.GetoptError:
        print("run_automode_cluster.py -m <mission> -c <controller file> -i <index of controller> -s <simulation (T or F)> -n <name of exp>")
        sys.exit(2)
    for opt, arg in opts:
        if opt == "-h":
            print("run_automode_cluster.py -m <mission> -c <controller file> -i <index of controller> -s <simulation (T or F)> -n <name of exp>")
            sys.exit()
        elif opt in ("-m", "--mission"):
            mission_file = arg
        elif opt in ("-c", "--controllers"):
            controller_file = arg
        elif opt in ("-i", "--index"):
            controller_index = arg
        elif opt in ("-s", "--simulation"):
            simulation_flag = arg
	    if (simulation_flag == "False"):
		simulation_flag = False
        elif opt in ("-n", "--name"):
            name = arg

    if (simulation_flag):
	run_automode_simu(name, controller_index, controller_file, mission_file)
    else:
        print("Error runing 'run_automode_simu', simulation flag set as False")
