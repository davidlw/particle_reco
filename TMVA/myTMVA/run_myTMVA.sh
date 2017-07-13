#!/bin/bash

config_file_path="/home/jg61/particle_reco/TMVA/myTMVA/config/config_L_10up.xml"

nohup unbuffer root -l -b -q `echo 'TMVAClassification.C+("'${config_file_path}'")'` &> myTMVA.log &