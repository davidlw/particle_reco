#!/bin/bash

READXML_PATH="~/particle_reco/TMVA/readxml"
LOG_PATH="logs"

echo "ssh bonner01"
ssh bonner01 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_1a.xml", "_K_1a")' &> ${LOG_PATH}/readxml_K_lowpt_1a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_1b.xml", "_K_1b")' &> ${LOG_PATH}/readxml_K_lowpt_1b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_1c.xml", "_K_1c")' &> ${LOG_PATH}/readxml_K_lowpt_1c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_2a.xml", "_K_2a")' &> ${LOG_PATH}/readxml_K_lowpt_2a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_2b.xml", "_K_2b")' &> ${LOG_PATH}/readxml_K_lowpt_2b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_2c.xml", "_K_2c")' &> ${LOG_PATH}/readxml_K_lowpt_2c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_3a.xml", "_K_3a")' &> ${LOG_PATH}/readxml_K_lowpt_3a.log &

HERE



echo "ssh bonner02"
ssh bonner02 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_3b.xml", "_K_3b")' &> ${LOG_PATH}/readxml_K_lowpt_3b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_3c.xml", "_K_3c")' &> ${LOG_PATH}/readxml_K_lowpt_3c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_4a.xml", "_K_4a")' &> ${LOG_PATH}/readxml_K_lowpt_4a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_4b.xml", "_K_4b")' &> ${LOG_PATH}/readxml_K_lowpt_4b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_4c.xml", "_K_4c")' &> ${LOG_PATH}/readxml_K_lowpt_4c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_5a.xml", "_K_5a")' &> ${LOG_PATH}/readxml_K_lowpt_5a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_5b.xml", "_K_5b")' &> ${LOG_PATH}/readxml_K_lowpt_5b.log &

HERE


echo "ssh bonner03"
ssh bonner03 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_5c.xml", "_K_5c")' &> ${LOG_PATH}/readxml_K_lowpt_5c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_6a.xml", "_K_6a")' &> ${LOG_PATH}/readxml_K_lowpt_6a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_6b.xml", "_K_6b")' &> ${LOG_PATH}/readxml_K_lowpt_6b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_6c.xml", "_K_6c")' &> ${LOG_PATH}/readxml_K_lowpt_6c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_7a.xml", "_K_7a")' &> ${LOG_PATH}/readxml_K_lowpt_7a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_7b.xml", "_K_7b")' &> ${LOG_PATH}/readxml_K_lowpt_7b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_7c.xml", "_K_7c")' &> ${LOG_PATH}/readxml_K_lowpt_7c.log &

HERE


echo "ssh bonner04"
ssh bonner04 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_8a.xml", "_K_8a")' &> ${LOG_PATH}/readxml_K_lowpt_8a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_8b.xml", "_K_8b")' &> ${LOG_PATH}/readxml_K_lowpt_8b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_8c.xml", "_K_8c")' &> ${LOG_PATH}/readxml_K_lowpt_8c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_9a.xml", "_K_9a")' &> ${LOG_PATH}/readxml_K_lowpt_9a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_9b.xml", "_K_9b")' &> ${LOG_PATH}/readxml_K_lowpt_9b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_9c.xml", "_K_9c")' &> ${LOG_PATH}/readxml_K_lowpt_9c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_10a.xml", "_K_10a")' &> ${LOG_PATH}/readxml_K_lowpt_10a.log &

HERE



echo "ssh bonner05"
ssh bonner05 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_10b.xml", "_K_10b")' &> ${LOG_PATH}/readxml_K_lowpt_10b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_10c.xml", "_K_10c")' &> ${LOG_PATH}/readxml_K_lowpt_10c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_11a.xml", "_K_11a")' &> ${LOG_PATH}/readxml_K_lowpt_11a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_11b.xml", "_K_11b")' &> ${LOG_PATH}/readxml_K_lowpt_11b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_11c.xml", "_K_11c")' &> ${LOG_PATH}/readxml_K_lowpt_11c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_12a.xml", "_K_12a")' &> ${LOG_PATH}/readxml_K_lowpt_12a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_12b.xml", "_K_12b")' &> ${LOG_PATH}/readxml_K_lowpt_12b.log &

HERE



echo "ssh bonner06"
ssh bonner06 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_12c.xml", "_K_12c")' &> ${LOG_PATH}/readxml_K_lowpt_12c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_13a.xml", "_K_13a")' &> ${LOG_PATH}/readxml_K_lowpt_13a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_13b.xml", "_K_13b")' &> ${LOG_PATH}/readxml_K_lowpt_13b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_13c.xml", "_K_13c")' &> ${LOG_PATH}/readxml_K_lowpt_13c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_14a.xml", "_K_14a")' &> ${LOG_PATH}/readxml_K_lowpt_14a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_14b.xml", "_K_14b")' &> ${LOG_PATH}/readxml_K_lowpt_14b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_14c.xml", "_K_14c")' &> ${LOG_PATH}/readxml_K_lowpt_14c.log &

HERE


echo "ssh bonner07"
ssh bonner07 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_15a.xml", "_K_15a")' &> ${LOG_PATH}/readxml_K_lowpt_15a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_15b.xml", "_K_15b")' &> ${LOG_PATH}/readxml_K_lowpt_15b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_15c.xml", "_K_15c")' &> ${LOG_PATH}/readxml_K_lowpt_15c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_16a.xml", "_K_16a")' &> ${LOG_PATH}/readxml_K_lowpt_16a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_16b.xml", "_K_16b")' &> ${LOG_PATH}/readxml_K_lowpt_16b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_K/backgroundTrees_K_16c.xml", "_K_16c")' &> ${LOG_PATH}/readxml_K_lowpt_16c.log &

HERE

echo
echo "Finished executing."
echo