/**
 *  Filename          : cliplines.c
 *  Created by        : StephPen - stephpen@gmail.com
 *  Update            : 00:11 20/03/2012

 *  (c) 2008 by Stephane PENOT
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "gshhs.h"
#include "gpc.h"
#include "PolyUtil.h"

#include "libtiles.h"

#define CLIPLINE "\n \
  ____ _ _       _     _             \n \
 / ___| (_)_ __ | |   (_)_ __   ___  \n \
| |   | | | '_ \\| |   | | '_ \\ / _ \\ \n \
| |___| | | |_) | |___| | | | |  __/ \n \
 \\____|_|_| .__/|_____|_|_| |_|\\___| \n \
          |_|                        \n \
      ClipLine V1.0 - 15/03/2012 \n \
          stephpen@gmail.com\n\n"

#define USAGE   "\n \
Usage: clipline GSHHS_LineFile LineData_File\n\n"


int main (int argc, char **argv)
{
    FILE *GshhsFile=NULL;
    char GshhsFileName[1024];
    
    FILE *LineDataFile=NULL;
    char LineDataFileName[1024];

    gshhs_polygons *polygons;

    int n;

    gshhs_contour *contour[360][180];
    gshhs_line *temp_contour;
    int u, v, i, j, k;
    int xmin, xmax, x1min, x1max, x2min, x2max;
    int ymin, ymax, y1min, y1max, y2min, y2max;

    PolygonFileHeader header_out;

    int pos_data;
    int tab_data;

    int result;
    Rectangle ClipRect;
    Line InitialLine, FinalLine;

    //Gestion de la ligne de commande
    printf("Gestion de la ligne de commande\n");
    if (argc < 3 || argc > 4)
    {
        fprintf (stderr, CLIPLINE);
        fprintf (stderr, USAGE);
        exit (EXIT_FAILURE);
    }
    
    printf (CLIPLINE);

    /* Ouverture du fichier GSHHS */
    sprintf(GshhsFileName, "%s", argv[1]);
    if ((GshhsFile = fopen (GshhsFileName, "rb")) == NULL ) {
        fprintf (stderr, "gshhs:  Could not find file %s\n", GshhsFileName);
        exit (EXIT_FAILURE);
    }

    // Allocation de la memoire pour stocker toutes les données du GSHHS
    polygons = NULL;
    polygons = malloc(1 * sizeof(gshhs_polygons));
    if ( polygons == NULL )
    {
        fprintf(stderr,"Allocation impossible \n");
        exit(EXIT_FAILURE);
    }

    /* Lecture et mise en memoire des données */
    printf("Read GSHHS= %8s\n", GshhsFileName );
    read_gshhs(GshhsFile, polygons);

    /* fermeture du fichier GSHHS */
    fclose (GshhsFile);

    // Allocation de la memoire pour stocker toutes les données par tranche de 1deg x 1deg
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

    printf("Clipping\n");
    for(n = 0; n <= polygons->nb_poly; n++ )
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

            if (x1min<x2min) xmin=x1min; else xmin=x2min;
            if (x1max<x2max) xmax=x2max; else xmax=x1max;

            if (y1min<y2min) ymin=y1min; else ymin=y2min;
            if (y1max<y2max) ymax=y2max; else ymax=y1max;

            for (i=xmin; i<xmax; i++)
            {
                for (j=ymin; j<ymax; j++)
                {

                    ClipRect.p1.x = i;
                    ClipRect.p1.y = j-90;
                    ClipRect.p2.x = i+1;
                    ClipRect.p2.y = (j-90)+1;

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
                        }

                        if (i>=0 && i<360)
                        {
                            contour[i][j]->nb_line = contour[i][j]->nb_line + 1;
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
                        }

                        if (i>=360)
                        {
                            contour[i-360][j]->nb_line = contour[i-360][j]->nb_line + 1;
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
                        }
                    }
                }
            }
        }
    }


    pos_data = 0;
    /* Ouverture du fichier LineData */
    sprintf(LineDataFileName, "%s", argv[2]);
    if ((LineDataFile = fopen (LineDataFileName, "wb")) == NULL ) {
        fprintf (stderr, "gshhs:  Could not find file %s\n", LineDataFileName);
        exit (EXIT_FAILURE);
    }
    printf("Write: %s\n", LineDataFileName);

    header_out.version=  300;
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
    fwrite(&header_out, sizeof(PolygonFileHeader), 1, LineDataFile);

    // Initialisation de la table
    //printf("passe\n");
    for (i=0; i<360; i=i+1)
    {
        for (j=-90; j<90; j=j+1)
        {
            fwrite(&pos_data, sizeof(int), 1, LineDataFile);
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
                fseek(LineDataFile, 0L, SEEK_END);
                pos_data = ftell(LineDataFile);
                fwrite(&(contour[i][j]->nb_line), sizeof(int), 1, LineDataFile);

                tab_data = i*180 + j;
                //printf("tab_data: %ld\n\n", tab_data);
                fseek(LineDataFile, sizeof(PolygonFileHeader) + tab_data*sizeof(int), SEEK_SET);
                fwrite(&pos_data, sizeof(int), 1, LineDataFile);


            }
            else
            {
                fseek(LineDataFile, 0L, SEEK_END);
                pos_data = ftell(LineDataFile);
                fwrite(&(contour[i][j]->nb_line), sizeof(int), 1, LineDataFile);
                for (k=0; k<contour[i][j]->nb_line; k++)
                {
                    fwrite(&(contour[i][j]->line[k]), sizeof(gshhs_line), 1, LineDataFile);
                    if (DEBUG) printf("DAT X1: %lf\tY1: %lf\tX2: %lf\tY2: %lf\n", contour[i][j]->line[k].x1, contour[i][j]->line[k].y1, contour[i][j]->line[k].x2, contour[i][j]->line[k].y2);

                }

                tab_data = i*180 + j;
                //printf("tab_data: %ld\n\n", tab_data);
                fseek(LineDataFile, sizeof(PolygonFileHeader) + tab_data*sizeof(int), SEEK_SET);
                fwrite(&pos_data, sizeof(int), 1, LineDataFile);
            }
        }
    }

    fclose(LineDataFile);

    /* libération de la memoire des données GSHHS */
    printf("Purge GSHHS\n");
    free_gshhs(polygons, polygons->nb_poly);

    free(polygons);
    polygons = NULL;


    /* libération de la mémoire des tranches */
    printf("Purge data\n");
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

    printf("The end!\n");
    
    exit (EXIT_SUCCESS);
}
