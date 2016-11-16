#include <gui.h>
#include <gtk/gtk.h>
#include <gnome.h>
#include <libgnomeui/gnome-messagebox.h>
#include "../cam.h"
GtkWidget *create_save_win()
{
	GtkWidget *win;
	GtkWidget *save_text;
	GtkWidget *save_table;
	GtkWidget *save_button;
	GtkWidget *save_label;
	GtkWidget *check_jpg;
	GtkTooltips *tips;
	
	/* Janela para salvar arquivo */

	tips = gtk_tooltips_new();
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	save_button = gtk_button_new_with_label("OK");
	save_text = gtk_entry_new_with_max_length (100); 
	save_label = gtk_label_new("Enter the base filename for saving");
	save_table = gtk_table_new(3,2,FALSE);
	check_jpg = gtk_check_button_new_with_label("Save as jpg instead of xpm");
	gtk_tooltips_set_tip(tips, save_text, 
			"Browsers are more compatible with JPEGs", NULL);
	
	gtk_table_attach (GTK_TABLE(save_table), save_label, 0, 1, 0, 1,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);
	gtk_table_attach (GTK_TABLE(save_table), save_text, 1, 2, 0, 1,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);
	gtk_table_attach (GTK_TABLE(save_table), save_button, 2, 3, 0, 1,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);
	gtk_table_attach (GTK_TABLE(save_table), check_jpg, 0, 3, 1, 2,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);
	gtk_signal_connect (GTK_OBJECT (save_button),
			"clicked", (GtkSignalFunc) save_files, save_text);
	gtk_signal_connect (GTK_OBJECT (save_button),
			"destroy", (GtkSignalFunc) destroy_save, NULL);
	gtk_signal_connect (GTK_OBJECT (save_text),
			"activate", (GtkSignalFunc) save_files, save_text);
	gtk_signal_connect(GTK_OBJECT(check_jpg), "toggled",
			(GtkSignalFunc)toggle_album, NULL);
	GTK_CHECK_BUTTON(check_jpg)->toggle_button.active = CONVERT2JPEG;

	gtk_tooltips_set_tip(tips, save_text, 
			"Enter a 'string' to be used as base for the filenames (the output will be string01.xmp, string02.xpm ...). You may leave it blank", NULL);
	gtk_container_add (GTK_CONTAINER(win), save_table);
	gtk_widget_show_all (win);
	return win;

}

void selector_show(GtkWidget *none, GtkSignalFunc target_sig)
{
	GtkWidget *filew;
	if (selector_on == TRUE) return;
	selector_on = TRUE;
	
	filew = gtk_file_selection_new("Pick a file");
	gtk_widget_show(filew);
	
	gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
			"clicked", (GtkSignalFunc) selector_destroy, GTK_OBJECT (filew));
	gtk_signal_connect (GTK_OBJECT(filew),
			"destroy", (GtkSignalFunc) selector_destroy, GTK_OBJECT (filew));

	gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
			"clicked", target_sig, filew);
	
}
GtkWidget *create_general_win()
{
}

GtkWidget *create_error_win(char *label)
{

	GtkWidget *button_error;
	GtkWidget *win;
	GtkWidget *table_error; /* tabela da janela de erro */
	GtkWidget *error_label;
	/* Janela de erro */
	
	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	button_error = gtk_button_new_with_label ("Rats!");
	table_error = gtk_table_new (2,1,FALSE);
	error_label = gtk_label_new(label);

	gtk_table_attach (GTK_TABLE(table_error), error_label, 0, 1, 0, 1,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_error), button_error, 0, 1, 1, 2,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);

	gtk_signal_connect (GTK_OBJECT (button_error),
			"clicked", (GtkSignalFunc) hide_error, win);

	gtk_container_add (GTK_CONTAINER(win), table_error);
	gtk_widget_show_all(win);
}

