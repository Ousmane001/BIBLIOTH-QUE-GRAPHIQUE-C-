/**
 * @file	ei_widget_configure.h
 *
 * @brief 	API for widgets configuration.
 *
 *  Created by François Bérard on 10.04.2023
 *  Copyright 2023 Ensimag. All rights reserved.
 */

#ifndef EI_WIDGET_CONFIGURE_H
#define EI_WIDGET_CONFIGURE_H

#include "ei_widget.h"

/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "optional parameters" protocol: if a parameter is "NULL", the state
 *		of the configured frame for that parameter does not change.
 *		When a frame is created, its parameters are set with the default values specified here.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including the widget's borders.
 *				The geometry manager may override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the border and the text or the image. This may be (0, 0)
 *				if the widget has border_width=0, and no text and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed within the widget.
 *				Defines both the anchoring point on the parent and on the widget.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
 				Defaults to NULL.
 * @param	img_rect	If not NULL, the rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed within the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
void			ei_frame_configure		(ei_widget_t		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_relief_t*		relief,
							 ei_string_t*		text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_ptr_t*		img_rect,
							 ei_anchor_t*		img_anchor);



static const int	k_default_button_border_width	= 4;	///< The default border width of button widgets.
static const int	k_default_button_corner_radius	= 10;	///< The default corner radius of button widgets.

static inline void ei_frame_set_bg_color	(ei_widget_t frame,  ei_color_t bg_color)	{ ei_frame_configure(frame, NULL, &bg_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_frame_set_text		(ei_widget_t frame,  ei_string_t text)		{ ei_frame_configure(frame, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_frame_set_image		(ei_widget_t frame,  ei_surface_t image)	{ ei_frame_configure(frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL); }


/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void			ei_button_configure		(ei_widget_t		widget,
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

static inline void ei_button_set_bg_color	(ei_widget_t button, ei_color_t bg_color)	{ ei_button_configure(button, NULL, &bg_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_button_set_text		(ei_widget_t button, ei_string_t text)		{ ei_button_configure(button, NULL, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_button_set_image		(ei_widget_t button, ei_surface_t image)	{ ei_button_configure(button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL, NULL, NULL); }

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title displayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to true.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size of its content.
 *				The default minimum size of a toplevel is (160, 120).
 *				If *min_size is NULL, this requires the min_size to be configured to
 *				the default size.
 */
void			ei_toplevel_configure		(ei_widget_t		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_string_t*		title,
							 bool*			closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_ptr_t*		min_size);




#endif //EI_WIDGET_CONFIGURE_H
