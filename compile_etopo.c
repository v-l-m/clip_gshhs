/**
 *  Filename          : compile_etopo.c
 *  Created by        : StephPen - stephpen@gmail.com
 *  Update            : 11:14 02/01/2011

 *  (c) 2008 by Stephane PENOT
 *  See COPYING file for copying and redistribution conditions.
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

int main (int argc, char **argv)
{

    struct header {
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
    };

    char EtopoPath[1024];
    FILE *EtopoFile;

    char EtopoCompPath[1024];
    FILE *EtopoCompFile;

    char buffer[1024];

	short int z;
	double x, y;

	int c;


	if (argc < 2 || argc > 3) {
		fprintf (stderr, "Sorry !!\n");
		fprintf (stderr, "Usage:  compile_etopo file\n\n");

		return 0;
	}

	sprintf(EtopoPath, "%s", argv[1]);
    if ((EtopoFile = fopen (EtopoPath, "rb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", EtopoPath);
        return 0;
    }

	sprintf(EtopoCompPath, "%s.dat", EtopoPath);
    if ((EtopoCompFile = fopen (EtopoCompPath, "wb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", EtopoCompPath);
        return 0;
    }
    c=0;
    while (fgets(buffer, sizeof(buffer), EtopoFile))
    {
        //printf("%s", buffer);
        if (sscanf(buffer, "%lf, %lf, %hd", &x, &y, &z) == 3)
        {
            //printf("x: %lf y: %lf z: %hd\n\n", x, y, z);
            fwrite(&z, sizeof(short int), 1, EtopoCompFile);
            c++;
        }
        else
        {
            printf("%s", buffer);
            printf("c: %d\n", c);
            printf("x: %lf y: %lf z: %d\n", x, y, z);
            break;
        }
    }

    // printf("c: %d\n", c);
    // printf("x: %lf y: %lf z: %d\n", x, y, z);


	fclose(EtopoFile);
	fclose(EtopoCompFile);

    return 0;

}
