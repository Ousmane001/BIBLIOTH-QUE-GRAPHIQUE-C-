#include "ei_draw.h"
#include <stdio.h>
#include "ei_implementation.h"

/*******************************************************************************************************************************************/

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

void	ei_draw_polygon		(ei_surface_t		surface,
                 ei_point_t*		point_array,
                 size_t			point_array_size,
                 ei_color_t		color,
                 const ei_rect_t*	clipper)
{

    // on cherche le y_min et y_max du polygone pour determiner la taille de la TC
    int y_min = point_array[0].y;
    int y_max = point_array[0].y;
    for (uint32_t i = 1; i < point_array_size; i++)
    {
        if (point_array[i].y < y_min)
        {
            y_min = point_array[i].y;
        }
        if (point_array[i].y > y_max)
        {
            y_max = point_array[i].y;
        }
    }

    //  construction de TC:
    uint32_t TC_size = y_max - y_min + 1;
    TC_t* TC = construire_TC(point_array, point_array_size, y_min, y_max);

    // Initialisation de TCA à une liste vide:
    TC_line_table* TCA = (TC_line_table*)malloc(sizeof(TC_line_table));
    TCA->head = NULL;
    TCA->tail = NULL;

    // on parcours la figure en hauteur :
    for (uint32_t cpt = 0; cpt <= TC_size; cpt++)
    {
        // si la scanline est un sommet du polygone
        if (TC[cpt].head_of_TC != NULL)
        {
            // on ajoute la ligne de tc à TCA
            if (TCA->head == NULL)
            {
                TCA->head = TC[cpt].head_of_TC->head;
                TCA->tail = TCA->head;
            }
            else
            {
                TCA->tail->next = TC[cpt].head_of_TC->head;
                TCA->tail = TCA->tail->next;

            }
        }
        // verifie si un segment à attein son ymax:
        supprime_cote_ymax_atteint(TCA, TC[cpt].scan_line + 1);  // +1 car on veut quand meme l'afficher pour ce tour de boucle
        
        // on trie TCA :
        TCA = tri_line_table(TCA);

        // on affiche enfin les pixel sous reserve du clipper:
        affiche_pixel_scanline(surface, TCA, cpt + y_min, &color, clipper);

        // on met à jour les x_ymin :
        maj_en_x_ymin(TCA);
    }



    // liberation de la memoire:
    free(TC);
    free(TCA);
    TC = NULL;
    TCA = NULL;
}

/********************************************************************************************************************/

void supprime_cote_ymax_atteint(TC_line_table* TCA, uint32_t scanline)
{
    // programmation defensive:
    if (TCA == NULL || TCA->head == NULL)
    {
        return;
    }
    // on parcours la TCA et on supprime tous les segments  dont ymax <= scanline
    segment* cour = TCA->head;
    segment* prec = cour;
    while (cour != NULL)
    {
        // si on a un ymax a supp
        if (cour->ymax < scanline)
        {
            segment* save_cour = cour;
            if (cour == TCA->head)
            {
                TCA->head = cour->next;
                prec = TCA->head;
                cour = prec;
            }
            else if (cour == TCA->tail)
            {
                TCA->tail = prec;
                cour =  prec->next;
                break;
            }
            else
            {
                prec->next = cour->next;
                cour =  prec->next;
            }

            free(save_cour);
        }
        else
            // on continue sur la liste
        {
            prec = cour;
            cour = cour->next;
        }

    }
}

/********************************************************************************************************************/

TC_line_table* tri_line_table(TC_line_table* table)
{
    // programmation defensive:
    if (table == NULL)
    {
        return NULL;
    }

    // on initialise une nouvelle TCA qui est celle triee
    TC_line_table* TCA = (TC_line_table*)malloc(sizeof(TC_line_table));
    TCA->head = NULL;
    TCA->tail = NULL;

    // on parcours la table actuelle à chaque fois on cherche le min de xymin, on le rajoute en queue de TCA
    // et on l'enleve de la table, on repete l'operation tant que la table n'est pas vide
    segment* cour_min = table->head;
    segment* prec_min = cour_min;

    // tant que la table n'est pas vide on chrche le min:
    while (table->head != NULL)
    {
        // on cherche le min:
        segment* cour = table->head;
        while (cour->next != NULL)
        {
            if (cour->next->x_ymin < cour_min->x_ymin)
            {
                cour_min = cour->next;
                prec_min = cour;
            }
            cour = cour->next;
        }
        // on ajoute le min a TCA:
        if (TCA->head == NULL)
        {
            TCA->head = cour_min;
            TCA->tail = cour_min;
        }
        else
        {
            TCA->tail->next = cour_min;
            TCA->tail = cour_min;

        }
        // on l'enleve de la table:
        // si le min est en tete:
        if (cour_min == table->head)
        {
            table->head = cour_min->next;
        }
        // si le min est en queue:
        else if (cour_min == table->tail)
        {
            table->tail = prec_min;
            prec_min->next = NULL;

        }
        else
        {
            prec_min->next = cour_min->next;
        }
        // on reinitialise le min:
        cour_min = table->head;
        prec_min = cour_min;


    }
    free(table);
    // on retourne la TCA triee:
    return TCA;

}

