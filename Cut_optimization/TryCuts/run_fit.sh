#!/bin/bash

PTBIN="L_10up"
CFG_TAG="md1"
FIT_EXEC="src/TryCuts_fit"
LOG_PATH="logs/${PTBIN}"
CONFIG_FILE="config/${CFG_TAG}/TryCutsCfg_${PTBIN}.xml"


source start.sh

nohup unbuffer ${FIT_EXEC} ${CONFIG_FILE} _${CFG_TAG} &> ${LOG_PATH}/fits_${CFG_TAG}.log &