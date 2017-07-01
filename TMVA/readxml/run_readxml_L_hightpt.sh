#!/bin/bash

LOG_PATH="logs"

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_L_highpt.xml", "_L_high")' &> ${LOG_PATH}/readxml_L_highpt.log &