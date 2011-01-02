#!/bin/bash

#
#  Filename          : clean_all.sh
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


#PATHBD=/home/spenot/gshhs
PATHBD=./

for RESOL in c l i h f; do
    echo Nettoyage
        cd bd
        nice -10 rm -r bd_$RESOL
        rm $RESOL*.dat
        cd ..
done

