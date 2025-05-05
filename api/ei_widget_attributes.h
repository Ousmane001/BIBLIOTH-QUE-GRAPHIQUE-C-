/**
 * @file	ei_widget_attributes.h
 *
 * @brief 	API for accessing widgets attributes.
 *
 *  Created by François Bérard on 12.04.2023
 *  Copyright 2023 Ensimag. All rights reserved.
 */

#ifndef EI_WIDGET_ATTRIBUTES_H
#define EI_WIDGET_ATTRIBUTES_H

#include "ei_widget.h"


/**
 * @brief	Get the class of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The structure that describe the class of this widget.
 */
ei_widgetclass_t*	ei_widget_get_class		(ei_widget_t		widget);

/**
 * @brief	Get the identifier of the widget in the picking offscreen under the form
 * 		of a color.
 *
 * @param	widget		The widget.
 *
 * @return			The (false)color of the pixels that represent this widget
 * 				in the picking offscreen.
 */
const ei_color_t*	ei_widget_get_pick_color	(ei_widget_t		widget);

/**
 * @brief	Get the parent of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The parent of the widget, or NULL if called on the root widget.
 */
ei_widget_t 		ei_widget_get_parent		(ei_widget_t		widget);

/**
 * @brief	Get the first child of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The first child, or NULL if the widget has no child.
 */
ei_widget_t 		ei_widget_get_first_child	(ei_widget_t		widget);

/**
 * @brief	Get the last child of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The last child, or NULL if the widget has no child.
 */
ei_widget_t 		ei_widget_get_last_child	(ei_widget_t		widget);

/**
 * @brief	Get the next sibling of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The sibling next to this widget in the widget's parent sibling order.
 * 				NULL if the widget is the last child or the root widget.
 */
ei_widget_t 		ei_widget_get_next_sibling	(ei_widget_t		widget);

/**
 * @brief	Get the user data associated to a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The user data associated with the widget.
 */
void*			ei_widget_get_user_data		(ei_widget_t		widget);

/**
 * @brief	Get the requested size of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The size requested by the widget (big enough for its label, for example) or by the programmer.
 * 				Geometry managers use this information to compute an actual size on the screen that may be different.
 */
const ei_size_t*	ei_widget_get_requested_size	(ei_widget_t		widget);

/**
 * @brief	Sets the requested size of a widget.
 *
 * @param	widget		The widget.
 * @param	requested_size	See \ref ei_widget_get_requested_size for a definition.
 */
void	 		ei_widget_set_requested_size	(ei_widget_t		widget,
							 ei_size_t 		requested_size);

/**
 * @brief	Get the screen location of the widget.
 *
 * @param	widget		The widget.
 *
 * @return			The location of the widget on the screen, expressed in the screen reference
 * 				(origin at the top/left of the screen, ordinates increasing downward).
 */
const ei_rect_t*	ei_widget_get_screen_location	(ei_widget_t		widget);

/**
 * @brief	Get the content rect of a widget.
 *
 * @param	widget		The widget.
 *
 * @return			The rectangle, expressed in screen coordinates, that defines where the children can be drawn.
 */
const ei_rect_t*	ei_widget_get_content_rect	(ei_widget_t		widget);

/**
 * @brief	Sets the content rect of a widget.
 *
 * @param	widget		The widget.
 * @param	content_rect	See \ref ei_widget_get_content_rect for a definition.
 */
void	 		ei_widget_set_content_rect	(ei_widget_t		widget,
							 const ei_rect_t*	content_rect);



#endif //EI_WIDGET_ATTRIBUTES_H