GtkWidget *create_album_win()
{
	GtkWidget *win;
	GtkWidget *button_pick_file;
	GtkWidget *save_comment;
	GtkWidget *check_create_album;
	GtkWidget *file_win;
	GtkWidget *labels[31];
	GtkWidget *scrolled;
	GtkWidget *big_table;
	GtkWidget *table;
	GtkWidget *table2;
	GtkWidget *label_m;
	int i;
	char b[64];
	
	if(album_dialog == TRUE) {
		return NULL;
	}
	album_dialog = TRUE;
	win = gtk_window_new(0);
	gtk_widget_set_usize(win, 400, 420);
	scrolled = gtk_scrolled_window_new(NULL, NULL);
	big_table = gtk_table_new(2, 31, FALSE);
	gtk_table_set_row_spacings (GTK_TABLE (big_table), 7);
	label_m = gtk_label_new("Enter comments below");
	gtk_scrolled_window_set_policy  (GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	

	save_comment = gtk_button_new_with_label("Save");
	button_pick_file = gtk_button_new_with_label("Open");
	check_create_album = gtk_check_button_new_with_label("Generate album after converting");
	GTK_CHECK_BUTTON(check_create_album)->toggle_button.active = generate_album;

	table = gtk_table_new(1,2, FALSE);
	table2 = gtk_table_new(3,2, FALSE);

	gtk_table_attach(GTK_TABLE(table), table2, 0, 1, 0, 1,
			0, 0, 0, 0);

	gtk_table_attach(GTK_TABLE(table), scrolled, 0, 1, 1, 2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	gtk_table_attach(GTK_TABLE(table2), button_pick_file, 0, 1, 0, 1,
			0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table2), save_comment, 1, 2, 0, 1,
			0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table2), check_create_album, 2, 3, 0, 1,
			0, 0, 0,0);
	gtk_table_attach(GTK_TABLE(table2), label_m, 0, 3, 1, 2,
			0, 0, 0,0);

	entrys[0] = gtk_entry_new_with_max_length(250);
	gtk_entry_set_text(GTK_ENTRY(entrys[0]), comments[0]);
	labels[0] = gtk_label_new("Game Face");
	
	gtk_table_attach(GTK_TABLE(big_table), labels[0], 0, 1, 0, 1,
		0, 0, 0, 0);
	gtk_table_attach(GTK_TABLE(big_table), entrys[0], 1, 2, 0, 1,
		GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);

	for (i = 1; i < 31; ++i) {
		sprintf(b,"Comment %02d",i);
		entrys[i] = gtk_entry_new_with_max_length(250);
		gtk_entry_set_text(GTK_ENTRY(entrys[i]), comments[i]);
		labels[i] = gtk_label_new(b);
		
		gtk_table_attach(GTK_TABLE(big_table), labels[i], 0, 1, i+1, i+2,
			0, 0, 0, 0);
		gtk_table_attach(GTK_TABLE(big_table), entrys[i], 1, 2, i+1, i+2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
	}
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolled), big_table);

	gtk_signal_connect(GTK_OBJECT(win), "destroy",(GtkSignalFunc)destroy_album, NULL);
	
	gtk_signal_connect(GTK_OBJECT(button_pick_file), "clicked",
			(GtkSignalFunc)selector_show, (GtkSignalFunc)open_album_file);
	gtk_signal_connect(GTK_OBJECT(save_comment), "clicked",
			(GtkSignalFunc)selector_show, (GtkSignalFunc)save_album_file);
	gtk_signal_connect(GTK_OBJECT(check_create_album), "toggled",
			(GtkSignalFunc)toggle_album, NULL);
	
	gtk_container_add(GTK_CONTAINER(win), table);
	gtk_widget_show_all(win);
	return win;	
}

