//gcc -O0 -g gtk_paned_error.c -o gtk_paned_error `pkg-config --cflags --libs gtk+-3.0`
#include <gtk/gtk.h>

// This is a minimal test that causes a lot of CRITICAL output messages when the
// splitter is very thin. I have a fix for this in my gtk_utils.c library in
// other projects. So far I've had to include this in almos every project.

gboolean delete_callback (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    gtk_main_quit ();
    return FALSE;
}

int main (int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_resize (GTK_WINDOW(window), 900, 600);
    g_signal_connect (G_OBJECT(window), "delete-event", G_CALLBACK (delete_callback), NULL);

    GtkWidget *list = gtk_list_box_new ();
    GtkWidget *row = gtk_label_new ("test label");
    gtk_container_add (GTK_CONTAINER(list), row);

    GtkWidget *button = gtk_button_new_with_label ("A TEST BUTTON");

    GtkWidget *sidebar = gtk_grid_new ();
    gtk_grid_attach (GTK_GRID(sidebar), list, 0, 0, 1, 1);
    gtk_grid_attach (GTK_GRID(sidebar), button, 0, 1, 1, 1);

    GtkWidget *draw_area = gtk_drawing_area_new ();

    GtkWidget *paned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_pack1 (GTK_PANED(paned), sidebar, FALSE, FALSE);
    gtk_paned_pack2 (GTK_PANED(paned), draw_area, TRUE, TRUE);
    gtk_container_add(GTK_CONTAINER(window), paned);

    gtk_widget_show_all(window);
    gtk_main();
}
