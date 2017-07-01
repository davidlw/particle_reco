#!/bin/bash

cat ../runTMVA_config.h > config.log
nohup unbuffer root -l -b -q TMVAClassification.C &> myTMVA.log &