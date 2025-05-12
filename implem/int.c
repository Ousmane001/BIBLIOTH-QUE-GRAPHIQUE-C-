#include "ei_draw.h"
#include <stdio.h>
#include "ei_implementation.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_widget_attributes.h"
#include "ei_widget_configure.h"
#include "ei_application.h"

ei_widgetclass_t* create_frame_widgetclass(){
    ei_widgetclass_t* frame_widgetclass=malloc(sizeof(ei_widgetclass_t));
    strncpy(frame_widgetclass->name, "frame", sizeof(frame_widgetclass->name) - 1);
    frame_widgetclass->allocfunc=frame_alloc;
    frame_widgetclass->releasefunc=frame_release;
    frame_widgetclass->drawfunc=frame_draw;
    frame_widgetclass->setdefaultsfunc=frame_setdefaults;
    frame_widgetclass->geomnotifyfunc=frame_geonotify;
    frame_widgetclass->handlefunc=frame_handle;
    frame_widgetclass->next=NULL;
    return frame_widgetclass;
}

void ei_app_create(ei_size_t main_window_size,bool fullscreen){
    // Crée une fenêtre principale avec la taille spécifiée et le mode plein écran.
    hw_init();
    ei_widgetclass_register(create_frame_widgetclass());
    ei_widget_create("frame",NULL,NULL,NULL);
    ei_surface_t main_window = hw_create_window(main_window_size, fullscreen);
    ei_size_t offscreen_size = hw_surface_get_size(main_window);
    ei_surface_t off_screen=hw_surface_create(main_window, offscreen_size, false);
    
}

void ei_app_run(){
    getchar();
}


ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data,  ei_widget_destructor_t destructor){}

void ei_frame_configure(ei_widget_t widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width, ei_relief_t* relief, ei_string_t* text, ei_font_t* text_font,ei_color_t* text_color, ei_anchor_t* text_anchor, ei_surface_t* img, ei_rect_ptr_t* img_rect, ei_anchor_t* img_anchor){}

static inline void ei_place_xy(ei_widget_t widget, int x, int y){}

void ei_app_free(void){}

ei_widget_t ei_app_root_widget(){}

void frame_setdefaults(ei_widget_t widget){}

void frame_geonotify(ei_widget_t		widget){}

bool	frame_handle(ei_widget_t		widget,
    struct ei_event_t*	event){}

void			ei_widgetclass_register(ei_widgetclass_t* widgetclass){}

void		ei_place	(ei_widget_t		widget,
    ei_anchor_t*		anchor,
    int*			x,
    int*			y,
    int*			width,
    int*			height,
    float*			rel_x,
    float*			rel_y,
    float*			rel_width,
    float*			rel_height){}
/*******************************************************************************************************************************************/


ei_widget_t frame_alloc(){
    ei_widget_t frame = malloc(sizeof(ei_impl_frame_t));
    return frame;
}

void frame_release(ei_widget_t widget){
    free(widget);
}

void frame_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    ei_color_t couleur=widget->pick_color;
    ei_fill(surface,&couleur,clipper);
    ei_widget_t fils_cour = widget->children_head;
    ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;
    ei_string_t* texte= frame->text;
    if (texte){
        ei_const_string_t filename=(ei_const_string_t)widget->user_data;
        ei_fontstyle_t font_style;
        hw_text_font_create(filename,font_style,)
    }
    
    while (fils_cour!=NULL){
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, clipper);
        fils_cour=fils_cour->next_sibling;
    }

}