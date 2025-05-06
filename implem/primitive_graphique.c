#include "ei_draw.h"
#include <stdio.h>


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
    int x0 = origine.x;
    int y0 = origine.y;
    int x1 = extremite.x;
    int y1 = extremite.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (1) {

        // on verifie si le point est dans le clipper avant meme de l'afficher
        // attention à modifier si optimisation analytique 
        if(est_dans_clipper(&(ei_point_t){x0, y0}, clipper)){
            draw_point(pixel_ptr, dimension, (ei_point_t){x0, y0}, color);
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
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

