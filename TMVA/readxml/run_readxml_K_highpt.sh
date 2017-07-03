#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="/home/jg61/particle_reco/TMVA/readxml/logs"

source start.sh
cd ${READXML_PATH}

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_K_highpt.xml", "_K_high")' &> ${LOG_PATH}/readxml_K_highpt.log &