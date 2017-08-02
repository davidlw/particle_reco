#!/bin/bash

PTBIN="K_4_6"
READXML_PATH="."
LOG_PATH="logs/${PTBIN}"
CONFIG_FILE="../config/config_${PTBIN}.xml"
EXEC="src/readxml"

source start.sh
cd ${READXML_PATH}

i=pPb
nohup unbuffer ${EXEC} ${CONFIG_FILE} "../../InputChain/config/K_${i}_files.xml" _${i} &> ${LOG_PATH}/readxml_${PTBIN}_${i}.log &