GtkWidget *create_palette_win() 
{
	GtkWidget *win;
	GtkWidget *palette_label_c0;
	GtkWidget *palette_label_c1;
	GtkWidget *palette_label_c2;
	GtkWidget *palette_label_c3;
	GtkWidget *palette_table1;
	GtkWidget *palette_table2;
	GtkWidget *palette_button_ok;
	GtkWidget *palette_pick;
	char blabel[8];
	
	win = gtk_window_new(0);
	palette_pick = gtk_color_selection_new();

	sprintf(blabel,"#%06x", colors[0]);
	palette_button_c0 = gtk_button_new_with_label(blabel);
	sprintf(blabel,"#%06x", colors[1]);
	palette_button_c1 = gtk_button_new_with_label(blabel);
	sprintf(blabel,"#%06x", colors[2]);
	palette_button_c2 = gtk_button_new_with_label(blabel);
	sprintf(blabel,"#%06x", colors[3]);
	palette_button_c3 = gtk_button_new_with_label(blabel);
	palette_button_ok = gtk_button_new_with_label("OK");
	palette_label_c0 = gtk_label_new("Color 0");
	palette_label_c1 = gtk_label_new("Color 1");
	palette_label_c2 = gtk_label_new("Color 2");
	palette_label_c3 = gtk_label_new("Color 3");
	palette_table1 = gtk_table_new(1,3, FALSE);
	palette_table2 = gtk_table_new(4,2, FALSE);
	gtk_table_attach (GTK_TABLE(palette_table1), palette_table2, 0,1, 0,1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	gtk_table_attach (GTK_TABLE(palette_table1), palette_button_ok, 0,1, 1,2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	gtk_table_attach (GTK_TABLE(palette_table1), palette_pick, 0,1, 2,3,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);

	gtk_table_attach (GTK_TABLE(palette_table2), palette_label_c0, 0,1, 0,1,
			GTK_EXPAND | GTK_FILL,GTK_EXPAND | GTK_FILL,  0,0);
	gtk_table_attach (GTK_TABLE(palette_table2), palette_label_c1, 1,2, 0,1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	gtk_table_attach (GTK_TABLE(palette_table2), palette_label_c2, 2,3, 0,1,
			GTK_EXPAND | GTK_FILL,GTK_EXPAND | GTK_FILL,  0,0);
	gtk_table_attach (GTK_TABLE(palette_table2), palette_label_c3, 3,4, 0,1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	
	gtk_table_attach (GTK_TABLE(palette_table2), palette_button_c0, 0,1, 1,2,
			GTK_EXPAND | GTK_FILL,GTK_EXPAND | GTK_FILL,  0,0);
	gtk_table_attach (GTK_TABLE(palette_table2), palette_button_c1, 1,2, 1,2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	gtk_table_attach (GTK_TABLE(palette_table2), palette_button_c2, 2,3, 1,2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	gtk_table_attach (GTK_TABLE(palette_table2), palette_button_c3, 3,4, 1,2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0,0);
	
	gtk_container_add (GTK_CONTAINER(win), palette_table1);
	
	gtk_signal_connect (GTK_OBJECT (palette_button_ok),
			"clicked", (GtkSignalFunc) apply_palette, GTK_OBJECT(win));

	gtk_signal_connect (GTK_OBJECT (palette_button_c3),
			"clicked", (GtkSignalFunc) color_pick3, palette_pick);
	gtk_signal_connect (GTK_OBJECT (palette_button_c2),
			"clicked", (GtkSignalFunc) color_pick2, palette_pick);
	gtk_signal_connect (GTK_OBJECT (palette_button_c1),
			"clicked", (GtkSignalFunc) color_pick1, palette_pick);
	gtk_signal_connect (GTK_OBJECT (palette_button_c0),
			"clicked", (GtkSignalFunc) color_pick0, palette_pick);
	
	return win;
}

GtkWidget *create_help_win()
{
	GtkWidget *win;
	GtkWidget *text;
	GtkWidget *close;
	GtkWidget *table;
	win = gtk_window_new(0);
	gtk_window_set_title(GTK_WINDOW(win), "Help");
	text = gtk_label_new_with_mnemonic("Not available yet(view the README file on the source distribution tree)");
	gtk_container_add(GTK_CONTAINER(win), text);
	gtk_widget_show_all(win);
}
void create_about_win()
{
	GtkDialog *a;
	
	a = GTK_DIALOG(gnome_about_new (_("GBCamC GTK"), _("1.0"), "Copyright 2002-2004 Vinicius Kursancew",
	_("A GTK program to handle Gameboy Camera sRAM files."),
	(const char**)authors, NULL, NULL, NULL));
	
	gtk_widget_show(GTK_WIDGET(a));
}
