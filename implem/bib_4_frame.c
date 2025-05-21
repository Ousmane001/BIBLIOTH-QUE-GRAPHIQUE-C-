//
// Created by Ousmane Diakite on 17/05/2025.
//

#include "bib_4_frame.h"


/*####################################################################################################################*/


ei_widgetclass_t* create_frame_widgetclass(){

    ei_widgetclass_t* frame_widgetclass = malloc(sizeof(ei_widgetclass_t));
    strcpy(frame_widgetclass->name, "frame");
    frame_widgetclass->allocfunc = frame_alloc;
    frame_widgetclass->releasefunc = frame_release;
    frame_widgetclass->drawfunc = frame_draw;
    frame_widgetclass->setdefaultsfunc = frame_setdefaults;
    frame_widgetclass->geomnotifyfunc = frame_geonotify;
    frame_widgetclass->handlefunc = frame_handle;
    frame_widgetclass->next=NULL;
    return frame_widgetclass;
}

/*####################################################################################################################*/

void ei_frame_configure( ei_widget_t widget, ei_size_t* requested_size, const ei_color_t* color, int* border_width,
    ei_relief_t* relief, ei_string_t* text, ei_font_t* text_font, ei_color_t* text_color, ei_anchor_t* text_anchor,
    ei_surface_t* img, ei_rect_ptr_t* img_rect, ei_anchor_t* img_anchor)
{
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;



    if (requested_size != NULL) {
        if (frame->requested_size == NULL)
            frame->requested_size = malloc(sizeof(ei_size_t));
        widget->requested_size=*requested_size;
        *(frame->requested_size) = *requested_size;
    }

    if (color != NULL) {
        if (frame->color == NULL)
            frame->color = malloc(sizeof(ei_color_t));
        *(frame->color) = *color;
    }

    if (text != NULL && *text != NULL) {
        if (frame->text != NULL)
            free(frame->text);
        frame->text = strdup(*text);  // on copie la chaÃ®ne
    }

    if (border_width != NULL) {
        if (frame->border_width == NULL)
            frame->border_width = malloc(sizeof(int));
        *(frame->border_width) = *border_width;
    }

    if (relief != NULL) {
        if (frame->relief == NULL)
            frame->relief = malloc(sizeof(ei_relief_t));
        *(frame->relief) = *relief;
    }

    if (text_font != NULL) {
        if (frame->text_font == NULL)
            frame->text_font = malloc(sizeof(ei_font_t));
        *(frame->text_font) = *text_font;
    }

    if (text_color != NULL) {
        if (frame->text_color == NULL)
            frame->text_color = malloc(sizeof(ei_color_t));
        *(frame->text_color) = *text_color;
    }

    if (text_anchor != NULL) {
        if (frame->text_anchor == NULL)
            frame->text_anchor = malloc(sizeof(ei_anchor_t));
        *(frame->text_anchor) = *text_anchor;
    }

    if (img != NULL) {
        if (frame->img == NULL)
            frame->img = malloc(sizeof(ei_surface_t));
        *(frame->img) = *img;
    }

    if (img_rect != NULL && *img_rect != NULL) {
        frame->img_rect = malloc(sizeof(ei_rect_t));
        *(frame->img_rect) = **img_rect;
    }

    if (img_anchor != NULL) {
        if (frame->img_anchor == NULL)
            frame->img_anchor = malloc(sizeof(ei_anchor_t));
        *(frame->img_anchor) = *img_anchor;
    }

    // on initialise content reect Ã  screen location
    widget->content_rect = (widget->content_rect == NULL)? &(widget->screen_location) : widget->content_rect;
}

/*####################################################################################################################*/

void frame_setdefaults(ei_widget_t widget) {
    if (widget == NULL)
        return;

    // Couleur de fond par dÃ©faut
    const ei_color_t color = ei_default_background_color;

    // Bordure
    int border_width = 0;

    // Relief
    ei_relief_t relief = ei_relief_none;

    // Texte
    ei_string_t text = NULL;

    // Police
    ei_font_t text_font = ei_default_font;

    // Couleur du texte
    ei_color_t text_color = ei_font_default_color;

    // Ancrage du texte
    ei_anchor_t text_anchor = ei_anc_center;

    // Image
    ei_surface_t img = NULL;

    // Rectangle de lâ€™image
    ei_rect_ptr_t img_rect = NULL;

    // Ancrage de lâ€™image
    ei_anchor_t img_anchor = ei_anc_center;

    // Appelle la fonction de configuration standard pour tout initialiser
    ei_frame_configure(
        widget,
        NULL,                // on attent le config pour mettre le requested size car on est pas des devin !!!
        &color,
        &border_width,
        &relief,
        &text,
        &text_font,
        &text_color,
        &text_anchor,
        &img,
        &img_rect,
        &img_anchor
    );
    ei_widget_set_content_rect(widget, NULL);
}

/*####################################################################################################################*/

void frame_geonotify(ei_widget_t		widget){}
/*####################################################################################################################*/

bool	frame_handle(ei_widget_t widget, struct ei_event_t*	event){
        return true;
    }
/*####################################################################################################################*/


