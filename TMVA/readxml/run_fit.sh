#!/bin/bash

PTBIN="K_2_4"
READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/myTMVA_${PTBIN}/config_${PTBIN}.xml"
OUT_TAG="_${PTBIN}"

source start.sh
cd ${READXML_PATH}

nohup unbuffer root -l -b -q `echo 'readxml_fit.cc("'${CONFIG_FILE}'","'${OUT_TAG}'")'` &> ${LOG_PATH}/fit_histos.log &