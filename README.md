# Clip_gshhs 
L'intérêt de cet ensemble de programmes et scripts python est de pouvoir découper les fichiers GSHHS en petits morceaux pour que cela soit plus facile à utiliser.

## Données GSHHS 
 * [http://www.soest.hawaii.edu/wessel/gshhs/gshhs.html] version 1.10
 * Les données GSHHS sont présentées sous la forme suivante:
   * gshhs_[c, l, i, h, f].b qui contiennent les lignes de cotes sur toute la surface du globe.
   * wdb_borders_[c, l, i, h, f].b  qui contiennent les limites de frontières de tous les pays.
   * wdb_rivers_[c, l, i, h, f].b  qui contiennent les fleuves et les rivières.
 * Résolution:

   ||File||Content||Tolerance
   ||xxx_f.b||full resolution||0.1 km
   ||xxx_h.c||High resolution||0.2 km
   ||xxx_i.c||Interm. resolution||1.0 km
   ||xxx_l.c||Low resolution||5.0 km
   ||xxx_c.c||Crude resolution||25  km

 * Niveaux des données:
   * Niveau 1: la terre
   * Niveau 2: les lacs
   * Niveau 3: les iles dans les lacs
   * Niveau 4: les flaques :) sur les iles dans les lacs
   * Et un niveau 5 qui est défini  par les lignes de récifs. A l'origine ils sont de niveau 1 mais avec une surface nulle. Pour des raisons pratique on les dissocies du niveau 1. Tout au long de la procédure ils conserveront ce niveau 5 et pour le tracé sur la carte, ils seront dessinés avec une fonction "polygone vide".

