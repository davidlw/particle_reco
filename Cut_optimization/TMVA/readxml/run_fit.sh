#!/bin/bash

PTBIN="L_10up"
READXML_PATH="."
LOG_NAME="logs/${PTBIN}/fit_histos.log"
CONFIG_FILE="../config/config_${PTBIN}.xml"
EXEC="src/readxml_fit"
OUT_TAG="_${PTBIN}"

cd ${READXML_PATH}

nohup unbuffer ${EXEC} ${CONFIG_FILE} ${OUT_TAG} &> ${LOG_NAME} &