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

for line in sys.stdin :
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

Sum_Occ = 0
for i in range(n) :
	Sum_Occ = Sum_Occ + mOcc[i]

print 'Sum Occupation: %.6f %.2f' % (Sum_Occ, 100.0*Sum_Occ)

