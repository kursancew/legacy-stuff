#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gnome.h>
#include <libgnomeui/gnome-messagebox.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sensors/sensors.h>
#include <sensors/error.h>
#include <sensors/chips.h>
#include <linux/sensors.h>
#include <panel-applet.h>
#include <gdk/gdk.h>
#include <panel-applet-gconf.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include "gnome-sensors.h"
#include <config.h>
#include <sys/types.h>
#include <sys/wait.h>

static int already_did;
static const BonoboUIVerb 
menu_verbs [] = { BONOBO_UI_VERB("AppletAbout", about_dialog), 
	BONOBO_UI_VERB("SensorsWin", create_main_window),
	BONOBO_UI_VERB_END };

static const char 
applet_menu_xml [] = 
	"<popup name=\"button3\">\n"
	"   <menuitem name=\"About Item\" verb=\"AppletAbout\" label=\"%s\"\n"
	"             pixtype=\"stock\" pixname=\"gnome-stock-about\"/>\n"
	"   <menuitem name=\"Sensors Item\" verb=\"SensorsWin\" label=\"%s\"\n"
	"             pixtype=\"stock\" pixname=\"gnome-stock-about\"/>\n"
	"</popup>\n";

int log_enter(char *string)
{
	char log_line[1024];
	char now_str[64];
	time_t now;
	struct tm *now_tm;
	char v[128];
	char *fcontents;
	struct stat st;
	if(LOG == FALSE) return 0;
	if(LOG_RUNNING == FALSE) return -1;
	now = time(NULL);
	//now_tm = malloc(sizeof(struct tm));
	now_tm = localtime(&now);
	sprintf(now_str,"%04d/%02d/%02d - %02d:%02d:%02d",
			now_tm->tm_year + 1900,
			now_tm->tm_mon + 1,
			now_tm->tm_mday,
			now_tm->tm_hour,
			now_tm->tm_min,
			now_tm->tm_sec);
	//free(now_tm);
	fseek(LOG_DESCRIPTOR,0,SEEK_SET);
	sprintf(log_line,"%s -> %s\n",now_str, string);
	stat(lpath,&st);
	fcontents = (char*)malloc(st.st_size);
	fseek(LOG_DESCRIPTOR,0,SEEK_SET);
	fread(fcontents, st.st_size, 1, LOG_DESCRIPTOR);
	fflush(LOG_DESCRIPTOR);
	truncate(lpath,0);
	if(fwrite(log_line, strlen(log_line), 1, LOG_DESCRIPTOR) != 1)
	{
		free(fcontents);
		LOG_ERR = LOG_ERR_FWRITE;
		sprintf(v,"Error while writing to %s %s",lpath, strerror(errno));
		error_win(v);
		return -1;
	}

	fseek(LOG_DESCRIPTOR,0,SEEK_END);
	
	if(fwrite(fcontents, st.st_size, 1, LOG_DESCRIPTOR) != 1 && st.st_size > 0) {
		free(fcontents);
		LOG_ERR = LOG_ERR_FWRITE;
		sprintf(v,"Error while writing to %s %s",lpath, strerror(errno));
		error_win(v);
		return -1;
	}
	free(fcontents);
	if(LOG_LIMIT > 0 && st.st_size > LOG_LIMIT * 1024) {
		fflush(LOG_DESCRIPTOR);
		truncate(lpath, LOG_LIMIT * 1024);
	}
	if(st.st_size > 10 * 1024 *1024) {
		fflush(LOG_DESCRIPTOR);
		truncate(lpath, 10 * 1024 * 1024);
	}
}

int log_start_service(char *file)
{
	char v[128];
	if (LOG_RUNNING == TRUE) return 0;
	if(LOG == FALSE) {
		LOG_ERR = LOG_ERR_ENABLE;
		error_win("Logging not enabled");
		LOG = FALSE;
		return -1;
	}
	
	LOG_DESCRIPTOR = fopen(file,"a+");
	if(LOG_DESCRIPTOR == NULL) {
		LOG_ERR = LOG_ERR_CNOPEN;
		sprintf(v,"Error while opening file %s %s",file, strerror(errno));
		error_win(v);
		LOG = FALSE;
		return(-1);
	} else LOG_ERR = LOG_OK;
	if(access(file,W_OK) != 0) {
		error_win("You don't have write access to the log.");
		LOG = FALSE;
		return -1;
	}
	fseek(LOG_DESCRIPTOR, 0, SEEK_SET);
	LOG_RUNNING = TRUE;
	if(log_enter("Logging Started") < 0) {
		LOG_ERR = LOG_ERR_FWRITE;
		LOG = FALSE;
		return -1;
	}
	return 0;
}

void log_end_service()
{
	if(LOG_RUNNING == FALSE) return;
	fclose(LOG_DESCRIPTOR);
	LOG = FALSE;
	LOG_RUNNING = FALSE;
}

