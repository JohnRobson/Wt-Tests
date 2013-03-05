#!/bin/bash
# sh ${root}script/run.sh -a

################################################################################

project="Chart"; # choose what project to compile and run

################################################################################

root="`pwd`"
path=${root}"/"${project};

# creating directory for compile this test
rm -rf /tmp/build/wt/tests; mkdir -p /tmp/build/wt/tests; cd /tmp/build/wt/tests;

cmake ${path}

time make -j2

/tmp/build/wt/tests/test.wt --approot ${root} --docroot ${root} --http-addr 0.0.0.0 --http-port 10100

#gdb --args /tmp/build/wt/tests/test.wt --approot . --docroot . --http-addr 0.0.0.0 --http-port 10100
