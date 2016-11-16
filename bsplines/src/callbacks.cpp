#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include <vector>

int REC = 5;

bool dpoli = true;
bool dpts = true;
bool dbpts = true;
bool drec = false;

static GdkPixmap *pixmap = NULL;
class point {
public:
  double x,y;	
  point() { x = y = 0; }
  point(double px, double py): x(px), y(py) {}
};

std::vector<point> points;
std::vector<point> bpoints;

point midpoint(point& p1, point& p2)
{
  return point( (p1.x + p2.x)/2, (p1.y+p2.y)/2 );
}

void draw_curve(point& p0, point& p1, point& p2, point& p3, int rec, GtkWidget* widget)
{
  
  if(rec == 0) {
    GdkColor r;
    r.red = 0;
    r.blue = 0;
    r.green = 65000;
    GdkGC* g = gdk_gc_new(widget->window);
    gdk_gc_set_rgb_fg_color(g, &r);
    gdk_draw_line(widget->window, g, (int)p0.x, (int)p0.y, (int)p3.x, (int)p3.y);
  } else {
    point L0 = p0;
    point L1 = midpoint(p0,p1);
    point H = midpoint(p1,p2);
    point R2 = midpoint(p2,p3);
    point R3 = p3;
    point L2 = midpoint(L1,H);
    point R1 = midpoint(R2,H);
    point L3 = midpoint(L2,R1);
    point R0 = L3;
    draw_curve(L0, L1, L2, L3, rec-1, widget);
    draw_curve(R0, R1, R2, R3, rec-1, widget);
  }
}
void draw_curve2(point& p0, point& p1, point& p2, point& p3, int rec, GtkWidget* widget)
{
  double incr = 1.0/rec;
  GdkColor r;
  r.red = 60000;
  r.blue = 0;
  r.green = 30000;
  GdkGC* g = gdk_gc_new(widget->window);
  gdk_gc_set_rgb_fg_color(g, &r);
  point prev;
  for(double u = 0; u <= 1; u+= incr) {
    std::vector<point> q;
    q.push_back(p0);
    q.push_back(p1);
    q.push_back(p2);
    q.push_back(p3);
    for(int i = 1; i < 4; ++i) {
      for (int j = 0; j < 4-i; ++j) {
        q[j].x = (1-u)*q[j].x+u*q[j+1].x;
        q[j].y = (1-u)*q[j].y+u*q[j+1].y;
      }
    }
    if(u != 0.0) {
      gdk_draw_line(widget->window, g, (int)prev.x, (int)prev.y, (int)q[0].x, (int)q[0].y);
    }
    prev = q[0];
  }
}

static void draw_brush( GtkWidget *widget,
                        gdouble    x,
                        gdouble    y)
{
  GdkRectangle update_rect;
                                                                                                    
  update_rect.x = (int)(x - 1);
  update_rect.y = (int)(y - 1);
  update_rect.width = 3;
  update_rect.height = 3;
  gdk_draw_rectangle (pixmap,
                      widget->style->white_gc,
                      TRUE,
                      update_rect.x, update_rect.y,
                      update_rect.width, update_rect.height);
  gtk_widget_queue_draw_area (widget,
                      update_rect.x, update_rect.y,
                      update_rect.width, update_rect.height);
}


gboolean
on_canvas_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  if (event->button == 1 && pixmap != NULL) {
    points.push_back(point(event->x, event->y));
    int s = points.size()-1;
    printf("P%d(%02f, %02f)\n", s, points[s].x, points[s].y);
    draw_brush (widget, event->x, event->y);
  }
  return FALSE;
}


gboolean
on_canvas_configure_event              (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
  if (pixmap)
    g_object_unref (pixmap);
                                                                                                    
  pixmap = gdk_pixmap_new (widget->window,
                           widget->allocation.width,
                           widget->allocation.height,
                           -1);
  gdk_draw_rectangle (pixmap,
                      widget->style->black_gc,
                      TRUE,
                      0, 0,
                      widget->allocation.width,
                      widget->allocation.height);
                                                                                                    
  return TRUE;
}


gboolean
on_canvas_expose_event                 (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  gdk_draw_drawable (widget->window,
                     widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                     pixmap,
                     event->area.x, event->area.y,
                     event->area.x, event->area.y,
                     event->area.width, event->area.height);
  
  return FALSE;
}


