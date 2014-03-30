#!/bin/bash
# sh ${root}script/run.sh -a

################################################################################

project="RInside"; # choose what project to compile and run eg:"0Simple"

################################################################################

tmp="/tmp/build/wt/tests"
root="`pwd`"
path=${root}"/"${project}

# creating directory for compile this test
rm -rf ${tmp}; mkdir -p ${tmp}; cd ${tmp};

cmake ${path}

time make -j2

#/tmp/build/wt/tests/test.wt --approot ${root} --docroot ${root} --http-addr 0.0.0.0 --http-port 10100 # uncomment this line

#gdb --args /tmp/build/wt/tests/test.wt --approot . --docroot . --http-addr 0.0.0.0 --http-port 10100


# Temporary - project: RInside

${tmp}/wtdensity --approot /home/user/projects/wt/tests/RInside --docroot /home/user/projects/wt/tests/RInside --http-addr 0.0.0.0 --http-port 10100

#/tmp/build/wt/tests/wtdensity --approot /tmp/Wt-Tests/RInside --docroot /tmp/Wt-Tests/RInside --http-addr 0.0.0.0 --http-port 10100

