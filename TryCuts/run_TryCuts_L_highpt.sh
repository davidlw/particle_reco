#!/bin/bash

TRYCUTS_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/TryCutsCfg_L_highpt.xml"

source start.sh

nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} /home/jg61/particle_reco/InputChain/config/dataTrees_L_highpt.xml _L_highpt &> ${LOG_PATH}/L_highpt.log &