/********************************************************************************************************************/

void affiche_pixel_scanline(ei_surface_t surface, TC_line_table* TCA, uint32_t scanline, ei_color_t* color, const ei_rect_t* clipper)
{
    // on recupere les info de la surface:
    ei_size_t dimension = hw_surface_get_size(surface);
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(surface);

    segment* cour = TCA->head;

    // on parcours la TCA:
    while (cour != NULL)
    {
        segment* suivant = cour->next;

        uint32_t fin = suivant->x_ymin;
        ei_point_t point = {cour->x_ymin, scanline};
        // on affiche les pixels de la scanline courante:
        for (int x = cour->x_ymin; x <= fin; x++)
        {
            point.x = x;
            // on verifie si le point est dans le clipper:
            if (est_dans_clipper(&point, clipper))
            {
                draw_point(pixel_ptr, dimension, point, color);
            }
        }
        // on passe au suivant segment:
        cour = suivant->next;

    }


}

/********************************************************************************************************************/

void maj_en_x_ymin(TC_line_table* TCA)
{
    // programmation defensive:
    if (TCA == NULL)
    {
        return;
    }
    // on parcours la TCA et on met a jour les x_ymin de tous les segments:
    segment* cour = TCA->head;
    while (cour != NULL)
    {
        cour->x_ymin += cour->pente;
        cour = cour->next;
    }
}

/********************************************************************************************************************/

TC_t* construire_TC(ei_point_t* point_array, uint32_t point_array_size, int y_min, int y_max)
{
    // programmation defensive:
    if (point_array == NULL || point_array_size == 0)
    {
        return NULL;
    }
    // on initialise la taille de TC:
    uint32_t taille_TC = y_max - y_min + 1;



    // on initialise  La TC
    TC_t* TC = (TC_t*)malloc(sizeof(TC_t) * (taille_TC));
    //initialisaton de des champs de elements du tableau TC
    for (uint32_t i = 0; i < taille_TC; i++)
    {
        TC[i].scan_line = y_min + i;
        TC[i].head_of_TC = NULL;
    }

    // on rajoute la liste des segments correspondant aux cotés du polygone:
    for (uint32_t i = 0; i < point_array_size - 1; i++)
    {
        ei_point_t point1 = point_array[i], point2 = point_array[i + 1];
        if (point1.y < point2.y)
        {
            ajouter_segment(TC, point1, point2, y_min);
        }
        else if (point1.y > point2.y)
        {
            ajouter_segment(TC, point2, point1, y_min);
        }
        else
        {
            // on ne fait rien car coté horizontale
        }
    }


    // on return TC
    return TC;
}

/********************************************************************************************************************/

void ajouter_segment(TC_t *TC, ei_point_t point1, ei_point_t point2, int y_min)
{
    // si head_line TC est null, on l'alloue
    if (TC[point1.y - y_min].head_of_TC == NULL)
    {
        TC[point1.y - y_min].head_of_TC = (TC_line_table*)malloc(sizeof(TC_line_table));
        TC[point1.y - y_min].head_of_TC->head = creer_segment(point1,point2);

       TC[point1.y - y_min].head_of_TC->tail = TC[point1.y - y_min].head_of_TC->head;
    }
    else
    {
        TC[point1.y - y_min].head_of_TC->tail->next = creer_segment(point1, point2);
        TC[point1.y  - y_min].head_of_TC->tail = TC[point1.y  - y_min].head_of_TC->tail->next;
    }

}

/********************************************************************************************************************/

segment* creer_segment(ei_point_t point1, ei_point_t point2)
{
    // Allocation dynamique pour un nouveau segment
    segment* nouveau_seg = (segment*)malloc(sizeof(segment));
    if (nouveau_seg == NULL) {
        // Si l'allocation échoue, retourner NULL
        return NULL;
    }

    nouveau_seg->next = NULL;
    nouveau_seg->x_ymin = point1.x;
    nouveau_seg->ymax = point2.y;
    nouveau_seg->pente = ((float)(point1.x - point2.x) / (float)(point1.y - point2.y));

    // Initialiser le champ next à NULL
    nouveau_seg->next = NULL;

    return nouveau_seg;
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
