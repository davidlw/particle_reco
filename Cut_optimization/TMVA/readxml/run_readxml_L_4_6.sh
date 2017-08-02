#!/bin/bash

PTBIN="L_4_6"
READXML_PATH="."
LOG_PATH="logs/${PTBIN}"
CONFIG_FILE="../config/config_${PTBIN}.xml"
EXEC="src/readxml"

source start.sh
cd ${READXML_PATH}

for i in pPb Pbp; do
    nohup unbuffer ${EXEC} ${CONFIG_FILE} "../../InputChain/config/L_${i}_4_6.xml" _${i} &> ${LOG_PATH}/readxml_${PTBIN}_${i}.log &
done