#include <unistd.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <gui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cam.h"
#include "logo.h"

int verify(FILE *fd) {
	char *b;
	int ret = 0;
	unsigned int i;
	if (fseek(fd, 0x1FFFFF, SEEK_SET) != 0) {
		g_print("File too small to be a gb cam sav file\n");
		return -1;
	}
	b = (char*)malloc(8);
	for (i = 0x2FB1; i <= 0x1FFB1; i += 0x1000) {
		fseek(fd, i, SEEK_SET);
		fread(b, 1, 5, fd);
		b[5] = '\0';
		if(strcmp("Magic", b) != 0) {
			printf("%x %s\n",i,b);
			ret = -1;
			g_print("File is not a genuine gb camera sram image\n");
			show_error("File is not a genuine gb camera sram image");
			break;
		}
	}
	free(b);
	return ret;
}

int open_and_process() {
	int i, i2, i3, i4,out_seek = 0x2000, posx, posy, tx, ty, color[8][8];
	FILE *fd;
	char *buf, comparison;
	char img[117][128];
	GdkBitmap *mask;
	buf = (char*)malloc(0x0F00);

	fd = fopen(filename,"r");
	g_print("%s\n",filename);
	if (fd == NULL) {
		show_error("Could not open file");
		g_print("Could not open file\n");
		perror("error: ");
		return -1;
	}

	if (verify(fd) != 0) {
		return -1;
	}

	for (i2=0; i2 < 31; ++i2) {
		fseek(fd, out_seek, SEEK_SET);
		if (i2 == 0) fseek(fd, 0x11FC, SEEK_SET);

		if(fread(buf, 0x0E00, 1, fd) != 1) {
			char b[256];
			sprintf(b,"gbcamc: file %s", filename);
			perror(b);
			return -1;
		}

		sprintf(img[0], "128 112 4 1");
		sprintf(img[1], "Z      c #%06x", colors[0]);
		sprintf(img[2], "C      c #%06x", colors[1]);
		sprintf(img[3], ".      c #%06x", colors[2]);
		sprintf(img[4], "X      c #%06x", colors[3]);

		i = 0;
		for( ty = 0; ty < 105; ty+=8 ) {
			for( tx = 0; tx <= 120; tx+=8 ) {
				for(posy = 0; posy < 8; ++posy) {
					comparison = 0x01;
					for(posx = 7; posx >= 0; --posx) {
						color[posy][posx] = 0;

						if(buf[i+1] & comparison)
							color[posy][posx] += 1;
						if(buf[i] & comparison)
							color[posy][posx] += 2;
						comparison <<= 1;
						if(color[posy][posx] == 0)
							img[posy + ty + 5][posx + tx] = C0;
						if(color[posy][posx] == 1)
							img[posy + ty + 5][posx + tx] = C1;
						if(color[posy][posx] == 2)
							img[posy + ty + 5][posx + tx] = C2;
						if(color[posy][posx] == 3)
							img[posy + ty + 5][posx + tx] = C3;
					}
				i += 2;
				}
			}
		}
		for ( i3 = 0; i3 < 117; ++i3) {
			for ( i4 = 0; i4 < 128; ++i4) 
				images[i2][i3][i4] = img[i3][i4];
			images[i2][i3][128] = '\0';
		}
		if (i2 != 0) out_seek += 0x1000;
	}
	fclose(fd);

	if (preview_pixmap != NULL)
		gdk_pixmap_unref(preview_pixmap);

	if (picnum != 0) sprintf(buf, "Preview: %d", picnum);
	else sprintf(buf, "Preview: Game Face", picnum);

	gtk_label_set_text(GTK_LABEL(label_preview), buf);
	free(buf);
	preview_pixmap = gdk_pixmap_create_from_xpm_d (preview->window,
                                        &mask,
					&preview->style->bg[GTK_STATE_NORMAL],
					(gchar**)images[picnum]);

	gdk_draw_pixmap(preview->window,
		preview->style->fg_gc[GTK_WIDGET_STATE(preview)],
		preview_pixmap,
		0, 0,
		0, 0,
		128, 112);

	return 0;
}