void panel_resize(PanelApplet *wid, int s, gpointer *data) {
	if(s >= 32){
		gtk_container_remove(GTK_CONTAINER(wid), aptable);
		gtk_widget_destroy(sep1);
		gtk_widget_destroy(sep2);
		gtk_widget_destroy(applet_l[0]);
		gtk_widget_destroy(applet_l[1]);
		gtk_widget_destroy(aptable);
		aptable = gtk_table_new(2,1,FALSE);
		sep1 = gtk_button_new();
		sep2 = gtk_button_new();
		gtk_widget_set_usize(sep1,7,s);
		gtk_widget_set_usize(sep2,7,s);

		applet_l[0] = gtk_label_new("");
		applet_l[1] = gtk_label_new("");
		gtk_label_set_use_markup (GTK_LABEL(applet_l[0]), TRUE);
		gtk_label_set_use_markup (GTK_LABEL(applet_l[1]), TRUE);
	
		gtk_table_attach(GTK_TABLE(aptable), applet_l[0], 0,1,0,1, 
				GTK_EXPAND|GTK_FILL,0,0,0);
		gtk_table_attach(GTK_TABLE(aptable), applet_l[1], 0,1,1,2, 
				GTK_EXPAND|GTK_FILL,0,0,0);

		gtk_container_add(GTK_CONTAINER(wid), aptable);
		gtk_widget_set_usize(aptable, 61, s);
		gtk_widget_show_all(GTK_WIDGET (wid));
	} else if(s < 32) {
		gtk_container_remove(GTK_CONTAINER(wid), aptable);
		gtk_widget_destroy(sep1);
		gtk_widget_destroy(sep2);
		gtk_widget_destroy(applet_l[0]);
		gtk_widget_destroy(applet_l[1]);
		gtk_widget_destroy(aptable);
		aptable = gtk_table_new(1,4,FALSE);
		sep1 = gtk_button_new();
		sep2 = gtk_button_new();
		gtk_widget_set_usize(sep1,7,s);
		gtk_widget_set_usize(sep2,7,s);
		
		applet_l[0] = gtk_label_new("");
		gtk_label_set_use_markup (GTK_LABEL(applet_l[0]), TRUE);
		applet_l[1] = gtk_label_new("");
		gtk_label_set_use_markup (GTK_LABEL(applet_l[1]), TRUE);
	
		gtk_table_attach(GTK_TABLE(aptable), sep1, 0,1,0,1,
				0,GTK_FILL,0,0);
		gtk_table_attach(GTK_TABLE(aptable), applet_l[0], 1,2,0,1, 
				GTK_EXPAND|GTK_FILL,0,0,0);
		gtk_table_attach(GTK_TABLE(aptable), applet_l[1], 2,3,0,1, 
				GTK_EXPAND|GTK_FILL,0,0,0);
		gtk_table_attach(GTK_TABLE(aptable), sep2, 3,4,0,1, 
				0,GTK_FILL,0,0);
		gtk_container_add(GTK_CONTAINER(my_applet->applet), aptable);
		gtk_widget_set_usize(aptable, 100, 24);
		gtk_widget_show_all(GTK_WIDGET (wid));
	}
}

void update(const sensors_chip_name name)
{
	int a,b,valid,i;
	const sensors_feature_data *data;
	char *label;
	double val;
 
	total_entrys = 0;
	a=b=i=0;
	while((data=sensors_get_all_features(name,&a,&b))) {
		if (sensors_get_label_and_valid(name,data->number,&label,&valid)) {
			//fprintf(stderr,"ERROR: Can't get feature `%s' data!",data->name);
			if (label != 0) { free(label); label = 0; }
			continue;
		}
		if (!valid){
			if (label != 0) { free(label); label = 0; }
			continue;
		}
		if (data->mode & SENSORS_MODE_R) {
			if(sensors_get_feature(name,data->number,&val)) {
				//fprintf(stderr,"ERROR: Can't get feature `%s' data!",data->name);
				if (label != 0) { free(label); label = 0; }
				continue;
			}
			if(strcmp(label, data->name) != 0) {
				entry_vals[total_entrys] = val;
				sprintf(entry_labels[total_entrys],label);
				sprintf(entry_names[total_entrys], data->name);
				total_entrys++;
			}
		}
		if (label != 0) { free(label); label = 0; }
	}
}

#define HYST 0
#define MINMAX 1
int sensors_get_label_and_valid(sensors_chip_name name, int feature, char **label,
                        int *valid)
{
  int err;
  err = sensors_get_label(name,feature,label);
  if (!err)
    err = sensors_get_ignored(name,feature);
  if (err >= 0) {
    *valid = err;
    err = 0;
  }
  return err;
}

const char *sprintf_chip_name(sensors_chip_name name)
{
  #define BUF_SIZE 200
  static char buf[BUF_SIZE];

  if (name.bus == SENSORS_CHIP_NAME_BUS_ISA)
    snprintf(buf,BUF_SIZE,"%s-isa-%04x",name.prefix,name.addr);
  else
    snprintf(buf,BUF_SIZE,"%s-i2c-%d-%02x",name.prefix,name.bus,name.addr);
  return buf;
}

void do_a_print(sensors_chip_name name)
{
	foo = name;
	update(name);
}
 
