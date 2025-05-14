// #include "ei_draw.h"
// #include <stdio.h>
// #include "ei_implementation.h"
// #include "ei_widget.h"
// #include "ei_widgetclass.h"
// #include "ei_widget_attributes.h"
// #include "ei_widget_configure.h"
// #include "ei_application.h"


/*
je vais essayer de travailler sur le ei_placeur
*/ 
typedef struct ei_impl_placer_params_t {
    ei_anchor_t   anchor;
    int           x;
    int           y;
    int           width;
    int           height;
    float         rel_x;
    float         rel_y;
    float         rel_width;
    float         rel_height;
} ei_impl_placer_params_t;

void ei_place (ei_widget_t widget,
ei_anchor_t* anchor,
int* x,
int* y,
int* width,
int* height,
float* rel_x,
float* rel_y,
float* rel_width,
float* rel_height){
    // Allouer la structure si nécessaire et on véirifie si un des paramètres est NULL
    // Dans ce cas on lui donne sa valeur par défaut (voir déf dans ei_placer.h)
    if (widget->placer_param == NULL) {
        widget->placer_param = calloc(1, sizeof(ei_impl_placer_params_t));
        ei_impl_placer_params_t* params = (ei_impl_placer_params_t*)widget->placer_param;

        // Valeurs par défaut
        params->anchor = ei_anc_northwest;
        params->x = 0;
        params->y = 0;
        params->width = 0;
        params->height = 0;
        params->rel_x = 0.0f;
        params->rel_y = 0.0f;
        params->rel_width = 0.0f;
        params->rel_height = 0.0f;
    }

    ei_impl_placer_params_t* p = (ei_impl_placer_params_t*)widget->placer_param;

    // Mise à jour des paramètres si fournis
    if (anchor)       p->anchor = *anchor;
    if (x)            p->x = *x;
    if (y)            p->y = *y;
    if (width)        p->width = *width;
    if (height)       p->height = *height;
    if (rel_x)        p->rel_x = *rel_x;
    if (rel_y)        p->rel_y = *rel_y;
    if (rel_width)    p->rel_width = *rel_width;
    if (rel_height)   p->rel_height = *rel_height;

    // Recalcul de la géométrie
    ei_impl_placer_run(widget);
}



void ei_impl_placer_run(ei_widget_t widget) {
    if (widget == NULL || widget->placer_param == NULL || widget->parent == NULL)
        return;

    ei_impl_placer_params_t* p = (ei_impl_placer_params_t*)widget->placer_param;
    ei_rect_t parent_rect = widget->parent->screen_location;

    // Calcul des coordonnées absolues (position + position relative)
    int abs_x = parent_rect.top_left.x + (int)(p->rel_x * parent_rect.size.width) + p->x;
    int abs_y = parent_rect.top_left.y + (int)(p->rel_y * parent_rect.size.height) + p->y;

    // Détermination de la taille finale
    int width  = (p->width > 0)  ? p->width  : widget->requested_size.width;
    int height = (p->height > 0) ? p->height : widget->requested_size.height;

    // Ajustement de la position selon l'ancrage
    switch (p->anchor) {
        case ei_anc_center:
            abs_x -= width / 2;
            abs_y -= height / 2;
            break;
        case ei_anc_north:
            abs_x -= width / 2;
            break;
        case ei_anc_northeast:
            abs_x -= width;
            break;
        case ei_anc_east:
            abs_x -= width;
            abs_y -= height / 2;
            break;
        case ei_anc_southeast:
            abs_x -= width;
            abs_y -= height;
            break;
        case ei_anc_south:
            abs_x -= width / 2;
            abs_y -= height;
            break;
        case ei_anc_southwest:
            abs_y -= height;
            break;
        case ei_anc_west:
            abs_y -= height / 2;
            break;
        case ei_anc_northwest:
            // Aucun ajustement
            break;
        case ei_anc_none:
        default:
            // Comportement par défaut identique à northwest
            break;
    }

    // Mise à jour de la géométrie réelle du widget
    widget->screen_location = (ei_rect_t){{abs_x, abs_y}, {width, height}};
    // Il faut fiare ceci aux enfants aussi sinon le placeur va générer des erreurs visuelles
    ei_widget_t enfant = widget->children_head;
    while (enfant != NULL) {
        ei_impl_placer_run(enfant);  // Appel récursif sur chaque enfant
        enfant = enfant->next_sibling;
    }
}
// je pense c'est possible d'encore mieux optimiser ce code
// A revoir, il faudrait peut être modifier content rect (d'après un ami) 
// widget->content_rect = &widget->screen_location;
