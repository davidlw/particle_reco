#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"

# SERVER_LIST and INDICES are the same size
SERVER_LIST=(01 02 03 04 05 06 07)
INDICES=("1a 1b 1c 2a 2b 2c 3a" \
    "3b 3c 4a 4b 4c 5a 5b" \
    "5c 6a 6b 6c 7a 7b 7c" \
    "8a 8b 8c 9a 9b 9c 10a" \
    "10b 10c 11a 11b 11c 12a 12b" \
    "12c 13a 13b 13c 14a 14b 14c" \
    "15a 15b 15c 16a 16b 16c")

LIST_LENGTH=${#SERVER_LIST[*]}


source start.sh
cd ${READXML_PATH}

echo "Compiling..."
root -l -b -q compile_readxml.cpp

# Login to each server and launch the corresponding cluster of jobs
for i in $(seq 0 $(($LIST_LENGTH - 1))); do
    echo "ssh bonner"${SERVER_LIST[$i]}
    ssh bonner${i} << HERE

        source start.sh
        cd ${READXML_PATH}

        echo "Executing..."
        for j in ${INDICES[$i]}; do
            nohup unbuffer root -l -b -q `echo 'readxml.cc+("'${READXML_PATH}'/IOconfig/split_K/backgroundTrees_K_'${j}'.xml", "_K_'${j}'")'` &> ${LOG_PATH}/readxml_K_lowpt_${j}.log &
        done 

    HERE   




for i in SERVER_LIST; do
    echo "ssh bonner"${i}
    ssh bonner${i} << HERE

        source start.sh
        cd ${READXML_PATH}

        echo "Executing..."
        for j in '${INDICES_'${i}'[*]}'; do
            nohup unbuffer root -l -b -q `echo 'readxml.cc+("'${READXML_PATH}'/IOconfig/split_K/backgroundTrees_K_'${j}'.xml", "_K_'${j}'")'` &> ${LOG_PATH}/readxml_K_lowpt_${j}.log &
        done 

    HERE

echo
echo "Finished executing."
echo