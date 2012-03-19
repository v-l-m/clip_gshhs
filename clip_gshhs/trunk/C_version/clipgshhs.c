/**
 *  Filename          : clipgshhs.c
 *  Created by        : StephPen - stephpen@gmail.com
 *  Update            : 00:10 20/03/2012

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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "gshhs.h"
#include "gpc.h"
#include "PolyUtil.h"

#include "libtiles.h"

#define CLIPGSHHS "\n \
  ____ _ _        ____ ____  _   _ _   _ ____ \n \
 / ___| (_)_ __  / ___/ ___|| | | | | | / ___| \n \
| |   | | | '_ \\| |  _\\___ \\| |_| | |_| \\___ \\ \n \
| |___| | | |_) | |_| |___) |  _  |  _  |___) | \n \
 \\____|_|_| .__/ \\____|____/|_| |_|_| |_|____/ \n \
          |_|                                       \n \
          ClipGSHHS V1.0 - 15/03/2012 \n \
             stephpen@gmail.com\n\n"
#define USAGE   "\n \
Usage: clipgshhs GSHHS_File PolyData_File\n\n"

int main (int argc, char **argv)
{
    FILE *GshhsFile=NULL;
    char GshhsFileName[1024];
    
    FILE *PolyDataFile=NULL;
    char PolyDataFileName[1024];
    
    gshhs_polygons *polygons;
    
    gpc_polygon GpcPolygon;
    gpc_polygon clip;
    gpc_polygon result;
    
    //PolyData Poly90;
    PolyData Poly45;
    PolyData Poly15;
    PolyData Poly05;
    PolyData Poly01;
    
    int x,y;
    int id;
    int i;
    int pos_data;
    int level;
    int n_contour;
    int n_point;
    
    PolygonFileHeader PolyDataFileHeader;

    /* gestion de la ligne de commande */
    if (argc < 3 || argc > 4)
    {
        fprintf (stderr, CLIPGSHHS);
        fprintf (stderr, USAGE);
        exit (EXIT_FAILURE);
    }
    
    printf (CLIPGSHHS);
    
    /* Ouverture du fichier GSHHS */
    sprintf(GshhsFileName, "%s", argv[1]);
    if ((GshhsFile = fopen (GshhsFileName, "rb")) == NULL ) {
        fprintf (stderr, "gshhs:  Could not find file %s\n", GshhsFileName);
        exit (EXIT_FAILURE);
    }

    /*Allocation de la mémoire pour stocker toutes les données du GSHHS */
    polygons = NULL;
    polygons = malloc(1 * sizeof(gshhs_polygons));
    if ( polygons == NULL )
    {
        fprintf(stderr,"Allocation impossible \n");
        exit(EXIT_FAILURE);
    }

    /* Lecture et mise en memoire des données */
    printf("Lecture du fichier GSHHS= %8s\n", GshhsFileName );
    read_gshhs(GshhsFile, polygons);

    /* Affichage du nombre de polygones contenu dans le GSHHS */
    printf("Nb polygone= %8d\n", polygons->nb_poly );

    /* fermeture du fichier GSHHS */
    fclose (GshhsFile);
    
    //PolyDataCreate(&Poly90, 90);
    PolyDataCreate(&Poly45, 45);
    PolyDataCreate(&Poly15, 15);
    PolyDataCreate(&Poly05, 5);
    PolyDataCreate(&Poly01, 1);
    
    printf("GSHHS > Poly45\n");
    
    for(id=0; id<polygons->nb_poly; id++)
    {
        GshhsToGpc(polygons, &GpcPolygon, id);
        //printf("Id: %d\n", id);
        //printf("num_vertices: %d\n", GpcPolygon.contour[0].num_vertices);
        //printf("Level: %d\n", polygons->contour[id].level);
        
        // On recupere ce qui est inferieur à 0 de longitude
        CreateClip(&clip, -180 / GSHHS_SCL, -90 / GSHHS_SCL, 0 / GSHHS_SCL, 90 / GSHHS_SCL);
        gpc_polygon_clip(GPC_INT, &GpcPolygon, &clip, &result);
        poly_p_shift(&result, 360 / GSHHS_SCL, 0);   // Translation de 360° pour le remettre dans le domaine [0..360°]
        for(i=0; i<result.num_contours; i++)
        {
            gpc_add_contour(&GpcPolygon, &result.contour[i], 0); // On réecrit les données à leur place
        }
        
        gpc_free_polygon(&clip);
        gpc_free_polygon(&result);
        

        // Découpage en carré de 45° et mise en mémoire dans Poly45
        for (x=0; x<(360/Poly45.step); x++)
        {
            for (y=0; y<(180/Poly45.step); y++)
            {
                CreateClip(&clip, (x*Poly45.step) / GSHHS_SCL, (-90+y*Poly45.step) / GSHHS_SCL,
                                    ((x+1)*Poly45.step) / GSHHS_SCL, (-90+(y+1)*Poly45.step) / GSHHS_SCL);
                gpc_polygon_clip(GPC_INT, &GpcPolygon, &clip, &result);
                
                //printf("result1.num_contours: %d\n", result1.num_contours);
                for(i=0; i<result.num_contours; i++)
                {
                    gpc_add_contour(Poly45.poly[polygons->contour[id].level-1][x][y], &result.contour[i], 0);
                }
                
                gpc_free_polygon(&clip);
                gpc_free_polygon(&result);
            }
        }
        gpc_free_polygon(&GpcPolygon);
    }
    
    /* vidage de la memoire des données GSHHS */
    free_gshhs(polygons, polygons->nb_poly);

    free(polygons);
    polygons = NULL;
    
    //Clipping des polygones
    //printf("Poly90 > Poly45\n");
    //ReClipping(&Poly90, &Poly45);
    printf("Poly45 > Poly15\n");
    ReClipping(&Poly45, &Poly15);
    printf("Poly15 > Poly05\n");
    ReClipping(&Poly15, &Poly05);
    printf("Poly05 > Poly01\n");
    ReClipping(&Poly05, &Poly01);

    // Ecriture du fichier PolyData
    sprintf(PolyDataFileName, "%s", argv[2]);
    printf("Ecriture du fichier PolyData01: %s\n", PolyDataFileName );

    if ((PolyDataFile = fopen (PolyDataFileName, "wb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", PolyDataFileName);
        exit (EXIT_FAILURE);
    }

    pos_data = 0;
    
    PolyDataFileHeader.version=  300;
    PolyDataFileHeader.pasx=     1;
    PolyDataFileHeader.pasy=     1;
    PolyDataFileHeader.xmin=     0;
    PolyDataFileHeader.ymin=     -90;
    PolyDataFileHeader.xmax=     360;
    PolyDataFileHeader.ymax=     90;
    PolyDataFileHeader.p1=       1;
    PolyDataFileHeader.p2=       2;
    PolyDataFileHeader.p3=       3;
    PolyDataFileHeader.p4=       4;
    PolyDataFileHeader.p5=       5;

    fwrite(&PolyDataFileHeader, sizeof(PolygonFileHeader), 1, PolyDataFile);

    // Initialisation de la table avec des zéro
    for (x=0; x<360; x++)
    {
        for (y=0; y<180; y++)
        {
            fwrite(&pos_data, sizeof(int), 1, PolyDataFile);
        }
    }

    for (x=0; x<360; x++)
    {
        for (y=0; y<180; y++)
        {
            fseek(PolyDataFile, 0L, SEEK_END);
            pos_data = ftell(PolyDataFile);

            for (level=0; level<4; level++)
            {
                //printf("Poly01.poly[level][x][y]->num_contours: %d\n", Poly01.poly[level][x][y]->num_contours);
                fwrite(&Poly01.poly[level][x][y]->num_contours, sizeof(int), 1, PolyDataFile);
            
                for (n_contour=0; n_contour<Poly01.poly[level][x][y]->num_contours; n_contour++)
                {
                    fwrite(&Poly01.poly[level][x][y]->hole[n_contour], sizeof(int), 1, PolyDataFile);

                    fwrite(&Poly01.poly[level][x][y]->contour[n_contour].num_vertices, sizeof(int), 1, PolyDataFile);
                    for (n_point=0; n_point<Poly01.poly[level][x][y]->contour[n_contour].num_vertices; n_point++)
                    {
                        fwrite(&Poly01.poly[level][x][y]->contour[n_contour].vertex[n_point].x, sizeof(double), 1, PolyDataFile);
                        fwrite(&Poly01.poly[level][x][y]->contour[n_contour].vertex[n_point].y, sizeof(double), 1, PolyDataFile);
                    }
                }
            }

            fseek(PolyDataFile, sizeof(PolygonFileHeader) + (x*180 + y) * sizeof(int), SEEK_SET);
            fwrite(&pos_data, sizeof(int), 1, PolyDataFile);
        }
    }
            
    fclose(PolyDataFile);
    
    //PolyDataDelete(&Poly90);
    PolyDataDelete(&Poly45);
    PolyDataDelete(&Poly15);
    PolyDataDelete(&Poly05);
    PolyDataDelete(&Poly01);

    printf("The End!\n");
    
    exit (EXIT_SUCCESS);
}
