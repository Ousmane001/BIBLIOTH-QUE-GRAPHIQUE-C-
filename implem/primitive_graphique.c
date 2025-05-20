#include "ei_draw.h"
#include <stdio.h>
#include "ei_implementation.h"
#include "interracteur.h"
/*******************************************************************************************************************************************/
void ei_fill(ei_surface_t surface, const ei_color_t* color, const ei_rect_t* clipper) {
    // Récupération des informations de la surface
    ei_size_t dimension = hw_surface_get_size(surface);
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(surface);

    // Si le clipper est NULL, on remplit toute la surface
    if (clipper == NULL) {
        uint32_t color_value = *(uint32_t*)color; // Convertir la couleur en uint32_t
        uint32_t taille_surf = dimension.height * dimension.width;
        for (uint32_t i = 0; i < taille_surf; i++) {
            *pixel_ptr++ = color_value;
        }
        return;
    }

    // Récupération du rectangle de la surface
    ei_rect_t surf_rect = hw_surface_get_rect(surface);

    // Vérification de l'intersection entre la surface et le clipper
    int x_start = (surf_rect.top_left.x > clipper->top_left.x) ? surf_rect.top_left.x : clipper->top_left.x;
    int y_start = (surf_rect.top_left.y > clipper->top_left.y) ? surf_rect.top_left.y : clipper->top_left.y;


    int x_end = (surf_rect.top_left.x + surf_rect.size.width < clipper->top_left.x + clipper->size.width) ?
                surf_rect.top_left.x + surf_rect.size.width : clipper->top_left.x + clipper->size.width;

                
    int y_end = (surf_rect.top_left.y + surf_rect.size.height < clipper->top_left.y + clipper->size.height) ?
                surf_rect.top_left.y + surf_rect.size.height : clipper->top_left.y + clipper->size.height;

    // Si l'intersection est vide (clipper est complètement en dehors de la surface)
    if (x_start >= x_end || y_start >= y_end) {
        return; // Aucune zone à remplir
    }

    // Pré-calcul de la couleur (en uint32_t)
    uint32_t color_value = *(uint32_t*)color;

    // Remplissage de la zone d'intersection ligne par ligne
    for (int y = y_start; y < y_end; y++) {
        // Calcul du pointeur de départ pour la ligne en cours (pas de décalage pour y)
        uint32_t* row_ptr = pixel_ptr + (y * dimension.width); // L'index de la ligne est directement y

        // Remplissage de la ligne entre x_start et x_end
        for (int x = x_start; x < x_end; x++) {
            row_ptr[x] = color_value;  // Remplir le pixel
        }
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

ei_rect_t* ei_rect_intersection(ei_rect_t* rect1, ei_rect_t* rect2) {
    // programmation defensive :
    if(!(rect1 || rect2)){
        return NULL;
    }
    
    if(rect1 && !(rect2)){
        return rect1;
    }
        
    
    if(rect2 && !(rect1)){
        return rect2;
    }
    
// Calcul des coordonnées du rectangle d'intersection
    int x_start = (rect1->top_left.x > rect2->top_left.x) ? rect1->top_left.x : rect2->top_left.x;
    int y_start = (rect1->top_left.y > rect2->top_left.y) ? rect1->top_left.y : rect2->top_left.y;

    int x_end = (rect1->top_left.x + rect1->size.width < rect2->top_left.x + rect2->size.width) ?
                rect1->top_left.x + rect1->size.width : rect2->top_left.x + rect2->size.width;

    int y_end = (rect1->top_left.y + rect1->size.height < rect2->top_left.y + rect2->size.height) ?
                rect1->top_left.y + rect1->size.height : rect2->top_left.y + rect2->size.height;

    // Si les rectangles ne se croisent pas, retourner NULL
    if (x_start >= x_end || y_start >= y_end) {
        return NULL; // Pas d'intersection
    }

    // Créer un rectangle d'intersection
    ei_rect_t* intersection = malloc(sizeof(ei_rect_t));
    if (!intersection) {
        return NULL; // Erreur d'allocation mémoire
    }

    intersection->top_left.x = x_start;
    intersection->top_left.y = y_start;
    intersection->size.width = x_end - x_start;
    intersection->size.height = y_end - y_start;

    return intersection;
}

void afficher_attributs_rectangle(ei_rect_t* rect) {
    if (rect == NULL) {
        printf("Rectangle: NULL\n");
    } else {
        printf("Rectangle: (%d, %d), Width: %d, Height: %d\n",
               rect->top_left.x, rect->top_left.y, rect->size.width, rect->size.height);
    }
}

void ei_fill_optim(ei_surface_t surface, ei_color_t* couleur, ei_rect_t* clipper1, ei_rect_t* clipper2){
    ei_rect_t* intersection = ei_rect_intersection(clipper1, clipper2);

    if(intersection == NULL)
        return;

    ei_fill(surface, couleur, intersection);
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

void	ei_draw_polyline_analytique	(ei_surface_t		surface,
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
            // sinon on itere sur les couples de points successivemnt en appliquant l'algo de Bresenham

            for (uint32_t i = 0; i < point_array_size - 1; i++) {
                ei_point_t courant = point_array[i];
                ei_point_t suivant = point_array[i + 1];
                if (clip_segment(&courant, &suivant, clipper)) {
                    algo_Bresenham(courant, suivant, &color, pixel_ptr, dimension, clipper);
                }
            }
            
            }
        }
    }




/*******************************************************************************************************************************************/

void algo_Bresenham_analytique(ei_point_t origine, ei_point_t extremite, ei_color_t* color, uint32_t* pixel_ptr, ei_size_t dimension, const ei_rect_t* clipper) {
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
        draw_point(pixel_ptr, dimension, (ei_point_t){x0, y0}, color);
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

// Fonction de calcul du code Cohen-Sutherland
int cohen_code(ei_point_t p, const ei_rect_t* clipper) {
    int code = 0;
    if (p.y > clipper->top_left.y + clipper->size.height) {code |= CODE_DESSOUS;}
    if (p.y < clipper->top_left.y)                       {code |= CODE_DESSUS;}
    if (p.x > clipper->top_left.x + clipper->size.width) {code |= CODE_DROITE;}
    if (p.x < clipper->top_left.x)                       {code |= CODE_GAUCHE;}
    return code;
}


bool clip_segment(ei_point_t* origine, ei_point_t* extremite, const ei_rect_t* clipper) {
    if (clipper==NULL) return true;
    /// On vérifie que les deux extrémités du segment dans le clipper ou pas (retourne 0000 si c'est le cas)
    int origine_dans_clipper = cohen_code(*origine, clipper);
    int extremite_dans_clipper = cohen_code(*extremite, clipper);

    while (1) {
        if ((origine_dans_clipper | extremite_dans_clipper) == 0) {
            // on accepte
            return true;
        } else if ((origine_dans_clipper & extremite_dans_clipper) != 0) {
            // On rejecte
            return false;
        } else {
            // Au moins un point est à l'extérieur : on clippe
            int code_out = origine_dans_clipper ? origine_dans_clipper : extremite_dans_clipper;
            ei_point_t nouvelle_extremite;

            // Coordonnées du clipper
            int xmin = clipper->top_left.x;
            int ymin = clipper->top_left.y;
            int xmax = xmin + clipper->size.width;
            int ymax = ymin + clipper->size.height;

            // Coordonnées initiales
            float x0 = origine->x;
            float y0 = origine->y;
            float x1 = extremite->x;
            float y1 = extremite->y;

            float x, y;

            if (code_out & CODE_DESSOUS) {
                // Intersection avec le bas
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (code_out & CODE_DESSUS) {
                // Intersection avec le haut
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (code_out & CODE_DROITE) {
                // Intersection avec la droite
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (code_out & CODE_GAUCHE) {
                // Intersection avec la gauche
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }
            nouvelle_extremite.x = x;
            nouvelle_extremite.y = y;
            if (code_out == origine_dans_clipper) {
                *origine = nouvelle_extremite;
                origine_dans_clipper = cohen_code(*origine, clipper);
            } else {
                *extremite = nouvelle_extremite;
                extremite_dans_clipper = cohen_code(*extremite, clipper);
            }
        }
    }
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
    for (uint32_t cpt = 0; cpt < TC_size; cpt++)
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

        if (suivant == NULL)
            break;
        uint32_t fin = floor(suivant->x_ymin);
        ei_point_t point = {cour->x_ymin, scanline};
        // on affiche les pixels de la scanline courante:
        for (int x = ceil(cour->x_ymin); x <= fin; x++)
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


void draw_arc(ei_point_t* arc, uint32_t nb_points, int x0, int y0, uint32_t rayon, double angle_debut, double angle_fin) {
    // Programmation defensive
    if (arc == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }


    // if (angle_fin < angle_debut) {
    //     angle_fin += 2 * PI;
    // }

    for (uint32_t i = 0; i < nb_points; i++) {
        double t = (double)i / (nb_points - 1);
        double angle = angle_debut + t * (angle_fin - angle_debut);
        arc[i].x = x0 + rayon * cos(angle);
        arc[i].y = y0 - rayon * sin(angle);
    }

}


void rounded_frame(ei_point_t* points, ei_rect_t* cadre, uint32_t rayon, partie_button partie)
{

    // on recupe les infos du boutton:
    uint32_t hauteur = cadre->size.height, largeur = cadre->size.width;
    ei_point_t top_left = cadre->top_left;

    // on definit les 2 points principaux diagononaux :

    ei_point_t pt1 = {top_left.x + largeur/3, top_left.y + hauteur/2}, pt2 = {top_left.x + 2*largeur/3, top_left.y + hauteur/2};
    uint32_t demi_rayon = rayon/2;

    // on traite en fonction de la partie à afficher
    switch (partie)
    {
        case HAUT:
            draw_arc(points,demi_rayon, top_left.x + largeur - rayon, top_left.y  + rayon, rayon, PI_SUR_4, PI_SUR_2);
            draw_arc(points + demi_rayon,rayon, top_left.x + rayon, top_left.y + rayon, rayon, PI_SUR_2, PI);
            draw_arc(points + 3 * demi_rayon, demi_rayon, top_left.x + rayon, top_left.y + hauteur - rayon, rayon, PI, PI_PLUS_QUART);
            points[2*rayon] = pt1;
            points[2*rayon + 1] = pt2;
            points[2*rayon + 2] = points[0];
            break;

        case  BAS:
            draw_arc(points,rayon/2, top_left.x + largeur - rayon, top_left.y  + rayon, rayon, PI_SUR_4, 0);
            draw_arc(points + (rayon/2),rayon, top_left.x + largeur - rayon, top_left.y + hauteur - rayon, rayon, 0, -PI_SUR_2);
            draw_arc(points + (3*rayon/2),rayon/2, top_left.x + rayon, top_left.y + hauteur - rayon, rayon, -PI_SUR_2, -3*PI/4);
            points[2*rayon] = pt1;
            points[2*rayon + 1] = pt2;
            points[2*rayon + 2] = points[0];
            break;
        default:
        draw_arc(points + 0,rayon, top_left.x + largeur - rayon, top_left.y  + rayon, rayon, 0, PI_SUR_2);
        draw_arc(points + rayon,rayon, top_left.x + rayon, top_left.y + rayon, rayon, PI_SUR_2, PI);
        draw_arc(points + 2 * rayon, rayon, top_left.x + rayon, top_left.y + hauteur - rayon, rayon, PI, 3*PI_SUR_2);
        draw_arc(points + 3 * rayon, rayon, top_left.x + largeur - rayon, top_left.y + hauteur - rayon, rayon, 3*PI_SUR_2, 2*PI);
        points[4*rayon] = points[0];

    }


}

void afficher_point_array(ei_point_t* points, uint32_t taille){
    for(uint32_t i = 0; i < taille; i++){
        printf("point %u est (%d,%d) \n", i+1, points[i].x, points[i].y);
    }
}

void draw_button(ei_surface_t surface, ei_rect_t* cadre, uint32_t rayon, uint32_t bordure, ei_color_t couleur, ei_color_t* couleur_haut_button, ei_color_t* couleur_bas_button, const ei_rect_t* clipper){

    // calcul fréquents : 
    uint32_t nb_pts_moitie = 2*rayon+3, nb_pts_total = 4*rayon + 1, rayon_div_4 = rayon/4;
    // allocation nécessaire 
    ei_point_t* points= malloc(sizeof(ei_point_t)*(2 * nb_pts_moitie));  // pour le button du milieu 


    // on trace la moitié haute du button et la moitié bas 
    rounded_frame(points, cadre, rayon, HAUT);
    rounded_frame(points + nb_pts_moitie, cadre, rayon, BAS);

    // affichage des moitier 
    ei_draw_polygon(surface, points, nb_pts_moitie, *couleur_haut_button, clipper);
    ei_draw_polygon(surface, points + nb_pts_moitie, nb_pts_moitie, *couleur_bas_button, clipper);


    // on trace la partie milieux qui contient le texte : 
    ei_rect_t nouveau_cadre = { {cadre->top_left.x + bordure, cadre->top_left.y + bordure}, 
                                {cadre->size.width - bordure * 2, cadre->size.height - bordure * 2}};
    rounded_frame(points, &nouveau_cadre, rayon, COMPLET);
    ei_draw_polygon(surface, points, nb_pts_total, couleur, clipper);


    free(points);
}

ei_color_t* change_color(ei_color_t* base, bool foncee){
    ei_color_t* couleur = (ei_color_t*)malloc(sizeof(ei_color_t));

    if(foncee){
        *couleur = (ei_color_t){
            base->red * FONCEE,
            base->green * FONCEE,
            base->blue * FONCEE,
            255
        };
    }
    else{
        *couleur = (ei_color_t){
            base->red * CLAIRE,
            base->green * CLAIRE,
            base->blue * CLAIRE,
            255
        };
    }
    return couleur;
}

/*******************************************************************************************************************************************/

void	ei_draw_text		(ei_surface_t		surface,
    const ei_point_t*	where,
    ei_const_string_t	text,
    ei_font_t		font,
    ei_color_t		color,
    const ei_rect_t*	clipper){

        // on determine la surface de texte :
        ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
        hw_surface_lock(surface);
        hw_surface_lock(surface_texte);
        // on copie la surface texte dans la surface principale :
        if((ei_copy_surface(surface, &(ei_rect_t){*where, hw_surface_get_size(surface_texte)}, surface_texte, NULL, false))){
            printf("taille de la source et destination incorrecte dans copy surface\n");
            exit(EXIT_FAILURE);
        }
        hw_surface_unlock(surface_texte);
        hw_surface_unlock(surface);
        return; 

    }

/*******************************************************************************************************************************************/

void	ei_draw_img(ei_surface_t surface, ei_surface_t img_surf, const ei_rect_t* rect_img, ei_point_t* where, ei_rect_t* clipper){
    // on recupere les infos de la surface
    ei_size_t dimension = hw_surface_get_size(surface);
    uint32_t* pixel_ptr = (uint32_t*) hw_surface_get_buffer(surface);

    //hw_surface_lock(img_surf);

    if (ei_copy_surface(surface,&(ei_rect_t){*where, rect_img->size},img_surf,rect_img, false)) {
        printf("taille de la source et destination incorrecte dans copy surface en img\n");
        exit(EXIT_FAILURE);
    }
   // hw_surface_unlock(img_surf);

}


int ei_copy_surface(ei_surface_t destination,
    const ei_rect_t* dst_rect,
    ei_surface_t source,
    const ei_rect_t* src_rect,
    bool alpha)
    {
        ei_size_t src_size = hw_surface_get_size(source);
        ei_size_t dst_size = hw_surface_get_size(destination);
    
        uint8_t* src_buf = hw_surface_get_buffer(source);
        uint8_t* dst_buf = hw_surface_get_buffer(destination);
    
        ei_rect_t src_r = src_rect ? *src_rect : hw_surface_get_rect(source);
        ei_rect_t dst_r = dst_rect ? *dst_rect : hw_surface_get_rect(destination);
    
        if (src_r.size.width != dst_r.size.width || src_r.size.height != dst_r.size.height)
            return 1;

        // on trouve l'intersection clippaire (je sais meme pas si le mot existe)
        ei_rect_t* inter = ei_rect_intersection(&dst_r, get_surf_app_rect()); 

        // si la zone memoire est complementement clippée: 
        if(!inter)
            return 0;

        // sinon on affiche : 
        int width = inter->size.width;
        int height = inter->size.height;



    
        int src_stride = src_size.width * 4;
        int dst_stride = dst_size.width * 4;

        for (int y = 0; y < height; ++y) {
            uint8_t* src_line = src_buf + (src_r.top_left.y + y) * src_stride + src_r.top_left.x * 4;
            uint8_t* dst_line = dst_buf + (dst_r.top_left.y + y) * dst_stride + dst_r.top_left.x * 4;
    
            for (int x = 0; x < width; ++x) {
                uint8_t* src_px = src_line + x * 4;
                uint8_t* dst_px = dst_line + x * 4;
    
                uint8_t pixel_alpha = src_px[3];  // canal alpha à l’indice 3
                if (pixel_alpha != 0) {
                    // Respect de l'ordre BGRA
                    dst_px[0] = (alpha)? src_px[0] : (dst_px[0]*(255 - pixel_alpha) + pixel_alpha * src_px[0] )/255 ; // Blue
                    dst_px[1] = (alpha)? src_px[1] : (dst_px[1]*(255 - pixel_alpha) + pixel_alpha * src_px[1] )/255 ; // Green
                    dst_px[2] = (alpha)? src_px[2] : (dst_px[2]*(255 - pixel_alpha) + pixel_alpha * src_px[2] )/255 ; // Red
                    dst_px[3] = 255;       // on force à opaque
                }
            }
        }
    
        return 0;
    }
