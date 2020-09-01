#!/bin/bash

basis=$1

mkdir -p inp-gaussian
rm inp-gaussian/*

cd src
list=`ls *.xyz`
for i in $list
do
   Name=`basename $i .xyz`
   InName="../inp-gaussian/$Name"_"$1-$2-Opt.gjf"
   echo "New file: "$Name
   echo "%chk=$Name"_"$1-$2-Opt.chk" > $InName
   cat ../head-gaussian | sed -e "s/METHOD/$1/" | sed -e "s/BASIS/$3/" | sed -e "s/SOLV/$4/" >> $InName
   echo "$Name"_"$1-$2" >> $InName
   echo "" >> $InName
   SP="1"
   if [[ $i =~ "t-" ]] ; then 
      SP="3"
   fi
   Charge="0 $SP"
   if [[ $i =~ "-.xyz" ]] ; then 
      Charge="-1 $SP"
   fi
   if [[ $i =~ "+.xyz" ]] ; then 
      Charge="1 $SP"
   fi
   if [[ $i =~ "~" ]] ; then 
      Charge="0 $SP"
   fi
   echo "Charge: [$Charge]"
   echo "$Charge" >> $InName
   babel -ixyz $i -ogjf - | tail -n +6 >> $InName
   cat ../tail-gaussian >> $InName
#   mv $InName ../inp-gaussian
#   echo "g09.exe $InName" >> inp-gaussian/start-all.bat
done
cd ..
