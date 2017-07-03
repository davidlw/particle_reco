#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"
OUT_TAG="_L_high"

source start.sh
cd ${READXML_PATH}
cat ../runTMVA_config.h > ${LOG_PATH}/config_L_highpt.log

nohup unbuffer root -l -b -q `echo 'readxml.cc+("config/backgroundTrees_L_highpt.xml", "'${OUT_TAG}'")'` &> ${LOG_PATH}/readxml_L_highpt.log &