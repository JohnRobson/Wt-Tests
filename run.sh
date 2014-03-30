#!/bin/bash
# sh ${root}script/run.sh -a

################################################################################

project="Emails"; # choose what project to compile and run eg:"0Simple"
output="/tmp/build/wt/tests/"${project} # output compiled files

################################################################################

root="`pwd`"
path=${root}"/"${project}

# creating directory for compile this test
rm -rf ${output}; mkdir -p ${output}; cd ${output};

cmake ${path}

time make -j8

${output}/test.wt --approot ${root}/${project} --docroot ${root}/${project} --http-addr 0.0.0.0 --http-port 10100

#gdb -ex run --args ${output}/wtdensity --approot ${root}/${project} --docroot ${root}/${project} --http-addr 0.0.0.0 --http-port 10100

# Temporary RInside
#${output}/wtdensity --approot ${root}/${project} --docroot ${root}/${project} --http-addr 0.0.0.0 --http-port 10100
