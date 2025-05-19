//
// Created by Ousmane Diakite on 17/05/2025.
//

#include "interracteur.h"

/*###############################      Variables static utilisés entre widget    ######################################*/


// surface total de l'ecran  et de l'offscreen qu'on va user pour le clipping vers la fin du projet
static ei_surface_t root_surface, offscreen;
// widget racine (frame root)
static ei_widget_t root_widget;

// Liste chaînée globale des classes de widgets
static ei_widgetclass_t* g_widgetclass_list = NULL; //c'est le début de la liste chainee des widgetclass

// dictionnaire de correspondance entre cle et widget
static dictionnaire dicco_app;

// variable qui determine la cle actuel a incrementer pour le suivant
static int key_actuel = 0;

static ei_linked_rect_t* ei_linked_rect_t_list = NULL;


// variable precisant le widget actif : 
ei_widget_t active_widget;



/*####################################################################################################################*/

ei_widget_t ei_event_get_active_widget(void){
	return active_widget;
}
/*####################################################################################################################*/

void ei_event_set_active_widget(ei_widget_t widget){
	active_widget = widget;
}
/*####################################################################################################################*/

int get_key_actuel(void){
	return key_actuel;
}
/*####################################################################################################################*/

ei_widgetclass_t* get_widget_class_list(void){
	return g_widgetclass_list;
}

/*####################################################################################################################*/

void initialise_dicco_app(void){
	dicco_app = creer_dictionnaire();
}
/*####################################################################################################################*/

void incremente(void){
	key_actuel++;
}

/*####################################################################################################################*/

dictionnaire* get_dicco_app(void){
	return &dicco_app;
}

/*####################################################################################################################*/

void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
	if (widgetclass == NULL) {
		return;
	}

	// on verifie si une classe avec le même nom est déjà enregistrée
	ei_widgetclass_t* current = g_widgetclass_list;
	while (current != NULL) {
		if (strcmp(current->name, widgetclass->name) == 0) {
			// Une classe avec ce nom est déjà enregistrée, on ne fait rien
			return;
		}
		current = current->next;
	}

	// Insérer la nouvelle classe en tête de liste
	widgetclass->next = g_widgetclass_list;
	g_widgetclass_list = widgetclass;
}
/*####################################################################################################################*/

void ei_app_create(ei_size_t main_window_size, bool fullscreen) {

    hw_init();
    root_surface = hw_create_window(main_window_size, fullscreen);
    hw_surface_lock(root_surface);

	// on initialise le dictionnaire et on rajoute:
    initialise_dicco_app();

    // on cree une surface offscreen pour dessiner en mémoire
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    offscreen = hw_surface_create(root_surface, surface_size, false);

    // on enregistre les classes de widgets (ici, "frame" pour le root)
    ei_widgetclass_register(create_frame_widgetclass());

    // Meme chose pour la classe button
    ei_widgetclass_register(create_button_widgetclass());

	// on enregistre finalement aussi toplevel après tant de code ...
	ei_widgetclass_register(create_toplevel_widgetclass());

    // Crée le widget racine (de type "frame"), sans parent
    root_widget = ei_widget_create("frame", NULL, NULL, NULL);
    ei_frame_configure(root_widget,&main_window_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    root_widget->screen_location.top_left=(ei_point_t) {0,0};
    root_widget->screen_location.size=main_window_size;
    root_widget->content_rect = &(root_widget->screen_location);
    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, get_invalidate_rect_list());

    
}

/*####################################################################################################################*/
void ei_app_run(){
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    frame_draw(ei_app_root_widget(), ei_app_root_surface(), get_offscreen_picking(), NULL);

    ei_widget_t widget_pointee = NULL;
    ei_event_t event;
    event.type = ei_ev_none;

    while ((event.type != ei_ev_close)) {
        hw_event_wait_next(&event);

		// ce que va retourner le traitnat de l'interruption : 
        bool handled = false;

        ei_widget_t active_widget = ei_event_get_active_widget();
        if (active_widget != NULL) {
            handled = active_widget->wclass->handlefunc(active_widget, &event);
        }

        if (!handled && (event.type == ei_ev_mouse_buttondown || event.type == ei_ev_mouse_buttonup)) {
            widget_pointee = get_widget_by_pt(event.param.mouse.where.x, event.param.mouse.where.y);
            if (widget_pointee != NULL) {
				widget_pointee->wclass->geomnotifyfunc(widget_pointee);
                handled = widget_pointee->wclass->handlefunc(widget_pointee, &event);
            }
        }

        // if (!handled && ei_event_get_default_handle_func() != NULL) {
        //     ei_event_get_default_handle_func()(&event);
        // }

        // Mise à jour graphique différée
        draw_invalidate_rect();
        hw_surface_update_rects(root_surface, get_invalidate_rect_list());
    }
}



