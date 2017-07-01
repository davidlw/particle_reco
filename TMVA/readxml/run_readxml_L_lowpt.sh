#!/bin/bash

READXML_PATH="~/particle_reco/TMVA/readxml"
LOG_PATH="logs"

echo "ssh bonner01"
ssh bonner01 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1a.xml", "_L_1a")' &> ${LOG_PATH}/readxml_L_lowpt_1a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1b.xml", "_L_1b")' &> ${LOG_PATH}/readxml_L_lowpt_1b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1c.xml", "_L_1c")' &> ${LOG_PATH}/readxml_L_lowpt_1c.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1d.xml", "_L_1d")' &> ${LOG_PATH}/readxml_L_lowpt_1d.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1e.xml", "_L_1e")' &> ${LOG_PATH}/readxml_L_lowpt_1e.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1f.xml", "_L_1f")' &> ${LOG_PATH}/readxml_L_lowpt_1f.log &

HERE



echo "ssh bonner02"
ssh bonner02 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1g.xml", "_L_1g")' &> ${LOG_PATH}/readxml_L_lowpt_1g.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_1h.xml", "_L_1h")' &> ${LOG_PATH}/readxml_L_lowpt_1h.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_2a.xml", "_L_2a")' &> ${LOG_PATH}/readxml_L_lowpt_2a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_2b.xml", "_L_2b")' &> ${LOG_PATH}/readxml_L_lowpt_2b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_3a.xml", "_L_3a")' &> ${LOG_PATH}/readxml_L_lowpt_3a.log &

HERE


echo "ssh bonner03"
ssh bonner03 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_3b.xml", "_L_3b")' &> ${LOG_PATH}/readxml_L_lowpt_3b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_4a.xml", "_L_4a")' &> ${LOG_PATH}/readxml_L_lowpt_4a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_4b.xml", "_L_4b")' &> ${LOG_PATH}/readxml_L_lowpt_4b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_5a.xml", "_L_5a")' &> ${LOG_PATH}/readxml_L_lowpt_5a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_5b.xml", "_L_5b")' &> ${LOG_PATH}/readxml_L_lowpt_5b.log &

HERE


echo "ssh bonner04"
ssh bonner04 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_6a.xml", "_L_6a")' &> ${LOG_PATH}/readxml_L_lowpt_6a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_6b.xml", "_L_6b")' &> ${LOG_PATH}/readxml_L_lowpt_6b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_7a.xml", "_L_7a")' &> ${LOG_PATH}/readxml_L_lowpt_7a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_7b.xml", "_L_7b")' &> ${LOG_PATH}/readxml_L_lowpt_7b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_8a.xml", "_L_8a")' &> ${LOG_PATH}/readxml_L_lowpt_8a.log &

HERE



echo "ssh bonner05"
ssh bonner05 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_8b.xml", "_L_8b")' &> ${LOG_PATH}/readxml_L_lowpt_8b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_9.xml", "_L_9")' &> ${LOG_PATH}/readxml_L_lowpt_9.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_10a.xml", "_L_10a")' &> ${LOG_PATH}/readxml_L_lowpt_10a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_10b.xml", "_L_10b")' &> ${LOG_PATH}/readxml_L_lowpt_10b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_11a.xml", "_L_11a")' &> ${LOG_PATH}/readxml_L_lowpt_11a.log &

HERE



echo "ssh bonner06"
ssh bonner06 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_11b.xml", "_L_11b")' &> ${LOG_PATH}/readxml_L_lowpt_11b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_12a.xml", "_L_12a")' &> ${LOG_PATH}/readxml_L_lowpt_12a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_12b.xml", "_L_12b")' &> ${LOG_PATH}/readxml_L_lowpt_12b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_13a.xml", "_L_13a")' &> ${LOG_PATH}/readxml_L_lowpt_13a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_13b.xml", "_L_13b")' &> ${LOG_PATH}/readxml_L_lowpt_13b.log &

HERE


echo "ssh bonner07"
ssh bonner07 << HERE

source start.sh
cd ${READXML_PATH}

echo "Executing..."
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_14a.xml", "_L_14a")' &> ${LOG_PATH}/readxml_L_lowpt_14a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_14b.xml", "_L_14b")' &> ${LOG_PATH}/readxml_L_lowpt_14b.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_15.xml", "_L_15")' &> ${LOG_PATH}/readxml_L_lowpt_15.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_16a.xml", "_L_16a")' &> ${LOG_PATH}/readxml_L_lowpt_16a.log &
nohup unbuffer root -l -b -q 'readxml.cc+("config/split_L/backgroundTrees_L_16b.xml", "_L_16b")' &> ${LOG_PATH}/readxml_L_lowpt_16b.log &

HERE

echo
echo "Finished executing."
echo