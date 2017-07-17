#!/bin/bash

PTBIN="K_6_8"
CFG_TAG="md1"
TRYCUTS_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/${CFG_TAG}/TryCutsCfg_${PTBIN}.xml"

source start.sh

for i in pPb Pbp; do
    nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} "/home/jg61/particle_reco/InputChain/config/K_${i}_high_pt.xml" ${i} &> ${LOG_PATH}/TryCuts_${PTBIN}_${i}.log &
done