int get_sensor(void)
{
	int chip_nr,i;
	int cnt = 0;
	for (chip_nr = 0; chip = sensors_get_detected_chips(&chip_nr);) {
		for(i = 0; i < chips_count; i++) {
			if (sensors_match_chip(*chip,the_chip)) {
				do_a_print(*chip);
				i = chips_count;
				cnt++;
				break;
			}
		}
	}
	chip = (sensors_chip_name*) malloc(1);
	if (cnt == 0) NO_SENSORS = TRUE;
	return(cnt);
}

void update_fields(GtkWidget *widget, GtkWidget *e[30]) {
	/*int i;
	update(&foo);
	for (i = 0; i < total_entrys; ++i) {
		char v[16];
		if(warning_status[i] == TRUE) {
			sprintf(v, "= %.2f =", entry_vals[i]);
		} else 
			sprintf(v, "%.2f", entry_vals[i]);
		
		gtk_entry_set_text(GTK_ENTRY(e[i]),v);
		if(warning_status[i] == TRUE) 
			gtk_entry_select_region (GTK_ENTRY(e[i]), 0, strlen(v));
		else	
			gtk_entry_select_region (GTK_ENTRY(e[i]), 0, 0);
	}*/
	
}

void update_fieldst(GtkWidget *e[30]) {
	//update_fields(0,e);
}

static char nums[30][4];
void check_display_in_applet(GtkWidget *widget, char *c)
{
	int i,i2 = 0;
	char v[32];
	i = atoi(c);
	values_r[i] = GTK_CHECK_BUTTON(widget)->toggle_button.active;
	sprintf(v,"%sshow_sensor/%02d",cpath,i);
	gconf_client_set_int(main_conf, v, values_r[i], NULL);
	for(i = 0; i < total_entrys; ++i) 
		if (values_r[i] == TRUE) {
			nothing2show = FALSE;
			return;
		}
	nothing2show = TRUE;
		
}
void check_logging(GtkWidget *widget, GtkWidget *data)
{
	char v[64];
	LOG = GTK_CHECK_BUTTON(widget)->toggle_button.active;
	if (LOG == TRUE) {
		sprintf(lpath, gtk_entry_get_text(GTK_ENTRY(data)));
		if(log_start_service(lpath) < 0) {
			LOG = FALSE;
			GTK_CHECK_BUTTON(widget)->toggle_button.active = LOG;
		}
	} else {
		log_end_service();
		sprintf(lpath, gtk_entry_get_text(GTK_ENTRY(data)));
	}

	gtk_entry_set_editable(GTK_ENTRY(data), !LOG);
	sprintf(v,"%slogging/path",cpath);
	gconf_client_set_string(main_conf, v, lpath, NULL);

	sprintf(v,"%slogging/active",cpath);
	gconf_client_set_bool(main_conf, v, LOG, NULL);
}

void error_win(char *str) {
	GtkDialog *win;
	GtkWidget *lab;

	win = GTK_DIALOG(gtk_dialog_new_with_buttons(_("Error"), 
			NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT, NULL));
	lab = gtk_label_new(str);
	gtk_container_add(GTK_CONTAINER(win->vbox), lab);
	gtk_widget_show(lab);
	gtk_dialog_run(win);
	gtk_widget_destroy(GTK_WIDGET(win));
}

void change_log_path(GtkWidget *widget, GtkWidget *data)
{
	char v[64];
	sprintf(lpath, gtk_entry_get_text(GTK_ENTRY(widget)));
	sprintf(v,"%slogging/path",cpath);
	gconf_client_set_string(main_conf, v, lpath, NULL);
}

void check_beeper(GtkWidget *widget, gpointer *data)
{
	char v[64];
	BEEP = GTK_CHECK_BUTTON(widget)->toggle_button.active;
	sprintf(v,"%sbeep",cpath);
	gconf_client_set_bool(main_conf, v, BEEP, NULL);
}

void update_log_limit(GtkWidget *widget, gpointer *data)
{
	char v[64];
	LOG_LIMIT = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
	sprintf(v,"%slogging/limit",cpath);
	gconf_client_set_int(main_conf, v, LOG_LIMIT, NULL);
}

void update_timeout(GtkWidget *widget, GtkWidget *data)
{
	char v[64];
	timeout = gtk_spin_button_get_value(GTK_SPIN_BUTTON(data));
	if(timeout < 500) timeout = 500;
	if(timeout > 100000) timeout = 100000;
	gtk_timeout_remove(timer_id);
	timer_id = gtk_timeout_add(timeout, (GtkFunction)applet_rotate, 0);
	
	sprintf(v,"%srefresh",cpath);
	gconf_client_set_int(main_conf, v, timeout, NULL);
}

