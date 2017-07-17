#!/bin/bash

PTBIN="L_4_6"
CFG_TAG="md1"
TRYCUTS_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/${CFG_TAG}/TryCutsCfg_${PTBIN}.xml"

source start.sh

for i in pPb Pbp; do
    nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} "/home/jg61/particle_reco/InputChain/config/L_${i}_4_6.xml" _${i} &> ${LOG_PATH}/TryCuts_${PTBIN}_${i}.log &
done