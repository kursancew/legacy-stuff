#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <gui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cam.h"
#include <X11/Xlib.h>
#include <Imlib2.h>

void show_error (char *label) {
	error_win = create_error_win(label);
}


void exit_prog(GtkWidget *widget, gpointer *data) {
	gtk_main_quit();
}

void preview_expose(GtkWidget *widget, GdkEventExpose *event)
{
	gdk_draw_pixmap(widget->window,
		widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		preview_pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);
}

void select_save(GtkWidget *widget, gpointer *data) {
	if(filename == NULL) {
		show_error("Nothing to save");
		return;
	}
	if (save_dialog == FALSE) {
		save_dialog = TRUE;
		save_win = create_save_win();
	}
	
}

void file_picked(GtkWidget *widget, gpointer *data) {
	GdkBitmap *mask;
	int i,i2;
	if (filename == NULL)
		filename = (char*)malloc(768);

	sprintf(filename, gtk_file_selection_get_filename (GTK_FILE_SELECTION (data)));
	if(open_and_process() != 0) {
		g_print("error opening or processing file %s\n", filename);
		filename = NULL;
	}
	gtk_widget_destroy(GTK_WIDGET(data));
}
void selector_destroy(GtkWidget *widget, GtkWidget *data) {
	gtk_widget_destroy(data);
	selector_on = FALSE;
}
void save_files(GtkWidget *widget, gpointer *data) {
	gchar *basename;
	char *savename;
	int i,i2;
	FILE *desc;
	
	basename = (char*)malloc(256);
	savename = (char*)malloc(512);
	
	sprintf(basename, gtk_entry_get_text(GTK_ENTRY(data)));
	for(i = 0; i < 31;++i) {
		if (only_get_1 == TRUE && picnum != i) {
			continue;
		}

		sprintf(savename,"%s%02d.xpm", basename, i);

		desc = fopen(savename,"w+");

		if (desc == NULL) {
			perror(savename);
			free(savename);
			free(basename);
			show_error("Could not open file for saving, w00t!");
			return;
		}

		fprintf(desc,"/* XPM */\nstatic char *xpm[] = {\n");

		for(i2 = 0; i2 < 116; ++i2) {
			if( fprintf(desc, "\"%s\",\n",images[i][i2]) != (int)strlen(images[i][i2]) + 4) {
				perror(savename);
				free(basename);
				free(savename);
				show_error("Could not output to file");
				return;
			}
		}
		fflush(desc);
		if(fprintf(desc, "\"%s\"};", images[i][116]) != (int)strlen(images[i][i2]) + 4 ) {
			perror(savename);
			free(basename);
			free(savename);
			show_error("Could not output to file");
			return;
		}

		fclose(desc);
		if (CONVERT2JPEG == TRUE) {
			Imlib_Image image;
			image = imlib_load_image(savename);
			if (image)
			{
				char *tmp;
				tmp = malloc(256);
				sprintf(tmp, "%s%02d.jpg",basename,i);
				imlib_context_set_image(image);
				imlib_image_set_format("jpg");
				imlib_save_image(tmp);
				imlib_free_image();
				unlink(savename);
				free(tmp);
			}
		}
	}
	gtk_widget_hide_all(GTK_WIDGET(save_win));
	gtk_widget_destroy(save_win);
	save_dialog = FALSE;
	if (generate_album == TRUE && only_get_1 == FALSE) {
		int t = 0;
		if(CONVERT2JPEG == TRUE) t = 1;
		gen_album(basename, 0, 1);
	}
	free(savename);
	free(basename);
}
void gen_album(char *base, int type, int c) {
	char file[256];
	char *tp = 0;
	int i;
	FILE *fd;
	char *album_title = "My GB cam album";
	sprintf(file,"%sindex.html",base);
	if (type == 0)
		tp = ".xmp";
	else if (type == 1)
		tp = ".jpg";
				
	fd = fopen(file,"w");

	fprintf(fd,html_head,album_title,album_title);
	
	for(i=1; i<=30; ++i)  {
		fprintf(fd,"<td valign=top width=128><center><img src=%s%02d%s>",base,i,tp);
		if(c == 1) 
			fprintf(fd,"<br><font size=3>%s</font></center><br>",comments[i]);
		fprintf(fd,"</td>\n");
		if ( i % 5 == 0) fprintf(fd,"</tr>\n<tr>\n");
	}
	
	
	fprintf(fd,"</tr></table><center><img src=%sGameFace%s><br> \n\
			<font size=3>%s</font></center>\n",base,tp, comments[0]);

	fprintf(fd,html_foot);
	fclose(fd);
}

