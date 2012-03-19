/**
 *  Filename          : libgshhs.h
 *  Created by        : StephPen - stephpen@gmail.com
 *  Update            : 00:11 20/03/2012

 *  (c) 2011 by Stephane PENOT
 *      See COPYING file for copying and redistribution conditions.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Comments          :
 *
 *
 *
 *
 *
 *  Contact: <stephpen@gmail.com>
*/


#ifndef _LIBTILES_H_
#define _LIBTILES_H_

/*
===========================================================================
                               Includes
===========================================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gd.h>

#include <gdfontt.h>    /*Police gdFontTiny */
#include <gdfonts.h>    /*Police gdFontSmall */
#include <gdfontmb.h>   /*Police gdFontMediumBold */
#include <gdfontl.h>    /*Police gdFontLarge */
#include <gdfontg.h>    /*Police gdFontGiant */



#include "gshhs.h"
#include "gpc.h"
#include "PolyUtil.h"

/*
===========================================================================
                               Constants
===========================================================================
*/

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288
#endif

#ifndef DEBUG
#define DEBUG       0
#endif

#ifndef R_MAJOR
#define R_MAJOR         6378137.0
#endif

#ifndef R_MINOR
#define R_MINOR         6356752.3142
#endif

#ifndef MAXTHREADS
#define MAXTHREADS         12
#endif



/*
===========================================================================
                               Macro
===========================================================================
*/

#define MALLOC(p, b, s, t) {if ((b) > 0) { \
                            p= (t*)malloc(b); if (!(p)) { \
                            fprintf(stderr, "gpc malloc failure: %s\n", s); \
                            exit(0);}} else p= NULL;}

#define FREE(p)            {if (p) {free(p); (p)= NULL;}}

#define D_R (M_PI / 180.0)

#define R_D (180.0 / M_PI)

#define RATIO (R_MINOR/R_MAJOR)

#define ECCENT (sqrt(1.0 - (RATIO * RATIO)))

#define COM (0.5 * ECCENT)

#define MERCATOR_LIMIT RadToDeg(atan(sinh(M_PI)))  //85.051129...


/*
==================================================================================
                           Public Data Types
==================================================================================
*/

typedef struct                              /* Polygon vertex structure         */
{
    int                 x;                  /* Vertex x component               */
    int                 y;                  /* Vertex y component               */
} gshhs_vertex;

typedef struct                              /* Structure du polygone            */
{
    int                 id;                 /* Unique polygon id number         */
                                            /*    starting at 0                 */
    int                 nb_point;           /* Number of points in this polygon */
    int                 type;               /* (0) > Polygon                    */
                                            /* (1) > Line (if no area)          */
    int                 level;              /* 1 land                           */
                                            /* 2 lake                           */
                                            /* 3 island_in_lake                 */
                                            /* 4 pond_in_island_in_lake         */
    int                 version;            /* Set to 6 for GSHHS version 1.6   */
    int                 greenwich;          /* 1 if Greenwich is crossed        */
    int                 source;             /* 0 = CIA WDBII, 1 = WVS           */
    int                 river;              /* 0 = not river, 1 = river         */
    int                 long_max;           /* Longitude max in micro-degrees   */
    int                 long_min;           /* Longitude min in micro-degrees   */
    int                 lat_max;            /* Latitude max in micro-degrees    */
    int                 lat_min;            /* Latitude min in micro-degrees    */
    int                 area;               /* Area of polygon in km^2          */
    int                 area_full;          /* Area of polygon in km^2          */
    gshhs_vertex        *vertex;            /* Vertex array pointer             */
} gshhs_vertex_list;

typedef struct
{
    int                 nb_poly;            /* Nb de polygone dans le GSHHS     */
    gshhs_vertex_list   *contour;
} gshhs_polygons;

typedef struct
{
    double              x1;
    double              y1;
    double              x2;
    double              y2;
} gshhs_line;


typedef struct
{
    int                 nb_line;
    gshhs_line          *line;
} gshhs_contour;

typedef struct
{
    int                 step;
    gpc_polygon         ****poly;
} PolyData;

typedef struct
{
    int version;
    int pasx;
    int pasy;
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
} PolygonFileHeader;

typedef struct
{
    double  long_start;         // Longitude start      -> degrees [0, 360]
    double  lat_start;          // Latitude start       -> degrees [0, 360]
    double  long_extent;        // Longitude extend     -> degrees [-90, 90]
    double  lat_extent;         // Latitude extend      -> degrees [-90, 90]
    double  Zoom;               // Zoom                 -> pixel/degrees equator
    double  grid_space;         // Grid spacing         -> degrees [0, 360]
                                //                          0 = No grid
    double  LongCenter;         // Longitude center     -> degrees [0, 360]
    double  LatCenter;          // Latitude center      -> degrees [0, 360]
    int     MapWidth;           // Map Width            -> pixel
    int     MapHeight;          // Map Height           -> pixel

    int     WaterColorR;        // Water color red      -> [0-255]
    int     WaterColorG;        // Water color green    -> [0-255]
    int     WaterColorB;        // Water color blue     -> [0-255]

    int     CoastColorR;        // Coast color red      -> [0-255]
    int     CoastColorG;        // Coast color green    -> [0-255]
    int     CoastColorB;        // Coast color blue     -> [0-255]

    int     LandColorR;         // Land color red       -> [0-255]
    int     LandColorG;         // Land color green     -> [0-255]
    int     LandColorB;         // Land color blue      -> [0-255]

    int     GridColorR;         // Grid color red       -> [0-255]
    int     GridColorG;         // Grid color green     -> [0-255]
    int     GridColorB;         // Grid color blue      -> [0-255]

    char    resolution[10];     // GSHHS resolution     -> [c, l, i, h, f]
    char    projection[256];    // Map projection       -> [mercator,
                                //                          plate_carre,
                                //                          ...,
                                //                          ...]

    char    bd_path[256];       // Path to bd file
    char    bd_name[256];       // BD file name

    char    map_path[256];      // Path to map file
    char    map_name[256];      // Map file name


} CmdOrder;

