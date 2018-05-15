#include <gtk/gtk.h>



static void callback(GtkWidget *widget, gpointer data)
{
    g_print("Hello_world\n");
} 

static void solve(GtkWidget *widget, gpointer data)
{
    g_print("Solve\n");
} 

static void save(GtkWidget *widget, gpointer data)
{
    g_print("Save\n");
} 

static void open(GtkWidget *widget, gpointer data)
{
    g_print("Open\n");
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
    GtkWidget *box;
    GtkWidget *vbox;
    GtkWidget *menu_bar;
    GtkWidget *file_Mi;
    GtkWidget *file_Menu;
    GtkWidget *open_item;
    GtkWidget *solve_item;
    GtkWidget *save_item;
    
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Hello World!");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    
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
    
    gtk_main ();
    
    return 0;
}