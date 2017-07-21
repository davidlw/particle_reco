#!/bin/bash

PTBIN="L_10up"
CFG_TAG="md1"
FIT_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts_fit"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/${CFG_TAG}/TryCutsCfg_${PTBIN}.xml"


source start.sh

nohup unbuffer ${FIT_EXEC} ${CONFIG_FILE} _${CFG_TAG} &> ${LOG_PATH}/fits_${CFG_TAG}.log &