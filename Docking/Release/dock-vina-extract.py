#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import re

flag = False
mMod = []
mdG = []
mLb = []
mUb = []
n = 0

(Target, Ligand) = sys.argv[1].split('~')
#print Target, Ligand

for line in sys.stdin :
	s = line.rstrip('\r\n')
	if '-----+------------+----------+----------' in s :
		flag = True
		continue
	if 'Writing output ... done.' in s :
		flag = False
		continue
	if flag == True :
		mMod.append(int(s[0:5]))
		mdG.append(4.184*float(s[7:18]))
		mLb.append(float(s[20:32]))
		mUb.append(float(s[32:40]))
		n = n + 1

#print n
#print mMod
#print mdG
#print mLb
#print mUb
for i in range(n) :
	print '%s; %s; %ld; %8.2f; %6.3f; %8.3f; %8.3f;' % (Target, Ligand, mMod[i], mdG[i], 0.0, mLb[i], mUb[i])

#mode |   affinity | dist from best mode
#     | (kcal/mol) | rmsd l.b.| rmsd u.b.
#-----+------------+----------+----------
#   1         -2.3      0.000      0.000
#   2         -2.0     12.103     23.636
#   3         -1.9      1.644      2.298
#   4         -1.9      8.680     17.834
#   5         -1.7      3.048      4.416
#   6         -1.5      3.054      7.160
#   7         -1.2     12.084     23.481
#   8         -1.1      3.156      7.167
#   9         -1.1      3.433      5.914
#Writing output ... done.
