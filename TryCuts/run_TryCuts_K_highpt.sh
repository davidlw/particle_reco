#!/bin/bash

TRYCUTS_EXEC="/home/jg61/particle_reco/TryCuts/src/TryCuts"
LOG_PATH="/home/jg61/particle_reco/TryCuts/logs"
CONFIG_FILE="/home/jg61/particle_reco/TryCuts/config/TryCutsCfg_K_highpt.xml"

source start.sh

nohup unbuffer ${TRYCUTS_EXEC} ${CONFIG_FILE} /home/jg61/particle_reco/InputChain/config/dataTrees_K_highpt.xml _K_highpt &> ${LOG_PATH}/K_highpt.log &