void update_levels(GtkWidget *widget, char *c)
{
	int i,i2 = 0;
	char v[32];
	i = atoi(c);
	warning_level[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
	sprintf(v,"%swarning_level/%02d",cpath,i);
	gconf_client_set_float(main_conf, v, warning_level[i], NULL);
}

static GtkWidget *warns_unit[30];
void update_policys(GtkWidget *widget, char *c)
{
	int i,i2 = 0;
	char v[32];
	i = atoi(c);
	sprintf(warning_policy[i], GTK_ENTRY (GTK_COMBO(warns_unit[i])->entry)->text );
	sprintf(v,"%swarning_policy/%02d",cpath,i);
	gconf_client_set_string(main_conf, v, warning_policy[i], NULL);
}
void remove_timeout2(GtkWidget *widget,gpointer *data)
{
	gtk_timeout_remove(timeout2);
}

void set_prog_to_execute(GtkWidget *widget, GtkWidget *data)
{
	char v[64];
	sprintf(EXEC_W_ARGS, gtk_entry_get_text(GTK_ENTRY(data)));
	sprintf(v,"%sexecpath",cpath);
	gconf_client_set_string(main_conf, v, EXEC_W_ARGS, NULL);
}

void enable_exec_on_bad_sensor(GtkWidget *widget, gpointer *data)
{
	char v[64];
	ENABLE_EXEC = GTK_CHECK_BUTTON(widget)->toggle_button.active;
	sprintf(v,"%senableexec",cpath);
	gconf_client_set_bool(main_conf, v, ENABLE_EXEC, NULL);
}

void create_main_window(BonoboUIComponent *uic, gpointer data, const gchar *verbname) {
	GtkWidget *labels[30];
	GtkWidget *chex[30];
	
	GtkWidget *scrolled;
	GtkWidget *table;
	
	GtkWidget *label_page1;
	GtkWidget *label_page2;
	GtkWidget *book;
	GtkWidget *spin_delay;
	GtkWidget *button_apply_delay;
	GtkWidget *label_delay;
	
	GtkWidget *table_config;
	GtkWidget *table_config2;
	GtkWidget *check_enable_warn;
	GtkWidget *warns[30];
	GtkWidget *warns_labels[30];
	GtkWidget *scrolled_conf;
	GtkWidget *check_log;
	GtkWidget *log_path;
	GtkWidget *log_limit;
	GtkWidget *log_limit_label;
	GtkWidget *check_beep;
	GtkWidget *progexec;
	GtkWidget *checkexec;
	GtkWidget *button_apply_prog;
	int i;
	char v[64];

	if (win)
	{
		gtk_window_present(GTK_WINDOW(win));
		return;
	}
	win = GTK_DIALOG(gtk_dialog_new_with_buttons(_("gsensors"), 
			NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT, NULL));
	
	label_delay = gtk_label_new("Update delay(milisseconds)");
	spin_delay = gtk_spin_button_new_with_range(500,10000, 100);
	button_apply_delay = gtk_button_new_with_label("Set");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_delay), timeout);
	table = gtk_table_new(4, total_entrys, FALSE);
	
	label_page1 = gtk_label_new(sprintf_chip_name(foo));
	label_page2 = gtk_label_new("Settings");
	book = gtk_notebook_new();
	
	scrolled = gtk_scrolled_window_new(NULL,NULL);
	
	table_config = gtk_table_new(3,6,FALSE);
	table_config2 = gtk_table_new(4,30,FALSE);
	scrolled_conf = gtk_scrolled_window_new(NULL,NULL);

	check_log = gtk_check_button_new_with_label("Enable log");
	check_beep = gtk_check_button_new_with_label("Enable BEEP");
	log_path = gtk_entry_new();
	
	log_limit = gtk_spin_button_new_with_range(0, 100000, 1);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(log_limit), LOG_LIMIT);
	
	log_limit_label = gtk_label_new("Trim log file to KBytes (0 = unlimited)");
	gtk_entry_set_text(GTK_ENTRY(log_path), lpath);

	checkexec = gtk_check_button_new_with_label("Exec program in case a sensor goes bad");
	progexec = gtk_entry_new();
	button_apply_prog = gtk_button_new_with_label("Set");
	gtk_entry_set_text(GTK_ENTRY(progexec), EXEC_W_ARGS);
	
	for (i = 0; i < total_entrys; ++i) {
		char v[128];
		labels[i] = gtk_label_new(entry_labels[i]);
		values[i] = gtk_entry_new();
		chex[i] = gtk_check_button_new_with_label("Show in applet");
		GTK_CHECK_BUTTON(chex[i])->toggle_button.active = values_r[i];
		
		warns[i] = gtk_spin_button_new_with_range(-100000.00,100000.00,0.01);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(warns[i]), warning_level[i]);
		gtk_widget_set_usize(warns[i],70,-1);
		
		sprintf(v, "Warning level(%s):", entry_labels[i]);
		warns_labels[i] = gtk_label_new(v);
		warns_unit[i] = gtk_combo_new();
		gtk_combo_set_value_in_list(GTK_COMBO(warns_unit[i]), TRUE, FALSE);
		gtk_combo_set_popdown_strings (GTK_COMBO (warns_unit[i]), items);
		gtk_widget_set_usize(warns_unit[i],100,-1);
		gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(warns_unit[i])->entry), warning_policy[i]);
		
		sprintf(v, "%.2f", entry_vals[i]);
		gtk_entry_set_editable(GTK_ENTRY(values[i]),FALSE);
		gtk_entry_set_position(GTK_ENTRY(values[i]),2);
		gtk_entry_set_text(GTK_ENTRY(values[i]),v);
		gtk_widget_set_usize(values[i],70, -1);
		gtk_widget_set_usize(labels[i],100, -1);
		gtk_table_set_row_spacings(GTK_TABLE(table), 5);
		
		gtk_table_attach(GTK_TABLE(table), values[i], 1, 2, i, i+1, GTK_EXPAND, 0, 0,0);
		gtk_table_attach(GTK_TABLE(table), labels[i], 0, 1, i, i+1, GTK_EXPAND, 0, 0,0);
		gtk_table_attach(GTK_TABLE(table), chex[i], 2, 3, i, i+1, GTK_EXPAND, 0, 0,0);
		
		gtk_table_attach(GTK_TABLE(table_config2), warns_labels[i], 0, 1, i, i+1, GTK_EXPAND, 0, 0,0);
		gtk_table_attach(GTK_TABLE(table_config2), warns[i], 1, 2, i, i+1, GTK_EXPAND, 0, 0,0);
		gtk_table_attach(GTK_TABLE(table_config2), warns_unit[i], 2, 3, i, i+1, GTK_EXPAND, 0, 0,0);

		sprintf(nums[i], "%d",i);
		g_signal_connect(GTK_OBJECT(chex[i]), "toggled",
				(GtkSignalFunc)check_display_in_applet, nums[i]);
		g_signal_connect(GTK_OBJECT(warns[i]), "value_changed", 
				(GtkSignalFunc)update_levels, nums[i]);
		g_signal_connect(GTK_OBJECT(GTK_COMBO(warns_unit[i])->list), "selection-changed", 
				(GtkSignalFunc)update_policys, nums[i]);
		
	}


	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled), table);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_conf), table_config2);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_conf),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_widget_set_usize(scrolled, 350, 400);
	gtk_widget_set_usize(scrolled_conf, 350, 400);

	gtk_table_attach(GTK_TABLE(table_config), label_delay, 0, 1, 
			0, 1, 0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table_config), spin_delay, 1, 2, 
			0, 1, 0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table_config), button_apply_delay, 2, 3, 
			0, 1, 0, 0, 0,0);
	
	gtk_table_attach(GTK_TABLE(table_config), check_log, 0, 1, 
			1, 2, 0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table_config), log_path, 1, 3, 
			1, 2, 0, 0, 0,0);
	
	gtk_table_attach(GTK_TABLE(table_config), check_beep, 0, 1, 
			2, 3, 0, 0, 0,0);
	
	gtk_table_attach(GTK_TABLE(table_config), log_limit_label, 0, 1, 
			3, 4, 0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table_config), log_limit, 1, 3, 
			3, 4, 0, 0, 0,0);
	
	gtk_table_attach(GTK_TABLE(table_config), scrolled_conf, 0, 3, 
			4, 5, 0, 0,0,0);
	gtk_table_attach(GTK_TABLE(table_config), checkexec, 0, 1, 
			5, 6, 0, 0,0,0);
	gtk_table_attach(GTK_TABLE(table_config), progexec, 1, 2, 
			5, 6, 0, 0,0,0);
	gtk_table_attach(GTK_TABLE(table_config), button_apply_prog, 2, 3, 
			5, 6, 0, 0,0,0);
	
	gtk_notebook_append_page(GTK_NOTEBOOK(book), scrolled,label_page1);
	gtk_notebook_append_page(GTK_NOTEBOOK(book), table_config,label_page2);
	
	gtk_widget_show_all(book);
	gtk_container_add(GTK_CONTAINER(win->vbox), book);
	
	g_signal_connect(GTK_OBJECT(button_apply_delay), "clicked", 
			(GtkSignalFunc)update_timeout, spin_delay);
	g_signal_connect(GTK_OBJECT(log_limit), "value-changed", 
			(GtkSignalFunc)update_log_limit, NULL);

	g_signal_connect(GTK_OBJECT(log_path), "activate",
			(GtkSignalFunc)change_log_path, NULL);

	g_signal_connect(GTK_OBJECT(check_log), "toggled", 
			(GtkSignalFunc)check_logging, log_path);
	g_signal_connect(GTK_OBJECT(check_beep), "toggled", 
			(GtkSignalFunc)check_beeper, NULL);
	g_signal_connect(GTK_OBJECT(button_apply_prog), "clicked", 
			(GtkSignalFunc)set_prog_to_execute, progexec);
	g_signal_connect(GTK_OBJECT(checkexec), "toggled", 
			(GtkSignalFunc)enable_exec_on_bad_sensor, NULL);
	
	GTK_CHECK_BUTTON(check_log)->toggle_button.active = LOG;
	GTK_CHECK_BUTTON(checkexec)->toggle_button.active = ENABLE_EXEC;
	GTK_CHECK_BUTTON(check_beep)->toggle_button.active = BEEP;
	gtk_entry_set_editable(GTK_ENTRY(log_path), !LOG);

	timeout2 = gtk_timeout_add(1000, (GtkFunction)update_fieldst, values);
	g_signal_connect(GTK_OBJECT(win),"response",(GtkSignalFunc)remove_timeout2,0);
	
	gtk_dialog_run(win);
	gtk_widget_destroy(GTK_WIDGET(win));
	win = NULL;
	return;
}

