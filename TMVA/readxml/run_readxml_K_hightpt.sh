#!/bin/bash

LOG_PATH="logs"

nohup unbuffer root -l -b -q 'readxml.cc+("config/backgroundTrees_K_highpt.xml", "_K_high")' &> ${LOG_PATH}/readxml_K_highpt.log &