//
// Created by Ousmane Diakite on 17/05/2025.
//

#include "interracteur.h"


/*####################################################################################################################*/

//
// Created by Ousmane Diakite on 17/05/2025.
//

#include "interracteur.h"
#include "bib_4_toplevel.h"

/*####################################################################################################################*/

ei_widget_t toplevel_alloc() {
    ei_impl_toplevel_t* toplevel = malloc(sizeof(ei_impl_toplevel_t));
    if (toplevel == NULL) {
        fprintf(stderr, "Erreur\n");
        return NULL;
    }

    // On initialise tous les champs Ã  0 ou NULL tous les champs Ã  zÃ©ro.
    memset(toplevel, 0, sizeof(ei_impl_toplevel_t));

    // On retourne Ã§a comme un ei_widget_t
    return (ei_widget_t)toplevel;
}

/*####################################################################################################################*/

void ei_toplevel_configure(ei_widget_t		widget,
    ei_size_t*		requested_size,
    const ei_color_t*	color,
    int*			border_width,
    ei_string_t*		title,
    bool*			closable,
    ei_axis_set_t*		resizable,
    ei_size_ptr_t*		min_size) {
ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;

    if (requested_size != NULL) {
        if (toplevel->requested_size == NULL)
            toplevel->requested_size = malloc(sizeof(ei_size_t));
        widget->requested_size = *requested_size;
        *(toplevel->requested_size) = *requested_size;
    }

    if (color != NULL) {
        if (toplevel->color == NULL)
            toplevel->color = malloc(sizeof(ei_color_t));
        *(toplevel->color) = *(ei_color_t*)color; //Ã  revoir !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    if (border_width != NULL) {
        if (toplevel->border_width == NULL)
            toplevel->border_width = malloc(sizeof(int));
        *(toplevel->border_width) = *border_width;
    }

    if (title != NULL && *title != NULL) {
        if (toplevel->title != NULL)
            free(toplevel->title);
        toplevel->title = strdup(*title);  // on copie la chaÃ®ne
    }

    if (closable != NULL) {
        if (toplevel->closable == NULL)
            toplevel->closable = malloc(sizeof(bool));
        *(toplevel->closable) = *closable;
        if(*closable){
            // Pour finir en beauté, dessinons le button de fermeture de couleur rouge comme le sang de Dexter hahaha....
            ei_widget_t close_button = ei_widget_create("button", widget, NULL, NULL);
            ei_color_t rouge = {.blue = 255, .alpha = 255, .red = 0, .green = 0};
            ei_button_configure(close_button, &((ei_size_t){TAILLE_BOUTTON_CLOSE, TAILLE_BOUTTON_CLOSE}), &rouge, &(int){0},
                                &(int){6}, &(ei_relief_t){ei_relief_raised}, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
    }

    if (resizable != NULL) {
        if (toplevel->resizable == NULL)
            toplevel->resizable = malloc(sizeof(ei_axis_set_t));
        *(toplevel->resizable) = *resizable;
    }

    if (min_size != NULL && *min_size != NULL) {
        if (toplevel->min_size == NULL)
            toplevel->min_size = malloc(sizeof(ei_size_ptr_t));
        *(toplevel->min_size) = *min_size;
    *(*(toplevel->min_size)) = (ei_size_t){160, 120};//Ã  revoir!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
}

/*####################################################################################################################*/

void toplevel_setdefaults(ei_widget_t widget) {
    if (widget == NULL)
        return;

    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;

    // Taille par dÃ©faut. Attention, elle ne comprend pas les dÃ©corations(bordure, titre)
    ei_size_t requested_size = {320,240};

    // Couleur de fond par dÃ©faut
    const ei_color_t color = ei_default_background_color;

    // Bordure
    int border_width = 4;

    // Titre de la toplevel
    ei_string_t title = "Toplevel";

    // Couleur de la police du titre
    toplevel->title_color = (ei_color_t){0x00, 0x00, 0x00, 0xff};//Ã  vÃ©rifiers'il faut faire comme Ã§a, si non il faut aussi changer dans ei_implementation.h
    
    // Police du titre
    toplevel->title_font = ei_default_font;//de mÃªme

    // Indique si la fenÃªtre peut Ãªtre fermÃ©e
    bool closable = true;

    // Indique si la fenÃªtre est redimensionnable
    ei_axis_set_t resizable = ei_axis_both;

    // Pointeur vers la taille minimale de la fenÃªtre
    ei_size_ptr_t min_size = malloc(sizeof(ei_size_t));
    *min_size = (ei_size_t){160, 120};

    // Appelle la fonction de configuration standard pour tout initialiser
    ei_toplevel_configure(widget,
                           &requested_size,
                           &color,
                           &border_width,
                           &title,
                           &closable,
                           &resizable,
                           &min_size);
    // On initialise le rectangle de contenu Ã  la taille de la fenÃªtre

    ei_widget_set_content_rect(widget, NULL);

}

/*####################################################################################################################*/

void toplevel_release(ei_widget_t widget) {
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;
    /*libÃ©ration de ce qui est commun aux widgets*/
    free(toplevel->widget.wclass);
    free(toplevel->widget.user_data);
    free(toplevel->widget.placer_params);//il faudra l'ajouter aux autres widgets
    free(toplevel->widget.content_rect);

    /*libÃ©ration de ce qui est spÃ©cifique aux toplevel*/
    free(toplevel->requested_size);
    free(toplevel->color);
    free(toplevel->border_width);
    free(toplevel->title);
    free(toplevel->closable);
    free(toplevel->resizable);
    free(toplevel->min_size);

    /*libÃ©ration du widget*/
    free(toplevel);
}

/*####################################################################################################################*/

void toplevel_draw(ei_widget_t widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper) {
    int i, ir, ig, ib, ia;
   
    // on revient en type toplevel :
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*) widget;
    ei_impl_button_t* button_close;
    
   
    // zone est la variable contenant l'endroit ou doit etre trace la toplevel : 
    ei_rect_t zone = widget->screen_location;
   
    // gestion des couleurs
    ei_color_t couleur = reorder_color_channels(*(toplevel->color),ei_app_root_surface());
   
    ei_color_t *claire = change_color(&couleur, false), *foncee = change_color(&couleur, true);
   
    hw_surface_lock(surface);
   
   
    // on dessine uniquement l'entete du haut en avec les arrondis,
    // on va dans un premier temps utiliser draw button, ensuite, si le temps nous permet, essayer de 
    // de faire quelque chose de spécifique et optimisé:
    ei_rect_t cadre_haut = {zone.top_left, {zone.size.width, TAILLE_CADRE_HAUT}};
    draw_button(surface, &cadre_haut, RAYON_TOP_LEVEL, *foncee, foncee, foncee, clipper);
   
    // on dessine le texte aussi : 
    ei_draw_text(surface, &(ei_point_t){cadre_haut.top_left.x + 30, cadre_haut.top_left.y }, toplevel->title, ei_default_font, (ei_color_t){255,255,255,255}, clipper);
   
   
    // on dessine ensuite le bas de l'entete : 
    ei_point_t corps_top_level[5] = {
    {zone.top_left.x , zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL},
    {zone.top_left.x + zone.size.width, zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL},
    {zone.top_left.x + zone.size.width, zone.top_left.y + zone.size.height},
    {zone.top_left.x , zone.top_left.y + zone.size.height},
    {zone.top_left.x, zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL}
    };
   
    // on l'affiche en sombre : 
    ei_draw_polygon(surface, corps_top_level, 5, *foncee, clipper);
   
   
    // On dessine d'office sur l'offscreen
    // en passant on reycle le tableau prec: 
    ei_point_t dessin_offscreen[5] = {
    {zone.top_left.x , zone.top_left.y},
    {zone.top_left.x + zone.size.width, zone.top_left.y},
    {zone.top_left.x + zone.size.width, zone.top_left.y + zone.size.height},
    {zone.top_left.x , zone.top_left.y + zone.size.height},
    {zone.top_left.x, zone.top_left.y}
    };
    //ei_fill(get_offscreen_picking(), &(widget->pick_color), &(widget->screen_location));
    ei_draw_polygon(get_offscreen_picking(), dessin_offscreen, 5, widget->pick_color, clipper);
   
   
    int bordure = *(toplevel->border_width);
    // ensuite on s'occupe du content rect de la top level : 
    ei_point_t centre[5] = {
    {zone.top_left.x + bordure, zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL},
    {zone.top_left.x + zone.size.width - bordure, zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL},
    {zone.top_left.x + zone.size.width - bordure, zone.top_left.y + zone.size.height - bordure},
    {zone.top_left.x + bordure, zone.top_left.y + zone.size.height - bordure},
    {zone.top_left.x + bordure, zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL}
    }; 
    ei_rect_t rect_bas = {{zone.top_left.x + bordure,zone.top_left.y + TAILLE_ENTETE_TOP_LEVEL}, {zone.size.width - bordure,zone.size.height - bordure }};
   
    // On l'affiche cette fois ci en la couleur par defaut:
    ei_draw_polygon(surface, centre, 5, *(toplevel->color), clipper);
    //ei_fill(surface, (toplevel->color), &rect_bas);
   
   
    // donnees sur la zone de redimensionnment
    ei_point_t zone_redim[5] = {
    {zone.top_left.x + zone.size.width - TAILLE_BUTTON_RESIZE, zone.top_left.y + zone.size.height - TAILLE_BUTTON_RESIZE},
    {zone.top_left.x + zone.size.width , zone.top_left.y + zone.size.height - TAILLE_BUTTON_RESIZE},
    {zone.top_left.x + zone.size.width , zone.top_left.y + zone.size.height},
    {zone.top_left.x + zone.size.width - TAILLE_BUTTON_RESIZE, zone.top_left.y + zone.size.height},
    {zone.top_left.x + zone.size.width - TAILLE_BUTTON_RESIZE, zone.top_left.y + zone.size.height - TAILLE_BUTTON_RESIZE}
    };
    ei_draw_polygon(surface, zone_redim, 5, *foncee, clipper);
    
   
    hw_surface_unlock(surface);
   
    ei_widget_t fils_cour = widget->children_head;
    while (fils_cour!=NULL){
    //printf("j'affiche un fils dans toplevel de type %s\n", fils_cour->wclass->name);
    //printf("le secreen location est : %d %d %d %d \n", widget->content_rect->top_left.x, widget->content_rect->top_left.y, widget->screen_location.size.width, widget->screen_location.size.height);
    ei_impl_widget_draw_children(fils_cour, surface, pick_surface, widget->content_rect);
    fils_cour=fils_cour->next_sibling;
    }
   
   }

/*####################################################################################################################*/

void toplevel_geonotify(ei_widget_t widget){
    
    //ei_place_xy(widget->children_head, widget->screen_location.top_left.x + 10 ,widget->screen_location.top_left.y);
    ei_place(widget->children_head,           // widget du bouton
        &(ei_anchor_t){ei_anc_northwest},            // ancrage en haut à gauche
        &(int){10},                          // position x à 10px du bord gauche
        &(int){5},                           // position y à 0px pour la barre de titre
        NULL,                          // largeur (diamètre du bouton)
        NULL,                          // hauteur (diamètre du bouton)
        NULL,                        // pas de position relative en x
        NULL,                        // pas de position relative en y
        NULL,                        // pas de largeur relative
        NULL);                       // pas de hauteur relative
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;

    ei_widget_t fils = widget->children_head->next_sibling;

    while (fils)
    {
        ei_impl_placer_run(fils);
        fils = fils->next_sibling;
    }
    
}

/*####################################################################################################################*/


bool toplevel_handle(ei_widget_t widget, struct ei_event_t* event) {
    ei_impl_toplevel_t* toplevel = (ei_impl_toplevel_t*)widget;

    static bool is_dragging = false;
    static ei_point_t last_mouse_position;
    static bool is_resizing = false;
    ei_point_t topleft = widget->screen_location.top_left;
    ei_size_t taille = widget->screen_location.size;
    ei_axis_set_t resizing_axis = *(toplevel->resizable);

    ei_point_t mouse = event->param.mouse.where;
    bool inside_title_bar = mouse.y >= widget->screen_location.top_left.y &&
                            mouse.y <= widget->screen_location.top_left.y + 20; // ~titre haut

    bool inside_close_button = false;

    bool inside_resize_button = mouse.x >= widget->screen_location.top_left.x + widget->screen_location.size.width - TAILLE_BUTTON_RESIZE && mouse.x <= widget->screen_location.top_left.x + widget->screen_location.size.width &&
                                mouse.y >= widget->screen_location.top_left.y + widget->screen_location.size.height - TAILLE_BUTTON_RESIZE && mouse.y <= widget->screen_location.top_left.y + widget->screen_location.size.height;

    if (toplevel->closable != NULL && *(toplevel->closable)) {
        ei_rect_t close_button_area = {
            .top_left = {
                widget->screen_location.top_left.x + widget->screen_location.size.width - 20,
                widget->screen_location.top_left.y
            },
            .size = {20, 20}
        };
        inside_close_button = est_dans_rect(mouse, close_button_area);
    }

    switch (event->type) {

        case ei_ev_mouse_buttondown:
            if (inside_close_button) {
                //ei_widget_destroy(widget);
                return true;
            }

            if (inside_title_bar) {
                is_dragging = true;
                last_mouse_position = mouse;
                ei_event_set_active_widget(widget);
                return true;
            }

            if (inside_resize_button){
                is_resizing = true;
                last_mouse_position = mouse;
                ei_event_set_active_widget(widget);
                return true;
            }
            break;

        case ei_ev_mouse_move:
            if (is_dragging && ei_event_get_active_widget() == widget) {
                int dx = mouse.x - last_mouse_position.x;
                int dy = mouse.y - last_mouse_position.y;
                effacer(widget);
                widget->screen_location.top_left.x += dx;
                widget->screen_location.top_left.y += dy;
                last_mouse_position = mouse;
                //ei_app_invalidate_rect(&widget->screen_location);
                //draw_invalidate_rect();
                return true;
            }

            if (is_resizing && ei_event_get_active_widget() == widget && resizing_axis==ei_axis_both){
                int dx = mouse.x - last_mouse_position.x;
                int dy = mouse.y - last_mouse_position.y;
                last_mouse_position=mouse;
                effacer(widget);
                widget->screen_location.size.height += dy;
                widget->screen_location.size.width += dx;
                ei_app_invalidate_rect(&widget->screen_location);
                // ei_widget_t fils_cour = widget->children_head;
                // while (fils_cour){
                //     ei_app_invalidate_rect(&(fils_cour->screen_location));
                //     fils_cour = fils_cour->next_sibling;
                // }
                draw_invalidate_rect();
                return true;
            }

            if (is_resizing && ei_event_get_active_widget() == widget && resizing_axis==ei_axis_x){
                int dx = mouse.x - last_mouse_position.x;
                last_mouse_position=mouse;
                effacer(widget);
                widget->screen_location.size.width += dx;
                
                ei_app_invalidate_rect(&widget->screen_location);
                draw_invalidate_rect();
                return true;
            }

            if (is_resizing && ei_event_get_active_widget() == widget && resizing_axis==ei_axis_y){
                int dy = mouse.y - last_mouse_position.y;
                last_mouse_position=mouse;
                effacer(widget);
                widget->screen_location.size.width += dy;
                
                ei_app_invalidate_rect(&widget->screen_location);
                draw_invalidate_rect();
                return true;
            }
            break;

        case ei_ev_mouse_buttonup:
            if (is_dragging && ei_event_get_active_widget() == widget) {
                is_dragging = false;
                ei_event_set_active_widget(NULL);
                return true;
            }

            if (is_resizing && ei_event_get_active_widget() == widget) {
                is_resizing = false;
                ei_event_set_active_widget(NULL);
                return true;
            }
            break;

        default:
            break;
    }

    return false;
}

/*####################################################################################################################*/

ei_widgetclass_t* create_toplevel_widgetclass(){

    ei_widgetclass_t* toplevel_widgetclass = malloc(sizeof(ei_widgetclass_t));
    strcpy(toplevel_widgetclass->name, "toplevel");
    toplevel_widgetclass->allocfunc = toplevel_alloc;
    toplevel_widgetclass->releasefunc = toplevel_release;
    toplevel_widgetclass->drawfunc = toplevel_draw;
    toplevel_widgetclass->setdefaultsfunc = toplevel_setdefaults;
    toplevel_widgetclass->geomnotifyfunc = toplevel_geonotify;
    toplevel_widgetclass->handlefunc = toplevel_handle;
    toplevel_widgetclass->next=NULL;
    return toplevel_widgetclass;
}

/*####################################################################################################################*/

