#!/bin/bash

TRYCUTS_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/TryCutsCfg_lowpt.xml"

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

# Login to each server and launch the corresponding cluster of jobs
for i in $(seq 0 $(($LIST_LENGTH - 1))); do

echo "ssh bonner"${SERVER_LIST[$i]}
ssh bonner${SERVER_LIST[$i]} "
    source /home/jg61/bin/start.sh;

    echo 'Executing...';
    for j in ${INDICES[$i]}; do
        nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} /home/jg61/particle_reco/InputChain/config/split_K/dataTrees_K_\${j}.xml _\${j} &> ${LOG_PATH}/\${j}.log &
    done;"

done

echo
echo "Finished executing."
echo