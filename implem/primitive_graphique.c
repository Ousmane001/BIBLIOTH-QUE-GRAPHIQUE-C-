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

void ajouter_un_cote(table_de_cotes* table, ei_point_t* point1, ei_point_t* point2){
    cote* nouveau_cote = (cote*)malloc(sizeof(cote));

    // remplissage des donneés du coté :
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

    // si la table est non vide, alors insertion en queue
    if (table->queue != NULL) {
        table->queue->suivant = nouveau_cote;
    }
    else {
        // si la table est vide, alors nouveau_cote est le premier cote
        table->tete = nouveau_cote;
    }
    table->queue = nouveau_cote;
}

/*******************************************************************************************************************************************/

void supprimer_un_cote(table_de_cotes* TCA, cote* cote)
{
    // on verifie si TCA est alloueer et que la liste n'est pas vide 
    if (TCA!=NULL && TCA->tete!=NULL)
    {
        cote* cour = TCA->tete;
        
        // si l'element à supp est le premier element
        if (cour==cote)
        {
            TCA->tete = cour->suivant;
            free(cour);
        }
        else{
            // on parcours la liste jusqu'a l'element precedent l'eleement à supp
            while (cour!=NULL && cour->suivant != cote)
            {
                cour = cour->suivant;
            }
            
            // si on a trouve l'element à supp
            if(cour != NULL){
                // s'il s'agit de l'element queue de la liste
                if (cour->suivant->suivant==NULL)
                {
                    TCA->queue = cour;
                }
                // on supprime l'element enfin !!!!!
                cour->suivant =cour-> suivant->suivant;
                free(cote);
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

table_de_cotes* creer_TC(ei_point_t** point_array, size_t point_array_size, uint32_t indice_elem){

    // on verifie que c'est un polygone (minimum trois points )
    if(point_array_size >= 3){
    
        // on creer table de cote pour chaque scan:
        table_de_cotes* table = (table_de_cotes*)malloc(sizeof(table_de_cotes));
        table->queue = NULL;
        ei_point_t element_min_y = point_array[indice_elem];

        
        //on verifie si c'est  le premieer point:
        if(indice_elem == 0){
           if(point_array[point_array_size -2 ] != NULL || point_array[indice_elem + 1] != NULL){
                // si le deux adjacent n'ont pas deja ete traité:
                if(point_array[point_array_size -2 ] != NULL && point_array[indice_elem + 1 ] != NULL){
                    // on commence par le point adjacent de coordonnée y minimum
                    if( (point_array[point_array_size -2 ])->y <= (point_array[indice_elem + 1])->y){
                        // si le point precedent l'indice min est de coordonnée minimal par rapport au point suivant de l'indice min 
                        // si les deux points ne sont pas aligné horizontalement:
                        if(est_alignee_horizontal(element_min_y,point_array[point_array_size -2 ])){
                            ajouter_un_cote(table, element_min_y, point_array[point_array_size -2 ]);
                        }

                        if(est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote(table, element_min_y, point_array[indice_elem + 1]);
                        }
                    }
                    else{
                        // sinon on inverse l'ordre de placement dans la liste
                        if(est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote(table, element_min_y, point_array[indice_elem + 1]);
                        }
                        if(est_alignee_horizontal(element_min_y,point_array[point_array_size -2 ])){
                            ajouter_un_cote(table, element_min_y, point_array[point_array_size -2 ]);
                        }
                    }
                }else if(point_array[point_array_size -2] != NULL){
                    if(est_alignee_horizontal(element_min_y,point_array[point_array_size -2])){
                        ajouter_un_cote(table, element_min_y, point_array[point_array_size -2]);
                    }
                }
                else{
                    if(est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                        ajouter_un_cote(table, element_min_y, point_array[indice_elem + 1]);
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
                        if(est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                            ajouter_un_cote(table, element_min_y, point_array[indice_elem - 1]);
                        }

                        if(est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote(table, element_min_y, point_array[indice_elem + 1]);
                        }
                    }
                    else{
                        // sinon on inverse l'ordre de placement dans la liste
                        if(est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                            ajouter_un_cote(table, element_min_y, point_array[indice_elem + 1]);
                        }
                        if(est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                            ajouter_un_cote(table, element_min_y, point_array[indice_elem - 1]);
                        }
                    }
                }else if(point_array[indice_elem -1 ] != NULL){
                    if(est_alignee_horizontal(element_min_y,point_array[indice_elem - 1])){
                        ajouter_un_cote(table, element_min_y, point_array[indice_elem - 1]);
                    }
                }
                else{
                    if(est_alignee_horizontal(element_min_y,point_array[indice_elem + 1])){
                        ajouter_un_cote(table, element_min_y, point_array[indice_elem + 1]);
                    }
                }
            
            }
        }
        return table;
    }
    // sinon : 
    return NULL;
    
}

/*******************************************************************************************************************************************/

bool est_alignee_horizontal(ei_point_t* point_min, ei_point_t* adjacent){
    // annuyeuse programmation defensive , oh lala
    if (point_min != NULL  && adjacent != NULL){
        if(point_min->y == adjacent->y)
            return true;
        return false;
    }
    return false;
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

    // on desalloue addr-coordonnées-y
    free(addr_coordonnees_y);


    return indices;
}

/*******************************************************************************************************************************************/

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

    // on verifie que tous les addr ne sont pas null :
    if (cpt == point_array_size) {  //    à suppimer plutard car non nécéssaire $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
        return -1;  
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
