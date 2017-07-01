#!/bin/bash

READXML_PATH="/home/jg61/particle_reco/TMVA/readxml"
LOG_PATH="logs"
LIB_NAME="readxml_test.so"

source start.sh
cd ${READXML_PATH}
cat ../runTMVA_config.h > ${LOG_PATH}/config_K_highpt.log

echo "Compiling..."
`root -l -b -q \'compile_readxml.cpp(\"${LIB_NAME}\")\'`
`nohup unbuffer root -l -b -q \'call_readxml.cpp(\"${LIB_NAME}\", \"config/backgroundTrees_K_highpt.xml\", \"_K_high\")\' &> ${LOG_PATH}/readxml_K_highpt.log &`
