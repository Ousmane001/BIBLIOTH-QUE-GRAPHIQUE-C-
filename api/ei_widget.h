/**
 * @file	ei_widget.h
 *
 * @brief 	API for widgets management: creation, destruction
 *
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

#ifndef EI_WIDGET_H
#define EI_WIDGET_H

#include "ei_widgetclass.h"
#include "ei_draw.h"


struct ei_event_t;



/**
 * @brief	The type of functions that are called just before a widget is being destroyed
 * 		(the "widget" parameter and its fields are still valid).
 * 		Functions of this type are passed as a parameter to \ref ei_widget_create.
 *
 * @param	widget		The widget that is going to be destroyed.
 */
typedef void		(*ei_widget_destructor_t)	(ei_widget_t widget);





/**
 * @brief	A function that is called in response to a user event. For example, the function that
 *		a programmer wants to be called when the user has pressed on a graphical button.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 */
typedef void		(*ei_callback_t)		(ei_widget_t		widget,
							 struct ei_event_t*	event,
							 ei_user_param_t	user_param);



/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		an existing widget.
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		When no more needed, the widget must be released by calling \ref ei_widget_destroy.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can not be NULL.
 * @param	user_data	A pointer provided by the programmer for private use. May be NULL.
 * @param	destructor	A pointer to a function to call before destroying a widget structure. May be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */
ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
							 ei_widget_t		parent,
							 ei_user_param_t	user_data,
							 ei_widget_destructor_t destructor);

/**
 * @brief	Destroys a widget.
 * 		Removes the widget from the screen if it is currently displayed.
 * 		Destroys all its descendants.
 *		Calls its destructor if it was provided.
 * 		Frees the memory used by the widget (e.g. the widget param).
 *
 * @param	widget		The widget that is to be destroyed.
 */
void			ei_widget_destroy		(ei_widget_t		widget);


/**
 * @brief	Returns if the widget is currently displayed (i.e. managed by a geometry manager).
 *
 * @param	widget		The widget.
 *
 * @return			true if the widget is displayed, false otherwise.
 */
bool	 		ei_widget_is_displayed		(ei_widget_t		widget);


/**
 * @brief	Returns the widget that is at a given location on screen.
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 *
 * @return			The top-most widget at this location, or NULL if there is no widget
 *				at this location (except for the root widget).
 */
ei_widget_t		ei_widget_pick			(ei_point_t*		where);






#endif
