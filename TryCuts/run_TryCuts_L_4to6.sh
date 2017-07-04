#!/bin/bash

TRYCUTS_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/TryCutsCfg_L_4to6.xml"

source start.sh

nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} /home/jg61/particle_reco/InputChain/config/dataTrees_L_4to6.xml _L_4to6 &> ${LOG_PATH}/L_4to6.log &