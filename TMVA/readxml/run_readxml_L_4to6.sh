#!/bin/bash

LOG_PATH="logs"

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_L_4to6.xml", "_L_4to6")' &> ${LOG_PATH}/readxml_L_4to6.log &