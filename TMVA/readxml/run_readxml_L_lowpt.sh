#!/bin/bash

PTBIN="L_2_4"
READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/myTMVA_${PTBIN}/config_${PTBIN}.xml"
OUT_TAG="_${PTBIN}"

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
ssh bonner${SERVER_LIST[$i]} "
    source /home/jg61//bin/start.sh;
    cd ${READXML_PATH};

    echo 'Executing...';
    for j in ${INDICES[$i]}; do
        nohup unbuffer root -l -b -q \`echo 'readxml.cc+(\"${CONFIG_FILE}\",\"/home/jg61/particle_reco/InputChain/config/split_L/dataTrees_L_'\${j}'.xml\",\"${OUT_TAG}_'\${j}'\")'\` &> ${LOG_PATH}/readxml${OUT_TAG}_\${j}.log &
    done;"

done

echo
echo "Finished executing."
echo