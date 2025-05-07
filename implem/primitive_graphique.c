#include "ei_draw.h"
#include <stdio.h>
#include "ei_implementation.h"


void	ei_fill(ei_surface_t		surface, const ei_color_t*	color, const ei_rect_t*	clipper){

    // recupération des infos de la surface passée en paramètre :
    ei_size_t dimension = hw_surface_get_size(surface);
    uint32_t taille_surf = dimension.height * dimension.width;
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(surface);

    // j'ignore completement le clipper pour le debut:
    for(uint32_t cpt = 0; cpt < taille_surf; cpt++){
        *pixel_ptr++ = *(uint32_t*)color;
    }

}


/*******************************************************************************************************************************************/

void	ei_draw_polyline	(ei_surface_t		surface,
    ei_point_t*		point_array,
    size_t			point_array_size,
    ei_color_t		color,
    const ei_rect_t*	clipper)
{
    // recupération des infos de la surface passée en paramètre :
    ei_size_t dimension = hw_surface_get_size(surface);
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(surface);

    // on suppose que les points sont bien triés pour commencer :
    // si le tableau est non vide :
    if(point_array_size){
        // si le tableau contient un seul point, on l'affiche : 
        if (point_array_size == 1){
            // gestion du clipping même sur un point
            
            if(est_dans_clipper(&(*point_array), clipper)){

                draw_point(pixel_ptr, dimension, *point_array, &color);
            }}
        else{
            // sinon on itere sur les couples de points successivemnt en appliquant l'algo de Bresenham:
            ei_point_t courant = *(point_array)++;
            ei_point_t suivant = *(point_array)++;

            for(uint32_t cpt = point_array_size; cpt > 1; cpt--){
                algo_Bresenham(courant, suivant, &color, pixel_ptr, dimension, clipper);
                courant = suivant;
                suivant = *(point_array)++;
            
            }
        }
    }
}

/*******************************************************************************************************************************************/

void draw_point(uint32_t* pixel_ptr, ei_size_t dimension, ei_point_t point, ei_color_t* color){
    // on allume le point au bon endroit dans la surface avec la bonne couleur
    pixel_ptr[point.x + point.y * dimension.width] = *(uint32_t *)color;
}

/*******************************************************************************************************************************************/

