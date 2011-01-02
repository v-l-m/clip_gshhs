#!/bin/bash

#
#  Filename          : etopo.sh
#  Created by        : StephPen - stephpen@gmail.com
#  Update            : 11:14 02/01/2011
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


ETOPO=$1
ETOPO_DAT=$2

# Download ETOPO files
# wget http://www.ngdc.noaa.gov/mgg/global/relief/ETOPO1/data/ice_surface/grid_registered/xyz/ETOPO1_Ice_g_int.xyz.gz
# wget http://www.ngdc.noaa.gov/mgg/global/relief/ETOPO1/data/bedrock/grid_registered/xyz/ETOPO1_Bed_g_int.xyz.gz

echo Decompression
gunzip $ETOPO.gz

echo Passe 1
./compile_etopo $ETOPO $ETOPO.dat

echo Passe finale
./compile_etopo_060 $ETOPO.dat $ETOPO_DAT.dat

rm $ETOPO.dat
