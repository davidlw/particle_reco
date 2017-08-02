#!/bin/bash

PTBIN="L_10up"
CONFIG_FILE_PATH="../config/config_${PTBIN}.xml"
LOG_PATH="logs"
EXEC="src/TMVAClassification"

nohup unbuffer ${EXEC} ${CONFIG_FILE_PATH} &> ${LOG_PATH}/${PTBIN}.log &