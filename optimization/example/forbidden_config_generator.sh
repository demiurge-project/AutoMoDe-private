#!/bin/bash

# Syntax printing
function print_syntax() {
    echo
    echo "To generate the txt with the logical expressions that valid finite state machine should not satisfy,"
    echo "specify MAX_NBR_STATES MAX_NBR_CONNECTIONS and the txt file name in which the description will be saved:"
    echo "$0 <MAX_NBR_STATES> <MAX_NBR_CONNECTIONS> <TXT_FILE>"
    echo
    exit 1
}

if [ $# -lt 3 ]; then
    print_syntax
fi

# This rule prevents the variable indicating the destination state (i.e. behaviour) of the
# the transition (i.e. condition) to be out of range.
function write_destination_state_rule() {
  STATE=$1
  CONNECTION=$2
  echo "as.numeric(N${STATE}x${CONNECTION})>=as.numeric(NumStates)" >> ${TXT_FILE}
}

MAX_NBR_STATES=$(echo "$1-1" | bc)
MAX_NBR_CONNECTIONS=$(echo "$2-1" | bc)
TXT_FILE=$3

# Clear content of file
truncate -s 0 $TXT_FILE

# Write forbidden rules in file
for STATE in $(seq 0 $MAX_NBR_STATES)
do
  for CONNECTION in $(seq 0 $MAX_NBR_CONNECTIONS)
  do
    write_destination_state_rule $STATE $CONNECTION
  done
done