void algo_Bresenham(ei_point_t origine, ei_point_t extremite, ei_color_t* color, uint32_t* pixel_ptr, ei_size_t dimension, const ei_rect_t* clipper) {
    // on stocke dans des variables locales les argumments pour eviter les multiples accès memoires
    int x0 = origine.x;
    int y0 = origine.y;
    int x1 = extremite.x;
    int y1 = extremite.y;

    // on calcul la pente, bref, les données de Bresenham
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    // on determine les directions verticales et horizontales
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    // on calcul l'erreur de base e0
    int err = dx - dy;

    while (!(x0 == x1 && y0 == y1)) {

        // on verifie si le point est dans le clipper avant meme de l'afficher
        // attention à modifier si optimisation analytique 
        if(est_dans_clipper(&(ei_point_t){x0, y0}, clipper)){
            draw_point(pixel_ptr, dimension, (ei_point_t){x0, y0}, color);
        }

        //if () break;

        // on recalcul l'erreur instantanée
        int e2 = 2 * err;

        // on determine si on avance (resp. recule) en x
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        // on determine si on avance (resp. recule) en y
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/*******************************************************************************************************************************************/

bool est_dans_clipper(ei_point_t* point, const ei_rect_t* clipper){
    // programmation defensive
    if(clipper != NULL){

        // on teste l'appartenance ou non du pixel en question au clipper
        if(((point->x >= clipper->top_left.x )&& (point->x <= (clipper->top_left.x + clipper->size.width))) && (point->y >= clipper->top_left.y && (point->y <= (clipper->top_left.y + clipper->size.height)))){
                return true;
        }
        return false;
    }
    return true;
}

/*******************************************************************************************************************************************/
void ajouter_un_cote_deb(table_de_cotes* table, ei_point_t* point1, ei_point_t* point2){
    cote* nouveau_cote = (cote*)malloc(sizeof(cote));

    // remplissage des donneés du cote :
    nouveau_cote->dx = abs(point1->x - point2->x);
    nouveau_cote->dy = abs(point1->y - point2->y);
    if(point1->y >= point2->y){
        nouveau_cote->ymax = point1->y;
        nouveau_cote->x_ymin = point2->x;
    }
    else{
        nouveau_cote->ymax = point2->y;
        nouveau_cote->x_ymin = point1->x;
    }

    nouveau_cote->suivant = NULL;


    

    // si la table est vide, alors nouveau_cote est le premier cote
    if(table->tete == NULL){
        table->tete = nouveau_cote;
        table->queue = nouveau_cote;
    }
    else{
        // on choisi le nouveau point comme la fin de la table:
        (table->queue)->suivant = nouveau_cote;
        table->queue = nouveau_cote;
    }
}


void ajouter_un_cote(table_de_cotes* table, ei_point_t* point1, ei_point_t* point2){
    cote* nouveau_cote = (cote*)malloc(sizeof(cote));

    // remplissage des donneés du cote :
    nouveau_cote->dx = abs(point1->x - point2->x);
    nouveau_cote->dy = abs(point1->y - point2->y);
    if(point1->y >= point2->y){
        nouveau_cote->ymax = point1->y;
        nouveau_cote->x_ymin = cherche_xymin(point1,point2);
    }
    else{
        nouveau_cote->ymax = point2->y;
        nouveau_cote->x_ymin = cherche_xymin(point2,point1);
    }

    nouveau_cote->suivant = NULL;


    

    // si la table est vide, alors nouveau_cote est le premier cote
    if(table->tete == NULL){
        table->tete = nouveau_cote;
        table->queue = nouveau_cote;
    }
    else{
        // on choisi le nouveau point comme la fin de la table:
        (table->queue)->suivant = nouveau_cote;
        table->queue = nouveau_cote;
    }
}

/*******************************************************************************************************************************************/

void supprimer_un_cote(table_de_cotes* TCA, cote* cote_a_supp)
{
    // on verifie si TCA est alloueer et que la liste n'est pas vide 
    if (TCA!=NULL && TCA->tete!=NULL)
    {
        cote* cour = TCA->tete;
        
        // si l'element à supp est le premier element
        if (cour==cote_a_supp)
        {
            TCA->tete = cour->suivant;
            free(cour);
        }
        else{
            // on parcours la liste jusqu'a l'element precedent l'eleement à supp
            while (cour!=NULL && cour->suivant != cote_a_supp)
            {
                cour = cour->suivant;
            }
            
            // si on a trouve l'element à supp
            if(cour != NULL){
                // s'il s'agit de l'element queue de la liste
                if (cote_a_supp->suivant == NULL)
                {
                    TCA->queue = cour;
                }
                // on supprime l'element enfin !!!!!
                cour->suivant = cote_a_supp->suivant;
                free(cote_a_supp);
            }
            
        }
    }
}

/*******************************************************************************************************************************************/

int cherche_xymin(ei_point_t* origine, ei_point_t* extremite){
    // on stocke dans des variables locales les argumments pour eviter les multiples accès memoires
    int x0 = origine->x;
    int y0 = origine->y;
    int x1 = extremite->x;
    int y1 = extremite->y;

    // on calcul la pente, bref, les données de Bresenham
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    // on determine si de quelle direction on veut allez (gauche -> droite, haut-> bas et inversement) et inversement etc.....
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    // on calcul l'erreur de base e0
    int err = dx - dy;

    while (!(y0 == y1)) {
        // on recalcul l'erreur instantanée
        int e2 = 2 * err;

        // on determine si on avance (resp. recule) en x
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        // on determine si on avance (resp. recule) en y
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    // on retourne x_ymin
    return x0;
}

/*******************************************************************************************************************************************/

table_de_cotes* creer_ligne_TC(ei_point_t** point_array, size_t point_array_size, uint32_t indice_elem){

    // on verifie que c'est un polygone (minimum trois points )
    if(point_array_size >= 3){
    
        // on creer table de cote pour chaque scan:
        table_de_cotes* table = (table_de_cotes*)malloc(sizeof(table_de_cotes));
        table->tete = NULL;
        ei_point_t* element_min_y = (point_array[indice_elem]);

        
        //on verifie si c'est  le premieer point:
        if(indice_elem == 0){
           if(point_array[point_array_size -2 ] != NULL || point_array[indice_elem + 1] != NULL){
                // si le deux adjacent n'ont pas deja ete traité:
                if(point_array[point_array_size -2 ] != NULL && point_array[indice_elem + 1 ] != NULL){
                    // on commence par le point adjacent de coordonnée y minimum
                    if( (point_array[point_array_size -2 ])->y <= (point_array[indice_elem + 1])->y){
                        // si le point precedent l'indice min est de coordonnée minimal par rapport au point suivant de l'indice min 
                        // si les deux points ne sont pas aligné horizontalement:
                        if(!est_alignee_horizontal(element_min_y,point_array[point_array_size -2 ])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[point_array_size -2 ]);
                        }

                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem + 1]);
                        }
                    }
                    else{
                        // sinon on inverse l'ordre de placement dans la liste
                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem + 1]);
                        }
                        if(!est_alignee_horizontal(element_min_y,point_array[point_array_size -2 ])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[point_array_size -2 ]);
                        }
                    }
                }else if(point_array[point_array_size -2] != NULL){
                    if(!est_alignee_horizontal(element_min_y,point_array[point_array_size -2])){
                        ajouter_un_cote_deb(table, element_min_y, point_array[point_array_size -2]);
                    }
                }
                else{
                    if(!est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                        ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem + 1]);
                    }
                }
           }
            
        }
        else if(indice_elem == point_array_size - 2){
            if(point_array[0] != NULL || point_array[indice_elem - 1] != NULL){
                // si le deux adjacent n'ont pas deja ete traité:
                if(point_array[0] != NULL && point_array[indice_elem - 1 ] != NULL){
                    // on commence par le point adjacent de coordonnée y minimum
                    if( (point_array[0 ])->y <= (point_array[indice_elem - 1])->y){
                        // si le point precedent l'indice min est de coordonnée minimal par rapport au point suivant de l'indice min 
                        // si les deux points ne sont pas aligné horizontalement:
                        if(!est_alignee_horizontal(element_min_y,point_array[0])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[0]);
                        }

                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem - 1]);
                        }
                    }
                    else{
                        // sinon on inverse l'ordre de placement dans la liste
                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem - 1]);
                        }
                        if(!est_alignee_horizontal(element_min_y,point_array[0])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[0]);
                        }
                    }
                }else if(point_array[0] != NULL){
                    if(!est_alignee_horizontal(element_min_y,point_array[0])){
                        ajouter_un_cote_deb(table, element_min_y, point_array[0]);
                    }
                }
                else{
                    if(!est_alignee_horizontal(element_min_y,point_array[indice_elem  - 1])){
                        ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem - 1]);
                    }
                }
           }
        }
        else
        {
            // si le point a au moins un adjacent:
            if(point_array[indice_elem - 1 ] != NULL || point_array[indice_elem + 1 ] != NULL ){
                // si le deux adjacent n'ont pas deja ete traité:
                if(point_array[indice_elem -1] != NULL && point_array[indice_elem + 1] != NULL){
                    // on commence par le point adjacent de coordonnée y minimum
                    if( (point_array[indice_elem - 1])->y <= (point_array[indice_elem + 1])->y){
                        // si le point precedent l'indice min est de coordonnée minimal par rapport au point suivant de l'indice min 
                        // si les deux points ne sont pas aligné horizontalement:
                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem - 1]);
                        }

                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem + 1]);
                        }
                    }
                    else{
                        // sinon on inverse l'ordre de placement dans la liste
                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem + 1]);
                        }
                        if(!est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                            ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem - 1]);
                        }
                    }
                }else if(point_array[indice_elem -1 ] != NULL){
                    if(!est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                        ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem - 1]);
                    }
                }
                else{
                    if(!est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                        ajouter_un_cote_deb(table, element_min_y, point_array[indice_elem + 1]);
                    }
                }
            
            }
        }
        return table;
    }
    
}

