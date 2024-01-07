#!/bin/bash
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/setup.sh

if [[ $0 =~ ^\/.* ]]
then
    SH_NAME=$0
else
    SH_NAME=$(pwd)/$0
fi

SH_PATH=$(readlink -f ${SH_NAME%/*})
alias heprep="java -jar ${SH_PATH}/external/HepRApp/HepRApp.jar"
