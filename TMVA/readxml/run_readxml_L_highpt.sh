#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"

source start.sh
cd ${READXML_PATH}
cat ../runTMVA_config.h > ${LOG_PATH}/config_L_highpt.log

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_L_highpt.xml", "_L_high")' &> ${LOG_PATH}/readxml_L_highpt.log &