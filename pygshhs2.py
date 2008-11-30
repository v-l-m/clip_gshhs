#!/usr/bin/env python
# -*- coding: utf-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Copyright (C) 2007 Free Software Fundation

# auteur : Stephpen




from Polygon import *
import os.path
import sys


path = '/home/steph/gshhs/new2/'
path_BD = path + 'bd/'
if len(sys.argv) < 2 :
    print "Use pygshhs.py [c, l, i, h, f] !"
    print "By default it's resolution c"
    resol = 'c'
else :
    resol = sys.argv[1]





pas = 45        
x=0
while x<360 :
    y=-90
    while y<90 :
        path_poly_dir = path_BD + 'bd_' + resol + '/' + str(x) + '_' + str(y) + '_to_' + str(x + pas) + '_' + str(y + pas) + '/'
        
        level_all = ['1', '3', '5']
        for level in level_all :
            poly_file = path_poly_dir + resol + level + '.dat'
            poly_start = Polygon(poly_file)
            poly_start.scale(1/1000000.0, 1/1000000.0 , 0.0, 0.0)
            poly_start.write(poly_file)
            
        y=y+pas
            
    x=x+pas

