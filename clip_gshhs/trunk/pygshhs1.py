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
    level = 'c'
else :
    level = sys.argv[1]





print "Passe 1"
i = 1
while i <= 5 :
    path_poly_start = path_BD + level + str(i) + '.dat'
    poly_start = Polygon(path_poly_start)
    
    x = -45
    pas1 = 45

    while x < 360 :
        y = -90
        while y < 90 :
            xc = x * 1000000
            yc = y * 1000000
            pas1c = pas1 * 1000000
            
            if x < 0 :
                path_poly_dir = path_BD + str(y) + '_to_' + str(y + pas1)
            else :
                path_poly_dir = path_BD + str(x) + '_' + str(y) + '_to_' + str(x + pas1) + '_' + str(y + pas1)
                
            if os.path.isdir(path_poly_dir) == 0 :
                os.mkdir(path_poly_dir)
            
            path_poly_finish = path_poly_dir+ '/' + level + str(i) + '.dat'
            print path_poly_finish
            
            poly_clip = Polygon(((xc, yc), (xc + pas1c, yc), (xc + pas1c, yc +pas1c), (xc, yc +pas1c)))
            poly_finish = poly_start & poly_clip

            if x < 0 :
                poly_finish.shift(360000000, 0)
        
            poly_finish.write(path_poly_finish)

            y = y + pas1
        
        x = x + pas1

    i = i + 1

# On vide la memoire
poly_start=0
poly_finish=0
poly_clip=0

print "Passe 2"
i = 1
while i <= 5 :
    y=-90
    while y<90 :
        path_poly_o = path_BD + str(y) + '_to_' + str(y + pas1) + '/' + level + str(i) + '.dat'
        path_poly_f = path_BD + '315' + '_' + str(y) + '_to_' + '360' + '_' + str(y + pas1) + '/' + level + str(i) + '.dat'
        poly_start_o = Polygon(path_poly_o)
        poly_start_f = Polygon(path_poly_f)
        poly_f = poly_start_f + poly_start_o
        poly_f.write(path_poly_f)
        
        y = y + pas1
        
    i = i+1

# On vide la memoire    
poly_start_o = 0
poly_start_f = 0
poly_f = 0

print "Passe 3"
path_BD_f = path_BD + 'bd_' + level
if os.path.isdir(path_BD_f) == 0 :
    os.mkdir(path_BD_f)
  
i=1
while i<4 :
    x=0
    while x<360 :
        y=-90
        while y<90 :
            path_poly_dir = path_BD + str(x) + '_' + str(y) + '_to_' + str(x + pas1) + '_' + str(y + pas1)
            path_poly_1 = path_poly_dir +'/' + level + str(i) + '.dat'
            poly_start_1 = Polygon(path_poly_1)
            path_poly_2 = path_poly_dir +'/' + level + str(i+1) + '.dat'
            poly_start_2 = Polygon(path_poly_2)
            
            path_poly_f_dir = path_BD_f +'/'+ str(x) + '_' + str(y) + '_to_' + str(x + pas1) + '_' + str(y + pas1)
            if os.path.isdir(path_poly_f_dir) == 0 :
                os.mkdir(path_poly_f_dir)

            path_poly_f = path_poly_f_dir+ '/' + level + str(i) + '.dat'
            print path_poly_f
            
            poly_f = poly_start_1 - poly_start_2
            poly_f.write(path_poly_f)
            
            y=y+pas1
            
        x=x+pas1
        
    i=i+2

# On vide la memoire 
poly_start_1 = 0
poly_start_2 = 0
poly_f = 0

i=5
x=0
while x<360 :
    y=-90
    while y<90 :
        path_poly_dir = path_BD + str(x) + '_' + str(y) + '_to_' + str(x + pas1) + '_' + str(y + pas1)
        path_poly_1 = path_poly_dir +'/' + level + str(i) + '.dat'
        poly_start_1 = Polygon(path_poly_1)
            
        path_poly_f_dir = path_BD_f +'/'+ str(x) + '_' + str(y) + '_to_' + str(x + pas1) + '_' + str(y + pas1)

        path_poly_f = path_poly_f_dir+ '/' + level + str(i) + '.dat'
        print path_poly_f
        poly_start_1.write(path_poly_f)
            
        y=y+pas1
            
    x=x+pas1

# On vide la memoire 
poly_start_1 = 0



