/**
 *  Filename          : compile_etopo_060.c
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

int main (int argc, char **argv)
{
    struct ETOPO_Header {
        int NCOLS;
        int NROWS;
        double START_X;
        double START_Y;
        double FINISH_X;
        double FINISH_Y;
        double CELLSIZE;
        short int NODATA_VALUE;
        char NUMBERTYPE[32];
        char ZUNITS[32];
        short int MIN_VALUE;
        short int MAX_VALUE;
    };

    char EtopoFinishPath[256];
    FILE *EtopoFinishFile;

    char EtopoCompPath[256];
    FILE *EtopoCompFile;

	short int z;
    int FReadResult = 0;

	struct ETOPO_Header Header;


	if (argc < 3 || argc > 4) {
		fprintf (stderr, "Sorry !!\n");
		fprintf (stderr, "Usage:  compile_etopo_060 file_in file_out\n\n");

		return 0;
	}

	sprintf(EtopoCompPath, "%s", argv[1]);
    if ((EtopoCompFile = fopen (EtopoCompPath, "rb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", EtopoCompPath);
        return 0;
    }

	sprintf(EtopoFinishPath, "%s", argv[2]);
    if ((EtopoFinishFile = fopen (EtopoFinishPath, "wb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", EtopoFinishPath);
        return 0;
    }


    fseek(EtopoCompFile, 0 , SEEK_SET);

    Header.NCOLS    =21601;
    Header.NROWS    =10801;
    Header.START_X  =-180.0;
    Header.START_Y  =90.0;
    Header.FINISH_X =180.0;
    Header.FINISH_Y =-90.0;
    Header.CELLSIZE =60.0;
    Header.NODATA_VALUE=-32768;
    sprintf(Header.NUMBERTYPE, "2_BYTE_SHORT_INT");
    sprintf(Header.ZUNITS, "METERS");
    Header.MIN_VALUE=0;
    Header.MAX_VALUE=0;


    while (fread(&z, sizeof(short int), 1, EtopoCompFile))
    {
        if (z<Header.MIN_VALUE) Header.MIN_VALUE=z;
        if (z>Header.MAX_VALUE) Header.MAX_VALUE=z;
    }

	printf("MIN_VALUE: %d\tMAX_VALUE: %d\n", Header.MIN_VALUE, Header.MAX_VALUE);

	fseek(EtopoFinishFile, 0 , SEEK_SET);
	fwrite(&Header, sizeof(Header), 1, EtopoFinishFile);

    fseek(EtopoCompFile, 0 , SEEK_SET);
    while (fread(&z, sizeof(short int), 1, EtopoCompFile))
    {
        fwrite(&z, sizeof(short int), 1, EtopoFinishFile);
    }

	fclose(EtopoCompFile);
	fclose(EtopoFinishFile);

    if ((EtopoFinishFile = fopen (EtopoFinishPath, "rb")) == NULL )
    {
        fprintf (stderr, "Impossible d'ouvrir le fichier %s\n", EtopoFinishPath);
        return 0;
    }

    fseek(EtopoFinishFile, 0 , SEEK_SET);

    FReadResult = fread(&Header, sizeof(Header), 1, EtopoFinishFile);
    printf("Header.NCOLS=%d\n",         Header.NCOLS);
    printf("Header.NROWS=%d\n",         Header.NROWS);
    printf("Header.START_X=%lf\n",      Header.START_X);
    printf("Header.START_Y=%lf\n",      Header.START_Y);
    printf("Header.FINISH_X=%lf\n",     Header.FINISH_X);
    printf("Header.FINISH_Y=%lf\n",     Header.FINISH_Y);
    printf("Header.CELLSIZE=%lf\n",     Header.CELLSIZE);
    printf("Header.NODATA_VALUE=%d\n",  Header.NODATA_VALUE);
    printf("Header.NUMBERTYPE=%s\n",    Header.NUMBERTYPE);
    printf("Header.ZUNITS=%s\n",        Header.ZUNITS);
    printf("Header.MIN_VALUE=%d\n",     Header.MIN_VALUE);
    printf("Header.MAX_VALUE=%d\n",     Header.MAX_VALUE);

    /*
    FReadResult = fread(&z, sizeof(short int), 1, EtopoFinishFile);
    printf("Z: %d\n", z);

    FReadResult = fread(&z, sizeof(short int), 1, EtopoFinishFile);
    printf("Z: %d\n", z);

    printf("sizeof(Header): %lu\n", sizeof(Header));
    */

	fclose(EtopoFinishFile);

    return 0;

}
