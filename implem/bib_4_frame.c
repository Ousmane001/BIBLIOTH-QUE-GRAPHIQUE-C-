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
        frame->text = strdup(*text);  // on copie la chaîne
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

    // on initialise content reect à screen location
    widget->content_rect = &(widget->screen_location);
}

/*####################################################################################################################*/

void frame_setdefaults(ei_widget_t widget) {
    if (widget == NULL)
        return;

    // Couleur de fond par défaut
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

    // Rectangle de l’image
    ei_rect_ptr_t img_rect = NULL;

    // Ancrage de l’image
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

bool	frame_handle(ei_widget_t		widget,
    struct ei_event_t*	event){
        return true;
    }
/*####################################################################################################################*/


ei_widget_t frame_alloc() {
    ei_impl_frame_t* frame = malloc(sizeof(ei_impl_frame_t));
    if (frame == NULL) {
        fprintf(stderr, "Erreur\n");
        return NULL;
    }

    // On nettoie cette benne à ordures : tous les champs à zéro.
    memset(frame, 0, sizeof(ei_impl_frame_t));

    // Retourne ça comme un ei_widget_t parce que visiblement, tout est déguisé ici.
    return (ei_widget_t)frame;
}

/*####################################################################################################################*/

//cette fonction prend en parametre un widget et pas un ei_impl_frame_t car elle doit etre "commune" à toutes les classes cou
void frame_release(ei_widget_t widget){
    ei_impl_frame_t* frame = (ei_impl_frame_t* ) widget;

    // desallocation de ei_impl_widget_t
    free(frame->widget.wclass);
    free(frame->widget.user_data);
    free(frame->widget.content_rect);

    // désallocation des autres structures contenues dans frame:
    free(frame->requested_size);
    //free(frame->color);
    free(frame->border_width);
    free(frame->relief);
    free(frame->text_font);
    free(frame->text_color);
    free(frame->text_anchor);
    free(frame->img);
    free(frame->img_anchor);

    // on desaloue finaleemnt la frame:
    free(frame);
}

/*####################################################################################################################*/
void frame_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){

    int	i, ir, ig, ib, ia;

    // on revient en type frame :
    ei_impl_frame_t* frame= (ei_impl_frame_t*) widget;


    // gestion des couleurs
    ei_color_t couleur = reorder_color_channels(*(frame->color),ei_app_root_surface());
    hw_surface_get_channel_indices(ei_app_root_surface(), &ir, &ig, &ib, &ia);

    // on gernere une couleur unique pour cette frame pour la pick surface :
    ei_color_t couleur_pick = genere_couleur_suivante();
    ajouter(get_dicco_app(), *(uint32_t *)&couleur_pick, widget);
    widget->pick_color = couleur_pick;
    widget->pick_id = *(uint32_t *)&couleur_pick;

    ei_color_t *claire = change_color(&couleur, false), *foncee = change_color(&couleur, true);

    // creation des points poour les figures a dessineer
    ei_rect_t zone = widget->screen_location;
    int bordure = *(frame->border_width);
    ei_point_t carre[5] = {
                            {zone.top_left.x, zone.top_left.y},
                            {zone.top_left.x + zone.size.width, zone.top_left.y},
                            {zone.top_left.x + zone.size.width, zone.top_left.y + zone.size.height},
                            {zone.top_left.x , zone.top_left.y + zone.size.height},
                            {zone.top_left.x, zone.top_left.y}
    };
    ei_point_t triangle[4] = {
                                {zone.top_left.x, zone.top_left.y},
                                {zone.top_left.x + zone.size.width, zone.top_left.y},
                                {zone.top_left.x , zone.top_left.y + zone.size.height},
                                {zone.top_left.x, zone.top_left.y}
    };
    ei_point_t centre[5] = {
        {zone.top_left.x + bordure, zone.top_left.y + bordure},
        {zone.top_left.x + zone.size.width - bordure, zone.top_left.y + bordure},
        {zone.top_left.x + zone.size.width - bordure, zone.top_left.y + zone.size.height - bordure},
        {zone.top_left.x + bordure, zone.top_left.y + zone.size.height - bordure},
        {zone.top_left.x + bordure, zone.top_left.y + bordure}
    };

    // on dessine d'abord  dans l'offscreen de picking
    hw_surface_lock(get_offscreen_picking());
    ei_draw_polygon(get_offscreen_picking(), carre, 5, couleur_pick, clipper);
    hw_surface_unlock(get_offscreen_picking());

    // on lock avant tous la surface
    hw_surface_lock(surface);

    // on dessine la frame en profondeur en fonction du relief
    switch(*(frame->relief)){
        case ei_relief_raised:
            ei_draw_polygon(surface, carre, 5, *foncee, clipper);
            ei_draw_polygon(surface, triangle, 4, *claire, clipper);
            ei_draw_polygon(surface, centre, 5, couleur, clipper);

        break;

        case ei_relief_sunken:
            ei_draw_polygon(surface, carre, 5, *claire, clipper);
            ei_draw_polygon(surface, triangle, 4, *foncee, clipper);
            ei_draw_polygon(surface, centre, 5, couleur, clipper);

        break;

        default:
            ei_draw_polygon(surface, carre, 5, couleur, clipper);
    }





    ei_const_string_t texte =  frame->text;
    if (texte){
        int width_z = 0, height_z = 0;
        hw_text_compute_size(texte, *(frame->text_font), &width_z, &height_z);
        ei_draw_text(surface, surface_localistion(*(frame->widget.content_rect), width_z, height_z, frame->text_anchor, bordure), texte, *(frame->text_font), reorder_color_channels(*(frame->text_color), surface), widget->content_rect);
    }
    else{
        // ei_const_string_t* filename=(ei_const_string_t*)widget->user_data;
        // ei_surface_t image=hw_image_load(*filename, surface);
        // ei_rect_ptr_t* rect_img=frame->img_rect;
        // ei_anchor_t* img_anchor=frame->img_anchor;
    }
    ei_widget_t fils_cour = widget->children_head;

    // parcours en largeur des fils et dessins respectifs
    // while (fils_cour!=NULL){
    //     ei_impl_widget_draw_children(fils_cour, surface, pick_surface, clipper);
    //     fils_cour=fils_cour->next_sibling;
    // }

    // on delock la surface :
    hw_surface_unlock(surface);

    while (fils_cour!=NULL){
        printf("j'affiche un fils dans frame\n");
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, widget->content_rect);
        fils_cour=fils_cour->next_sibling;
    }
    hw_surface_update_rects(surface, get_invalidate_rect_list());

}

/*####################################################################################################################*/

