//
// Created by diakitao on 5/12/25.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "../implem/ei_implementation.h"
#include "../implem/interracteur.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv){

	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_rect_t*		clipper_ptr	= NULL;
	ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
	clipper_ptr		= &clipper;
	ei_event_t		event;

	verite_sur_safwane();
	hw_init();

	main_window = hw_create_window(win_size, false);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, clipper_ptr);

	
	draw_button(main_window, &(ei_rect_t){{100,100},{200,100}}, 20, (ei_color_t){200, 80, 60}, NULL, false);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);

	/* Wait for a character on command line. */
	event.type = ei_ev_none;
	while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown))
		hw_event_wait_next(&event);

	hw_quit();

	return (EXIT_SUCCESS);
}
