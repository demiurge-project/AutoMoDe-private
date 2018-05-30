#!/usr/bin/env python3

from pathlib import Path
import os

# TODO: Replace magic numbers by constants

CONTROL_FLOW_CATEGORIES = '(0,1,2,3)'
TERMINAL_CATEGORIES = '(5,6)'
ALL_NODES_CATEGORIES = '(0,1,2,3,5,6)'

ACTION_CATEGORIES = '(1,2,3,4,5,6,7)'
CONDITION_CATEGORIES = '(0,1,2,3,4,5)'

max_children = 4
max_level = 2

grammar_file_name = "grammar_bt.txt"

def generate_grammar():

    def write_line_to_grammar_file(name, cmd_id, type, range, conditions=""):
        if conditions != "":
            grammar_file.write('{}\t{}\t{}\t{}\t|\t{}\n'.format(name, cmd_id, type, range, conditions))
        else:
            grammar_file.write('{}\t{}\t{}\t{}\n'.format(name, cmd_id, type, range))

    def create_node_record(id, type_range, has_children=True):
        return {
            "id": str(id),
            # "node_name": 'Node{}'.format(id),
            "node_arg": '\"--n{} \"'.format(id),
            "type": 'c',
            "type_range": type_range,
            "has_children": has_children
        }

    def write_possible_actions_to_file(node_id):
        write_line_to_grammar_file(
            'Action{}'.format(node_id),
            '\"--a{} \"'.format(node_id),
            'c', # why can this not be "i" as well?
            ACTION_CATEGORIES,
            'as.numeric(Node{})==5'.format(node_id)
        )
        # write the possible parameters
        write_line_to_grammar_file(
            "RWM{}".format(node_id),
            '\"--rwm{} \"'.format(node_id),
            'i',
            '(1,100)',
            'as.numeric(Action{})==0'.format(node_id)
        )
        write_line_to_grammar_file(
            "ATT{}".format(node_id),
            '\"--att{} \"'.format(node_id),
            'r',
            '(1,5)',
            'as.numeric(Action{})==4'.format(node_id)
        )
        write_line_to_grammar_file(
            "REP{}".format(node_id),
            '\"--rep{} \"'.format(node_id),
            'r',
            '(1,5)',
            'as.numeric(Action{})==5'.format(node_id)
        )
        write_line_to_grammar_file(
            "ProbS{}".format(node_id),
            '\"--p{} \"'.format(node_id),
            'r',
            '(0,1)',
            'as.numeric(Node{})==5'.format(node_id)
        )

    def write_possible_conditions_to_file(node_id):
        write_line_to_grammar_file(
            'Condition{}'.format(node_id),
            '\"--c{} \"'.format(node_id),
            'c', # why can this not be "i" as well?
            CONDITION_CATEGORIES,
            'as.numeric(Node{})==6'.format(node_id)
        )
        # write the possible parameters
        write_line_to_grammar_file(
            "P{}".format(node_id),
            '\"--p{} \"'.format(node_id),
            'r',
            '(0,1)',
            'as.numeric(Condition{}) %in% c(0,1,2,5)'.format(node_id)
        )
        write_line_to_grammar_file(
            "B{}".format(node_id),
            '\"--p{} \"'.format(node_id),
            'i',
            '(1,20)',
            'as.numeric(Condition{})==3'.format(node_id)
        )
        write_line_to_grammar_file(
            "W{}".format(node_id),
            '\"--w{} \"'.format(node_id),
            'r',
            '(0,20)',
            'as.numeric(Condition{})==3'.format(node_id)
        )
        write_line_to_grammar_file(
            "BI{}".format(node_id),
            '\"--p{} \"'.format(node_id),
            'i',
            '(1,10)',
            'as.numeric(Condition{})==4'.format(node_id)
        )
        write_line_to_grammar_file(
            "WI{}".format(node_id),
            '\"--w{} \"'.format(node_id),
            'r',
            '(0,20)',
            'as.numeric(Condition{})==4'.format(node_id)
        )

    children_range = '(1,{})'.format(max_children)
    node_stack = []
    with open(grammar_file_name, "w") as grammar_file:
        write_line_to_grammar_file('Rootnode', '\"--nroot \"', 'c', CONTROL_FLOW_CATEGORIES)
        write_line_to_grammar_file('NumChildsRoot', '\"--nchildroot \"', 'i', children_range)
        # create the initial children of the root node and put them on the stack
        for i in range(max_children-1, -1, -1): # counting down to maintain stack order
            record = create_node_record(i, ALL_NODES_CATEGORIES)
            node_stack.append(record)
        # process all elements in the node_stack
        while node_stack:
            node = node_stack.pop()
            if len(node["id"]) >= max_level: #Id counts the number of levels, only works for less than 10 children per node
                # we are at the lowest level
                node["type_range"] = TERMINAL_CATEGORIES  # the last nodes can only be action or conditions
                node["has_children"] = False # we cannot have any children
            else:
                for i in range(max_children-1, -1, -1): # counting down to maintain stack order
                    node_id = "{}{}".format(node["id"], i)
                    record = create_node_record(node_id, ALL_NODES_CATEGORIES)
                    node_stack.append(record)
            if len(node["id"])==1: # just under the Rootnode
                conditions = 'as.numeric(NumChildsRoot)>{}'.format(node["id"][-1:])
            else:
                conditions = 'as.numeric(NumChilds{})>{}'.format(node["id"][:-1],node["id"][-1:])
            write_line_to_grammar_file(
                'Node{}'.format(node["id"]),
                node["node_arg"],
                node["type"],
                node["type_range"],
                conditions
            )
            if node["has_children"]: # the node can have children
                write_line_to_grammar_file(
                    'NumChilds{}'.format(node["id"]),
                    '\"--nchild{} \"'.format(node["id"]),
                    'i',
                    children_range,
                    'as.numeric(Node{}) %in% c{}'.format(node["id"], CONTROL_FLOW_CATEGORIES)
            )
            # TODO: At the moment every node (except the root) can be an action or a conditions
            # TODO: However in the future this might not be True
            # TODO: It needs to be checked, if action and condition are in the range
            write_possible_actions_to_file(node["id"])
            write_possible_conditions_to_file(node["id"])


if __name__ == "__main__":
    # Check that the grammar file doesn't exist yet
    p = Path(grammar_file_name)
    if p.is_file():
        # Otherwise warn about the file being overwritten and save backup of file
        print("You already have a grammar file. It will be backed up.")
        os.rename(grammar_file_name, "{}.old".format(grammar_file_name))
    # Generate the grammar
    generate_grammar()
    print("Grammar succesfully generated")