ei_widget_t frame_alloc() {
    ei_impl_frame_t* frame = malloc(sizeof(ei_impl_frame_t));
    if (frame == NULL) {
        fprintf(stderr, "Erreur\n");
        return NULL;
    }

    // On nettoie cette benne Ã  ordures : tous les champs Ã  zÃ©ro.
    memset(frame, 0, sizeof(ei_impl_frame_t));

    // Retourne Ã§a comme un ei_widget_t parce que visiblement, tout est dÃ©guisÃ© ici.
    return (ei_widget_t)frame;
}

/*####################################################################################################################*/

void frame_release(ei_widget_t widget){
    ei_impl_frame_t* frame = (ei_impl_frame_t*) widget;
    if (frame == NULL) return;

    //Ne pas free wclass car c'est commun Ã  tous les widgets de la mÃªme classe

    if (frame->widget.user_data != NULL) {
        free(frame->widget.user_data);
        frame->widget.user_data = NULL;
    }

    // if (frame->widget.content_rect != NULL) {
    //     free(frame->widget.content_rect);
    //     frame->widget.content_rect = NULL;
    // }

    if (frame->requested_size != NULL) {
        free(frame->requested_size);
        frame->requested_size = NULL;
    }

    if (frame->color != NULL) {
        free(frame->color);
        frame->color = NULL;
    }

    if (frame->border_width != NULL) {
        free(frame->border_width);
        frame->border_width = NULL;
    }

    if (frame->relief != NULL) {
        free(frame->relief);
        frame->relief = NULL;
    }

    if (frame->text_font != NULL) {
        free(frame->text_font); 
        frame->text_font = NULL;
    }

    if (frame->text_color != NULL) {
        free(frame->text_color);
        frame->text_color = NULL;
    }

    if (frame->text_anchor != NULL) {
        free(frame->text_anchor);
        frame->text_anchor = NULL;
    }

    if (frame->img != NULL) {
        free(frame->img); 
        frame->img = NULL;
    }

    if (frame->img_anchor != NULL) {
        free(frame->img_anchor);
        frame->img_anchor = NULL;
    }

}


/*####################################################################################################################*/
void frame_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){
    // on revient en type frame :
    ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;
    
    // gestion des couleurs
    ei_color_t couleur = reorder_color_channels(*(frame->color),ei_app_root_surface());

    ei_color_t *claire = change_color(&couleur, false), *foncee = change_color(&couleur, true);

    // creation des points poour les figures a dessineer
    ei_rect_t zone = widget->screen_location;
    int bordure = *(frame->border_width);


    ei_rect_t carre = {{zone.top_left.x, zone.top_left.y}, {zone.size.width, zone.size.height}};

    ei_point_t triangle[4] = {
                                {zone.top_left.x, zone.top_left.y},
                                {zone.top_left.x + zone.size.width, zone.top_left.y},
                                {zone.top_left.x , zone.top_left.y + zone.size.height},
                                {zone.top_left.x, zone.top_left.y}
    };

    ei_rect_t centre = {{zone.top_left.x + bordure, zone.top_left.y + bordure}, {zone.size.width - bordure * 2, zone.size.height - bordure * 2}};

    // on dessine d'abord  dans l'offscreen de picking
    hw_surface_lock(get_offscreen_picking());
    ei_fill_optim(get_offscreen_picking(), &(widget->pick_color), &carre, clipper);
    hw_surface_unlock(get_offscreen_picking());

    // on lock avant tous la surface
    hw_surface_lock(surface);

    // on dessine la frame en profondeur en fonction du relief
    switch(*(frame->relief)){
        case ei_relief_raised:
            ei_fill_optim(surface, foncee, &carre, clipper);
            ei_draw_polygon(surface, triangle, 4, *claire, clipper);
            ei_fill_optim(surface, &couleur, &centre, clipper);


        break;

        case ei_relief_sunken:
            ei_fill_optim(surface, claire, &carre, clipper);
            ei_draw_polygon(surface, triangle, 4, *foncee, clipper);
            ei_fill_optim(surface, &couleur, &centre, clipper);

        break;

        default:
            ei_fill_optim(surface, &couleur, &carre, clipper);
    }



// affichage du texte / image sur la frame:
    if (frame->text){
        int width_z = 0, height_z = 0;
        hw_text_compute_size(frame->text, *(frame->text_font), &width_z, &height_z);
        ei_draw_text(surface, surface_localistion(*(frame->widget.content_rect), width_z, height_z, frame->text_anchor, bordure), frame->text, *(frame->text_font), reorder_color_channels(*(frame->text_color), surface), clipper);
    }
    else if (frame->img_rect){
        // sinon si c'est une image
        ei_draw_img(surface,*(frame->img), frame->img_rect,
        surface_localistion(*(frame->widget.content_rect),frame->img_rect->size.width, frame->img_rect->size.height, frame->img_anchor, *(frame->border_width)), clipper);
    }

    // on delock la surface :
    hw_surface_unlock(surface);

    // on parcours et affiche les fils de cette frame
    ei_widget_t fils_cour = widget->children_head;
    while (fils_cour!=NULL){
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, ei_rect_intersection(widget->content_rect, clipper));
        fils_cour=fils_cour->next_sibling;
    }
    //hw_surface_update_rects(surface, get_invalidate_rect_list());

}

/*####################################################################################################################*/

