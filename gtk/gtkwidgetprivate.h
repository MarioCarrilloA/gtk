/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */

#ifndef __GTK_WIDGET_PRIVATE_H__
#define __GTK_WIDGET_PRIVATE_H__

#include "gtkwidget.h"

#include "gtkactionmuxerprivate.h"
#include "gtkcontainer.h"
#include "gtkcsstypesprivate.h"
#include "gtkeventcontroller.h"
#include "gtksizerequestcacheprivate.h"

G_BEGIN_DECLS

#define GTK_STATE_FLAGS_BITS 14

struct _GtkWidgetPrivate
{
  /* The state of the widget. Needs to be able to hold all GtkStateFlags bits
   * (defined in "gtkenums.h").
   */
  guint state_flags : GTK_STATE_FLAGS_BITS;

  guint direction             : 2;

#ifdef G_ENABLE_DEBUG
  guint highlight_resize      : 1;
#endif

  guint in_destruction        : 1;
  guint toplevel              : 1;
  guint anchored              : 1;
  guint no_window             : 1;
  guint no_window_set         : 1;
  guint realized              : 1;
  guint mapped                : 1;
  guint visible               : 1;
  guint sensitive             : 1;
  guint can_focus             : 1;
  guint has_focus             : 1;
  guint focus_on_click        : 1;
  guint can_default           : 1;
  guint has_default           : 1;
  guint receives_default      : 1;
  guint has_grab              : 1;
  guint shadowed              : 1;
  guint child_visible         : 1;
  guint multidevice           : 1;
  guint has_shape_mask        : 1;
  guint pass_through          : 1;

  /* Queue-resize related flags */
  guint resize_needed         : 1; /* queue_resize() has been called but no get_preferred_size() yet */
  guint alloc_needed          : 1; /* this widget needs a size_allocate() call */
  guint alloc_needed_on_child : 1; /* 0 or more children - or this widget - need a size_allocate() call */

  /* Expand-related flags */
  guint need_compute_expand   : 1; /* Need to recompute computed_[hv]_expand */
  guint computed_hexpand      : 1; /* computed results (composite of child flags) */
  guint computed_vexpand      : 1;
  guint hexpand               : 1; /* application-forced expand */
  guint vexpand               : 1;
  guint hexpand_set           : 1; /* whether to use application-forced  */
  guint vexpand_set           : 1; /* instead of computing from children */
  guint has_tooltip           : 1;

  /* SizeGroup related flags */
  guint have_size_groups      : 1;

  /* Alignment */
  guint   halign              : 4;
  guint   valign              : 4;

  guint8 alpha;
  guint8 user_alpha;

#ifdef G_ENABLE_CONSISTENCY_CHECKS
  /* Number of gtk_widget_push_verify_invariants () */
  guint8 verifying_invariants_count;
#endif

  gint width;
  gint height;
  GtkBorder margin;

  /* Animations and other things to update on clock ticks */
  guint clock_tick_id;
  GList *tick_callbacks;

  /* The widget's name. If the widget does not have a name
   * (the name is NULL), then its name (as returned by
   * "gtk_widget_get_name") is its class's name.
   * Among other things, the widget name is used to determine
   * the style to use for a widget.
   */
  gchar *name;

  /* The list of attached windows to this widget.
   * We keep a list in order to call reset_style to all of them,
   * recursively.
   */
  GList *attached_windows;

  /* The style for the widget. The style contains the
   * colors the widget should be drawn in for each state
   * along with graphics contexts used to draw with and
   * the font to use for text.
   */
  GtkCssNode *cssnode;
  GtkStyleContext *context;

  /* The widget's allocated size */
  GtkAllocation allocated_size;
  gint allocated_size_baseline;
  GtkAllocation allocation;
  GtkAllocation clip;
  GtkAllocation reported_clip;
  gint allocated_baseline;

  /* The widget's requested sizes */
  SizeRequestCache requests;

  /* The widget's window or its parent window if it does
   * not have a window. (Which will be indicated by the
   * no_window field being set).
   */
  GdkWindow *window;
  GList *registered_windows;

  GList *event_controllers;

  AtkObject *accessible;

  /* Widget tree */
  GtkWidget *parent;
  GtkWidget *prev_sibling;
  GtkWidget *next_sibling;
  GtkWidget *first_child;
  GtkWidget *last_child;

  GtkWidget *focus_child;

  /* Pointer cursor */
  GdkCursor *cursor;
};

GtkCssNode *  gtk_widget_get_css_node       (GtkWidget *widget);
void         _gtk_widget_set_visible_flag   (GtkWidget *widget,
                                             gboolean   visible);
gboolean     _gtk_widget_get_anchored       (GtkWidget *widget);
void         _gtk_widget_set_anchored       (GtkWidget *widget,
                                             gboolean   anchored);
gboolean     _gtk_widget_get_shadowed       (GtkWidget *widget);
void         _gtk_widget_set_shadowed       (GtkWidget *widget,
                                             gboolean   shadowed);
