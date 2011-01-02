#!/bin/bash

#
#  Filename          : clip_all.sh
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
    echo Resolution $RESOL
    echo Extraction de GSHHS
        ./readgshhs $RESOL
    echo Decoupe en carre de 45 x 45
        nice -10 python pygshhs1.py $RESOL
    echo Decoupe en carre de 15 x 15 - 5 x 5 - 1 x 1
        nice -10 python pygshhs2.py $RESOL
    echo Agglo
        ./read_bd $RESOL
    echo Copie
        cp $PATHBD/bd/bd_$RESOL/*.dat $PATHBD/bd
    echo Nettoyage
        cd bd
        nice -10 rm -r bd_$RESOL
        rm $RESOL*.dat
        cd ..
done