/*******************************************************************************************************************************************/

bool est_alignee_horizontal(ei_point_t* point_min, ei_point_t* adjacent){
    // annuyeuse programmation defensive , oh lala
    if (point_min != NULL  && adjacent != NULL){
        if(point_min->y == adjacent->y)
            return true;
        return false;
    }
}

/*******************************************************************************************************************************************/

uint32_t* construit_tab_y_min(ei_point_t* point_array, size_t point_array_size){


    // on creer un tableau d'indices et on evite le dernier point car il s'agit d'une figure et donc d'un cycle elementaire (cf. RO)
    uint32_t *indices = malloc((point_array_size -1 ) * sizeof(uint32_t));
    if (!indices) return NULL;

    // on creer un tableau de pointeur vers les coordonnées y :
    ei_point_t** addr_coordonnees_y = (ei_point_t**)malloc((point_array_size - 1) * sizeof(ei_point_t*));
    // on initialise les adresses des points :
    for(uint32_t cpt = 0; cpt < point_array_size -1; cpt++){
        addr_coordonnees_y[cpt] = & (point_array[cpt]);
    }

    // on cherche les mins respecctivement :
    for(uint32_t cpt = 0; cpt < point_array_size -1 ; cpt++){
        indices[cpt] = cherche_min(addr_coordonnees_y, point_array_size -1 );
    }


    return indices;
}