gboolean     _gtk_widget_get_alloc_needed   (GtkWidget *widget);
gboolean     gtk_widget_needs_allocate      (GtkWidget *widget);
void         gtk_widget_ensure_resize       (GtkWidget *widget);
void         gtk_widget_ensure_allocate     (GtkWidget *widget);
void          _gtk_widget_scale_changed     (GtkWidget *widget);


void         _gtk_widget_add_sizegroup         (GtkWidget    *widget,
						gpointer      group);
void         _gtk_widget_remove_sizegroup      (GtkWidget    *widget,
						gpointer      group);
GSList      *_gtk_widget_get_sizegroups        (GtkWidget    *widget);

void         _gtk_widget_add_attached_window    (GtkWidget    *widget,
                                                 GtkWindow    *window);
void         _gtk_widget_remove_attached_window (GtkWidget    *widget,
                                                 GtkWindow    *window);

void _gtk_widget_get_preferred_size_and_baseline(GtkWidget        *widget,
                                                GtkRequisition    *minimum_size,
                                                GtkRequisition    *natural_size,
                                                gint              *minimum_baseline,
                                                gint              *natural_baseline);

const gchar*      _gtk_widget_get_accel_path               (GtkWidget *widget,
                                                            gboolean  *locked);

AtkObject *       _gtk_widget_peek_accessible              (GtkWidget *widget);

void              _gtk_widget_set_has_default              (GtkWidget *widget,
                                                            gboolean   has_default);
void              _gtk_widget_set_has_grab                 (GtkWidget *widget,
                                                            gboolean   has_grab);
void              _gtk_widget_set_is_toplevel              (GtkWidget *widget,
                                                            gboolean   is_toplevel);

void              _gtk_widget_grab_notify                  (GtkWidget *widget,
                                                            gboolean   was_grabbed);

void              _gtk_widget_propagate_hierarchy_changed  (GtkWidget  *widget,
                                                            GtkWidget  *previous_toplevel);
void              _gtk_widget_propagate_display_changed    (GtkWidget  *widget,
                                                            GdkDisplay *previous_display);

void              _gtk_widget_set_device_window            (GtkWidget *widget,
                                                            GdkDevice *device,
                                                            GdkWindow *pointer_window);
GdkWindow *       _gtk_widget_get_device_window            (GtkWidget *widget,
                                                            GdkDevice *device);
GList *           _gtk_widget_list_devices                 (GtkWidget *widget);

void              _gtk_widget_synthesize_crossing          (GtkWidget       *from,
                                                            GtkWidget       *to,
                                                            GdkDevice       *device,
                                                            GdkCrossingMode  mode);

static inline gpointer _gtk_widget_peek_request_cache           (GtkWidget *widget);

void              _gtk_widget_buildable_finish_accelerator (GtkWidget *widget,
                                                            GtkWidget *toplevel,
                                                            gpointer   user_data);
GtkStyleContext * _gtk_widget_peek_style_context           (GtkWidget *widget);

typedef gboolean (*GtkCapturedEventHandler) (GtkWidget *widget, GdkEvent *event);

void              _gtk_widget_set_captured_event_handler (GtkWidget               *widget,
                                                          GtkCapturedEventHandler  handler);

gboolean          _gtk_widget_captured_event               (GtkWidget      *widget,
                                                            const GdkEvent *event);

GtkWidgetPath *   _gtk_widget_create_path                  (GtkWidget    *widget);
void              gtk_widget_clear_path                    (GtkWidget    *widget);
void              _gtk_widget_style_context_invalidated    (GtkWidget    *widget);

void              _gtk_widget_update_parent_muxer          (GtkWidget    *widget);
GtkActionMuxer *  _gtk_widget_get_action_muxer             (GtkWidget    *widget,
                                                            gboolean      create);

void              _gtk_widget_add_controller               (GtkWidget           *widget,
                                                            GtkEventController  *controller);
void              _gtk_widget_remove_controller            (GtkWidget           *widget,
                                                            GtkEventController  *controller);
GList *           _gtk_widget_list_controllers             (GtkWidget           *widget,
                                                            GtkPropagationPhase  phase);
gboolean          _gtk_widget_consumes_motion              (GtkWidget           *widget,
                                                            GdkEventSequence    *sequence);

gboolean          gtk_widget_has_tick_callback             (GtkWidget *widget);

void              gtk_widget_set_csd_input_shape           (GtkWidget            *widget,
                                                            const cairo_region_t *region);

gboolean          gtk_widget_has_size_request              (GtkWidget *widget);

void              gtk_widget_reset_controllers             (GtkWidget *widget);

gboolean          gtk_widget_query_tooltip                 (GtkWidget  *widget,
                                                            gint        x,
                                                            gint        y,
                                                            gboolean    keyboard_mode,
                                                            GtkTooltip *tooltip);

void              gtk_widget_render                        (GtkWidget            *widget,
                                                            GdkWindow            *window,
                                                            const cairo_region_t *region);


void              gtk_widget_snapshot                      (GtkWidget            *widget,
                                                            GtkSnapshot          *snapshot);
