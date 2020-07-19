//gcc -g combobox_destroys_itself.c -o combobox_destroys_itself `pkg-config --cflags --libs gtk+-3.0`
/*
 * Copiright (C) 2018 Santiago León O.
 */

// This simple Gtk application tests the case where a widget causes its own
// destruction from inside a handler to one of its signals.  This is something I
// expected would be handled cleanly internelly by Gtk but at least in the case
// of GtkCombobox it outputs CRITICAL warning messages at runtime. The solution
// is to defer widget destruction to later through a callback, but it's ugly and
// hard to reason about.
//
// As a sidenote, this is why I try to stay away from callbacks. Although you
// may think they are just function calls, they are being called at a place
// where there is a lot of state in a hidden context. Some things may work, but
// others won't. This is an example of something that breaks, and as far as I
// can tell it behaves differently on different widget types, I think buttons
// and labels don't generate errors.

#include <gtk/gtk.h>
#define combo_box_text_append_text_with_id(combobox,text) \
    gtk_combo_box_text_append (combobox, text, text);

gboolean delete_callback (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    gtk_main_quit ();
    return FALSE;
}

void replace_wrapped_widget (GtkWidget **original, GtkWidget *new_widget)
{
    GtkWidget *parent = gtk_widget_get_parent (*original);
    gtk_container_remove (GTK_CONTAINER(parent), *original);
    *original = new_widget;
    gtk_container_add (GTK_CONTAINER(parent), new_widget);
    gtk_widget_show_all (new_widget);
}

struct replace_wrapped_widget_closure_t {
    GtkWidget **original;
    GtkWidget *parent;
    GtkWidget *new_widget;
};

gboolean idle_widget_destroy (gpointer user_data)
{
    gtk_widget_destroy ((GtkWidget *) user_data);
    return FALSE;
}

void replace_wrapped_widget_defered_cb (GtkWidget *object, gpointer user_data)
{
    struct replace_wrapped_widget_closure_t *clsr = (struct replace_wrapped_widget_closure_t *) user_data;
    *clsr->original = clsr->new_widget;
    gtk_container_add (GTK_CONTAINER(clsr->parent), clsr->new_widget);
    gtk_widget_show_all (clsr->new_widget);
    free (clsr);
}

void replace_wrapped_widget_defered (GtkWidget **original, GtkWidget *new_widget)
{
    struct replace_wrapped_widget_closure_t *clsr = malloc (sizeof (struct replace_wrapped_widget_closure_t));
    clsr->original = original;
    clsr->parent = gtk_widget_get_parent (*original);
    clsr->new_widget = new_widget;
    g_signal_connect (G_OBJECT(*original), "destroy", G_CALLBACK (replace_wrapped_widget_defered_cb), clsr);
    g_idle_add (idle_widget_destroy, *original);
}

gboolean state;
GtkWidget *selector;
GtkWidget* new_test_selector (gboolean is_caps);
void on_active_changed (GtkComboBox *combobox, gpointer user_data)
{
    state = !state;
    GtkWidget *new_selector = new_test_selector (state);
#if 1
    // This causes critical errors
    replace_wrapped_widget (&selector, new_selector);
#else
    // This does not cause any error
    replace_wrapped_widget_defered (&selector, new_selector);
#endif
}

GtkWidget* new_test_selector (gboolean is_caps)
{
    GtkWidget *new_combobox = gtk_combo_box_text_new ();
    GtkWidget *label_widget = gtk_label_new ("Test Combobox");

    GtkWidget *new_selector = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign (GTK_WIDGET(new_selector), GTK_ALIGN_CENTER);
    gtk_container_add (GTK_CONTAINER(new_selector), label_widget);
    gtk_container_add (GTK_CONTAINER(new_selector), new_combobox);

    if (is_caps) {
        combo_box_text_append_text_with_id (GTK_COMBO_BOX_TEXT(new_combobox), "lowercase");
        combo_box_text_append_text_with_id (GTK_COMBO_BOX_TEXT(new_combobox), "to caps");
        gtk_combo_box_set_active_id (GTK_COMBO_BOX(new_combobox), "lowercase");
    } else {
        combo_box_text_append_text_with_id (GTK_COMBO_BOX_TEXT(new_combobox), "TO LOWERCASE");
        combo_box_text_append_text_with_id (GTK_COMBO_BOX_TEXT(new_combobox), "CAPS");
        gtk_combo_box_set_active_id (GTK_COMBO_BOX(new_combobox), "CAPS");
    }

    g_signal_connect (G_OBJECT(new_combobox), "changed", G_CALLBACK (on_active_changed), NULL);
    return new_selector;
}

int main (int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT(window), "delete-event", G_CALLBACK (delete_callback), NULL);

    state = TRUE;
    selector = new_test_selector (state);

    gtk_container_add(GTK_CONTAINER(window), selector);

    gtk_widget_show_all(window);
    gtk_main();
}
