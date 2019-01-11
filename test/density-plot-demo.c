/*
 * demo.c
 *
 * Copyright (C) 1999 EMC Capital Management, Inc.
 * Copyright (C) 2001 The Free Software Foundation
 *
 * Developed by Jon Trowbridge <trow@gnu.org> and
 * Havoc Pennington <hp@pobox.com>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include <math.h>
#include <gtk/gtk.h>
#include "data/y-data-simple.h"
#include "plot/y-plot-widget.h"
#include "plot/y-axis-view.h"
#include "plot/y-scatter-series.h"
#include "plot/y-density-view.h"

#define DATA_COUNT 2000

GtkWidget *window;
YPlotWidget * scatter_plot;
YDensityView *dens;

YData *d1;

GTimer *timer;

gint counter = 0;

static void
init (gint argc, gchar *argv[])
{
  gtk_init(&argc, &argv);
}

static void
quit (GtkWidget *w, GdkEventAny *ev, gpointer closure)
{
  //g_timeout_remove (timeout);
  gtk_widget_destroy (window);

  gtk_main_quit ();
}

static void
build_gui (void)
{
  window = g_object_new(GTK_TYPE_WINDOW,NULL);
  gtk_window_set_default_size(GTK_WINDOW(window),640,400);
  gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(scatter_plot));

  g_signal_connect (G_OBJECT (window),
		      "delete_event",
		      G_CALLBACK (quit),
		      NULL);

  gtk_widget_show_all (window);

	g_message("built GUI: %f s",g_timer_elapsed(timer,NULL));
}

static void
build_data (void)
{
  gint i;
  double t;
  double *x;

  x=g_malloc(sizeof(double)*DATA_COUNT*DATA_COUNT/2);
  for (i=0; i<DATA_COUNT*DATA_COUNT/2; ++i) {
    t = 2*G_PI*i/(double)DATA_COUNT;
    x[i] = 2*sin (4*t*1.01);
  }
  d1 = y_val_matrix_new (x, DATA_COUNT/2, DATA_COUNT, NULL);

  timer = g_timer_new();
}

static void
build_elements (void)
{
  //YScatterSeries *series1 = g_object_new(Y_TYPE_SCATTER_SERIES,"x-data",d1,"y-data",d2,NULL);
  //YScatterSeries *series2 = g_object_new(Y_TYPE_SCATTER_SERIES,"x-data",d1,"y-data",d3,"draw-markers",TRUE,"marker",MARKER_PLUS,NULL);

	//y_scatter_series_set_line_color_from_string(series1,"#ff0000");
	//y_scatter_series_set_marker_color_from_string(series2,"#0000ff");

	//g_message("created series: %f s",g_timer_elapsed(timer,NULL));

  scatter_plot = y_plot_widget_new_density();

	g_message("created plot: %f s",g_timer_elapsed(timer,NULL));

  dens = Y_DENSITY_VIEW(y_plot_widget_get_main_view(scatter_plot));
	g_object_set(dens,"data",d1,"preserve-aspect",FALSE,NULL);

  g_object_set(y_plot_widget_get_axis_view (scatter_plot, Y_COMPASS_SOUTH),"axis_label","this is the x axis",NULL);
  g_object_set(y_plot_widget_get_axis_view (scatter_plot, Y_COMPASS_WEST),"axis_label","this is the y axis",NULL);
  //g_object_set(scatter_plot->east_axis,"axis_label","this is the y axis",NULL);

	g_message("built elements: %f s",g_timer_elapsed(timer,NULL));
}

int
main (int argc, char *argv[])
{

  init (argc, argv);

  g_message ("building data");
  build_data ();

  g_message ("building elements");
  build_elements ();

  g_message ("building gui");
  build_gui ();

	y_data_emit_changed(Y_DATA(d1));

  gtk_main ();

  return 0;
}
