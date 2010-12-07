/**
 *    Filename          : readlines.c

 *    Created           : 11:40 05/12/2010
 *    Created by        : StephPen - stephpen@gmail.com

 *    Last Updated      : 11:40 05/12/2010
 *    Updated by        : StephPen - stephpen@gmail.com

 *    (c) 2008 by Stephane PENOT
 *        See COPYING file for copying and redistribution conditions.
 *
 *        This program is free software; you can redistribute it and/or modify
 *        it under the terms of the GNU General Public License as published by
 *        the Free Software Foundation; version 2 of the License.
 *
 *        This program is distributed in the hope that it will be useful,
 *        but WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *        GNU General Public License for more details.
 *
 *    Comments          :
 *
 *
 *
 *
 *
 *    Contact: <stephpen@gmail.com>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>
#include <gd.h>

#include "read_gshhs.h"
#include "gpc.h"
#include "map_functions.h"
#include "map_projection.h"
#include "gshhs.h"

int main (int argc, char **argv)
{
    FILE *gshhs_file;
    char path_gshhs_file[256];
    gshhs_polygons *polygons;

    int n;

    gshhs_contour *contour[360][180];
    gshhs_line *temp_contour;
    int u, v, i, j, k;
    int xmin, xmax, x1min, x1max, x2min, x2max;
    int ymin, ymax, y1min, y1max, y2min, y2max;

    PolygonFileHeader header_out;

    FILE  *out_file;
    char path_out_file[256];

    int LineType;

    int pos_data;
    int tab_data;

    int result;
    Rectangle ClipRect;
    Line InitialLine, FinalLine;

    //Gestion de la ligne de commande
    printf("Gestion de la ligne de commande\n");
    if (argc < 4 || argc > 5)
    {
        fprintf (stderr, "Readlines\n");
        fprintf (stderr, "Usage:  readline GSHHS_File Type Out_File\n");
        fprintf (stderr, "GSHHS_file: gshhs_[c, l, i, h, f].b (complete path)\n");
        fprintf (stderr, "            wdb_borders_[c, l, i, h, f].b (complete path)\n");
        fprintf (stderr, "            wdb_rivers_[c, l, i, h, f].b (complete path)\n");
        fprintf (stderr, "Type:       0 for gshhs_[c, l, i, h, f].b\n");
        fprintf (stderr, "            1 for wdb_[borders, rivers]_[c, l, i, h, f].b\n");
        fprintf (stderr, "Out_File:   output file (complete path)\n");
        fprintf (stderr, "Thanks\n");
        exit (EXIT_FAILURE);
    }
    sscanf(argv[1], "%s", path_gshhs_file);
    sscanf(argv[2], "%d", &LineType);
    sscanf(argv[3], "%s", path_out_file);

    if (DEBUG) printf("Gshhs file: %s, LineType: %d, OutFile: %s\n", path_gshhs_file, LineType, path_out_file);

    // Ouverture du fichier GSHHS
    printf("Ouverture du fichier GSHHS\n");
    if ((gshhs_file = fopen (path_gshhs_file, "rb")) == NULL ) {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", path_gshhs_file);
        exit (EXIT_FAILURE);
    }


    // Allocation de la memoire pour stocker toutes les données du GSHHS
    printf("Allocation de la memoire pour stocker toutes les données du GSHHS\n");
    polygons = NULL;
    polygons = malloc(1 * sizeof(gshhs_polygons));
    if ( polygons == NULL )
    {
        fprintf(stderr,"Allocation impossible \n");
        exit(EXIT_FAILURE);
    }

    // Allocation de la memoire pour stocker toutes les données par tranche de 1deg x 1deg
    printf("Allocation de la memoire pour stocker toutes les données par tranche de 1deg x 1deg\n");
    for (u=0; u<360; u++)
    {
        for (v=0; v<180; v++)
        {
            contour[u][v] = NULL;
            contour[u][v] = malloc(1 * sizeof(gshhs_contour));
            if ( contour[u][v] == NULL )
            {
                fprintf(stderr,"Allocation impossible %d, %d\n", u, v);
                exit(EXIT_FAILURE);
            }
            contour[u][v]->nb_line=0;
        }
    }


    /* Lecture et mise en memoire des données */
    printf("Lecture GSHHS et mise en memoire des données\n");
    read_gshhs(gshhs_file, polygons, 0);

    /* Affichage du nombre de polygones contenu dans le GSHHS */
    printf("Nb polygones= %8d\n", polygons->nb_poly );


    /* fermeture du fichier GSHHS */
    fclose (gshhs_file);

    printf("Decoupage par tranche de 1deg x 1deg\n");
    for(n = 0; n <= polygons->nb_poly; n++ )
    {
        //printf("%d\n", n);
        if (polygons->contour[n].type == LineType)
        {

            for (k=0; k<polygons->contour[n].nb_point-2; k++)
            {
                // if (polygons->contour[n].vertex[k].x <0) printf("negatif: \n");
                //printf("x1= %d\n", polygons->contour[n].vertex[k].x);
                if (DEBUG) printf("\n");
                x1min=floor(polygons->contour[n].vertex[k].x * GSHHS_SCL);
                x1max=ceil(polygons->contour[n].vertex[k].x * GSHHS_SCL);
                x2min=floor(polygons->contour[n].vertex[k+1].x * GSHHS_SCL);
                x2max=ceil(polygons->contour[n].vertex[k+1].x * GSHHS_SCL);
                y1min=floor(polygons->contour[n].vertex[k].y * GSHHS_SCL + 90);
                y1max=ceil(polygons->contour[n].vertex[k].y * GSHHS_SCL + 90);
                y2min=floor(polygons->contour[n].vertex[k+1].y * GSHHS_SCL + 90);
                y2max=ceil(polygons->contour[n].vertex[k+1].y * GSHHS_SCL + 90);

                if (DEBUG)
                {
                    printf("x1= %lf\n", polygons->contour[n].vertex[k].x* GSHHS_SCL);
                    printf("y1= %lf\n", polygons->contour[n].vertex[k].y* GSHHS_SCL);
                    printf("x2= %lf\n", polygons->contour[n].vertex[k+1].x* GSHHS_SCL);
                    printf("y2= %lf\n", polygons->contour[n].vertex[k+1].y* GSHHS_SCL);
                }

                if (x1min<x2min) xmin=x1min; else xmin=x2min;
                if (x1max<x2max) xmax=x2max; else xmax=x1max;

                if (y1min<y2min) ymin=y1min; else ymin=y2min;
                if (y1max<y2max) ymax=y2max; else ymax=y1max;

                if (DEBUG) printf("Xmin: %d\tXmax: %d\tYmin: %d\tYmax: %d\n", xmin, xmax, ymin, ymax);

                for (i=xmin; i<xmax; i++)
                {
                    for (j=ymin; j<ymax; j++)
                    {

                        ClipRect.p1.x = i;
                        ClipRect.p1.y = j-90;
                        ClipRect.p2.x = i+1;
                        ClipRect.p2.y = (j-90)+1;
                        if (DEBUG) printf("Clip Xmin: %lf\tYmin: %lf\tXmax: %lf\tYmax: %lf\n", ClipRect.p1.x, ClipRect.p1.y, ClipRect.p2.x, ClipRect.p2.y);

                        InitialLine.p1.x = polygons->contour[n].vertex[k].x* GSHHS_SCL;
                        InitialLine.p1.y = polygons->contour[n].vertex[k].y* GSHHS_SCL;
                        InitialLine.p2.x = polygons->contour[n].vertex[k+1].x* GSHHS_SCL;
                        InitialLine.p2.y = polygons->contour[n].vertex[k+1].y* GSHHS_SCL;
                        if (DEBUG) printf("Initial X1: %lf\tY1: %lf\tX2: %lf\tY2: %lf\n", InitialLine.p1.x, InitialLine.p1.y, InitialLine.p2.x, InitialLine.p2.y);

                        FinalLine.p1.x = 0;
                        FinalLine.p1.y = 0;
                        FinalLine.p2.x = 0;
                        FinalLine.p2.y = 0;

                        result = FALSE;
                        result = cohen_sutherland (InitialLine, ClipRect, &FinalLine);

                        if (result == TRUE)
                        {
                            if (i<0)
                            {
                                contour[i+360][j]->nb_line = contour[i+360][j]->nb_line + 1;
                                if (DEBUG) printf("Nb ligne: %d\n", contour[i+360][j]->nb_line);
                                if (contour[i+360][j]->nb_line == 1)
                                {
                                    contour[i+360][j]->line = NULL;
                                    contour[i+360][j]->line = malloc(sizeof(gshhs_line));
                                    if ( contour[i+360][j]->line == NULL )
                                    {
                                        fprintf(stderr,"Allocation impossible %d, %d\n", i+360, j);
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                else
                                {
                                    temp_contour = NULL;
                                    temp_contour = realloc(contour[i+360][j]->line, contour[i+360][j]->nb_line * sizeof(gshhs_line));
                                    if (temp_contour == NULL)
                                    {
                                        fprintf(stderr,"Reallocation impossible \n");
                                        free(contour[i+360][j]->line);
                                        exit(EXIT_FAILURE);
                                    }
                                    else
                                    {
                                        contour[i+360][j]->line = temp_contour;
                                    }
                                }

                                contour[i+360][j]->line[contour[i+360][j]->nb_line - 1].x1 = (FinalLine.p1.x + 360) / GSHHS_SCL;
                                contour[i+360][j]->line[contour[i+360][j]->nb_line - 1].y1 = FinalLine.p1.y / GSHHS_SCL;
                                contour[i+360][j]->line[contour[i+360][j]->nb_line - 1].x2 = (FinalLine.p2.x + 360) / GSHHS_SCL;
                                contour[i+360][j]->line[contour[i+360][j]->nb_line - 1].y2 = FinalLine.p2.y / GSHHS_SCL;
                                if (DEBUG) printf("Final X1: %lf\tY1: %lf\tX2: %lf\tY2: %lf\n", FinalLine.p1.x+360, FinalLine.p1.y, FinalLine.p2.x+360, FinalLine.p2.y);
                            }

                            if (i>=0 && i<360)
                            {
                                contour[i][j]->nb_line = contour[i][j]->nb_line + 1;
                                if (DEBUG) printf("Nb ligne: %d\n", contour[i][j]->nb_line);
                                if (contour[i][j]->nb_line == 1)
                                {
                                    contour[i][j]->line = NULL;
                                    contour[i][j]->line = malloc(sizeof(gshhs_line));
                                    if ( contour[i][j]->line == NULL )
                                    {
                                        fprintf(stderr,"Allocation impossible %d, %d\n", i, j);
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                else
                                {
                                    temp_contour = NULL;
                                    temp_contour = realloc(contour[i][j]->line, contour[i][j]->nb_line * sizeof(gshhs_line));
                                    if (temp_contour == NULL)
                                    {
                                        fprintf(stderr,"Reallocation impossible \n");
                                        free(contour[i][j]->line);
                                        exit(EXIT_FAILURE);
                                    }
                                    else
                                    {
                                        contour[i][j]->line = temp_contour;
                                    }
                                }

                                contour[i][j]->line[contour[i][j]->nb_line - 1].x1 = FinalLine.p1.x / GSHHS_SCL;
                                contour[i][j]->line[contour[i][j]->nb_line - 1].y1 = FinalLine.p1.y / GSHHS_SCL;
                                contour[i][j]->line[contour[i][j]->nb_line - 1].x2 = FinalLine.p2.x / GSHHS_SCL;
                                contour[i][j]->line[contour[i][j]->nb_line - 1].y2 = FinalLine.p2.y / GSHHS_SCL;
                                if (DEBUG) printf("Final X1: %lf\tY1: %lf\tX2: %lf\tY2: %lf\n", FinalLine.p1.x, FinalLine.p1.y, FinalLine.p2.x, FinalLine.p2.y);
                            }

                            if (i>=360)
                            {
                                contour[i-360][j]->nb_line = contour[i-360][j]->nb_line + 1;
                                if (DEBUG) printf("Nb ligne: %d\n", contour[i-360][j]->nb_line);
                                if (contour[i-360][j]->nb_line == 1)
                                {
                                    contour[i-360][j]->line = NULL;
                                    contour[i-360][j]->line = malloc(sizeof(gshhs_line));
                                    if ( contour[i-360][j]->line == NULL )
                                    {
                                        fprintf(stderr,"Allocation impossible %d, %d\n", i-360, j);
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                else
                                {
                                    temp_contour = NULL;
                                    temp_contour = realloc(contour[i-360][j]->line, contour[i-360][j]->nb_line * sizeof(gshhs_line));
                                    if (temp_contour == NULL)
                                    {
                                        fprintf(stderr,"Reallocation impossible \n");
                                        free(contour[i-360][j]->line);
                                        exit(EXIT_FAILURE);
                                    }
                                    else
                                    {
                                        contour[i-360][j]->line = temp_contour;
                                    }
                                }

                                contour[i-360][j]->line[contour[i-360][j]->nb_line - 1].x1 = (FinalLine.p1.x - 360) / GSHHS_SCL;
                                contour[i-360][j]->line[contour[i-360][j]->nb_line - 1].y1 = FinalLine.p1.y / GSHHS_SCL;
                                contour[i-360][j]->line[contour[i-360][j]->nb_line - 1].x2 = (FinalLine.p2.x - 360) / GSHHS_SCL;
                                contour[i-360][j]->line[contour[i-360][j]->nb_line - 1].y2 = FinalLine.p2.y / GSHHS_SCL;
                                if (DEBUG) printf("Final X1: %lf\tY1: %lf\tX2: %lf\tY2: %lf\n", FinalLine.p1.x-360, FinalLine.p1.y, FinalLine.p2.x+360, FinalLine.p2.y);
                            }
                        }
                    }
                }
            }
        }
    }


    printf("Ecriture du fichier final\n");
    pos_data = 0;
    //sprintf(bd_path, "./bd");

    //sprintf(path_out_file, "%s/%s.dat", bd_path, argv[1]);
    if ((out_file = fopen (path_out_file, "wb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", path_out_file);
        exit (EXIT_FAILURE);
    }
    header_out.version=  210;
    header_out.pasx=     1;
    header_out.pasy=     1;
    header_out.xmin=     0;
    header_out.ymin=     -90;
    header_out.xmax=     360;
    header_out.ymax=     90;
    header_out.p1=       1;
    header_out.p2=       2;
    header_out.p3=       3;
    header_out.p4=       4;
    header_out.p5=       5;

    // Ecriture du header
    fwrite(&header_out, sizeof(PolygonFileHeader), 1, out_file);

    // Initialisation de la table
    //printf("passe\n");
    for (i=0; i<360; i=i+1)
    {
        for (j=-90; j<90; j=j+1)
        {
            fwrite(&pos_data, sizeof(int), 1, out_file);
        }
    }

    // Ecriture des données
    for (i=0; i<360; i=i+1)
    {

        for (j=0; j<180; j=j+1)
        {
            if (DEBUG) printf("i: %d j: %d line: %d\n", i, j, contour[i][j]->nb_line);
            if (contour[i][j]->nb_line == 0)
            {
                fseek(out_file, 0L, SEEK_END);
                pos_data = ftell(out_file);
                fwrite(&(contour[i][j]->nb_line), sizeof(int), 1, out_file);

                tab_data = i*180 + j;
                //printf("tab_data: %ld\n\n", tab_data);
                fseek(out_file, sizeof(PolygonFileHeader) + tab_data*sizeof(int), SEEK_SET);
                fwrite(&pos_data, sizeof(int), 1, out_file);


            }
            else
            {
                fseek(out_file, 0L, SEEK_END);
                pos_data = ftell(out_file);
                fwrite(&(contour[i][j]->nb_line), sizeof(int), 1, out_file);
                for (k=0; k<contour[i][j]->nb_line; k++)
                {
                    fwrite(&(contour[i][j]->line[k]), sizeof(gshhs_line), 1, out_file);
                    if (DEBUG) printf("DAT X1: %lf\tY1: %lf\tX2: %lf\tY2: %lf\n", contour[i][j]->line[k].x1, contour[i][j]->line[k].y1, contour[i][j]->line[k].x2, contour[i][j]->line[k].y2);

                }

                tab_data = i*180 + j;
                //printf("tab_data: %ld\n\n", tab_data);
                fseek(out_file, sizeof(PolygonFileHeader) + tab_data*sizeof(int), SEEK_SET);
                fwrite(&pos_data, sizeof(int), 1, out_file);
            }
        }
    }

    fclose(out_file);

    /* libération de la memoire des données GSHHS */
    printf("Liberation de la memoire des données GSHHS\n");
    free_gshhs(polygons, polygons->nb_poly);

    free(polygons);
    polygons = NULL;


    /* libération de la mémoire des tranches */
    printf("Libération de la mémoire des tranches\n");
    for (u=0; u<360; u++)
    {
        for (v=0; v<180; v++)
        {
            if (DEBUG) printf("u: %d v: %d\n", u, v);
            if (contour[u][v]->nb_line>0)
            {
                free(contour[u][v]->line);
                contour[u][v]->line=NULL;
            }

            free(contour[u][v]);
            contour[u][v] = NULL;
        }
    }

    exit (EXIT_SUCCESS);
}