void destroy_win(GtkWidget *widget, GtkWidget *data) {
	win_visible = FALSE;
}

void about_dialog(BonoboUIComponent *uic, gpointer data, const gchar *verbname) {
	GtkDialog *a;
	
	a = GTK_DIALOG(gnome_about_new (_("GSensors"), VERSION, "Copyright 2003 Vinicius Kursancew",
	_("This applet displays the status of your hardware sensors"),
	(const char**)authors, NULL, NULL, NULL));
	
	gtk_widget_show(GTK_WIDGET(a));
}

static int pee=0;
void applet_rotate(int a)
{
	char buf[256];
	char buf2[32];
	int i;
	char logging_str[256];
	if(NO_SENSORS || total_entrys == 0) {
		sprintf(buf, markup_bad, "NO SEONSOR");
		gtk_label_set_markup(GTK_LABEL(applet_l[0]),buf);
		sprintf(buf, markup_bad, "FOUND");
		gtk_label_set_markup(GTK_LABEL(applet_l[1]),buf);
		return;
	}
	update(foo);

	for (i = 0; i < total_entrys; ++i)
	{
		if(strstr(warning_policy[i],"No Warn") != NULL || warning_level[i] == 0) {
			warning_status[i] = FALSE;
			continue;
		}
		if(strstr(warning_policy[i],"/100") != NULL) {
			static double max,min;
			static double mult1,mult2;
			double tmp;
			mult1 = 1 + (atof(warning_policy[i] + 4)/100);
			mult2 = 1 - (atof(warning_policy[i] + 4)/100);
			
			max = warning_level[i] * mult1;
			min = warning_level[i] * mult2;
			
			if(
			((entry_vals[i] < min || entry_vals[i] > max) && entry_vals[i] > 0) ||
			((entry_vals[i] > min || entry_vals[i] < max) && entry_vals[i] < 0)
					){
				warning_status[i] = TRUE;
				if(BEEP)gdk_beep();
				sprintf(logging_str, "Sensor %s(%.2f) is out of specified %s range \
						(Level %.2f   Min: %.2f  Max %.2f)",
						entry_labels[i], entry_vals[i], warning_policy[i], 
						warning_level[i], min, max);
				log_enter(logging_str);
			} else warning_status[i] = FALSE;
			
		} else if (strstr(warning_policy[i],"Min Threshold") != NULL) {
			if(entry_vals[i] > warning_level[i]){
				warning_status[i] = FALSE;
			} else {
				warning_status[i] = TRUE;
				if(BEEP)gdk_beep();
				sprintf(logging_str, "Sensor %s(%.2f) is under Min Threshold %.2f",
						entry_labels[i], entry_vals[i], warning_level[i]);
				log_enter(logging_str);
			}
			
		} else if (strstr(warning_policy[i],"Max Threshold") != NULL) {
			if(entry_vals[i] < warning_level[i]){
				warning_status[i] = FALSE;
			} else {
				warning_status[i] = TRUE;
				if(BEEP)gdk_beep();
				sprintf(logging_str, "Sensor %s(%.2f) is over Max Threshold %.2f",
						entry_labels[i], entry_vals[i], warning_level[i]);
				log_enter(logging_str);
			}
			
		} else if (strstr(warning_policy[i],"Absolute") != NULL) {
			if(entry_vals[i] != warning_level[i]){
				warning_status[i] = TRUE;
				if(BEEP)gdk_beep();
				sprintf(logging_str, "Sensor %s(%.2f) is not Absolute %.2f",
						entry_labels[i], entry_vals[i], warning_level[i]);
				log_enter(logging_str);
			} else warning_status[i] = FALSE;
		}
		
		if (warning_status[i] == TRUE && ENABLE_EXEC == TRUE) {
			static int frk;
			if(waitpid(frk,0,WNOHANG)) {
				frk = fork();
			}
			if(frk == 0) {
				char **separated;
				char *tok;
				char tbuf[512];
				int tct = 0,bleh=0;

				sprintf(tbuf,EXEC_W_ARGS);
				for(tok = strtok(tbuf," ");tok;tok=strtok(0," ")) {
					++tct;
				}
				
				separated = (char**)malloc(tct*64);
				
				for(bleh = 0;bleh < tct;bleh++) {
					//separated[bleh] = (char*)malloc(64);
				}
				
				separated[tct] = NULL;
				bleh = 0;
				sprintf(tbuf,EXEC_W_ARGS);
				for(tok = strtok(tbuf," ");tok;tok=strtok(0," ")) {
					sprintf(separated[bleh],tok);
					++bleh;
				}
				execv(separated[0],separated);
				free(separated);
				//for(bleh = 0; bleh < tct; bleh++)
					//free(separated[bleh]);
				_exit(-1);
			}
			
		}
		
	}

	if(nothing2show == TRUE) {
		const static char *steps="|/-\\";
		char str[4];
		str[0] = steps[pee];
		str[1] = '\0';
		++pee;

		if(pee == strlen(steps)) pee = 0;
		gtk_label_set_markup(GTK_LABEL(applet_l[0]),str);
		gtk_label_set_markup(GTK_LABEL(applet_l[1]),"");
	} else {
foobar:
		if(values_r[this_r] == TRUE) {
			char teste[10];
			//char *buff;
			//buff = (char*)malloc(128);

			if(warning_status[this_r] == TRUE) 
				sprintf(buf,markup_bad,entry_labels[this_r]);
			else sprintf(buf,markup,entry_labels[this_r]);
			
			gtk_label_set_markup(GTK_LABEL(applet_l[0]),buf);
			//gtk_label_set_text(GTK_LABEL(applet_l[0]),entry_labels[this_r]);
	
			sprintf(buf2,"%.2f",entry_vals[this_r]);
			//sprintf(teste,"%.2f",entry_vals[this_r]);
			
			sprintf(buf,markup,buf2);
			
			gtk_label_set_markup(GTK_LABEL(applet_l[1]),buf);
			//gtk_label_set_text(GTK_LABEL(applet_l[1]),teste);

		} else {
			this_r++;
			if(this_r >= total_entrys) this_r = 0;
			goto foobar;
		}
	}
	this_r++;
	if(this_r >= total_entrys) this_r = 0;

}

