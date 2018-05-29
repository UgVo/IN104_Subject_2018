#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

/* Pixmap to store drawings*/
static GdkPixmap *pixmap = NULL;
static GtkWidget *drawing_area = NULL;
static GtkWidget *filew;
    
static void draw_grid(GtkWidget *);
static gboolean configure_event_cb(GtkWidget *, GdkEventConfigure *, gpointer );
static void close_window(void);
static void activate(GtkWidget *);


/* Draw a rectangle on the surface at the given position */
static void
draw_grid (GtkWidget *widget)
{
  GdkGC *gc;
  GdkRectangle update_rect;
  PangoRectangle extent;
  PangoFontDescription *fd;
  int font_size = 0;
  int i = 0;
  int size_square = 50;

  PangoLayout *ly = gtk_widget_create_pango_layout(widget,"5");

  gc = widget->style->black_gc;
  
  gdk_gc_set_line_attributes (gc,1,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_MITER);
  
    for (i = 0; i < 10; ++i)
    {
        if (i%3 != 0)
        {
            gdk_draw_line(pixmap, gc, i*size_square, 0,i*size_square, size_square*9);
        }
    }
    for (i = 0; i < 10; ++i)
    {
        if (i%3 != 0)
        {
            gdk_draw_line(pixmap, gc, 0, i*size_square, size_square*9, i*size_square);
        }
    }

    gdk_gc_set_line_attributes (gc,3,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_MITER);
    for (i = 0; i < 10; ++i)
    {
        if (i%3 == 0)
        {
            gdk_draw_line(pixmap, gc, i*size_square, 0,i*size_square, size_square*9);
        }
    }
    for (i = 0; i < 10; ++i)
    {
        if (i%3 == 0)
        {
            gdk_draw_line(pixmap, gc, 0, i*size_square, size_square*9, i*size_square);
        }
    }
    gdk_draw_layout(pixmap,gc,size_square/2 - 4,size_square/2 - 4,ly);

}
/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
  if (pixmap)
     g_object_unref (pixmap);

  pixmap = gdk_pixmap_new (widget->window,
                           widget->allocation.width,
                           widget->allocation.height,
                           -1);
  gdk_draw_rectangle (pixmap,
                      widget->style->white_gc,
                      TRUE,
                      0, 0,
                      widget->allocation.width,
                      widget->allocation.height);

  draw_grid(drawing_area);
  return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gboolean
expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_drawable (widget->window,
                     widget->style->fg_gc[gtk_widget_get_state (widget)],
                     pixmap,
                     event->area.x, event->area.y,
                     event->area.x, event->area.y,
                     event->area.width, event->area.height);

  return FALSE;
}

/* ______________________________Creating a drop-down menu_________________________________ */

static void file_ok_sel( GtkWidget        *w,
                         GtkFileSelection *fs )
{
    g_print ("%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
} 

static void solve(GtkWidget *widget, gpointer data)
{
    g_print("Solve\n");
    draw_grid(drawing_area);
} 

static void save(GtkWidget *widget, gpointer data)
{
    g_print("Save\n");
} 

static void open(GtkWidget *widget, gpointer data)
{
    g_print("Open\n");
    /* fenetre de dialogue pour récupérer le nom du fichier */
    filew = gtk_file_selection_new ("File selection");
    
    /*g_signal_connect (G_OBJECT (filew), "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);*/
    /* Connect the ok_button to file_ok_sel function */
    g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked", G_CALLBACK (file_ok_sel), (gpointer) filew);
    
    g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) filew);

    /* Connect the cancel_button to destroy the widget */
    g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
	                      "clicked", G_CALLBACK (gtk_widget_destroy),
			      G_OBJECT (filew));
    
    gtk_widget_show (filew);
} 

static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    g_print ("delete event occurred\n");
    return FALSE;
}

static void destroy( GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}

void activate (GtkWidget *window) {
    GtkWidget *frame;
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *vbox;
    GtkWidget *menu_bar;
    GtkWidget *file_Mi;
    GtkWidget *file_Menu;
    GtkWidget *open_item;
    GtkWidget *solve_item;
    GtkWidget *save_item;
    
    menu_bar = gtk_menu_bar_new ();

    // Configuration du menu
    file_Mi = gtk_menu_item_new_with_label ("File");  
    gtk_menu_shell_append ((GtkMenuShell*) menu_bar, file_Mi);

    file_Menu = gtk_menu_new();
    
    gtk_menu_item_set_submenu((GtkMenuItem*) file_Mi, file_Menu);
    
    // Création des élements du menu 
    open_item = gtk_menu_item_new_with_label ("Open");
    save_item = gtk_menu_item_new_with_label ("Save");
    solve_item = gtk_menu_item_new_with_label ("Solve");

    // Ajout des éléments au menu
    gtk_menu_shell_append ((GtkMenuShell*) file_Menu, open_item);
    gtk_menu_shell_append ((GtkMenuShell*) file_Menu, save_item);
    gtk_menu_shell_append ((GtkMenuShell*) file_Menu, solve_item);

    g_signal_connect(G_OBJECT(open_item), "activate", G_CALLBACK(open), NULL);
    g_signal_connect(G_OBJECT(save_item), "activate", G_CALLBACK(save), NULL);
    g_signal_connect(G_OBJECT(solve_item), "activate", G_CALLBACK(solve), NULL);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);
    
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all (window);

    //gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

    //g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    frame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (vbox), frame);

    drawing_area = gtk_drawing_area_new ();
    /* set a minimum size */
    gtk_widget_set_size_request (drawing_area, 450, 450);
    gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
                         | GDK_LEAVE_NOTIFY_MASK
                         | GDK_BUTTON_PRESS_MASK
                         | GDK_POINTER_MOTION_MASK
                         | GDK_POINTER_MOTION_HINT_MASK);

    gtk_container_add (GTK_CONTAINER (frame), drawing_area);

    /* Signals used to handle the backing surface */
    g_signal_connect (drawing_area, "expose-event",
                        G_CALLBACK (expose_event), NULL);
    g_signal_connect (drawing_area,"configure-event",
                    G_CALLBACK (configure_event), NULL);

    gtk_widget_show_all (window);

    draw_grid(drawing_area);
}


int main( int   argc, char *argv[] )
{
    GtkWidget *window;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Hello World!");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    
    //Main
    activate(window);
   
    gtk_main ();
    
    return 0;
}
