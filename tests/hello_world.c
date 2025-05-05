#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param)
{
	printf("Click !\n");
}

/*
 * default_handler --
 *
 *	Callback called to handle keypress and window close events.
 */
void default_handler(ei_event_t* event)
{
	if ( (event->type == ei_ev_close) ||
	    ((event->type == ei_ev_keydown) && (event->param.key_code == SDLK_ESCAPE)))
		ei_app_quit_request();
}

int main(int argc, char** argv)
{
	ei_widget_t	button;
	ei_widget_t	window;


	/* Create the application and change the color of the background. */
	ei_app_create			((ei_size_t){800, 600}, false);
	ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

	/* Create, configure and place a toplevel window on screen. */
	window = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
	ei_toplevel_configure		(window, &(ei_size_t){320,240},
					      	 &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
						 &(int){2},
						 &(ei_string_t){"Hello World"}, NULL, NULL, NULL);
	ei_place_xy			(window, 30, 10);

	/* Create, configure and place a button as a descendant of the toplevel window. */
	button = ei_widget_create	("button", window, NULL, NULL);
	ei_button_configure		(button, NULL,
					    	&(ei_color_t){0x88, 0x88, 0x88, 0xff},
						&(int){2}, NULL,
						&(ei_relief_t){ei_relief_raised},
						&(ei_string_t){"click"}, NULL,
						&(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
						&(ei_callback_t){button_press}, NULL);
	ei_place			(button, &(ei_anchor_t){ei_anc_southeast},
					 	&(int){-20}, &(int){-20}, NULL, NULL,
					 	&(float){1.0f}, &(float){1.0f},
					 	&(float){0.5f}, NULL);

	ei_event_set_default_handle_func(default_handler);

	ei_app_run();

	ei_app_free();

	return (EXIT_SUCCESS);
}
