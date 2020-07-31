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


def GibbsR(T, n, mE) :
	R = 8.31
	mK = []
	Emin = min(mE)
	Ksum = 0.0
	for i in range(n) :
		mK.append(math.exp(-1000.0*(mE[i]-Emin)/(R*T)))
		Ksum = Ksum + mK[i]
	mKn = []
	for i in range(n) : mKn.append(mK[i]/Ksum)
	return mKn

mOcc = GibbsR(298.0, n, mdG)

for i in range(n) :
	print '%s; %s; %ld; %8.2f; %8.6f; %8.3f; %8.3f;' % (mTarget[i], mLigand[i], mMod[i], mdG[i], mOcc[i], mLb[i], mUb[i])
