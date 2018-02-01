//gcc -o pango_test pango_test.c -lpango-1.0 -lpangocairo-1.0 -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include

/*
 * Copiright (C) 2018 Santiago León O.
 */

#include <cairo/cairo-xlib.h>
#include <pango/pangocairo.h>

int main (void)
{
    //PangoLayout *text_layout = pango_cairo_create_layout (cr);
    //PangoFontDescription *font_desc = pango_font_description_from_string ("Open Sans 9");
    //pango_layout_set_font_description (text_layout, font_desc);
    //pango_font_description_free (font_desc);

    PangoFontMap *fontmap = pango_cairo_font_map_get_default ();
    PangoContext *ctx = pango_font_map_create_context (fontmap);
    PangoFontFamily **ff;
    int num_ff;
    pango_context_list_families (ctx, &ff, &num_ff);

    printf ("num_families: %d\n", num_ff);
    int i;
    for (i=0; i<num_ff; i++) {
        const char *str = pango_font_family_get_name (ff[i]); 
        printf ("%s\n", str);
    }
}
