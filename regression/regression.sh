#!/bin/bash

# test suite definition
#   even index = test case name
#   odd index = cmd line params

TEST[0]="min1"
TEST[1]=""

TEST[2]="min2"
TEST[3]=""

TEST[4]="sub1"
TEST[5]=""

TEST[6]="sub2"
TEST[7]=""

TEST[8]="best1"
TEST[9]="-best 1"

TEST[10]="best2"
TEST[11]="-best 2"

TEST[12]="must1"
TEST[13]="-best 2"

TEST[14]="must2"
TEST[15]="-best 2"


LEN=${#TEST[*]}

# run tests
for (( i=0; i<LEN; i=i+2 ))
do
  ../src/example/tsmin ${TEST[$i+1]} < ${TEST[$i]}.in > ${TEST[$i]}.out 2> /dev/null
  diff ${TEST[$i]}.res ${TEST[$i]}.out
  RETVAL=$?; [ $RETVAL -eq 0 ] && echo ${TEST[$i]} ":" Passed; [ $RETVAL -ne 0 ] && echo ${TEST[$i]} ":" Failed
done

# clean up
rm -f *.out
