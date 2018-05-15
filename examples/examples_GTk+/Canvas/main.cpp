#include <gtk/gtk.h>

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

static void clear_surface (void);
static void draw_grid (GtkWidget *);
static gboolean configure_event_cb (GtkWidget *, GdkEventConfigure *, gpointer );
static gboolean draw_cb (GtkWidget *, cairo_t *, gpointer );
static void close_window (void);
static void activate (GtkApplication *, gpointer);

static void clear_surface (void) {
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);

    cairo_destroy (cr);
}

/* Draw a rectangle on the surface at the given position */
static void draw_grid (GtkWidget *widget) {
    cairo_t *cr;
    cairo_t *cr2;
    cairo_t *text;
    cairo_text_extents_t extents;
    int i,size_square;
    size_square = 50;
    /* Paint to the surface, where we store our state */
    cr = cairo_create (surface);
    cr2 = cairo_create (surface);
    text = cairo_create(surface);
    cairo_select_font_face(text, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(text, 30);
    
    cairo_set_line_width(cr, 1);
    cairo_set_line_width(cr2, 3);
    for (i = 0; i < 10; ++i)
    {
        if (i%3 == 0)
        {
            cairo_move_to(cr2, i*size_square, 0);
            cairo_line_to(cr2, i*size_square, size_square*9);
        }
        else
        {
            cairo_move_to(cr, i*size_square, 0);
            cairo_line_to(cr, i*size_square, size_square*9);
        }
    }
    for (i = 0; i < 10; ++i)
    {
        if (i%3 == 0)
        {
            cairo_move_to(cr2, 0, i*size_square);
            cairo_line_to(cr2, size_square*9, i*size_square);
        }
        else
        {
            cairo_move_to(cr, 0, i*size_square);
            cairo_line_to(cr, size_square*9, i*size_square);
        }
    }
    cairo_text_extents(text, "5", &extents);
    cairo_move_to(text, size_square/2 - extents.width/2 , size_square/2 + extents.height/2);
    cairo_show_text(text, "5");
    cairo_stroke(cr);
    cairo_stroke(cr2);

    cairo_destroy (cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area (widget, 0,0, 6, 6);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
    if (surface)
    {
        cairo_surface_destroy (surface);
    }
    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget), CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width (widget), gtk_widget_get_allocated_height (widget));

    // Initialize the surface to white 
    clear_surface ();
    draw_grid(widget);
    // We've handled the configure event, no need for further processing. 
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_surface (cr, surface, 0, 0);
    cairo_paint (cr);

    return FALSE;
}

static void close_window (void) {
    if (surface)
    {
        cairo_surface_destroy (surface);
    }
}

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *drawing_area;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

    g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    frame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (window), frame);

    drawing_area = gtk_drawing_area_new ();
    /* set a minimum size */
    gtk_widget_set_size_request (drawing_area, 450, 450);

    gtk_container_add (GTK_CONTAINER (frame), drawing_area);

    /* Signals used to handle the backing surface */
    g_signal_connect (drawing_area,"draw", G_CALLBACK (draw_cb), NULL);
    g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);

    gtk_widget_show_all (window);
}

int main (int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
