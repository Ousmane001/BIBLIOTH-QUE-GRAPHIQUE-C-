//
// Created by Ousmane Diakite on 17/05/2025.
//

#ifndef BIB_4_BUTTON_H
#define BIB_4_BUTTON_H

#include "interracteur.h"


ei_widgetclass_t* create_button_widgetclass();

void button_setdefaults(ei_widget_t widget);

ei_widget_t button_alloc();

void button_release(ei_widget_t widget);

void button_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper);

void ei_button_configure(ei_widget_t		widget,ei_size_t*	requested_size, const ei_color_t* color, int* border_width, int* corner_radius, ei_relief_t* relief, ei_string_t* text,ei_font_t*		text_font, ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img,ei_rect_ptr_t* img_rect, ei_anchor_t* img_anchor,ei_callback_t* callback, ei_user_param_t* user_param);

void	button_geonotify(ei_widget_t		widget);

bool	button_handle_intern(ei_widget_t		widget,
    struct ei_event_t*	event);

bool button_handle_intern(ei_widget_t widget, struct ei_event_t* event);


#endif //BIB_4_BUTTON_H
