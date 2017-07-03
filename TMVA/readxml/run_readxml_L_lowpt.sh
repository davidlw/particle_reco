#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"

# SERVER_LIST and INDICES are the same size
SERVER_LIST=(01 02 03 04 05 06 07)
INDICES=("1a 1b 1c 1d 1e 1f" \
    "1g 1h 2a 2b 3a" \
    "3b 4a 4b 5a 5b" \
    "6a 6b 7a 7b 8a" \
    "8b 9 10a 10b 11a" \
    "11b 12a 12b 13a 13b" \
    "14a 14b 15 16a 16b")

LIST_LENGTH=${#SERVER_LIST[*]}


# Login to each server and launch the corresponding cluster of jobs
for i in $(seq 0 $(($LIST_LENGTH - 1))); do
    echo "ssh bonner"${SERVER_LIST[$i]}
    ssh bonner${i} << HERE

        source start.sh
        cd ${READXML_PATH}

        echo "Executing..."
        for j in ${INDICES[$i]}; do
            nohup unbuffer root -l -b -q `echo 'readxml.cc+("'${READXML_PATH}'/IOconfig/split_L/backgroundTrees_L_'${j}'.xml", "_L_'${j}'")'` &> ${LOG_PATH}/readxml_L_lowpt_${j}.log &
        done

    HERE

echo
echo "Finished executing."
echo