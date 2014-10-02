#!/bin/sh

#for njet in 2 3 4 5 ; do 
#for nlep in 1 2 3 4 ; do
for njet in 1 2 ; do 
for nlep in 1 2 ; do
rm -f caca$njet
./runthemall.sh NJ${njet}_NL${nlep}_NLB1
done
done