/**
 * $Id: mercator.c,v 0.0 2008/06/18 $
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * Mercator transformation
 * accounts for the fact that the earth is not a sphere, but a spheroid
 */
#define D_R (M_PI / 180.0)
#define R_MAJOR 6378137.0
#define R_MINOR 6356752.3142
#define RATIO (R_MINOR/R_MAJOR)
#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))
#define COM (0.5 * ECCENT)

static double deg_rad (double ang)
{
        return ang * D_R;
}

double merc_x (double lon)
{
        return R_MAJOR * deg_rad (lon);
}

double merc_y (double lat)
{
        if (lat>85) lat=85;
        if (lat<-85) lat=-85; 
        
        double phi = deg_rad(lat);
        double sinphi = sin(phi);
        double con = ECCENT * sinphi;
        con = pow(((1.0 - con) / (1.0 + con)), COM);
        double ts = tan(0.5 * ((M_PI * 0.5) - phi)) / con;
        return 0 - R_MAJOR * log(ts);
}


