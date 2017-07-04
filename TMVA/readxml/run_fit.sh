#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/myTMVA/config.xml"
OUT_TAG="_K_high"

source start.sh
cd ${READXML_PATH}

nohup unbuffer root -l -b -q `echo 'readxml_fit.cc("'${CONFIG_FILE}'","'${OUT_TAG}'")'` &> ${LOG_PATH}/fit_histos.log &