#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"

source start.sh
cd ${READXML_PATH}

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_L_4to6.xml", "_L_4to6")' &> ${LOG_PATH}/readxml_L_4to6.log &