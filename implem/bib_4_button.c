//
// Created by Ousmane Diakite on 17/05/2025.
//

#include "interracteur.h"


/*####################################################################################################################*/

void button_setdefaults(ei_widget_t widget) {
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
    ei_button_configure(widget,
                        NULL,
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
                        NULL);

    ei_widget_set_content_rect(widget, NULL);

}

/*####################################################################################################################*/

ei_widget_t button_alloc() {
    ei_impl_button_t* button = malloc(sizeof(ei_impl_button_t));
    if (button == NULL) {
        fprintf(stderr, "Erreur\n");
        return NULL;
    }

    // On nettoie cette benne à ordures : tous les champs à zéro.
    memset(button, 0, sizeof(ei_impl_button_t));

    // Retourne ça comme un ei_widget_t parce que visiblement, tout est déguisé ici.
    return (ei_widget_t)button;
}

/*####################################################################################################################*/

void button_release(ei_widget_t widget){
    ei_impl_button_t* button = (ei_impl_button_t* ) widget;

    // desallocation de ei_impl_widget_t
    free(button->widget.wclass);
    free(button->widget.user_data);
    free(button->widget.content_rect);

    // désallocation des autres structures contenues dans button:
    free(button->requested_size);
    //free(button->color);
    free(button->border_width);
    free(button->corner_radius);
    free(button->relief);
    free(button->text_font);
    free(button->text_color);
    free(button->text_anchor);
    free(button->img);
    free(button->img_anchor);
    //free(button->callback);
    free(button->user_param);

    // on desaloue finaleemnt la button:
    free(button);
}

/*####################################################################################################################*/

ei_widgetclass_t* create_button_widgetclass(){

    ei_widgetclass_t* button_widgetclass = malloc(sizeof(ei_widgetclass_t));
    strcpy(button_widgetclass->name, "button");
    button_widgetclass->allocfunc = button_alloc;
    button_widgetclass->releasefunc = button_release;
    button_widgetclass->drawfunc = button_draw;
    button_widgetclass->setdefaultsfunc = button_setdefaults;
    button_widgetclass->geomnotifyfunc = button_geonotify;
    button_widgetclass->handlefunc = button_handle_intern;
    button_widgetclass->next=NULL;
    return button_widgetclass;
}

/*####################################################################################################################*/

void button_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

    // on revient en type frame :
    ei_impl_button_t* button = (ei_impl_button_t*) widget;


    // gestion des couleurs
    ei_color_t* couleur = button->color;
    ei_rect_t* cadre = (ei_rect_t*) ei_widget_get_screen_location(widget);
    //ei_rect_t* cadre = ei_rect_intersection(clipper, (ei_rect_t*) ei_widget_get_screen_location(widget));

    if(!cadre)
        return;
    else
        //printf("%d %d %d %d  \n",cadre->top_left.x, cadre->top_left.y, cadre->size.width, cadre->size.height);

    // on dessine d'abord  dans l'offscreen de picking
    hw_surface_lock(pick_surface);
    draw_button(pick_surface, cadre, *(button->corner_radius), *(button->border_width), widget->pick_color, &widget->pick_color, &widget->pick_color, clipper);
    hw_surface_unlock(pick_surface);

    // on lock avant tous la surface
    hw_surface_lock(surface);

    // on dessine la frame en profondeur en fonction du relief
    switch(*(button->relief)){
        case ei_relief_raised:
            draw_button(surface,cadre,*(button->corner_radius), *(button->border_width), *couleur, change_color(couleur, true), change_color(couleur, false),clipper);
        break;

        case ei_relief_sunken:
        draw_button(surface,cadre,*(button->corner_radius), *(button->border_width), *couleur, change_color(couleur, false), change_color(couleur, true), clipper);

        break;

        default:
        draw_button(surface,cadre,*(button->corner_radius), *(button->border_width), *couleur, change_color(couleur, true), change_color(couleur, false),clipper);
    }

    // écriture du texte

    ei_const_string_t texte =  button->text;
    if (texte){
        int width_z = 0, height_z = 0;
        hw_text_compute_size(texte, *(button->text_font), &width_z, &height_z);
        ei_draw_text(surface, surface_localistion(*(button->widget.content_rect), width_z, height_z, button->text_anchor, *(button->border_width)), texte, *(button->text_font), reorder_color_channels(*(button->text_color), surface), clipper);
    }
    else if (button->img_rect){
        // sinon si c'est une image
        ei_draw_img(surface,*(button->img), button->img_rect,
        surface_localistion(*(button->widget.content_rect),button->img_rect->size.width, button->img_rect->size.height, button->img_anchor, *(button->border_width)), clipper );
    }

    // parcours en largeur des fils et dessins respectifs

    // on delock la surface :
    hw_surface_unlock(surface);


    // parcours en largeur des fils et dessins respectifs
    ei_widget_t fils_cour = widget->children_head;
    while (fils_cour!=NULL){
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, ei_rect_intersection(widget->content_rect, clipper));
        fils_cour=fils_cour->next_sibling;
    }

    //hw_surface_update_rects(surface, get_invalidate_rect_list());

}

