#!/bin/bash

PTBIN="L_10up"
CONFIG_FILE_PATH="/home/jg61/particle_reco/TMVA/config/config_${PTBIN}.xml"
LOG_PATH="/home/jg61/particle_reco/TMVA/myTMVA/logs"
EXEC="/home/jg61/particle_reco/TMVA/myTMVA/src/TMVAClassification"

nohup unbuffer ${EXEC} ${CONFIG_FILE_PATH} &> ${LOG_PATH}/${PTBIN}.log &