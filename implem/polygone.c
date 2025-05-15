#include "ei_draw.h"
#include <stdio.h>
#include "ei_implementation.h"

typedef struct ei_impl_button_t {
  ei_impl_widget_t widget;
    /*spécificités*/
  ei_size_t*		requested_size;
  const ei_color_t*	color;
  int*			border_width;
  int*			corner_radius;
  ei_relief_t*		relief;
  ei_string_t*		text;
  ei_font_t*		text_font;
  ei_color_t*		text_color;
  ei_anchor_t*		text_anchor;
  ei_surface_t*		img;
  ei_rect_ptr_t*		img_rect;
  ei_anchor_t*		img_anchor;
  ei_callback_t*		callback;
  ei_user_param_t*	user_param;
}ei_impl_button_t;