/*####################################################################################################################*/
void ei_app_free(void){

	// on libere le dicco de correspondance entre cle et widget :
	liberer(get_dicco_app());
}

/*####################################################################################################################*/

ei_widget_t ei_app_root_widget(){
	return root_widget;
}
/*####################################################################################################################*/

ei_surface_t get_offscreen_picking(void){
	return offscreen;
}
/*####################################################################################################################*/

void ei_event_set_default_handle_func(ei_default_handle_func_t func){}

/*####################################################################################################################*/

void ei_app_quit_request(void){}

/*####################################################################################################################*/

void ei_app_invalidate_rect(const ei_rect_t* rect){
	// ei_linked_rect_t* a_traiter = malloc(sizeof(ei_linked_rect_t));
	// if (a_traiter==NULL){
	// 	fprintf(stderr,"memoire finito, va t'acheter de la ram\n");
	// 	exit(EXIT_FAILURE);
	// }
	// a_traiter->rect = *rect;
	// a_traiter->next = ei_linked_rect_t_list;
	// ei_linked_rect_t_list = a_traiter;

	ei_linked_rect_t* a_traiter = calloc(1,sizeof(ei_linked_rect_t));
	if (a_traiter==NULL){
		fprintf(stderr,"memoire finito, va t'acheter de la ram\n");
		exit(EXIT_FAILURE);
	}
	if (ei_linked_rect_t_list){
		ei_linked_rect_t* cour =get_invalidate_rect_list();
		while(cour->next){
			cour=cour->next;
		}
		a_traiter->rect = *rect;
		cour->next = a_traiter; // checkpoint
	}
	else{
		a_traiter->rect = *rect;
		ei_linked_rect_t_list = a_traiter;
	}
}


/*####################################################################################################################*/

ei_linked_rect_t* get_invalidate_rect_list(void){
	return ei_linked_rect_t_list;
}

/*####################################################################################################################*/

ei_surface_t ei_app_root_surface(void){
	return root_surface;
}

/*####################################################################################################################*/

void draw_invalidate_rect(void){

	// on lock la surface avant de tracer
	// hw_surface_lock(ei_app_root_surface());
	// hw_surface_lock(get_offscreen_picking());

	//on récupere le 1er rectangle a redessiner
	ei_linked_rect_t* cour = get_invalidate_rect_list();
	if(cour != NULL)
	{
		ei_widget_t widget;
		ei_widgetclass_t* class;
		while(cour!=NULL){
			widget = get_widget_by_pt(cour->rect.top_left.x,cour->rect.top_left.y);
			if(widget->wclass->drawfunc)
				widget->wclass->drawfunc(widget,ei_app_root_surface(), get_offscreen_picking(),&(cour->rect));
			cour=cour->next;
		}
	}
	// on delock la surface root:
	// hw_surface_unlock(ei_app_root_surface());
	// hw_surface_unlock(get_offscreen_picking());
	// on update les rects :
	hw_surface_update_rects(ei_app_root_surface(), get_invalidate_rect_list());
	ei_linked_rect_t_list = NULL;
}

/*####################################################################################################################*/

ei_widget_t get_widget_by_pt(int x0, int y0){
	ei_size_t dimension = hw_surface_get_size(ei_app_root_surface());
	int indice_surface = y0 * dimension.width + x0;
	hw_surface_lock(get_offscreen_picking());
	uint32_t* pixel_ptr_offscreen = (uint32_t*)hw_surface_get_buffer(get_offscreen_picking());
	hw_surface_unlock(get_offscreen_picking());
	return obtenir(get_dicco_app(), pixel_ptr_offscreen[indice_surface]);
}

/*####################################################################################################################*/

void effacer(ei_widget_t widget){
	if (widget==NULL || widget->parent==NULL){
		return;
	}

	ei_widget_t pere = widget->parent;
	ei_app_invalidate_rect(&(pere->screen_location));
	// pere->wclass->drawfunc(pere,ei_app_root_surface(),get_offscreen_picking(),NULL);
	ei_widget_t fils_cour=pere->children_head;
	while (fils_cour){
		if (fils_cour!=widget){
			ei_app_invalidate_rect(&(fils_cour->screen_location));
		}
		fils_cour=fils_cour->next_sibling;
	}
}