#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import re

fname = sys.argv[1].split('.')[0]
print(fname)

flag = False

for line in open('%s.pdb' % (fname), 'r') :
#	s = line.rstrip('\r\n')
	s = line
	if s[:5]=='MODEL' :
		n = int(s[6:])
		print('Model:%3d' % (n))
		fout = open('%s-v%02ld.pdb' % (fname, n), 'w')
		flag = True
	if s[:6]=='ENDMDL' :
		if len(ter)>0 : fout.write('%s' % (ter))
		fout.write('%s' % (s))
		fout.close()
		flag = False
	if s[:3]=='TER' and flag :
		ter = s
		continue
	if flag :
		fout.write('%s' % (s))


#MODEL        2                                                                  
#ENDMDL                                                                          
