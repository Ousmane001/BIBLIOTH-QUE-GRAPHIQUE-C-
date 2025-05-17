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



    // on cree une surface offscreen pour dessiner en mémoire
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    offscreen = hw_surface_create(root_surface, surface_size, false);

    // on enregistre les classes de widgets (ici, "frame" pour le root)
    ei_widgetclass_register(create_frame_widgetclass());

    // Meme chose pour la classe button
    ei_widgetclass_register(create_button_widgetclass());

    // Crée le widget racine (de type "frame"), sans parent
    root_widget = ei_widget_create("frame", NULL, NULL, NULL);
    ei_frame_configure(root_widget,&main_window_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    root_widget->screen_location.top_left=(ei_point_t) {0,0};
    root_widget->screen_location.size=main_window_size;
    root_widget->content_rect = &(root_widget->screen_location);
    hw_surface_unlock(root_surface);
    hw_surface_update_rects(root_surface, get_invalidate_rect_list());

    // on initialise le dictionnaire et on rajoute:
    initialise_dicco_app();
}

/*####################################################################################################################*/

void ei_app_run(){
    ei_size_t surface_size = hw_surface_get_size(root_surface);
    ei_frame_configure(root_widget,&surface_size,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    frame_draw(root_widget, root_surface, offscreen, NULL);
	// printf("tout est affichee\n");
    // getchar();
	ei_widget_t widget_pointee;
    ei_event_t event;
    event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown)){

        switch (event.type)
        {
        case ei_ev_mouse_buttondown:
            widget_pointee = get_widget_by_pt(event.param.mouse.where.x, event.param.mouse.where.y);
            widget_pointee->wclass->handlefunc(widget_pointee, &event);
            break;

           default:
            break;
        }
	    hw_event_wait_next(&event);

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
	ei_linked_rect_t* a_traiter = malloc(sizeof(ei_linked_rect_t));
	if (a_traiter==NULL){
		fprintf(stderr,"memoire finito, va t'acheter de la ram\n");
		exit(EXIT_FAILURE);
	}
	a_traiter->rect = *rect;
	a_traiter->next = ei_linked_rect_t_list;
	ei_linked_rect_t_list = a_traiter;
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
			printf(" invalidate rect pour -> %s\n",widget->wclass->name);

			if(widget->wclass->drawfunc)
				widget->wclass->drawfunc(widget,ei_app_root_surface(),NULL,&(cour->rect));
			printf(" fin inv rect pour -> %s\n\n\n",widget->wclass->name);
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
