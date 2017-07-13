#!/bin/bash

PTBIN="L_10up"
READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_NAME="/home/jg61/particle_reco/TMVA/readxml/logs/${PTBIN}/fit_histos_exec_test.log"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/config/config_${PTBIN}.xml"
EXEC="/home/jg61/particle_reco/TMVA/readxml/src/readxml_fit"
OUT_TAG="_${PTBIN}"

cd ${READXML_PATH}

nohup unbuffer ${EXEC} ${CONFIG_FILE} ${OUT_TAG} &> ${LOG_NAME} &