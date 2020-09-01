#!/bin/bash

list=`ls *PEP-FOLD*.zip`
for i in $list
do
    Name=`echo $i | cut -d'-' -f1`
    echo $Name
    unzip $i model1.pdb
    mv model1.pdb ../$Name-v1.pdb
done