## Lib utilisées 
 * GPC [http://www.cs.man.ac.uk/~toby/alan/software/] et son binding python [http://polygon.origo.ethz.ch/wiki/polygon], c'est elle qui permet de découper.
 * LibZIP [http://www.nih.at/libzip/libzip.html], les fichiers découpés seront archivés dans un zip, plus facile à utiliser, utilisant moins de place. Cette lib permet de lire un zip sans avoir à le décompresser.

## Fonctionnement 
### 1. Extraction des données des fichiers GSHHS 
C'est le rôle du programme readgshhs.c (Explication du code dans le code)
 1. Faire un $make readgshhs ou $make qui va compiler le programme.
 2. $readgshhs [c, l, i, h, f] qui va extraire les données dans le répertoire /bd. Un fichier par niveau par résolution.
A la fin, vous devez avoir dans le répertoire /bd, les fichiers suivants:
 * bd
  * c[1, 2, 3, 4, 5].dat
  * f[1, 2, 3, 4, 5].dat
  * h[1, 2, 3, 4, 5].dat
  * i[1, 2, 3, 4, 5].dat
  * l[1, 2, 3, 4, 5].dat

#### Cas du polygone de l'Antarctique 
Ce polygone est un peu particulier, car si on regarde la terre par en dessous effectivement on voit bien l'Antarctique. Par contre, si on veut le représenter sur une carte à plat, on va avoir un problème. Donc Au polygone on vas rajouter deux points.

 || ||longitude||latitude||
 ||point 1||0.0||-90.0||
 ||point 2||360.0||-90.0||

De cette façon on on couvre bien tout l'Antarctique et on est plus embêté pour sa représentation à plat.

Le bout de code qui fait ça: (fichier read_gshhs.c)

{{{
...
            if (h.south ## -90000000 && k  (h.n - 3) ) {
                polygons->contour[h.id].vertex[k].x = 0;
                polygons->contour[h.id].vertex[k].y = -90000000;
                lon = polygons->contour[h.id].vertex[k].x * GSHHS_SCL;
                lat = polygons->contour[h.id].vertex[k].y * GSHHS_SCL;
                k++;
                
                if (debug#1)
                {
                    printf ("lon(%2d)#%10d ,%10.6f , lat(%2d)%10d ,%10.6f\n",  sizeof(p.x), polygons->contour[h.id].vertex[k].x, lon,
                                                                                sizeof(p.y), polygons->contour[h.id].vertex[k].y, lat);
                }
                polygons->contour[h.id].vertex[k].x = 360000000;
                polygons->contour[h.id].vertex[k].y = -90000000;
                lon = polygons->contour[h.id].vertex[k].x * GSHHS_SCL;
                lat = polygons->contour[h.id].vertex[k].y * GSHHS_SCL;
                k++;
                if (debug#1)
                {
                    printf ("lon(%2d)#%10d ,%10.6f , lat(%2d)%10d ,%10.6f\n",  sizeof(p.x), polygons->contour[h.id].vertex[k].x, lon,
                                                                                sizeof(p.y), polygons->contour[h.id].vertex[k].y, lat);
                }
            }
...
}}}


#### Format des fichiers .dat 
On utilise le format de fichier de la lib GPC.
 1. Nombre de polygones dans le fichier (cette ligne existe toujours =0 si aucun polygone)
 2. Nombre de points du premier polygone
 3. 0 ou 1 (0#de la terre, 1un trou dans la terre). Dans cette première version des fichiers .dat tout les polygones sont considérés comme de la terre. On verra plus loin pourquoi!
 4. Coordonnées de longitude, coordonnées de latitude du 1er point (en micro degrés)
 5. Coordonnées de longitude, coordonnées de latitude du 2ème point (en micro degrés)
 6. .....
 7. Coordonnées de longitude, coordonnées de latitude du Nème point (en micro degrés) ne doit pas boucler sur le premier point. Le polygone est ouvert, c'est normal.
 8. ..... jusqu'au dernier polygone.

### 2. Préparation des fichiers . Script pygshhs1.py 

Script pygshhs1.py  ('''ATTENTION''' c'est une opération longue: 24h de calcul)
  * Ajuster les chemins directement dans le script python.
 * '''Passe 1''': on découpe les fichiers .dat en carré de 45° de coté. On déborde en négatif car, dans les fichiers GSHHS, il y a un grand polygone (Europe + Asie + Afrique) qui déborde dans les longitudes négatives. On va découper tous ce qui dépasse en négatif et le remettre en place dans le positif (translation de + 360°)
 * '''Passe 2''': on additionne les polygones qui étaient en négatif avec leur homologue qui sont déjà à leur place. Tous les polygones entre -45 et 0 de longitude (qui ont été translatés durant la passe 1) sont additionnés aux polygones entre 315 et 360 de longitude.
#### Arborescence 
   xd_yd_to_xf_yf
 * xd: longitude de départ
 * yd: latitude de départ
 * xf: longitude de fin
 * yf: latitude de fin

Chaque répertoire contient les fichiers
 * xd_yd_to_xf_yf
   * c[1, 2, 3, 4, 5].dat
   * f[1, 2, 3, 4, 5].dat
   * h[1, 2, 3, 4, 5].dat
   * i[1, 2, 3, 4, 5].dat
   * l[1, 2, 3, 4, 5].dat

 
A la fin on se retrouve avec l'arborescence suivante: 
 * bd_c
   * 0_0_to_45_45
     * c1.dat
     * c2.dat
     * c3.dat
     * c4.dat
     * c5.dat
   * 0_45_to_45_90
     * c1.dat
     * c2.dat
     * c3.dat
     * c4.dat
     * c5.dat
   * .
   * .
   * .
 * bd_f
   * 0_0_to_45_45
     * f1.dat
     * f2.dat
     * f3.dat
     * f4.dat
     * f5.dat
   * 0_45_to_45_90
     * f1.dat
     * f2.dat
     * f3.dat
     * f4.dat
     * f5.dat
   * .
   * .
   * .
 * bd_h
   * 0_0_to_45_45
     * h1.dat
     * h2.dat
     * h3.dat
     * h4.dat
     * h5.dat
   * 0_45_to_45_90
     * h1.dat
     * h2.dat
     * h3.dat
     * h4.dat
     * h5.dat
   * .
   * .
   * .
 * bd_i
   * 0_0_to_45_45
     * i1.dat
     * i2.dat
     * i3.dat
     * i4.dat
     * i5.dat
   * 0_45_to_45_90
     * i1.dat
     * i2.dat
     * i3.dat
     * i4.dat
     * i5.dat
   * .
   * .
   * .
 * bd_l
   * 0_0_to_45_45
     * l1.dat
     * l2.dat
     * l3.dat
     * l4.dat
     * l5.dat
   * 0_45_to_45_90
     * l1.dat
     * l2.dat
     * l3.dat
     * l4.dat
     * l5.dat
   * .
   * .
   * .

### 2. Préparation des fichiers . Script pygshhs2.py 

Script pygshhs2.py  ('''ATTENTION''' c'est une opération longue: 24h de calcul)
Prévoir de l'espace disque car 67712 répertoires et 338560 fichiers vont être créés.
  * Ajuster les chemins directement dans le script python.
Script chargé de redécouper les polygones de 45° x 45°
L'arborescence va se compléter avec les polygones de:
  * 15° x 15°
  * 5° x 5°
  * 1° x 1°

### 3. Agglomération des fichiers . Programme read_bd 

Ce programme va lire les fichiers créées précédemment, pour les agglomérer en un seul fichier.
Ainsi, il y aura 4 fichiers par résolutions:
   ||Résolution||Taille||Fichier
   ||Full|| 1° x  1°|| poly-f-1.dat
   || || 5° x  5°|| poly-f-5.dat
   || ||15° x 15°|| poly-f-15.dat
   || ||45° x 45°|| poly-f-45.dat
   ||High|| 1° x  1°|| poly-h-1.dat
   || || 5° x  5°|| poly-h-5.dat
   || ||15° x 15°|| poly-h-15.dat
   || ||45° x 45°|| poly-h-45.dat
   ||Inter|| 1° x  1°|| poly-i-1.dat
   || || 5° x  5°|| poly-i-5.dat
   || ||15° x 15°|| poly-i-15.dat
   || ||45° x 45°|| poly-i-45.dat
   ||Low|| 1° x  1°|| poly-l-1.dat
   || || 5° x  5°|| poly-l-5.dat
   || ||15° x 15°|| poly-l-15.dat
   || ||45° x 45°|| poly-l-45.dat
   ||Crude|| 1° x  1°|| poly-c-1.dat
   || || 5° x  5°|| poly-c-5.dat
   || ||15° x 15°|| poly-c-15.dat
   || ||45° x 45°|| poly-c-45.dat

Le script clip.sh est chargé de faire le travail (à écrire)

# Structure des fichiers 

## 1. Un header (facile) 
    struct header_01 {
        int version;      // Version du fichier

        int pasx;         // Le pas de découpe en x (longitude) (degrés)

        int pasy;         // Le pas de découpe en y (latitude) (degrés)

        int xmin;         // Longitude mini  (degrés) [0, 360]

        int ymin;         // Latitude mini  (degrés) [-90, 90]

        int xmax;         // Longitude maxi  (degrés) [0, 360]

        int ymax;         // latitude maxi  (degrés) [-90, 90]

        int p1;           // paramètre réservé en cas de besoin

        int p2;           // paramètre réservé en cas de besoin

        int p3;           // paramètre réservé en cas de besoin

        int p4;           // paramètre réservé en cas de besoin

        int p5;           // paramètre réservé en cas de besoin

    };


## 2. Une table d'index (plus dur) 
C'est un tableau qui va permettre de retrouver les données des polygones dans le fichier, c'est une adresse.
Dans le cas de la découpe de 1° x 1°:
Le 1er élément pointe sur le polygone situé en 0, -90
Le 2ème élément pointe sur le polygone situé en 0, -89
.
.
.

Pour retrouver l'adresse d'un polygone connaissant ses coordonnées de départ, en C ça donne:
Fonction ReadPolygonFile dans le fichier map_functions.c

{{{
void ReadPolygonFile (FILE *polyfile,
                        int x, int y,
                        int pas_x, int pas_y,
                        gpc_polygon *p1, gpc_polygon *p2, gpc_polygon *p3, gpc_polygon *p4, gpc_polygon *p5)
{
    long pos_data;
    long tab_data;



    //Calcul de l'adresse dans la table d'index
    tab_data = (x/pas_x)*(180/pas_y) + (y+90)/pas_y;

    //On se deplace dans le fichier pour aller lire l'adresse du polygone
    fseek(polyfile, sizeof(PolygonFileHeader) + tab_data*sizeof(int), SEEK_SET);

    //Lecture de l'adresse du polygone
    fread(&pos_data, sizeof(long), 1, polyfile);


    //On se deplace dans le fichier pour aller lire le polygone
    fseek(polyfile, pos_data, SEEK_SET);

.....
}}}


## 3. les polygones (encore plus dur) 
Vous vous souvenez que les GSHHS ont 5 niveaux (voir plus haut), et bien on vas lire les 5 niveaux en une seule passe.
Le format des données est:
 * le nombre de polygones de niveau 1(integer)
 * le type du polygone 1 (0 plein, 1 trou) (integer)
 * le nombre de points dans le premier polygone (integer)
 * coordonnée longitude en microdegrés du premier point (double)
 * coordonnée latitude en microdegrés du premier point (double)
 * .
 * .
 * .
 * le nombre de polygones de niveau 2(integer)
 * le type du polygone 1 (0 plein, 1 trou) (integer)
 * le nombre de points dans le premier polygone (integer)
 * coordonnée longitude en microdegrés du premier point (double)
 * coordonnée latitude en microdegrés du premier point (double)

En fait on réutilise la structure des polygones de la lib GPC

Voilà c'est tout!

* Les fichiers déjà découpés: /hone/commun/map/bd
* Les programmes et les scripts: le svn vlmtools branche clip_gshhs
* Une version se trouve dans /home/commun
{{{$ map_g cmd.txt}}}
cmd.txt est le fichier de commande à éditer pour changer les options et la vue de la carte (point au centre de l'image)