typedef struct
{
    int     NCOLS;
    int     NROWS;
    double  START_X;
    double  START_Y;
    double  FINISH_X;
    double  FINISH_Y;
    double  CELLSIZE;
    short int NODATA_VALUE;
    char    NUMBERTYPE[32];
    char    ZUNITS[32];
    short int MIN_VALUE;
    short int MAX_VALUE;
} ETOPO_Header ;

typedef struct
{
    double  x;
    double  y;
} Point;

typedef struct
{
    Point   p1;
    Point   p2;
} Line;

typedef struct
{
    Point   p1;
    Point   p2;
} Rectangle;

enum { TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8 };

enum { FALSE, TRUE };

typedef struct
{
    int th_num;
    PolyData *DataIn;
    PolyData *DataOut;
    int PolyStart;
    int PolyFinish;
} ThreadData;

/*
===========================================================================
                       Public Function Prototypes
===========================================================================
*/

void    GshhsToGpcFile          (FILE               *gpc_file,
                                gshhs_polygons      *polygons,
                                int                 id_poly);

void    GshhsToGpc              (gshhs_polygons     *GshhsPolygon,
                                gpc_polygon         *GpcPolygon,
                                int                 GshhsIdPoly);
                            
void    read_gshhs              (FILE               *gshhs_file,
                                gshhs_polygons      *polygons);

void    free_gshhs              (gshhs_polygons     *polygons,
                                int                 nb_poly);

void    CreateClip              (gpc_polygon        *GpcPolygon,
                                double              xmin,
                                double              ymin,
                                double              xmax,
                                double              ymax);

void    PolyDataCreate          (PolyData           *Data,
                                int                 step);
                            
void    PolyDataDelete          (PolyData           *Data);

void    ReClipping              (PolyData           *DataIn,
                                PolyData            *DataOut); 

void *ReClippingM(void *p_data);                                
                                
void    ReadCmdFile             (FILE               *cmdfile,
                                CmdOrder            *cmd);

void    ReadPolygonFileHeader   (FILE               *polyfile,
                                PolygonFileHeader   *header);

gpc_polygon ***PolygonToMemory  (FILE               *polyfile);

void    FreePolygonToMemory     (gpc_polygon        ***PolyRAM);

void    ReadPolygonFile         (FILE               *polyfile,
                                int                 x,
                                int                 y,
                                int                 pas_x,
                                int                 pas_y,
                                gpc_polygon         *p1,
                                gpc_polygon         *p2,
                                gpc_polygon         *p3,
                                gpc_polygon         *p4);

void    FreePolygon             (gpc_polygon        *p);

void    DegToHMS                (char               *hms,
                                double              deg,
                                char                *type);

void    DrawPolygonFilled       (gdImagePtr         Image,
                                gpc_polygon         *p,
                                double              X_Origine,
                                double              Y_Origine,
                                double              Zoom,
                                int                 Fill_Color);

void    DrawPolygonContour      (gdImagePtr         Image,
                                gpc_polygon         *p,
                                int                 x,
                                int                 y,
                                int                 pas_x,
                                int                 pas_y,
                                double              X_Origine,
                                double              Y_Origine,
                                double              Zoom,
                                int                 Contour_Color);

void    DrawGrid                (gdImagePtr         Image,
                                int                 MapWidth,
                                int                 MapHeight,
                                double              long_min,
                                double              long_max,
                                double              lat_min,
                                double              lat_max,
                                double              X_Origine,
                                double              Y_Origine,
                                double              Zoom,
                                double              Grid_Space,
                                int                 Grid_Color,
                                int                 Text_Color);

void    DrawLine                (gdImagePtr         Image,
                                gshhs_contour       *p,
                                double              X_Origine,
                                double              Y_Origine,
                                double              Zoom,
                                int                 Contour_Color);

void    ReadLineFile            (FILE               *linefile,
                                int                 x,
                                int                 y,
                                gshhs_contour       *contour);

void    FreeLine                (gshhs_contour      *p);

gshhs_contour **LineToMemory    (FILE               *linefile);

void    FreeLineToMemory        (gshhs_contour      **LineRAM);

void    PolygonToGML            (gpc_polygon        *p,
                                FILE                *gmlfile,
                                int                 translate);

void    DrawEtopo               (gdImagePtr         Image,
                                FILE                *EtopoFile,
                                int                 flag_memory,
                                int                 TileDim,
                                int                 bord,
                                int                 origine_x,
                                int                 origine_y,
                                double              zoom,
                                int                 Land_Color,
                                int                 Water_Color,
                                int                 FlagAlpha);

unsigned int compute_outcode    (Point              p,
                                Rectangle           r);
                                
int cohen_sutherland            (Line               LineStart,
                                Rectangle           ClippingRectangle,
                                Line                *LineFinish);

double  MercatorLongitude               (double     lon);

double  MercatorLatitude                (double     lat);

double  MercatorLongitudeSimple         (double     lon);

double  MercatorLatitudeSimple          (double     lat);

double  MercatorInverseLongitudeSimple  (double     x);

double  MercatorInverseLatitudeSimple   (double     y);



                                
#endif

/*
===========================================================================
                           End of file: libgshhs.h
===========================================================================
*/






