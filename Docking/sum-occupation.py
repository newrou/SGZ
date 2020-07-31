#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import re
import math

flag = False
mTarget = []
mLigand = []
mMod = []
mdG = []
mOcc = []
mLb = []
mUb = []
n = 0

for line in open(sys.argv[1],'r') :
	s = line.rstrip('\r\n')
	lst = s.split(';')
	mTarget.append(lst[0])
	mLigand.append(lst[1])
	mMod.append(int(lst[2]))
	mdG.append(float(lst[3]))
	mOcc.append(float(lst[4]))
	mLb.append(float(lst[5]))
	mUb.append(float(lst[6]))
	n = n + 1

#print n
#print mMod
#print mdG
#print mLb
#print mUb

sum_Occ = 0.0
min_E = mdG[0]
mi = 0
for i in range(n) :
	sum_Occ = sum_Occ + mOcc[i]
	if mdG[i] < min_E :
		min_E = mdG[i]
		mi = i

print '%s' % (sys.argv[1])
print ' min: %s; %s; %ld; %8.2f; %8.6f; %8.3f; %8.3f;' % (mTarget[mi], mLigand[mi], mMod[mi], mdG[mi], mOcc[mi], mLb[mi], mUb[mi])
print ' Sum Occupation: %.6f (%.2f %%)\n' % (sum_Occ, 100.0*sum_Occ)
