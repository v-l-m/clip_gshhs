/**
 *  Filename          : read_gshhs.c
 *  Created by        : StephPen - stephpen@gmail.com
 *  Update            : 11:14 02/01/2011

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>

#include "gshhs.h"
#include "read_gshhs.h"

void GshhsToGpcFile(FILE *gpc_file, gshhs_polygons *polygons, int id_poly)
{
    int i;
    for (i = 0; i < polygons->contour[id_poly].nb_point - 1; i++) {
        fprintf(gpc_file, "%10d %10d\n", polygons->contour[id_poly].vertex[i].x,
                                         polygons->contour[id_poly].vertex[i].y);
    }
}

void read_gshhs(FILE *gshhs_file, gshhs_polygons *polygons, int debug)
{
    /* declaration des variables */
    double  lon, lat;
    int     k, max_east = 270000000, n_read;
    int     lastx, lasty, fakey;
    int     pos = 1;
    int     version;
    int     i = 0;
    /*Polygone *tableau;*/
    gshhs_vertex_list *temp_polygons;

    struct POINT p;
    struct GSHHS h;

    n_read = fread ((void *)&h, (size_t)sizeof (struct GSHHS), (size_t)1, gshhs_file);
    version = (h.flag >> 8) & 255;

    while (n_read == 1)
    {
        h.id = ntohl(h.id);
        h.n  = ntohl(h.n);
        h.west  = ntohl(h.west);
        h.east  = ntohl(h.east);
        h.south = ntohl(h.south);
        h.north = ntohl(h.north);
        h.area  = ntohl(h.area);
        h.flag  = ntohl(h.flag);

        if (h.id == 0)
        {
            polygons->contour = NULL;
            i = 1024;
            polygons->contour = malloc(i * sizeof(gshhs_vertex_list));
            if ( polygons->contour == NULL )
            {
                fprintf(stderr,"Allocation impossible \n");
                exit(EXIT_FAILURE);
            }
        }
        if (h.id == i)
        {
            /*Polygone *temp_tableau;*/
            i = i + 1024;
            temp_polygons = NULL;
            temp_polygons = realloc(polygons->contour, i * sizeof(gshhs_vertex_list));
            if (temp_polygons == NULL)
            {
                fprintf(stderr,"Reellocation impossible \n");
                free(polygons->contour);
                exit(EXIT_FAILURE);
            }
            else
            {
                polygons->contour = temp_polygons;
            }
        }

        polygons->contour[h.id].id = h.id;
        if (h.south == -90000000) {
	  // we will add 4 points
	  h.n = h.n + 4;
        }

        polygons->contour[h.id].nb_point    = h.n;
        polygons->contour[h.id].level       = h.flag & 255;
        polygons->contour[h.id].version     = (h.flag >> 8) & 255;
        polygons->contour[h.id].greenwich   = (h.flag >> 16) & 0x3;
        polygons->contour[h.id].source      = (h.flag >> 24) & 255; /* Either =1 = WVS or <>1 = CIA (WDBII) pedigree */
        polygons->contour[h.id].long_min    = h.west;
        polygons->contour[h.id].long_max    = h.east;
        polygons->contour[h.id].lat_min     = h.south;
        polygons->contour[h.id].lat_max     = h.north;
        polygons->contour[h.id].type        = (h.area) ? 0 : 1;        /* Either Polygon (0) or Line (1) (if no area) */
        polygons->contour[h.id].area        = h.area;

        polygons->contour[h.id].vertex = NULL;
        polygons->contour[h.id].vertex = malloc(h.n * sizeof(gshhs_vertex));
        if ( polygons->contour[h.id].vertex == NULL )
        {
            fprintf(stderr,"Allocation impossible \n");
            exit(EXIT_FAILURE);
        }

        if (debug==1)
        {
            printf("ID polygone= %6d\n", polygons->contour[h.id].id);
            printf("Type= %3d\n", polygons->contour[h.id].type);
            printf("Nombre de point: %8d\n", polygons->contour[h.id].nb_point);
            printf("Niveau: %8d\n", polygons->contour[h.id].level);
            printf("Version: %8d\n", polygons->contour[h.id].version);
            printf("Greenwich: %8d\n", polygons->contour[h.id].greenwich);
            printf("Source: %8d\n", polygons->contour[h.id].source);
            printf("Surface: %8d\n", polygons->contour[h.id].area);
            printf("Longitude min: %8d\n", polygons->contour[h.id].long_min);
            printf("Longitude max: %8d\n", polygons->contour[h.id].long_max);
            printf("Latitude min: %8d\n", polygons->contour[h.id].lat_min);
            printf("Latitude max: %8d\n", polygons->contour[h.id].lat_max);
        }

        for (k = 0; k < h.n; k++)
        {
            if (fread ((void *)&p, (size_t)sizeof(struct POINT), (size_t)1, gshhs_file) != 1)
            {
                fprintf (stderr, "gshhs:  Error reading file\n");
                exit (EXIT_FAILURE);
            }
            p.x = ntohl(p.x);
            p.y = ntohl(p.y);

            if (h.south == -90000000) {
	      // when we cross the 0 line...
	      if (p.x < 0) {
		if (pos) {
		  // ok, we need to create some fake points...
		  fakey = lasty + (int)( ((double)(p.y - lasty)) * ((double)lastx)/(double)(lastx - p.x) );
		  pos = 0;
		  polygons->contour[h.id].vertex[k].x = 0;
		  polygons->contour[h.id].vertex[k].y = fakey;
		  k++;
		  polygons->contour[h.id].vertex[k].x = 0;
		  polygons->contour[h.id].vertex[k].y = -90000000;
		  k++;
		  polygons->contour[h.id].vertex[k].x = 360000000;
		  polygons->contour[h.id].vertex[k].y = -90000000;
		  k++;	
		  polygons->contour[h.id].vertex[k].x = 360000000;
		  polygons->contour[h.id].vertex[k].y = fakey;
		  k++;
		}
		polygons->contour[h.id].vertex[k].x = p.x + 360000000;
		polygons->contour[h.id].vertex[k].y = p.y;
	      } else {
		polygons->contour[h.id].vertex[k].x = p.x;
		polygons->contour[h.id].vertex[k].y = p.y;
		lastx = p.x;
		lasty = p.y;
	      }
	    } else { /* not southern polygon */
	      if ((polygons->contour[h.id].greenwich & 0x1) && p.x > max_east) p.x -= 360000000;
	      polygons->contour[h.id].vertex[k].x = p.x;
	      polygons->contour[h.id].vertex[k].y = p.y;
	    }
        }
        max_east = 180000000;    /* Only Eurasiafrica needs 270 */
        n_read = fread((void *)&h, (size_t)sizeof (struct GSHHS), (size_t)1, gshhs_file);
    }
    polygons->nb_poly = h.id;

}

void free_gshhs(gshhs_polygons *polygons, int nb_poly)
{
    int k;
    /* vidage de la memoire des données GSHHS */
    for (k = 0; k <= nb_poly; k++)
    {
        free(polygons->contour[k].vertex);
        polygons->contour[k].vertex = NULL;
    }

    free(polygons->contour);
    polygons->contour = NULL;

}


