#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import re

xmin = 0.0
ymin = 0.0
zmin = 0.0

xmax = 0.0
ymax = 0.0
zmax = 0.0

for line in sys.stdin :
	s = line.rstrip('\r\n')
	if ('HETATM' in s) or ('ATOM' in s) :
		x = float(s[30:37])
		y = float(s[38:45])
		z = float(s[46:53])
		if x < xmin : xmin = x
		if y < ymin : ymin = y
		if z < zmin : zmin = z
		if x > xmax : xmax = x
		if y > ymax : ymax = y
		if z > zmax : zmax = z

cx = (xmax + xmin)/2
cy = (ymax + ymin)/2
cz = (zmax + zmin)/2

dx = abs(xmax - xmin)*1.2
dy = abs(ymax - ymin)*1.2
dz = abs(zmax - zmin)*1.2

print('--center_x %.3f --center_y %.3f --center_z %.3f --size_x %.3f --size_y %.3f --size_z %.3f' % (cx, cy, cz, dx, dy, dz) )
