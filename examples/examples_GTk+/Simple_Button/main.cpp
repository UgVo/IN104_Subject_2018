#include <gtk/gtk.h>



static void hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello_world\n");
} 

static gboolean delete_event(GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    g_print ("delete event occurred\n");
    return false;
}

static void destroy( GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}


int main( int   argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Hello World!");
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    button = gtk_button_new_with_label ("Hello World");
    
    // Connection des signaux aux callback respectifs
    // g_signal_connect(instance, signal, callback, donnee)
    // 'instance' correspond à l'élément qui envoie le signal
    // 'signal' correspond au nom du signal envoyé par l'instance 
    // 'callback' correspond à la fonction qui doit être appelée lorsque le signal est émis
    // 'data' correspond à l'information qui peut être transmise avec le signal au callback
    g_signal_connect (window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL);
    g_signal_connect (button, "clicked", G_CALLBACK (hello), NULL);
    // g_signal_connect_swapped() fonctionne de la même manière que g_signal_connect, cependant 
    // la fonction appelée par le signal sera appelée sur l'objet 'data' et pas instance
    // Ici, permet de detruire la fenêtre et non pas le bouton (ce qui arrivera si on avait utilisé l'autre fonction)
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);

    // Ajout du bouton récement créé à la fenêtre principale
    gtk_container_add (GTK_CONTAINER (window), button);
    // Affichage du bouton
    gtk_widget_show (button);

    gtk_widget_show (window);
    
    gtk_main ();
    
    return 0;
}