uint32_t cherche_min(ei_point_t** addr_coorddonnee_y, size_t point_array_size){
    uint32_t ind_min, min, cpt;

    // on  cherche à initialise le min et donc, indice_min
    // pour cela, on cherche la premiere addresse non null de addr_coorddonnee_y
    for(cpt = 0; cpt < point_array_size; cpt++){
        if(addr_coorddonnee_y[cpt] != NULL){
            ind_min = cpt;
            min = (addr_coorddonnee_y[cpt])->y;
            cpt++;
            break;
        }
    }

    // on parcours le reste du tableau à la recherche du vrai min:
    for(uint32_t i = cpt; i < point_array_size; i++){
        // on ne compare que dans des addresses valides:
        if(addr_coorddonnee_y[i] != NULL){
            if(min > (addr_coorddonnee_y[i])->y){
                ind_min = i;
                min = (addr_coorddonnee_y[i])->y;
            }
        }
    }

    // on tag comme vu le min actuel:
    addr_coorddonnee_y[ind_min] = NULL;

    // on retourne l'indice du min
    return ind_min;
}

/*******************************************************************************************************************************************/

liste_de_table_de_cotes* creer_TC(ei_point_t* point_array, size_t point_array_size, uint32_t* taille_tc){
    // on construit un tableau d'indices croissant de y min 
    uint32_t* indice_y_min_triee = construit_tab_y_min(point_array, point_array_size);
    int ymin = (point_array[indice_y_min_triee[0]]).y;
    int ymax = (point_array[indice_y_min_triee[point_array_size - 2]]).y;
    *taille_tc = ymax - ymin + 1;


    // on alloue la table TC de taille ymax - ymin
    liste_de_table_de_cotes* TC = (liste_de_table_de_cotes*)malloc(((*taille_tc)) * sizeof(liste_de_table_de_cotes));   // segfault $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    // for(uint32_t cpt = 0; cpt < (*taille_tc) + 1; cpt++){
    //     TC[cpt].tete_ligne_tc = (table_de_cotes*) malloc(sizeof(table_de_cotes));
    //     TC[cpt].tete_ligne_tc->queue = NULL;
    //     TC[cpt].tete_ligne_tc->tete = NULL;
    // }

    // on creer un tableau qui contient l'adresse des points de point_array pour pouvoir le passer en parametre à creeer ligne TC
    
    ei_point_t** addr_point_array = (ei_point_t**)malloc(point_array_size * sizeof(ei_point_t*));
    for(uint32_t cpt = 0; cpt < point_array_size; cpt++)
        addr_point_array[cpt] = &(point_array[cpt]);

    // compteur sur les indices des ymin croissant : 
    uint32_t ind_y_min = indice_y_min_triee[0], prochain_y = 1;

    // on parcours TC et on le remplis par scanline
    // on utilise pour cela un compteur cpt allant de 0 à la (taille - 1) de TC 
    for(uint32_t cpt = 0; cpt < *taille_tc; cpt++){
        // on verifie si dans la scanline actuel de valeur (cpt + ymin) coincide avec le debut d'un coté
        TC[cpt].scan_line = cpt+ymin;

        if(cpt + ymin == point_array[ind_y_min].y){
            TC[cpt].tete_ligne_tc = creer_ligne_TC(addr_point_array, point_array_size, ind_y_min);
            addr_point_array[ind_y_min] = NULL;
            ind_y_min = indice_y_min_triee[prochain_y];
            prochain_y++;
        }
        else{
            // sinon si la scanline cpt ne correspond à aucun debut de segment;
            TC[cpt].tete_ligne_tc = NULL;
        }
    }

    printf("\n\n\nn\n\n\n\n\n **************** scan ling = %u voici le dernier ymax de TC %f\n\n\n\n\n\n",TC[(*taille_tc) -1].scan_line, TC[(*taille_tc) -1].tete_ligne_tc->queue->ymax);
    
    //afficher_TC(TC, *taille_tc);
    return TC;

}

