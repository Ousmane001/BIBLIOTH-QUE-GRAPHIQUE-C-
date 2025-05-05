/**
 *  @file	ei_widgetclass.h
 *  @brief	Definition and registration of widget classes.
 *
 *  \author
 *  Created by François Bérard on 02.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_WIDGETCLASS_H
#define EI_WIDGETCLASS_H

#include "hw_interface.h"
#include "ei_draw.h"


/**
 * \brief	Returns the size of the opaque struct that is pointed to by \ref ei_widget_t
 * 			(i.e. the size of \ref ei_impl_widget_t).
 */
size_t		ei_widget_struct_size();


/**
 * \brief	A name of a class of widget.
 */
typedef char 	ei_widgetclass_name_t[20];

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class: both the common attributes, that use
 *		\ref ei_widget_struct_size bytes, and the specific attributes of the class.
 *		After allocation, the function *must* initialize the memory to 0.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
typedef ei_widget_t (*ei_widgetclass_allocfunc_t)	();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
typedef void	(*ei_widgetclass_releasefunc_t)		(ei_widget_t	widget);

/**
 * \brief	A function that draws a widget of a class.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
typedef void	(*ei_widgetclass_drawfunc_t)		(ei_widget_t		widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

/**
 * \brief	A function that sets the default values for a widget of this class.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
typedef void	(*ei_widgetclass_setdefaultsfunc_t)	(ei_widget_t		widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
typedef void	(*ei_widgetclass_geomnotifyfunc_t)	(ei_widget_t		widget);


/**
 * @brief	A function that is called in response to an event. This function
 *		is internal to the library. It implements the generic behavior of
 *		a widget (for example a button looks sunken when clicked)
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If true, the library does not try to call other callbacks for this
 *				event. If false, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				false, or return true without having done anything.
 */
typedef bool	(*ei_widgetclass_handlefunc_t)		(ei_widget_t		widget,
						 	 struct ei_event_t*	event);


/**
 * \brief	A structure that stores information about a class of widgets.
 */
typedef struct ei_widgetclass_t {
	ei_widgetclass_name_t			name;			///< The string name of this class of widget.
	ei_widgetclass_allocfunc_t		allocfunc;		///< The function that allocated instances of this class of widget.
	ei_widgetclass_releasefunc_t		releasefunc;		///< The function that releases all the resources used by an instance of this class of widget.
	ei_widgetclass_drawfunc_t		drawfunc;		///< The function that draws on screen an instance of this class of widget.
	ei_widgetclass_setdefaultsfunc_t	setdefaultsfunc;	///< The function that sets the default values to all the parameters of an instance of this class of widget.
	ei_widgetclass_geomnotifyfunc_t		geomnotifyfunc;		///< The function that is called to notify an instance of widget of this class that its geometry has changed.
	ei_widgetclass_handlefunc_t		handlefunc;		///< The function that is called when the application has received a user event referring to an instance of this class of widget.
	struct ei_widgetclass_t*		next;			///< A pointer to the next instance of ei_widget_class_t, allows widget class descriptions to be chained.
} ei_widgetclass_t;






/**
 * @brief	Registers a class to the program so that widgets of this class can be created
 * 		with \ref ei_widget_create.
 *		This must be done only once per widget class in the application.
 *
 * @param	widgetclass	The structure describing the class.
 *				The structure is not copied by the library: the caller must make sure that it is valid
 *				until the end of the program's execution.
 */
void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass);


/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t*	ei_widgetclass_from_name	(ei_const_string_t name);






#endif
