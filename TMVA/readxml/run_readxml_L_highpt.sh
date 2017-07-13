#!/bin/bash

PTBIN="L_10up"
READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/config/config_${PTBIN}.xml"
OUT_TAG="_${PTBIN}"
EXEC="src/readxml"

cd ${READXML_PATH}

nohup unbuffer ${EXEC} ${CONFIG_FILE} /home/jg61/particle_reco/InputChain/config/dataTrees_L_highpt.xml ${OUT_TAG} &> ${LOG_PATH}/readxml${OUT_TAG}.log &