int main(gint argc, gchar *argv[]) {
	GtkWidget *win;
	GtkWidget *win_album_stuff;

	GdkVisual *visual;
	
	GtkWidget *button_previous;
	GtkWidget *button_next;
	GtkWidget *button_save;
	GtkWidget *button_palette;
	
	GtkWidget *check_all;
	GtkWidget *check_negate;
	
	GtkWidget *main_table; /* tabela principal, dividada em 2 colunas */
	GtkWidget *table_actions; /* tabela da direita, 3 linhas */
	GtkWidget *table_nav; /* tabela que contem os botoes de navegacao (dividida em 2 cols) */
	GtkWidget *table_options; /* tabela das opcoes (6 linhas) */
	GtkWidget *table_open_save; /* tabela de 2 cols que divide open-save */
	GtkWidget *table_palette_options; /* tabela que divide options e palette */
	GtkWidget *table_album;

	GtkWidget *label_options;
	GtkWidget *entry_album;

	GtkWidget *separator;

	GtkWidget *menu;
	GtkWidget *menu_file;
	GtkWidget *menu_options;
	GtkWidget *menu_help;
	GtkWidget *menu_actions;
	GtkWidget *item_file;
	GtkWidget *item_file_open;
	GtkWidget *item_file_save;
	GtkWidget *item_file_quit;
	GtkWidget *item_actions;
	GtkWidget *item_actions_refresh;
	GtkWidget *item_actions_next;
	GtkWidget *item_actions_previous;
	GtkWidget *item_options;
	GtkWidget *item_options_album;
	GtkWidget *item_options_palette;
	GtkWidget *item_options_setup;
	GtkWidget *item_help;
	GtkWidget *item_help_about;
	GtkWidget *item_help_help;

	GtkWidget *status_bar;

	GdkRectangle update_rect;
	GtkStyle *style;
	GdkGC *gc;

	GtkTooltips *tips;
	GdkBitmap *mask;
	
	char *gen_buf;
	int i, i2;

	gtk_init(&argc, &argv);

	filename = NULL;
	save_win = NULL;
	comment_file = NULL;
	album_dialog = FALSE;
	CONVERT2JPEG = TRUE;
	for (i = 0; i < 31; ++i)
		for (i2 = 0; i2 < 117; ++i2)
			images[i][i2] = (char*)malloc(129);
	for (i = 0; i < 31; ++i) {
		comments[i] = (char*)malloc(256);
		sprintf(comments[i], "");
	}
	colors[0] = 0x111111;
	colors[1] = 0x666666;
	colors[2] = 0xAAAAAA;
	colors[3] = 0xEEEEEE;
	picnum = 0;
	gen_buf = (char*)malloc(512);
	is_error = FALSE;
	only_get_1 = FALSE;

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), version);

	/* Janela Principal */

	main_table = gtk_table_new (3, 3, FALSE);
	tips = gtk_tooltips_new();

	/* Tabelas */
	
	table_actions = gtk_table_new (3, 1, FALSE);
	table_nav = gtk_table_new (1, 2, FALSE);
	table_options = gtk_table_new (6, 1, FALSE);
	table_open_save = gtk_table_new (1, 2, FALSE);
	table_palette_options = gtk_table_new (1, 2, FALSE);
	table_album = gtk_table_new(1,2, FALSE);

	/* Menu */
	menu = gtk_menu_bar_new	();
	item_file = gtk_menu_item_new_with_label("File");
	item_actions = gtk_menu_item_new_with_label("Actions");
	item_options = gtk_menu_item_new_with_label("Options");
	item_help = gtk_menu_item_new_with_label("Help");
	gtk_menu_bar_append(GTK_MENU_BAR(menu), item_file);
	gtk_menu_bar_append(GTK_MENU_BAR(menu), item_options);
	gtk_menu_bar_append(GTK_MENU_BAR(menu), item_help);

	menu_file = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_file), menu_file);
	item_file_open = gtk_menu_item_new_with_label("Open");
	item_file_save = gtk_menu_item_new_with_label("Save");
	item_file_quit = gtk_menu_item_new_with_label("Quit");
	gtk_menu_append(GTK_MENU(menu_file), item_file_open);
	gtk_menu_append(GTK_MENU(menu_file), item_file_save);
	gtk_menu_append(GTK_MENU(menu_file), item_file_quit);

	menu_actions = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_actions), menu_actions);
	item_actions_refresh = gtk_menu_item_new_with_label("Refresh");
	item_actions_next = gtk_menu_item_new_with_label("Next");
	item_actions_previous = gtk_menu_item_new_with_label("Previous");
	gtk_menu_append(GTK_MENU(menu_actions), item_actions_refresh);
	gtk_menu_append(GTK_MENU(menu_actions), item_actions_next);
	gtk_menu_append(GTK_MENU(menu_actions), item_actions_previous);
	
	menu_options = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_options), menu_options);
	item_options_album = gtk_menu_item_new_with_label("Album...");
	item_options_palette = gtk_menu_item_new_with_label("Palette...");
	item_options_setup = gtk_menu_item_new_with_label("General...");
	gtk_menu_append(GTK_MENU(menu_options), item_options_album);
	gtk_menu_append(GTK_MENU(menu_options), item_options_palette);
	/* gtk_menu_append(GTK_MENU(menu_options), item_options_setup);
	 * So far we don't need this menu, but it's here, ready to be used
	*/
	
	menu_help = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_help), menu_help);
	item_help_about = gtk_menu_item_new_with_label("About");
	item_help_help = gtk_menu_item_new_with_label("Help");
	gtk_menu_append(GTK_MENU(menu_help), item_help_about);
	gtk_menu_append(GTK_MENU(menu_help), item_help_help);

	
	/* Corpo */
	
	button_previous = gtk_button_new_with_label ("<");
	gtk_tooltips_set_tip(tips, button_previous, "Previous", NULL);
	button_next = gtk_button_new_with_label (">");
	gtk_tooltips_set_tip(tips, button_next, "Next", NULL);
	button_save = gtk_button_new_with_label ("Convert!");
	button_palette = gtk_button_new_with_label ("Pick Palette");

	check_all = gtk_check_button_new_with_label("Extract all");
	gtk_tooltips_set_tip(tips, check_all, "Uncheck to have only the selected picture saved", NULL);
	GTK_CHECK_BUTTON(check_all)->toggle_button.active = TRUE;

	check_negate = gtk_check_button_new_with_label("Negate Palette");
	gtk_tooltips_set_tip(tips, check_negate, "Color complement the palette", NULL);

	sprintf(gen_buf, "Preview", picnum);
	label_preview = gtk_label_new(gen_buf);
	gtk_widget_set_usize(label_preview, 200, 20);
	label_options = gtk_label_new("Options");

	separator = gtk_vseparator_new();
	
	/* Preview */
	
	visual = gdk_visual_get_system();
	style = gtk_widget_get_default_style();
	
	preview = gtk_drawing_area_new();
	gtk_drawing_area_size(GTK_DRAWING_AREA(preview), 128, 112);

	/* Montagem */
	gtk_table_attach (GTK_TABLE(main_table), menu, 0, 3, 0, 1,
			GTK_EXPAND | GTK_FILL , 0, 0 , 0);

	gtk_table_attach (GTK_TABLE(main_table), table_actions, 0, 1, 1, 2,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);

	gtk_table_attach (GTK_TABLE(main_table), table_options, 2, 3, 1, 2,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);

	gtk_table_attach (GTK_TABLE(main_table), separator, 1, 2, 1, 2,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0 , 0);
	
	gtk_table_set_col_spacings(GTK_TABLE(main_table), 10);
	
	gtk_table_attach (GTK_TABLE(table_actions), table_nav, 0, 1, 2, 3,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_options), table_open_save, 0, 1, 4, 5,
			GTK_EXPAND | GTK_FILL , GTK_EXPAND | GTK_FILL, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_options), table_palette_options, 0, 1, 5, 6,
			GTK_EXPAND | GTK_FILL , GTK_EXPAND | GTK_FILL, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_nav), button_previous, 0, 1, 0, 1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_nav), button_next, 1, 2, 0, 1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_actions), preview, 0, 1, 1, 2,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);
	
	gtk_table_attach (GTK_TABLE(table_actions), label_preview, 0, 1, 0, 1,
			GTK_EXPAND, GTK_EXPAND, 0 , 0);
	
	gtk_table_attach (GTK_TABLE(table_options), check_negate, 0, 1, 2, 3,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0 , 0);
	
	gtk_table_attach (GTK_TABLE(table_options), check_all, 0, 1, 3, 4,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0 , 0);

	gtk_table_attach (GTK_TABLE(table_palette_options), button_palette, 0, 1, 0, 1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0 , 0);
	
	gtk_table_attach (GTK_TABLE(table_open_save), button_save, 0, 2, 0, 1,
			GTK_EXPAND | GTK_FILL, GTK_EXPAND, 0 , 0);
	
	/* Sinais */

	gtk_signal_connect(GTK_OBJECT(win), "destroy", 
		GTK_SIGNAL_FUNC(exit_prog), NULL);

	gtk_signal_connect (GTK_OBJECT(button_previous), "clicked",
			(GtkSignalFunc) preview_previous, preview);
	
	gtk_signal_connect (GTK_OBJECT(button_next), "clicked",
			(GtkSignalFunc) preview_next, preview);

	gtk_signal_connect (GTK_OBJECT(preview), "expose_event",
		(GtkSignalFunc) preview_expose, NULL);
	
	gtk_signal_connect (GTK_OBJECT(preview),"configure_event",
		(GtkSignalFunc) preview_clear, NULL);

	gtk_signal_connect (GTK_OBJECT(button_save), "clicked", 
			(GtkSignalFunc) select_save, NULL);

	gtk_signal_connect (GTK_OBJECT(button_palette), "clicked", 
			(GtkSignalFunc) choose_palette, NULL);
	
	gtk_signal_connect (GTK_OBJECT (check_all),
			"toggled", (GtkSignalFunc) select_all, NULL);
	gtk_signal_connect (GTK_OBJECT (check_negate),
			"toggled", (GtkSignalFunc) apply_negated_palette, NULL);

	gtk_signal_connect (GTK_OBJECT (item_file_open),
			"activate", (GtkSignalFunc) selector_show, file_picked);
	gtk_signal_connect (GTK_OBJECT (item_file_save),
			"activate", (GtkSignalFunc) select_save, NULL);
	gtk_signal_connect (GTK_OBJECT (item_file_quit),
			"activate", (GtkSignalFunc) exit_prog, NULL);
	
	gtk_signal_connect (GTK_OBJECT (item_actions_next),
			"activate", (GtkSignalFunc) preview_next, NULL);
	gtk_signal_connect (GTK_OBJECT (item_actions_previous),
			"activate", (GtkSignalFunc) preview_previous, NULL);
	gtk_signal_connect (GTK_OBJECT (item_actions_refresh),
			"activate", (GtkSignalFunc) preview_expose, NULL);

	gtk_signal_connect (GTK_OBJECT (item_options_album),
			"activate", (GtkSignalFunc) create_album_win, NULL);
	gtk_signal_connect (GTK_OBJECT (item_options_palette),
			"activate", (GtkSignalFunc) choose_palette, NULL);
	gtk_signal_connect (GTK_OBJECT (item_options_setup),
			"activate", (GtkSignalFunc) create_general_win, NULL);

	gtk_signal_connect (GTK_OBJECT (item_help_about),
			"activate", (GtkSignalFunc) create_about_win, NULL);
	gtk_signal_connect (GTK_OBJECT (item_help_help),
			"activate", (GtkSignalFunc) create_help_win, NULL);

	gtk_container_add (GTK_CONTAINER(win), main_table);
	gtk_widget_show_all(win);
	preview_pixmap = gdk_pixmap_create_from_xpm_d (preview->window,
                                        &mask,
					&preview->style->bg[GTK_STATE_NORMAL],
					(gchar**)gui_xpm);
	gtk_main();
	exit(0);
}