gboolean
on_window_destroy_event                (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  exit(0);
  return FALSE;
}
void
on_window_destroy                (GtkObject       *object,
                                        gpointer         user_data)
{

  exit(0);
}


void
on_clear_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *widget = (GtkWidget*)button;
  GdkRectangle update_rect;
  update_rect.x = 0;
  update_rect.y = 0;
  update_rect.width = widget->allocation.width;
  update_rect.height = widget->allocation.height;
  gdk_draw_rectangle (pixmap,
                      widget->style->black_gc,
                      TRUE,
                      0, 0,
                      widget->allocation.width,
                      widget->allocation.height);
  gdk_window_invalidate_rect(widget->window, &update_rect, false);
  points.clear();
  printf("clear\n");

}

point
bar(point& a, point& b, double p1, double p2)
{
  return point(a.x*p1 + b.x * p2, a.y*p1 + b.y*p2 );
}

void
on_draw_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
  if(points.size() < 4) {
    return;
  }
  GtkWidget *widget = (GtkWidget*)button;
  char pname = 'a';
  point prev;
  gdk_draw_rectangle (widget->window,
                      widget->style->black_gc,
                      TRUE,
                      0, 0,
                      widget->allocation.width,
                      widget->allocation.height);
  if(dpts)
    for(int i = 0; i < points.size(); ++i)
      gdk_draw_rectangle(widget->window, widget->style->white_gc, TRUE, (int)points[i].x, (int)points[i].y, 2,2 );
      
  for(int i = 0; i < points.size()-2; ++i) {
    point p0, p1, p2;
    printf("P_%d(%02f,%02f) ", i, points[i].x, points[i].y);
    p0 = bar(points[i], points[i+1], 1.0/3, 2.0/3);
    printf("%c_0(%02f, %02f) ", pname, p0.x, p0.y);
    p2 = bar(points[i+1], points[i+2], 2.0/3, 1.0/3);
    p1 = bar(p0, p2, 0.5, 0.5);
    printf("%c_1(%02f, %02f) ", pname, p1.x, p1.y);
    printf("%c_2(%02f, %02f) ", pname, p2.x, p2.y);
    if(dpoli) {
      if(i != 0) {
        gdk_draw_line(widget->window, widget->style->white_gc, (int)prev.x, (int)prev.y, (int)p0.x, (int)p0.y);
        gdk_draw_line(widget->window, widget->style->white_gc, (int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y);
      }
      if(i != points.size()-3) {
        gdk_draw_line(widget->window, widget->style->white_gc, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
      }
    }
    prev = p2;

    GdkRectangle update_rect;
    GdkColor r;
    r.red = 65000;
    r.blue = 0;
    r.green = 0;
    GdkGC* g = gdk_gc_new(widget->window);
    gdk_gc_set_rgb_fg_color(g, &r);
    if(i != 0) {
      if(dbpts)
        gdk_draw_rectangle(widget->window, g, TRUE, (int)p0.x, (int)p0.y, 2,2 );
      bpoints.push_back(p0);
    }
    if(dbpts)
      gdk_draw_rectangle(widget->window, g, TRUE, (int)p1.x, (int)p1.y, 2,2 );
    bpoints.push_back(p1);
    if(i != points.size()-3) {
      if(dbpts)
        gdk_draw_rectangle(widget->window, g, TRUE, (int)p2.x, (int)p2.y, 2,2 );
      bpoints.push_back(p2);
    }

    printf("\n");
    ++pname;
  }
  if(drec)
  for(int i = 0; i < bpoints.size()-2; i+=3) {
    draw_curve(bpoints[i], bpoints[1+i], bpoints[2+i], bpoints[3+i], REC, widget);
  }
  for(int i = 0; i < bpoints.size()-2; i+=3) {
    draw_curve2(bpoints[i], bpoints[1+i], bpoints[2+i], bpoints[3+i], REC, widget);
  }
  bpoints.clear(); 
}


void
on_ndraw_changed                       (GtkEditable     *editable,
                                        gpointer         user_data)
{
  REC = (int)gtk_spin_button_get_value((GtkSpinButton*)editable);
}


void
on_draw_poli_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  dpoli = !dpoli;
}


void
on_draw_points_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  dbpts = !dbpts;
}


void
on_draw_pc_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  dpts = !dpts;
}

void
on_recursivo_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  drec = !drec;
}

