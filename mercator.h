/**
 * $Id: mercator.h,v 0.0 2008/06/18 $
 *
 * (c) 2008 by Stephane PENOT
 *      See COPYING file for copying and redistribution conditions.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; version 2 of the License.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 * Contact: <stephpen@gmail.com>
 */


#ifndef _MERCATOR_H_
#define _MERCATOR_H_
/*
===========================================================================
                               Constants
===========================================================================
*/

#define M_PI          3.14159265358979323846

/*
===========================================================================
                           Public Data Types
===========================================================================
*/



/*
===========================================================================
                       Public Function Prototypes
===========================================================================
*/

double merc_x       (double     lon);
double merc_y       (double     lat);


#endif

/*
===========================================================================
                           End of file: gpc.h
===========================================================================
*/

