/*
 *	Filename		: map.c

 *	Created			: 07 January 2009 (23:08:51)
 *	Created by		: StephPen - stephpen@gmail.com

 *	Last Updated	: 08 January 2009 (23:09:12)
 *	Updated by		: steph

 *	(c) 2008 by Stephane PENOT
 *	    See COPYING file for copying and redistribution conditions.
 *	 
 *	    This program is free software; you can redistribute it and/or modify
 *	    it under the terms of the GNU General Public License as published by
 *	    the Free Software Foundation; version 2 of the License.
 *	 
 *	    This program is distributed in the hope that it will be useful,
 *	    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	    GNU General Public License for more details.
 *	 
 *	Comments		: 
 *	 
 *	 
 *	 
 *	 
 *	 
 *	Contact: <stephpen@gmail.com>
*/


#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <math.h>

#include "gpc.h"
#include "map_functions.h"
#include "map_projection.h"
#include "gshhs.h"



int main (int argc, char **argv)
{
    
    FILE    *cmdfile;
    FILE    *polyfile;
    CmdOrder    cmd;
    
    char    poly_file_name[256];
    PolygonFileHeader     header;
    gpc_polygon p1, p2, p3, p4, p5;
    gpc_polygon p1p, p2p, p3p, p4p, p5p;
    
    int x, y;
    
    gdImagePtr image; /* Pointeur vers notre image */
    FILE *image_png; /* Fichier image PNG */
    int water_color, coast_color, land_color, grid_color;
    int text_color;
    double long_max, long_min, lat_max, lat_min;
    int long_max_int, long_min_int, lat_max_int, lat_min_int;
    int x_image, y_image;
    double origine_x, origine_y;
    int c, v;
    gdPoint *poly_pt;
    double c1, c2, c3;
        
    
    if (argc < 2 || argc > 3) {
        fprintf (stderr, "Map Generator V0.0 - 02/01/2009\n");
        fprintf (stderr, "Stephpen@gmail.com\n");
        fprintf (stderr, "Usage:  map command_file\n\n");
        
        exit (EXIT_FAILURE);
    }
    
    cmdfile = fopen(argv[1],"r");
    if (cmdfile == NULL)
    {
        printf("Impossible d'ouvrir le fichier: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    ReadCmdFile(cmdfile, &cmd);
    
    fclose(cmdfile);
    
    //printf("Long_Min: %f\n", cmd.long_start);
    
    sprintf(poly_file_name, "%s/%s", cmd.bd_path, cmd.bd_name);
    polyfile = fopen(poly_file_name, "rb");
    if (polyfile == NULL)
    {
        printf("Impossible d'ouvrir le fichier: %s\n", poly_file_name);
        exit(EXIT_FAILURE);
    }
    
    ReadPolygonFileHeader (polyfile, &header);
    //printf("Header_Pasx: %d\n", header.pasx);

    //Détermination des longitudes mini, maxi, et origine image x
    long_min = cmd.LongCenter - (cmd.MapWidth/2) / cmd.Zoom;
    long_max = cmd.LongCenter + (cmd.MapWidth/2) / cmd.Zoom;
    printf("long_min: %lf, long_max: %lf\n", long_min, long_max);
    
    if (long_min>=0)    long_min_int=   floor(fabs(long_min));
    if (long_min<0)     long_min_int=  -ceil(fabs(long_min));
    if (long_max>=0)    long_max_int=   ceil(fabs(long_max));
    if (long_max<0)     long_max_int=  -floor(fabs(long_max));

    origine_x = -long_min * cmd.Zoom;

    printf("long_min: %d, long_max: %d, origine_x: %lf\n", long_min_int, long_max_int, origine_x);
    
    
    //Détermination des latitudes mini, maxi, et origine image y
    lat_min = MercatorInverseLatitudeSimple(((MercatorLatitudeSimple(cmd.LatCenter)*180*cmd.Zoom/M_PI)-(cmd.MapHeight/2)) * M_PI/(180*cmd.Zoom));
    lat_max = MercatorInverseLatitudeSimple(((MercatorLatitudeSimple(cmd.LatCenter)*180*cmd.Zoom/M_PI)+(cmd.MapHeight/2)) * M_PI/(180*cmd.Zoom));
    printf("lat_min: %lf, lat_max: %lf\n", lat_min, lat_max);
    
    if (lat_min>=0)    lat_min_int=     floor(fabs(lat_min));
    if (lat_min<0)     lat_min_int=    -ceil(fabs(lat_min));
    if (lat_max>=0)    lat_max_int=     ceil(fabs(lat_max));
    if (lat_max<0)     lat_max_int=    -floor(fabs(lat_max));

    origine_y = -MercatorLatitudeSimple(lat_min)*180.0*cmd.Zoom/M_PI;

    printf("lat_min: %d, lat_max: %d, origine_y: %lf\n", lat_min_int, lat_max_int, origine_y);
    
    
    // Création de l'image
    printf("Map_Width: %d, Map_Height: %d\n", cmd.MapWidth, cmd.MapHeight);
    image = gdImageCreate(cmd.MapWidth, cmd.MapHeight);
    
    // Création des couleurs
    water_color =   gdImageColorAllocate(image, cmd.WaterColorR,    cmd.WaterColorG,    cmd.WaterColorB);
    land_color  =   gdImageColorAllocate(image, cmd.LandColorR,     cmd.LandColorG,     cmd.LandColorB);
    coast_color =   gdImageColorAllocate(image, cmd.CoastColorR,    cmd.CoastColorG,    cmd.CoastColorB);
    grid_color  =   gdImageColorAllocate(image, cmd.GridColorR,     cmd.GridColorG,     cmd.GridColorB);
    text_color  =   gdImageColorAllocate(image, 0,                  0,                  0); 
    
    
    // Cas où tout va bien
    if (long_min_int>=0 && long_max_int<=360)
    {
        for (x=long_min_int; x<long_max_int; x=x+header.pasx)
        {
            for (y=lat_min_int; y<lat_max_int; y=y+header.pasy)
            {
                ReadPolygonFile(polyfile, x, y, header.pasx, header.pasy, &p1, &p2, &p3, &p4, &p5);

                DrawPolygonFilled(image, &p1, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p2, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p3, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p4, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p5, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                
                DrawPolygonContour(image, &p1, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p2, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p3, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p4, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p5, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                
                FreePolygon(&p1);
                FreePolygon(&p2);
                FreePolygon(&p3);
                FreePolygon(&p4);
                FreePolygon(&p5);
            
            }
        }
        DrawGrid(image, cmd.MapWidth, cmd.MapHeight, long_min, long_max, lat_min, lat_max, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, cmd.grid_space, grid_color, text_color);

    }
    // Cas où long_min <0
    if (long_min_int<0)
    {
        for (x=0; x<long_max_int; x=x+header.pasx)
        {
            for (y=lat_min_int; y<lat_max_int; y=y+header.pasy)
            {
                ReadPolygonFile(polyfile, x, y, header.pasx, header.pasy, &p1, &p2, &p3, &p4, &p5);
            
                DrawPolygonFilled(image, &p1, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p2, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p3, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p4, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p5, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);

                DrawPolygonContour(image, &p1, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p2, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p3, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p4, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p5, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                
                FreePolygon(&p1);
                FreePolygon(&p2);
                FreePolygon(&p3);
                FreePolygon(&p4);
                FreePolygon(&p5);
            
            }
        }
        
        for (x=long_min_int+360; x<360; x=x+header.pasx)
        {
            for (y=lat_min_int; y<lat_max_int; y=y+header.pasy)
            {
                ReadPolygonFile(polyfile, x, y, header.pasx, header.pasy, &p1, &p2, &p3, &p4, &p5);

                DrawPolygonFilled(image, &p1, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p2, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p3, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p4, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p5, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, land_color);

                DrawPolygonContour(image, &p1, x, y, header.pasx, header.pasy, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p2, x, y, header.pasx, header.pasy, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p3, x, y, header.pasx, header.pasy, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p4, x, y, header.pasx, header.pasy, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p5, x, y, header.pasx, header.pasy, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                
                FreePolygon(&p1);
                FreePolygon(&p2);
                FreePolygon(&p3);
                FreePolygon(&p4);
                FreePolygon(&p5);
            
            }
        }
        DrawGrid(image, cmd.MapWidth, cmd.MapHeight, long_min, long_max, lat_min, lat_max, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, cmd.grid_space, grid_color, text_color);
        //DrawGrid(image, cmd.MapWidth, cmd.MapHeight, long_min, long_max, lat_min, lat_max, origine_x-360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, cmd.grid_space, grid_color);

    }
    
    // Cas où long_max >360
    if (long_max_int>360)
    {
        for (x=long_min_int; x<360; x=x+header.pasx)
        {
            for (y=lat_min_int; y<lat_max_int; y=y+header.pasy)
            {
                ReadPolygonFile(polyfile, x, y, header.pasx, header.pasy, &p1, &p2, &p3, &p4, &p5);

                DrawPolygonFilled(image, &p1, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p2, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p3, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p4, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p5, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, land_color);

                DrawPolygonContour(image, &p1, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p2, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p3, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p4, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p5, x, y, header.pasx, header.pasy, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                
                FreePolygon(&p1);
                FreePolygon(&p2);
                FreePolygon(&p3);
                FreePolygon(&p4);
                FreePolygon(&p5);
            
            }
        }
        
        for (x=0; x<long_max_int-360; x=x+header.pasx)
        {
            for (y=lat_min_int; y<lat_max_int; y=y+header.pasy)
            {
                ReadPolygonFile(polyfile, x, y, header.pasx, header.pasy, &p1, &p2, &p3, &p4, &p5);

                DrawPolygonFilled(image, &p1, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p2, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p3, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, land_color);
                DrawPolygonFilled(image, &p4, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, water_color);
                DrawPolygonFilled(image, &p5, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, land_color);

                DrawPolygonContour(image, &p1, x, y, header.pasx, header.pasy, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p2, x, y, header.pasx, header.pasy, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p3, x, y, header.pasx, header.pasy, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p4, x, y, header.pasx, header.pasy, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                DrawPolygonContour(image, &p5, x, y, header.pasx, header.pasy, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, coast_color);
                
                FreePolygon(&p1);
                FreePolygon(&p2);
                FreePolygon(&p3);
                FreePolygon(&p4);
                FreePolygon(&p5);
            
            }
        }
        DrawGrid(image, cmd.MapWidth, cmd.MapHeight, long_min, long_max, lat_min, lat_max, origine_x, cmd.MapHeight-origine_y, cmd.Zoom, cmd.grid_space, grid_color, text_color);
        //DrawGrid(image, cmd.MapWidth, cmd.MapHeight, long_min, long_max, lat_min, lat_max, origine_x+360*cmd.Zoom, cmd.MapHeight-origine_y, cmd.Zoom, cmd.grid_space, grid_color);

    }
     
    image_png = fopen("map.png", "w");
    gdImagePng(image, image_png);
    fclose(image_png);
    gdImageDestroy(image);

    
    fclose(polyfile);
    
    return 0;
    
}



