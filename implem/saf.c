#include "ei_draw.h"
#include "ei_implementation.h"

// ei_widgetclass_t* create_frame_button_widgetclass(){

//     ei_widgetclass_t* frame_button_widgetclass = malloc(sizeof(ei_widgetclass_t));
//     strcpy(frame_button_widgetclass->name, "frame_button");
//     frame_button_widgetclass->allocfunc = frame_button_alloc;
//     frame_button_widgetclass->releasefunc = frame_button_release;
//     frame_button_widgetclass->drawfunc = frame_button_draw;
//     frame_button_widgetclass->setdefaultsfunc = frame_button_setdefaults;
//     frame_button_widgetclass->geomnotifyfunc = frame_button_geonotify;
//     frame_button_widgetclass->handlefunc = button_handle;
//     frame_button_widgetclass->next=NULL;
//     return frame_button_widgetclass;
// }



void ei_button_configure(ei_widget_t		widget,
                        ei_size_t*		requested_size,
                        const ei_color_t*	color,
                        int*			border_width,
                        int*			corner_radius,
                        ei_relief_t*		relief,
                        ei_string_t*		text,
                        ei_font_t*		text_font,
                        ei_color_t*		text_color,
                        ei_anchor_t*		text_anchor,
                        ei_surface_t*		img,
                        ei_rect_ptr_t*		img_rect,
                        ei_anchor_t*		img_anchor,
                        ei_callback_t*		callback,
                        ei_user_param_t*	user_param);

{
    ei_impl_button_t* button = (ei_impl_button_t*) widget;



    if (requested_size != NULL) {
        if (button->requested_size == NULL)
            button->requested_size = malloc(sizeof(ei_size_t));
        widget->requested_size=*requested_size;
        *(button->requested_size) = *requested_size;
    }

    if (color != NULL) {
        if (button->color == NULL)
            button->color = malloc(sizeof(ei_color_t));
        *(button->color) = *color;
    }

    if (text != NULL && *text != NULL) {
        if (button->text != NULL)
            free(button->text);
        button->text = strdup(*text);  // on copie la chaîne
    }

    if (border_width != NULL) {
        if (button->border_width == NULL)
            button->border_width = malloc(sizeof(int));
        *(button->border_width) = *border_width;
    }

    if (corner_radius != NULL){
        if (corner_radius == NULL)
            button->corner_radius = malloc(sizeof(int));
        *(button->corner_radius) = *corner_radius;
    }

    if (relief != NULL) {
        if (button->relief == NULL)
            button->relief = malloc(sizeof(ei_relief_t));
        *(button->relief) = *relief;
    }

    if (text_font != NULL) {
        if (button->text_font == NULL)
            button->text_font = malloc(sizeof(ei_font_t));
        *(button->text_font) = *text_font;
    }

    if (text_color != NULL) {
        if (button->text_color == NULL)
            button->text_color = malloc(sizeof(ei_color_t));
        *(button->text_color) = *text_color;
    }

    if (text_anchor != NULL) {
        if (button->text_anchor == NULL)
            button->text_anchor = malloc(sizeof(ei_anchor_t));
        *(button->text_anchor) = *text_anchor;
    }

    if (img != NULL) {
        if (button->img == NULL)
            button->img = malloc(sizeof(ei_surface_t));
        *(button->img) = *img;
    }

    if (img_rect != NULL && *img_rect != NULL) {
        button->img_rect = malloc(sizeof(ei_rect_t));
        *(button->img_rect) = **img_rect;
    }

    if (img_anchor != NULL) {
        if (button->img_anchor == NULL)
            button->img_anchor = malloc(sizeof(ei_anchor_t));
        *(button->img_anchor) = *img_anchor;
    }

    if (callback != NULL) {
        if (button->callback == NULL)
            button->callback = malloc(sizeof(ei_callback_t));
        *(button->callback) = *callback;
    }

    if (user_param != NULL) {
        if (button->user_param == NULL)
            button->user_param = malloc(sizeof(ei_callback_t));
        *(button->user_param) = *callback;
    }

    // on initialise content reect à screen location
    widget->content_rect = &(widget->screen_location);
}


ei_widgetclass_t* create_button_widgetclass(){

    ei_widgetclass_t* button_widgetclass = malloc(sizeof(ei_widgetclass_t));
    strcpy(button_widgetclass->name, "button");
    button_widgetclass->allocfunc = button_alloc;
    button_widgetclass->releasefunc = button_release;
    button_widgetclass->drawfunc = button_draw;
    button_widgetclass->setdefaultsfunc = button_setdefaults;
    button_widgetclass->geomnotifyfunc = button_geonotify;
    button_widgetclass->handlefunc = button_handle;
    button_widgetclass->next=NULL;
    return button_widgetclass;
}

void buttonsetdefaults(ei_widget_t widget) {
    if (widget == NULL) 
        return;

    // Couleur de fond par défaut
    const ei_color_t color = ei_default_background_color;

    // Bordure
    int border_width = k_default_button_border_width;

    // Corner radius
    int corner_radius = k_default_button_corner_radius;

    // Relief
    ei_relief_t relief = ei_relief_raised;

    // Police
    ei_font_t text_font = ei_default_font;

    // Couleur du texte
    ei_color_t text_color = ei_font_default_color;

    // Ancrage du texte
    ei_anchor_t text_anchor = ei_anc_center;

    // Ancrage de l’image
    ei_anchor_t img_anchor = ei_anc_center;

    // Appelle la fonction de configuration standard pour tout initialiser
    ei_button_configure(&widget,
                        &requested_size,
                        &color,
                        &border_width,
                        &corner_radius,
                        &relief,
                        NULL,
                        &text_font,
                        &text_color,
                        &text_anchor,
                        NULL,
                        NULL,
                        &img_anchor,
                        NULL,
                        NULL)

    ei_widget_set_content_rect(widget, NULL);
}