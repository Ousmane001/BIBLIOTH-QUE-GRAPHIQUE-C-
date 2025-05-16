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

ei_widget_t button_alloc() {
    ei_impl_button_t* button = calloc(1, sizeof(ei_impl_button_t));
    if (button == NULL) {
        fprintf(stdout, "Erreur dans creation d'un boutton \n");
        return NULL;
    }

    // On nettoie cette benne à ordures : tous les champs à zéro.
    // memset(button, 0, sizeof(ei_impl_button_t));

    // Retourne ça comme un ei_widget_t parce que visiblement, tout est déguisé ici.
    return (ei_widget_t)button;
}


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
    free(button->corner_radius)
    free(button->relief);
    free(button->text_font);
    free(button->text_color);
    free(button->text_anchor);
    free(button->img);
    free(button->img_anchor);
    free(button->callback);
    free(button->user_param);
    
    // on desaloue finaleemnt la button:
    free(button);
}

void button_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper){


    // on revient en type frame :
    ei_impl_button_t* button = (ei_impl_button_t*) widget;


    // gestion des couleurs 
    ei_color_t couleur = button->color;
    ei_rect_t* cadre = ei_get_screen_location(widget);


    
    // on lock avant tous la surface 
    hw_surface_lock(surface);

    // on dessine la frame en profondeur en fonction du relief
    switch(*(button->relief)){
        case ei_relief_raised:
            ei_draw_button(surface,cadre,button->corner_radius,couleur,clipper,false);
        break;

        case ei_relief_sunken:
            ei_draw_button(surface,cadre,button->corner_radius,couleur,clipper,true);

        break;

        default:
            ei_draw_button(surface,cadre,button->corner_radius,couleur,clipper,false);
    }
    
    // écriture du texte
    
    ei_const_string_t texte =  button->text;
    if (texte){
        int width_z = 0, height_z = 0;
        hw_text_compute_size(texte, *(button->text_font), &width_z, &height_z);
        ei_draw_text(surface, surface_localistion(*(button->widget.content_rect), width_z, height_z, button->text_anchor, bordure), texte, *(button->text_font), reorder_color_channels(*(button->text_color), surface), widget->content_rect);
    }
    
    // affichage des images 

    else{
        // ei_const_string_t* filename=(ei_const_string_t*)widget->user_data;
        // ei_surface_t image=hw_image_load(*filename, surface);
        // ei_rect_ptr_t* rect_img=button->img_rect;
        // ei_anchor_t* img_anchor=button->img_anchor;
    }
    ei_widget_t fils_cour = widget->children_head;

    // parcours en largeur des fils et dessins respectifs 

    // on delock la surface : 
    hw_surface_unlock(surface);

    
    // parcours en largeur des fils et dessins respectifs 
    ei_widget_t fils_cour = widget->children_head;
    while (fils_cour!=NULL){
        ei_impl_widget_draw_children(fils_cour, surface, pick_surface, widget->content_rect);
        fils_cour=fils_cour->next_sibling;
    }
    
    hw_surface_update_rects(surface,NULL);

}

ei_relief_t inverse_relief(ei_relief_t relief){
    if (relief==ei_relief_raised){
        relief = ei_relief_sunken;
    }
    
    else{
        relief = ei_relief_raised;
    }
    return relief;
}



bool button_handle_intern(ei_widget_t widget, struct ei_event_t* event){
    
    ei_impl_button_t* button = (ei_impl_button_t*) widget;
    bool etait_dessus = false;
    if (relief!=ei_relief_none){
        ei_relief_t inv_relief = inverse_relief(button->relief);
        
        // tanque le bouton est maintenue en click bas : 
        while (event->type == ei_ev_mouse_buttondown  && event->type != ei_ev_close)
        {
            if(est_dans_rect(event->mouse.where,widget->screen_location)){
                button->relief = inv_relief;
                if(etait_dessus==false){
                    ei_app_invalidate_rect(widget->screen_location);
                    etait_dessus = true;
                } 
            }
            else{
                button->relief = inverse_relief(inv_relief);
                if(etait_dessus==true){
                    ei_app_invalidate_rect(widget->screen_location);                }
                etait_dessus = false;
            }
            hw_event_wait_next(&event);
        }
        
    }
    ei_impl_button_t* button = (ei_impl_button_t*)widget;
    button->callback(widget, event ,button->user_param);
}

