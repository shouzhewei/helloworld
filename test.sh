#!/bin/sh

if test ! -f equation_puzzle.cpp  ; then
    echo please run under sub-folder with \"sh ../sonar.sh\"
    exit 1
fi

export PATH=${PATH}:/opt/build-wrapper-linux-x86:/opt/sonar-scanner-3.2.0.1227-linux/bin
build-wrapper-linux-x86-64 --out-dir bw-output make clean all

sonar-scanner \
  -Dsonar.projectKey=cpp-sonar-way-with-cppcheck-63097 \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.sources=. \
  -Dsonar.host.url=https://sonarqube.int.net.nokia.com \
  -Dsonar.login=466ecf495ec4b338a64e45cfc272cd4098de3a92 \
  -Dsonar.analysis.mode=preview -Dsonar.issuesReport.html.enable=true \
  -Dsonar.projectName=hellotest -Dsonar.projectVersion=1 \
  -X