/*####################################################################################################################*/

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
                        ei_user_param_t*	user_param){

    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    if (requested_size != NULL) {
        if (button->requested_size == NULL)
            button->requested_size = malloc(sizeof(ei_size_t));
        widget->requested_size = *requested_size;
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
        if (button->corner_radius == NULL)
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
        *(button->img) = hw_surface_create(ei_app_root_surface(),hw_surface_get_size(*img), false);
        ei_copy_surface(*(button->img), NULL, *img, NULL, false);
    }

    if (img_rect != NULL && *img_rect != NULL) {
        button->img_rect = malloc(sizeof(ei_rect_t));
        // *(button->img_rect) = **img_rect;
        button->img_rect->top_left.x = (*img_rect)->top_left.x;
        button->img_rect->top_left.y = (*img_rect)->top_left.y;
        button->img_rect->size.width = (*img_rect)->size.width;
        button->img_rect->size.height = (*img_rect)->size.height;
    }

    if (img_anchor != NULL) {
        if (button->img_anchor == NULL)
            button->img_anchor = malloc(sizeof(ei_anchor_t));
        *(button->img_anchor) = *img_anchor;
    }

    if (callback != NULL) {
        if (button->callback == NULL)
            button->callback = malloc(sizeof(ei_callback_t));
        (button->callback) = *callback;
    }

    if (user_param != NULL) {
        if (button->user_param == NULL)
            button->user_param = malloc(sizeof(ei_callback_t));
        *(button->user_param) = *user_param;
    }

    // // on initialise content reect à screen location
     widget->content_rect = &(widget->screen_location);
}
/*####################################################################################################################*/

void button_geonotify(ei_widget_t widget){

    // on gere ce texte qui nous fatigue tant : 
    ei_impl_button_t* button = (ei_impl_button_t*)widget;
    ei_size_t dimension = {0,0};
    if(button->text != NULL){
        // on verifie qu'on se fait pas avoir avec un taille beaucoup plus petite que necessaire : 
        if(button->text_font != NULL){
            hw_text_compute_size((button->text), *(button->text_font), &(dimension.width), &(dimension.height));
        }
    }

    // ah cette image à laquelle on a faillit oublie et qui nous a bien enervee!!!!!!!!!!!!
    if(button->img_rect){
        //dimension = hw_surface_get_size(*(button->img));
        dimension = button->img_rect->size;

    }

    // les fameux tests : 
    if(widget->screen_location.size.width < dimension.width){
        widget->screen_location.size.width = dimension.width;
        widget->screen_location.top_left.x -= widget->screen_location.size.width;
    }        

    if(widget->screen_location.size.height < dimension.height){
        widget->screen_location.size.height = dimension.height;
        widget->screen_location.top_left.y -= widget->screen_location.size.height;
    }
    widget->wclass->drawfunc(widget, ei_app_root_surface(), get_offscreen_picking(), ei_rect_intersection(widget->parent->content_rect, get_surf_app_rect()));
    
}

/*####################################################################################################################*/

bool button_handle_intern(ei_widget_t widget, struct ei_event_t* event) {
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    ei_relief_t saved_relief = *(button->relief);

    bool was_inside = est_dans_rect(event->param.mouse.where, widget->screen_location);

    if (*(button->relief) != ei_relief_none && was_inside) {
        ei_button_set_relief(widget, inverse_relief(saved_relief));
        ei_app_invalidate_rect(&(widget->screen_location));
        draw_invalidate_rect();
    }

    // Déclarer le bouton comme interacteur actif
    ei_event_set_active_widget(widget);

    // Boucle jusqu'à relâchement du bouton ou fermeture
    while (event->type != ei_ev_mouse_buttonup && event->type != ei_ev_close) {
        hw_event_wait_next(event);

        // Vérifie que le widget est toujours actif
        if (ei_event_get_active_widget() != widget)
            continue;

        bool is_inside = est_dans_rect(event->param.mouse.where, widget->screen_location);

        if (*(button->relief) != ei_relief_none) {
            if (is_inside != was_inside) {
                ei_button_set_relief(widget, inverse_relief(*(button->relief)));
                ei_app_invalidate_rect(&(widget->screen_location));
                draw_invalidate_rect();
                was_inside = is_inside;
            }
        }
    }

    // Fin d'interaction
    ei_event_set_active_widget(NULL);

    // Callback externe uniquement si relâchement dans le bouton
    if (event->type == ei_ev_mouse_buttonup &&
        est_dans_rect(event->param.mouse.where, widget->screen_location)) {
        if (button->callback != NULL)
            button->callback(widget, event, button->user_param);
    }

    // Restauration du relief initial
    ei_button_set_relief(widget, saved_relief);
    ei_app_invalidate_rect(&(widget->screen_location));
    draw_invalidate_rect();

    return true;
}

/*####################################################################################################################*/
