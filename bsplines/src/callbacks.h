#include <gnome.h>


gboolean
on_window_destroy_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_canvas_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

gboolean
on_canvas_configure_event              (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);

gboolean
on_canvas_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

void
on_clear_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_draw_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_clear_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_draw_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_window_destroy                (GtkObject       *object,
                                        gpointer         user_data);

void
on_ndraw_changed                       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_draw_poli_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_draw_points_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_draw_pc_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_recursivo_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