void toggle_jpeg(GtkWidget *widget, gpointer *data)
{
	CONVERT2JPEG = GTK_CHECK_BUTTON(widget)->toggle_button.active;
}

void toggle_album(GtkWidget *widget, gpointer *data)
{
	generate_album = GTK_CHECK_BUTTON(widget)->toggle_button.active;
}
void preview_previous (GtkWidget *widget, GtkWidget *drawing) {
	char *buf;
	GdkBitmap *mask;
	int i,i2;

	if (picnum > 0 && filename != NULL) {
		buf = (char*)malloc(256);
		picnum--;
		if (picnum != 0) sprintf(buf, "Preview: %d",picnum);
		else sprintf(buf, "Preview: Game Face", picnum);
		
		gtk_label_set_text(GTK_LABEL(label_preview), buf);
		free(buf);

	if (preview_pixmap != NULL)
		gdk_pixmap_unref(preview_pixmap);
	
	preview_pixmap = gdk_pixmap_create_from_xpm_d (drawing->window,
                                        &mask,
					&drawing->style->bg[GTK_STATE_NORMAL],
					(gchar**)images[picnum]);
	
	gdk_draw_pixmap(drawing->window,
		drawing->style->fg_gc[GTK_WIDGET_STATE(widget)],
		preview_pixmap,
		0, 0,
		0, 0,
		128, 112);
	}
}

void preview_next (GtkWidget *widget, GtkWidget *drawing) {
	char *buf;
	GdkBitmap *mask;
	int i,i2;
	
	if (picnum < 30 && filename != NULL) {
		buf = (char*)malloc(256);
		picnum++;
		if (picnum != 0) sprintf(buf, "Preview: %d", picnum);
		else sprintf(buf, "Preview: Game Face", picnum);

		gtk_label_set_text(GTK_LABEL(label_preview), buf);
		free(buf);
	
	if (preview_pixmap != NULL)
		gdk_pixmap_unref(preview_pixmap);
	
	preview_pixmap = gdk_pixmap_create_from_xpm_d (drawing->window,
                                        &mask,
					&drawing->style->bg[GTK_STATE_NORMAL],
					(gchar**)images[picnum]);
	
	gdk_draw_pixmap(drawing->window,
		drawing->style->fg_gc[GTK_WIDGET_STATE(widget)],
		preview_pixmap,
		0, 0,
		0, 0,
		128, 112);
	}
}

void preview_clear(GtkWidget *widget, GdkEventConfigure *event )
{
}

void choose_palette(GtkWidget *widget, gpointer *data)
{
	if(filename == NULL) {
		show_error("Nothing to apply a palette on you big silly goose (Big Gay Al)");
			return;
	}
	win_palette = create_palette_win ();
	gtk_widget_show_all(win_palette);
}

void apply_palette(GtkWidget *widget, gpointer *data)
{
	gtk_widget_destroy (win_palette);
	open_and_process();
}
void hide_error (GtkWidget *widget, GtkWidget *data) {
	gtk_widget_destroy(data);
}

void destroy_save(GtkWidget *widget, gpointer *data)
{
	gtk_widget_hide_all(save_win);
	gtk_widget_destroy(save_win);
	save_dialog = FALSE;
	save_win = NULL;
}

void color_pick3(GtkWidget *widget, GtkWidget *data) {
	char blabel[8];
	gtk_color_selection_get_color (GTK_COLOR_SELECTION(data), color3);
	color3[0] *= 0xFF;
	color3[1] *= 0xFF;
	color3[2] *= 0xFF;
	colors[3] = 0;
	colors[3] |= (int)color3[0] << 16;
	colors[3] |= (int)color3[1] << 8;
	colors[3] |= (int)color3[2];
	sprintf(blabel,"#%06x", colors[3]);
	gtk_label_set_text(GTK_LABEL(GTK_BIN(palette_button_c3)->child), blabel);
}
void color_pick2(GtkWidget *widget, GtkWidget *data) {
	char blabel[8];
	gtk_color_selection_get_color (GTK_COLOR_SELECTION(data), color2);
	color2[0] *= 0xFF;
	color2[1] *= 0xFF;
	color2[2] *= 0xFF;
	colors[2] = 0;
	colors[2] |= (int)color2[0] << 16;
	colors[2] |= (int)color2[1] << 8;
	colors[2] |= (int)color2[2];
	sprintf(blabel,"#%06x", colors[2]);
	gtk_label_set_text(GTK_LABEL(GTK_BIN(palette_button_c2)->child), blabel);
}
void color_pick1(GtkWidget *widget, GtkWidget *data) {
	char blabel[8];
	gtk_color_selection_get_color (GTK_COLOR_SELECTION(data), color1);
	color1[0] *= 0xFF;
	color1[1] *= 0xFF;
	color1[2] *= 0xFF;
	colors[1] = 0;
	colors[1] |= (int)color1[0] << 16;
	colors[1] |= (int)color1[1] << 8;
	colors[1] |= (int)color1[2];
	sprintf(blabel,"#%06x", colors[1]);
	gtk_label_set_text(GTK_LABEL(GTK_BIN(palette_button_c1)->child), blabel);
}
void color_pick0(GtkWidget *widget, GtkWidget *data) {
	char blabel[8];
	gtk_color_selection_get_color (GTK_COLOR_SELECTION(data), color0);
	color0[0] *= 0xFF;
	color0[1] *= 0xFF;
	color0[2] *= 0xFF;
	colors[0] = 0;
	colors[0] |= (int)color0[0] << 16;
	colors[0] |= (int)color0[1] << 8;
	colors[0] |= (int)color0[2];
	sprintf(blabel,"#%06x", colors[0]);
	gtk_label_set_text(GTK_LABEL(GTK_BIN(palette_button_c0)->child), blabel);
}