void afficher_TC(liste_de_table_de_cotes* TC, uint32_t taille_tc) {
    if (TC == NULL) {
        printf("TC est vide.\n");
        return;
    }

    for (uint32_t i = 0; i < taille_tc; i++) {
        printf("Scanline %d :\n", TC[i].scan_line);

        table_de_cotes* ligne = TC[i].tete_ligne_tc;
        if (ligne == NULL || ligne->tete == NULL) {
            printf("  (aucune côte)");
            continue;
        }

        cote* courant = ligne->tete;
        while (courant != NULL) {
            printf("  Côte -> ymax: %.0f, x_ymin: %.0f, dx: %.0f, dy: %.0f   -->  ",
                   courant->ymax, courant->x_ymin, courant->dx, courant->dy);
            courant = courant->suivant;
            printf("\n");
        }
    }
}


/*******************************************************************************************************************************************/

void ei_draw_polygon(ei_surface_t surface, ei_point_t* point_array, size_t point_array_size, ei_color_t color, const ei_rect_t* clipper){
    // recupération des infos de la surface passée en paramètre :
    ei_size_t dimension = hw_surface_get_size(surface);
    uint32_t largeur = dimension.width;
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(surface);

    
    // on creer TC :
    uint32_t taille_tc = 0;
    liste_de_table_de_cotes* TC = creer_TC(point_array, point_array_size, &taille_tc);
    //afficher_TC(TC, taille_tc);
    // on initialise TCA à une liste vide de ligne de TC:
    table_de_cotes* TCA = (table_de_cotes*)malloc(sizeof(table_de_cotes));
    TCA->tete = NULL;
    TCA->queue = NULL;

    // on initialise les variables representant les segments : 
    cote* courant;
    cote*  suivant;

    liste_de_table_de_cotes scanline;
    for(uint32_t cpt = 0; cpt < taille_tc-1 ; cpt++){
        scanline = TC[cpt];
        //printf("%u = ymax\n", (scanline->tete_ligne_tc)->tete->ymax);
        // si dans cette scanline, il ya des nouveaux cotés :
        
        if(scanline.tete_ligne_tc != NULL){
            // on ajoute à TCA, la liste des coté sur cette scanline
            // si TCA est vide : 
            printf("avant => \n");
            afficher_x_ymin(TCA);
            if(TCA->queue == NULL){
                TCA->tete = scanline.tete_ligne_tc->tete;
                TCA->queue = scanline.tete_ligne_tc->queue;
            }
            else{
                // si la TCA non vide :
                TCA->queue->suivant = scanline.tete_ligne_tc->tete;
                TCA->queue = scanline.tete_ligne_tc->queue;
            }
            printf("arpès ------------------------------------------------ => \n");
            afficher_x_ymin(TCA);
            if(TCA == NULL){
                break;
            }
            
        }
        // on trie TCA en xymin
        //printf("avant trie : \n");
        TCA = tri_TCA_en_xymin(TCA);
       // printf("après le trie trie : \n");
        //afficher_x_ymin(TCA);

        // on affiche les segments :
        courant = TCA->tete;

        while (courant != NULL && courant->suivant != NULL)
        {
            suivant = courant->suivant;

            //colorie_segment(pixel_ptr, &color, (largeur * scanline.scan_line + courant->x_ymin),
            // (largeur * scanline.scan_line + suivant->x_ymin) );
            uint32_t produit = scanline.scan_line * dimension.width;
            for(uint32_t x = courant->x_ymin; x <= suivant->x_ymin; x++){
                draw_point(pixel_ptr, dimension,(ei_point_t){x, scanline.scan_line},&color);
            }
             courant = suivant->suivant;
           
        }
        

        // mise à jour des xymin
        maj_x_ymin(TCA);

        supprimer_cotes_ymax_sup_ou_egal(TCA, scanline.scan_line + 1);
        
        if(TCA == NULL && cpt == taille_tc - 2)
            break;
    }
    
}

