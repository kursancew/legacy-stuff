#ifndef __GNOMESENSORS_H__
#define __GNOMESENSORS_H__
#include <gconf/gconf-client.h>
static void do_a_print(sensors_chip_name name);
static int count_sensors(void);
static const char *sprintf_chip_name(sensors_chip_name name);
void destroy_win(GtkWidget *widget, GtkWidget *data);
void update(const sensors_chip_name name);
void about_dialog(BonoboUIComponent *uic, gpointer data, const gchar *verbname);
void create_main_window(BonoboUIComponent *uic, gpointer data, const gchar *verbname);
void applet_rotate(int a);
void update_timeout(GtkWidget *widget, GtkWidget *data);
int sensors_get_label_and_valid(sensors_chip_name name, int feature, char **label, int *valid);
void error_win(char *str);
void panel_resize(PanelApplet *widget, int s, gpointer *data);
void set_prog_to_execute(GtkWidget *widget, GtkWidget *data);
void enable_exec_on_bad_sensor(GtkWidget *widget, gpointer *data);

GConfClient *main_conf;
const static char *cpath = "/apps/gsensors/";
int BEEP;
int chips_count = 0;
int do_sets, do_unknown;
sensors_chip_name the_chip;
static sensors_chip_name foo;
const static sensors_chip_name *chip;
double entry_vals[30];
char entry_labels[30][128];
char entry_names[30][128];
int total_entrys = 0;
static int win_visible;
static GtkWidget *values[30];
static int values_r[30];
static int this_r = 0;
static GtkWidget *applet_l[2];
int nothing2show;
int timeout;
int timer_id;
int error_stat;
double warning_level[30];
char warning_policy[30][32];
int warning_status[30];
char lpath[256];
static GtkDialog *win;
int timeout2;
GList *items;
typedef struct {
	PanelApplet *applet;
} MY_APPLET;
static MY_APPLET *my_applet;
const static char* markup= "<span foreground=\"#0000FF\" size=\"8000\" face=\"fixed\">%s</span>";
const static char* markup_bad= "<span foreground=\"#DDDD00\" background=\"#FF0000\" size=\"8000\" face=\"fixed\">%s</span>";
const static char *authors[] = {
	"Vinicius Kursancew <linux@vkcorp.org>",NULL
};
enum WARN_POLICY {
	WARN_POLICY_PERCENT,
	WARN_POLICY_ABS
};
GtkWidget *aptable;
GtkWidget *apframe;
GtkWidget *aplabel;
GtkWidget *sep1,*sep2;

int LOG;
int LOG_RUNNING;
FILE *LOG_DESCRIPTOR;
int LOG_LIMIT;
int LOG_ERR;

int NO_SENSORS;

char EXEC_W_ARGS[512];
int ENABLE_EXEC;

enum LOG_ERR {
	LOG_OK,
	LOG_ERR_CNOPEN,
	LOG_ERR_FWRITE,
	LOG_ERR_ENABLE
};

#endif