void destroy_album(GtkWidget *widget, GtkWidget *data) {
	int i;
	for(i = 0; i < 31; ++i)
		sprintf(comments[i], gtk_entry_get_text(GTK_ENTRY(entrys[i])) );
	album_dialog = FALSE;
}

int open_album_file(GtkWidget *widget, GtkWidget *data) {
	FILE *fdes;
	struct stat stats;
	char *buf;
	char *line;
	int comntN;
	int i = 0;
	if (comment_file == NULL)
		comment_file = (char*)malloc(512);

	sprintf(comment_file, gtk_file_selection_get_filename (GTK_FILE_SELECTION (data)) );

	fdes = fopen(comment_file, "r");
	if (fdes == NULL) {
		perror("Comments file:");
		show_error("Not able to open comments file");
		return -1;
	}
	stat(comment_file, &stats);
	buf = (char*)malloc(stats.st_size);
	fseek(fdes,0,SEEK_SET);
	fread(buf, stats.st_size,1,fdes);
	line = (char*)malloc(1024);
	for (line = strtok(buf ,"\n");
			line;
			line=strtok(0,"\n")) {
		if (line[0] == '#') continue;
		comntN = atoi(line);
		for(i = 0; i < 31; ++i) {
			if (comntN == i) {
				line = strchr(line, '=');
				if (line == NULL) {
					continue;
				}
				sprintf(comments[i], line + 1);
				gtk_entry_set_text(GTK_ENTRY(entrys[i]), comments[i]);
			}
		}
	}
	free(line);
	free(buf);
	gtk_widget_destroy(GTK_WIDGET(data));
	return 1;
}

int save_album_file(GtkWidget *widget, GtkWidget *data)
{
	FILE *fdes;
	char *line;
	int i = 0;
	if (comment_file == NULL)
		comment_file = (char*)malloc(512);

	sprintf(comment_file, gtk_file_selection_get_filename (GTK_FILE_SELECTION (data)) );

	fdes = fopen(comment_file, "w+");
	if (fdes == NULL) {
		perror("Comments file:");
		show_error("Not able to open comments file");
		return -1;
	}
	fseek(fdes,0,SEEK_SET);
	line = (char*)malloc(1024);
	fprintf(fdes, "#GbCamC Album File\n\n");
	for(i = 0; i < 31; ++i) {
		sprintf(comments[i], gtk_entry_get_text(GTK_ENTRY(entrys[i])) );
		fprintf(fdes, "%d=%s\n", i, comments[i]);
	}
	fclose(fdes);
	free(line);
	gtk_widget_destroy(GTK_WIDGET(data));
	return 1;
}

void select_all(GtkWidget *widget, gpointer *data) {
	only_get_1 = !GTK_CHECK_BUTTON(widget)->toggle_button.active; 
}

void apply_negated_palette(GtkWidget *widget, gpointer *data) {
	if (filename == NULL &&
			GTK_CHECK_BUTTON(widget)->toggle_button.active == TRUE) {
		show_error("Nothing to apply a palette on you big silly goose (Big Gay All)");
			GTK_CHECK_BUTTON(widget)->toggle_button.active = FALSE;
		
	} else {
		colors[0] = 0xFFFFFF -colors[0];
		colors[1] = 0xFFFFFF -colors[1];
		colors[2] = 0xFFFFFF -colors[2];
		colors[3] = 0xFFFFFF -colors[3];
		open_and_process();
	}

}
