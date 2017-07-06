#!/bin/bash

PTBIN="L_6_8"
READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs/${PTBIN}"
CONFIG_FILE="/home/jg61/particle_reco/TMVA/myTMVA_${PTBIN}/config_${PTBIN}.xml"
OUT_TAG="_${PTBIN}"

source start.sh
cd ${READXML_PATH}

nohup unbuffer root -l -b -q `echo 'readxml.cc+("'${CONFIG_FILE}'","/home/jg61/particle_reco/InputChain/config/dataTrees_L_highpt.xml","'${OUT_TAG}'")'` &> ${LOG_PATH}/readxml${OUT_TAG}.log &