void supprimer_cotes_ymax_sup_ou_egal(table_de_cotes* table, int max) {
    if (table == NULL || table->tete == NULL)
        return;

    cote* courant = table->tete;
    cote* precedent = NULL;

    while (courant != NULL) {
        if (courant->ymax <= max) {
            cote* a_supprimer = courant;
            printf(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ on supprime %.0f\n", a_supprimer->ymax);
            afficher_x_ymin(table);
            // Cas où l'élément à supprimer est en tête
            if (precedent == NULL) {
                table->tete = courant->suivant;
                courant = table->tete;
            } else {
                precedent->suivant = courant->suivant;
                courant = precedent->suivant;
            }

            // Si l'élément supprimé était en queue
            if (a_supprimer == table->queue) {
                table->queue = precedent;
            }
            
            afficher_x_ymin(table);
            printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            free(a_supprimer);
        } else {
            precedent = courant;
            courant = courant->suivant;
        }
    }
}


void colorie_segment(uint32_t* pixel_ptr, ei_color_t* color, uint32_t debut, uint32_t fin){
    
    for(uint32_t cpt = debut; cpt < fin+ 1 ; cpt++){  // attention segfault $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
        pixel_ptr[cpt] = *(uint32_t *)color;
    }
}

void afficher_x_ymin(const table_de_cotes* table) {
    if (table == NULL || table->tete == NULL) {
        printf("La table de cotes est vide.\n");
        return;
    }

    cote* courant = table->tete;
    printf("Liste des x_ymin dans la table de cotes :\n");
    while (courant != NULL) {
        printf("x_ymin = %.0f -> ", courant->x_ymin);
        courant = courant->suivant;
    }
    printf(" END\n");
}
/*******************************************************************************************************************************************/

table_de_cotes* tri_TCA_en_xymin(table_de_cotes* TCA){
    // programmation defensive
    if(TCA != NULL && TCA->queue != NULL){
        table_de_cotes* TCA_triee = (table_de_cotes*)malloc(sizeof(table_de_cotes));
        TCA_triee->queue = NULL;
        TCA_triee->tete = NULL;
        cote* courant = NULL;
        cote* cellule_min_prec = NULL;
        int xymin_val = 0;

        while (TCA->tete != NULL)
        {
            // on parcours la liste en recherche du min parmi les xymin
            courant = TCA->tete;
            xymin_val = courant->x_ymin;
            cellule_min_prec = courant;
            bool debut = true;
            while (courant->suivant != NULL)
            {
                if(xymin_val >  courant->suivant->x_ymin){
                    cellule_min_prec = courant;
                    xymin_val = cellule_min_prec->suivant->x_ymin;
                    debut = false;
                }
                courant = courant->suivant;
            }
            
            // si tca triee est vide :
            if(TCA_triee->queue == NULL){
                TCA_triee->tete = couper_le_min(TCA,cellule_min_prec, debut);
                TCA_triee->queue = TCA_triee->tete;
            }
            else{
                TCA_triee->queue->suivant = couper_le_min(TCA, cellule_min_prec, debut);
                TCA_triee->queue = TCA_triee->queue->suivant;
            }
            
        }
        //free(TCA);
        return TCA_triee;
        
    }
    return NULL;
}

cote* couper_le_min(table_de_cotes* TCA, cote* cellule_min_prec, bool debut){
    // si le min esst en tete:
    if(debut){
        TCA->tete = cellule_min_prec->suivant;
        return cellule_min_prec;
    }
    else{
        if (cellule_min_prec->suivant == NULL){
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!     Erreur     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
        cote* save_cote = cellule_min_prec->suivant;
        cellule_min_prec->suivant = save_cote->suivant;   // attention segfault $$$$$$$$$$$$$$$$$$$$$$$$$$$$$
        return save_cote;
    }
}

/*******************************************************************************************************************************************/

void maj_x_ymin(table_de_cotes* TCA){
    
    if (TCA!=NULL){
        cote* cour=TCA->tete;
        int num = 0, den = 0;
        float frac = 0;
        
        while(cour!=NULL){
            num = cour->dx;
            den=cour->dy;
            frac=((float)num/den);
            cour->x_ymin=cour->x_ymin + frac;
            cour=cour->suivant;
        }
    }
}

/*******************************************************************************************************************************************/



// ei_point_t* arc(float cx, float cy, float rayon, float angle_debut_arc, float angle_fin_arc){
//     uint32_t nombre_de_points = (angle_fin_arc-angle_debut_arc)*rayon;
//     ei_point_t* points = malloc(nombre_de_points * sizeof(ei_point_t));
//     float angle_saut = (angle_fin_arc - angle_debut_arc) / (nombre_de_points - 1);
//     for (uint32_t i = 0; i < nombre_de_points; i++) {
//         float angle = start_angle + i * angle_saut;
//         points[i].x = (uint32_t)(cx + rayon * cosf(angle));
//         points[i].y = (uint32_t)(cy + rayon * sinf(angle));
//     }
//     return points;
// }