static void
applet_destroy(PanelApplet *applet_widget, MY_APPLET *applet)
{
	gtk_timeout_remove(timer_id);
	gtk_timeout_remove(timeout2);
	log_end_service();
	g_free(applet);
	gconf_client_suggest_sync(main_conf,NULL);
	return;
}

int gsensors_applet_start (PanelApplet *applet, const gchar *iid, gpointer data)
{
	int res,i;
	char *buf;
	FILE *fd;
	BEEP = TRUE;
	NO_SENSORS = FALSE;
	
	error_stat = FALSE;
	
	if (strcmp (iid, "OAFIID:GSensors") != 0)
		return FALSE;

	win_visible = FALSE;
	fd = fopen("/etc/sensors.conf", "r");
	if (fd == NULL) {
		fprintf(stderr, "Could not open config file trying ~/sensors.conf\n");
		fd = fopen("~/sensors.conf", "r");
		if(fd == NULL) {
			fprintf(stderr,"also failed\n");
			exit(1);
		}
	}
	the_chip.prefix = SENSORS_CHIP_NAME_PREFIX_ANY;
	the_chip.bus = SENSORS_CHIP_NAME_BUS_ANY;
	the_chip.addr = SENSORS_CHIP_NAME_ADDR_ANY;
	chips_count = 1;

	if ((res = sensors_init(fd))) {
		fprintf(stderr,"%s\n",sensors_strerror(res));
		if (res == -SENSORS_ERR_PROC)
		fprintf(stderr,
			"/proc/sys/dev/sensors/chips or /proc/bus/i2c unreadable;\n"
			"Make sure you have done 'modprobe i2c-proc'!\n");
		exit(1);
	}
	for (i = 0; i < 30; ++i) 
		warning_status[i] = FALSE;
	get_sensor();
	my_applet = g_malloc0(sizeof(MY_APPLET));
	my_applet->applet = applet;

	main_conf = gconf_client_get_default();

	if (gconf_client_get_bool(main_conf, "/apps/gsensors/hasconf", NULL) == TRUE ) 
	{
		for (i = 0; i < 30; ++i) {
			char v[64];
			char *str;
			
			sprintf(v,"%srefresh",cpath);
			timeout = gconf_client_get_int(main_conf, v, NULL);
			sprintf(v,"%sbeep",cpath);
			BEEP = gconf_client_get_bool(main_conf, v, NULL);
			sprintf(v,"%slogging/active",cpath);
			LOG = gconf_client_get_bool(main_conf, v, NULL);
			sprintf(v,"%slogging/limit",cpath);
			LOG_LIMIT = gconf_client_get_int(main_conf, v, NULL);
			
			sprintf(v,"%sshow_sensor/%02d",cpath,i);
			values_r[i] = gconf_client_get_int(main_conf, v, NULL);
			
			sprintf(v,"%swarning_level/%02d",cpath,i);
			warning_level[i] = gconf_client_get_float(main_conf, v, NULL);
			
			sprintf(v,"%swarning_policy/%02d",cpath,i);
			str = gconf_client_get_string(main_conf, v, NULL);
			if(str != NULL) {
				sprintf(warning_policy[i], str);
				free(str);
			} else {
				sprintf(warning_policy[i],"No Warn");
			}

			sprintf(v,"%slogging/path",cpath);
			str = gconf_client_get_string(main_conf, v, NULL);
			if(str != NULL) {
				sprintf(lpath, str);
				free(str);
			} else {
				sprintf(v, "%s%s",getenv("HOME"),"/gsensors.log");
				sprintf(lpath,v);
			}
			sprintf(v,"%senableexec",cpath);
			ENABLE_EXEC = gconf_client_get_bool(main_conf, v, NULL);
			
			sprintf(v,"%sexecpath",cpath);
			str = gconf_client_get_string(main_conf, v, NULL);
			if(str != NULL) {
				sprintf(EXEC_W_ARGS, str);
				free(str);
			} else {
				sprintf(v, " ");
				sprintf(EXEC_W_ARGS,v);
			}
			
		}
	} else {
		for (i = 0; i < 30; ++i) {
			char v[64];
			char path[128];
			values_r[i] = 0;
			warning_level[i] = 0;
			sprintf(warning_policy[i], "No Warn");
			timeout = 1000;
			LOG = FALSE;
			LOG_LIMIT = 0;
			BEEP = TRUE;
			ENABLE_EXEC = FALSE;
			
			sprintf(v,"%srefresh",cpath);
			gconf_client_set_int(main_conf, v, timeout, NULL);
			sprintf(v,"%sbeep",cpath);
			gconf_client_set_bool(main_conf, v, BEEP,NULL);

			sprintf(v,"%sshow_sensor/%02d",cpath,i);
			gconf_client_set_int(main_conf, v, values_r[i], NULL);
			
			sprintf(v,"%swarning_level/%02d",cpath,i);
			gconf_client_set_float(main_conf, v, warning_level[i], NULL);
			
			sprintf(v,"%swarning_policy/%02d",cpath,i);
			gconf_client_set_string(main_conf, v, warning_policy[i], NULL);
			
			sprintf(v,"%slogging/active",cpath);
			gconf_client_set_bool(main_conf, v, LOG, NULL);
			sprintf(v,"%slogging/limit",cpath);
			gconf_client_set_int(main_conf, v, LOG_LIMIT, NULL);
			
			sprintf(path, "%s%s",getenv("HOME"),"/gsensors.log");
			sprintf(v,"%slogging/path",cpath);
			gconf_client_set_string(main_conf, v, path, NULL);
			
			sprintf(v,"%senableexec",cpath);
			gconf_client_set_bool(main_conf, v, ENABLE_EXEC,NULL);

			sprintf(path, " ");
			sprintf(v,"%sexecpath",cpath);
			gconf_client_set_string(main_conf, v, path, NULL);

			gconf_client_set_bool(main_conf, "/apps/gsensors/hasconf", TRUE, NULL);
		}
	}
	LOG_RUNNING = FALSE;
	LOG_DESCRIPTOR = NULL;
	LOG_ERR = LOG_OK;
	already_did = 0;
	if(LOG) 
		log_start_service(lpath);
	
	win = NULL;
	items = g_list_append (items, "No Warn");
	items = g_list_append (items, "Min Threshold");
	items = g_list_append (items, "Max Threshold");
	items = g_list_append (items, "Absolute");
	items = g_list_append (items, "+/- 0.5/100");
	items = g_list_append (items, "+/- 1/100");
	items = g_list_append (items, "+/- 2/100");
	items = g_list_append (items, "+/- 3/100");
	items = g_list_append (items, "+/- 4/100");
	items = g_list_append (items, "+/- 5/100");
	items = g_list_append (items, "+/- 7.5/100");
	items = g_list_append (items, "+/- 10/100");
	items = g_list_append (items, "+/- 12.5/100");
	items = g_list_append (items, "+/- 15/100");
	items = g_list_append (items, "+/- 20/100");
	items = g_list_append (items, "+/- 25/100");
	items = g_list_append (items, "+/- 30/100");
	items = g_list_append (items, "+/- 40/100");
	items = g_list_append (items, "+/- 50/100");

	gtk_widget_get_size_request(GTK_WIDGET(my_applet),NULL,&i);
	panel_resize((PanelApplet*)my_applet,i,NULL);
	for(i = 0; i < total_entrys; ++i) {
		if (values_r[i] == TRUE) {
			nothing2show = FALSE;
			break;
		} else {
			nothing2show = TRUE;
		}
	}

	timer_id = gtk_timeout_add(timeout, (GtkFunction)applet_rotate, 0);

	buf = g_strdup_printf(applet_menu_xml, _("About..."), _("Config..."));

	if(!NO_SENSORS) panel_applet_setup_menu(applet, buf,
                                 menu_verbs,
                                 (gpointer)my_applet);

	g_signal_connect(GTK_OBJECT(applet), "destroy",
			GTK_SIGNAL_FUNC(applet_destroy),
			my_applet);

	g_signal_connect(GTK_OBJECT(applet), "change_size",
			GTK_SIGNAL_FUNC(panel_resize), my_applet);
	g_signal_connect(GTK_OBJECT(applet), "change_orient",
			GTK_SIGNAL_FUNC(panel_resize), my_applet);

	g_free(buf);
	gtk_widget_show_all(GTK_WIDGET (applet));

	return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY ("OAFIID:GSensors_Factory",
                             PANEL_TYPE_APPLET,
                             "GSensors",
                             "0",
                             gsensors_applet_start,
                             NULL);
