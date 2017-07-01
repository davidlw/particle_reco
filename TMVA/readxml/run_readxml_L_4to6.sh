#!/bin/bash

READXML_PATH="~/particle_reco/TMVA/readxml"
LOG_PATH="logs"

source start.sh
cd ${READXML_PATH}
cat ../runTMVA_config.h > ${LOG_PATH}/config_L_4to6.log

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_L_4to6.xml", "_L_4to6")' &> ${LOG_PATH}/readxml_L_4to6.log &