void              gtk_widget_adjust_size_request           (GtkWidget      *widget,
                                                            GtkOrientation  orientation,
                                                            gint           *minimum_size,
                                                            gint           *natural_size);
void              gtk_widget_adjust_size_allocation        (GtkWidget         *widget,
                                                            GtkOrientation     orientation,
                                                            gint              *minimum_size,
                                                            gint              *natural_size,
                                                            gint              *allocated_pos,
                                                            gint              *allocated_size);
void              gtk_widget_adjust_baseline_request       (GtkWidget *widget,
                                                            gint      *minimum_baseline,
                                                            gint      *natural_baseline);

void              gtk_widget_forall                        (GtkWidget            *widget,
                                                            GtkCallback           callback,
                                                            gpointer              user_data);

GtkWidget        *gtk_widget_get_focus_child               (GtkWidget            *widget);

void              gtk_widget_focus_sort                    (GtkWidget        *widget,
                                                            GtkDirectionType  direction,
                                                            GPtrArray        *focus_order);
gboolean          gtk_widget_focus_move                    (GtkWidget        *widget,
                                                            GtkDirectionType  direction,
                                                            GPtrArray        *focus_order);
void              gtk_widget_get_outer_allocation          (GtkWidget        *widget,
                                                            GtkAllocation    *allocation);
void              gtk_widget_get_own_allocation            (GtkWidget        *widget,
                                                            GtkAllocation    *allocation);

void              gtk_widget_get_window_allocation         (GtkWidget *widget,
                                                            GtkAllocation *allocation);


GtkWidget *       gtk_widget_common_ancestor               (GtkWidget *widget_a,
                                                            GtkWidget *widget_b);

void              gtk_widget_set_pass_through              (GtkWidget *widget,
                                                            gboolean   pass_through);
gboolean          gtk_widget_get_pass_through              (GtkWidget *widget);

gboolean          gtk_widget_emit_event_signals            (GtkWidget      *widget,
                                                            const GdkEvent *event);

void              gtk_widget_init_legacy_controller        (GtkWidget *widget);

void              gtk_widget_get_origin_relative_to_parent (GtkWidget        *widget,
                                                            int              *origin_x,
                                                            int              *origin_y);




/* inline getters */

static inline GtkWidget *
_gtk_widget_get_parent (GtkWidget *widget)
{
  return widget->priv->parent;
}

static inline gboolean
_gtk_widget_get_visible (GtkWidget *widget)
{
  return widget->priv->visible;
}

static inline gboolean
_gtk_widget_get_child_visible (GtkWidget *widget)
{
  return widget->priv->child_visible;
}

static inline gboolean
_gtk_widget_get_mapped (GtkWidget *widget)
{
  return widget->priv->mapped;
}

static inline gboolean
_gtk_widget_is_drawable (GtkWidget *widget)
{
  return widget->priv->visible && widget->priv->mapped;
}

static inline gboolean
_gtk_widget_get_has_window (GtkWidget *widget)
{
  return !widget->priv->no_window;
}

static inline gboolean
_gtk_widget_get_realized (GtkWidget *widget)
{
  return widget->priv->realized;
}

static inline gboolean
_gtk_widget_is_toplevel (GtkWidget *widget)
{
  return widget->priv->toplevel;
}

static inline GtkStateFlags
_gtk_widget_get_state_flags (GtkWidget *widget)
{
  return widget->priv->state_flags;
}

extern GtkTextDirection gtk_default_direction;

static inline GtkTextDirection
_gtk_widget_get_direction (GtkWidget *widget)
{
  if (widget->priv->direction == GTK_TEXT_DIR_NONE)
    return gtk_default_direction;
  else
    return widget->priv->direction;
}

static inline GtkWidget *
_gtk_widget_get_toplevel (GtkWidget *widget)
{
  while (widget->priv->parent)
    widget = widget->priv->parent;

  return widget;
}

static inline GtkStyleContext *
_gtk_widget_get_style_context (GtkWidget *widget)
{
  if (G_LIKELY (widget->priv->context))
    return widget->priv->context;

  return gtk_widget_get_style_context (widget);
}

static inline gpointer
_gtk_widget_peek_request_cache (GtkWidget *widget)
{
  return &widget->priv->requests;
}

static inline GdkWindow *
_gtk_widget_get_window (GtkWidget *widget)
{
  return widget->priv->window;
}

static inline void
_gtk_widget_get_allocation (GtkWidget     *widget,
                            GtkAllocation *allocation)
{
  *allocation = widget->priv->allocation;
}

static inline GtkWidget *
_gtk_widget_get_prev_sibling (GtkWidget *widget)
{
  return widget->priv->prev_sibling;
}

static inline GtkWidget *
_gtk_widget_get_next_sibling (GtkWidget *widget)
{
  return widget->priv->next_sibling;
}

static inline GtkWidget *
_gtk_widget_get_first_child (GtkWidget *widget)
{
  return widget->priv->first_child;
}

static inline GtkWidget *
_gtk_widget_get_last_child (GtkWidget *widget)
{
  return widget->priv->last_child;
}

G_END_DECLS

#endif /* __GTK_WIDGET_PRIVATE_H__ */