bool est_dans_rect(ei_point_t point, ei_rect_t rect){
    if(x>=rect.top_left.x && x<= rect.top_left.x + rect.size.width && y>= rect.top_left.y && y<= rect.top_left.y + rect.size.height){
        return true;
    }
    return false;
}

// typedef struct ei_linked_rect_t{
//     ei_rect_t rect;
//     struct ei_linked_rect_t* next;
// }ei_linked_rect_t;

static ei_linked_rect_t* ei_linked_rect_t_list = NULL;

void ei_app_invalidate_rect(const ei_rect_t* rect){
    ei_linked_rect_t* a_traiter = malloc(sizeof(ei_linked_rect_t));
    if (a_traiter==NULL){
        fprintf(stderr,"memoire finito, va t'acheter de la ram\n");
        exit(EXIT_FAILURE);
    }
    a_traiter->rect = *rect;
    a_traiter->next = ei_linked_rect_t_list;
    ei_linked_rect_t_list = a_traiter;
}

ei_linked_rect_t* get_invalidate_rect_list(void){
    return ei_linked_rect_t_list;
}

ei_surface_t ei_app_root_surface(void){
    return root_surface;
}

void draw_invalidate_rect(void){
    // on lock la surface avant de tracer
    hw_surface_lock(ei_app_root_surface());
    hw_surface_lock(get_offscreen_picking());
     
    //on récupere le 1er rectangle a redessiner
    ei_linked_rect_t* cour = get_invalidate_rect_list();
    ei_widget_t widget;
    ei_widgetclass_t* class;
    while(cour!=NULL){
        widget = get_widget_by_pt(cour->rect.top_left.x,cour->rect.top_left.y);
        class = ei_widget_get_class(widget);
        class->drawfunc(widget,ei_app_root_surface,NULL,ei_widget_get_parent(widget)->content_rect);
        cour=cour->next;
    }
    // on delock la surface root:
    hw_surface_unlock(ei_app_root_surface());
    hw_surface_unlock(get_offscreen_picking());
    // on update les rects : 
    hw_surface_update_rects(ei_app_root_surface(), get_invalidate_rect_list());
    hw_surface_update_rects(get_offscreen_picking(), get_invalidate_rect_list());
}


ei_widget_t get_widget_by_pt(int x0, int y0){
    ei_size_t dimension = hw_surface_get_size(ei_app_root_surface());
    int indice_surface = y0 * dimension.width + x0;
    uint32_t* pixel_ptr_offscreen = (uint32_t*)hw_surface_get_buffer(get_offscreen_picking());
    
    return obtenir(get_dicco_app(), pixel_ptr_offscreen[indice_surface]);
}



void ei_app_run(){
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    ei_frame_configure(root_widget,&surface_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    frame_draw(root_widget, root_surface, offscreen, NULL);
	// printf("tout est affichee\n");
    // getchar();
    ei_event_t event;
    // event.type = ei_ev_none;
    // hw_event_wait_next(&event);
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown)){
        event.type = ei_ev_mouse_buttondown;
        switch (event.type)
        {
        case ei_ev_mouse_buttondown:
            // ei_widget_t widget_pointee = get_widget_by_pt(event.param.mouse.where.x, event.param.mouse.where.y);
            ei_widget_t widget_pointee = get_widget_by_pt(155, 210);
            widget_pointee->wclass->handlefunc(widget_pointee, &event);
            break;
        
        case ei_ev_mou    ;
        default:
            break;
        }
	    hw_event_wait_next(&event);
    }
}