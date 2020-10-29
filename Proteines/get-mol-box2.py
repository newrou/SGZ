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

#print('--center_x %.3f --center_y %.3f --center_z %.3f --size_x %.3f --size_y %.3f --size_z %.3f' % (cx, cy, cz, dx, dy, dz) )

x1 = cx-dx/2
x2 = cx+dx/2
y1 = cy-dy/2
#y2 = cy+dy/2
y2 = cy
z1 = cz-dz/2
z2 = cz+dz/2

#print '8\nbbox\n'
#print 'Xe  %f  %f  %f' % (x1, y1, z1)
#print 'Xe  %f  %f  %f' % (x1, y2, z1)
#print 'Xe  %f  %f  %f' % (x2, y1, z1)
#print 'Xe  %f  %f  %f' % (x2, y2, z1)
#print 'Xe  %f  %f  %f' % (x1, y1, z2)
#print 'Xe  %f  %f  %f' % (x1, y2, z2)
#print 'Xe  %f  %f  %f' % (x2, y1, z2)
#print 'Xe  %f  %f  %f' % (x2, y2, z2)

print('--center_x %.3f --center_y %.3f --center_z %.3f --size_x %.3f --size_y %.3f --size_z %.3f' % ((x1+x2)/2, (y1+y2)/2, (z1+z2)/2, x2-x1, y2-y1, z2-z1) )
