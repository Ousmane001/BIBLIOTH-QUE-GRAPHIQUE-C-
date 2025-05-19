//
// Created by Ousmane Diakite on 17/05/2025.
//

#ifndef BIB_4_TOPLEVEL_H
#define BIB_4_TOPLEVEL_H

#include "interracteur.h"


#define TAILLE_BUTTON_RESIZE 15
#define RAYON_TOP_LEVEL 10
#define TAILLE_CADRE_HAUT 40
#define TAILLE_ENTETE_TOP_LEVEL 28

ei_widgetclass_t* create_toplevel_widgetclass();
void toplevel_setdefaults(ei_widget_t widget);
ei_widget_t toplevel_alloc();
void toplevel_release(ei_widget_t widget);
void toplevel_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);
void ei_toplevel_configure(ei_widget_t		widget,
                          ei_size_t*		requested_size,
                          const ei_color_t*	color,
                          int*			border_width,
                          ei_string_t*		title,
                          bool*			closable,
                          ei_axis_set_t*		resizable,
                          ei_size_ptr_t*		min_size);
void toplevel_geonotify(ei_widget_t		widget);
bool toplevel_handle_intern(ei_widget_t widget, struct ei_event_t* event);


#endif //BIB_4_TOPLEVEL_H