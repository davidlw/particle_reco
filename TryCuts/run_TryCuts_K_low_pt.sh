#!/bin/bash

PTBIN="K_4_6"
CFG_TAG="md1"
TRYCUTS_EXEC="src/TryCuts"
LOG_PATH="logs/${PTBIN}"
CONFIG_FILE="config/${CFG_TAG}/TryCutsCfg_${PTBIN}.xml"

source start.sh

i=pPb
nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} "../InputChain/config/K_${i}_files.xml" _${i} &> ${LOG_PATH}/TryCuts_${i}_${CFG_TAG}.log &