#
#  Filename          : Makefile
#  Created by        : StephPen - stephpen@gmail.com
#  Update            : 00:09 20/03/2012
#
#  (c) 2008 by Stephane PENOT
#      See COPYING file for copying and redistribution conditions.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Comments          :
#
#
#
#
#
#  Contact: <stephpen@gmail.com>
#


CC = gcc
CFLAGS = -Wall -O3 -std=gnu99
LDFLAGS = -lm -lgd -lpng

EXEC=clipgshhs clipline tiles_g

all: $(EXEC)

# ClipGSHHS
clipgshhs: clipgshhs.o libtiles.o gpc.o PolyUtil.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

# ClipLine
clipline: clipline.o libtiles.o gpc.o PolyUtil.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

# Tiles_Generator
tiles_g: tiles_g.o libtiles.o gpc.o PolyUtil.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)




#Cleaner!
clean:
	rm -rf *.o

cleanall: clean
	rm -rf $(EXEC)


