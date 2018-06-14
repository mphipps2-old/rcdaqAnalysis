#! /bin/sh


beg=$1
end=$2
inc=$3

for ((fileNum=beg; fileNum <= end; fileNum+=inc)); do  
f=$(printf '/home/uiuc/data/run/run_00000%d-0000.prdf' $fileNum)
root -b -q 'ProcessRawData.C("'$f'")'
#sleep 40
done
