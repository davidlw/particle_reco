#!/bin/bash

PTBIN="L_4_6"
READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/config/config_${PTBIN}.xml"
EXEC="/home/jg61/particle_reco/TMVA/readxml/src/readxml"

source start.sh
cd ${READXML_PATH}

for i in pPb Pbp; do
    nohup unbuffer ${EXEC} ${CONFIG_FILE} "/home/jg61/particle_reco/InputChain/config/L_${i}_4_6.xml" ${i} &> ${LOG_PATH}/readxml_${PTBIN}_${i}.log &
done