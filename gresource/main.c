//gcc -g -o gresource main.c $(pkg-config --cflags --libs glib-2.0 gio-2.0)
/*
 * Copiright (C) 2018 Santiago León O.
 */

#include <glib.h>
#include <stdio.h>

#if 0
// This way of using GResource requires compiling resources with
//
//    $ glib-compile-resources gresources.xml --target=resources.gresource
//
// It generates the file resources.gresource that must be available when the
// application runs. This dose not accomplish the goal of having a binary
// bundled with all data inside, for this use the other #if branch.

int main (void)
{
    GError *err = NULL;
    GResource *resource = g_resource_load ("resources.gresource", &err);
    if (resource == NULL) {
        printf ("Error: %s\n", err->message);
        return -1;
    }

    GBytes *bytes = g_resource_lookup_data (resource, "/file.txt", G_RESOURCE_LOOKUP_FLAGS_NONE, &err);
    if (bytes == NULL) {
        printf ("Error: %s\n", err->message);
        return -1;
    }
    gsize len;
    gconstpointer s = g_bytes_get_data (bytes, &len);
    printf ("%s", (char*)s);
    return 0;
}

#else

// For this to work resources must be compiled using the following commands:
//
//      $ glib-compile-resources gresources.xml --generate-header --c-name=foo
//      $ glib-compile-resources gresources.xml --generate-source --c-name=foo
//
// NOTE: --generate-header and --generate-source can't be used at the same time.
// NOTE: The default value for --c-name is 'gresources'.
//
// Even though GResource can only access files that have been registered,
// normally we only get the function foo_get_resource() to get the resource and
// registration happens automatically. GResource does this by adding a
// constructor and a destructor to the binary. Which must not be confused with
// GTK's constructors and destructors. Binary constructors and destructors are
// provided by the compiler, and are called whenever the binary is loaded or
// unloaded from memory.
//
// If you add the --manual-register to both resource compilation commands then
// GResource does not do any magic. Functions foo_register_resource() and
// foo_unregister_resource() will be created to register and unregister the
// resource. The function foo_register_resource() MUST be called BEFORE
// foo_get_resource(), otherwise NULL is returned.

#include "gresources.h"
// You either have to pass gresources.c to the compiler to be processed as a
// separate compilation unit. Or #include it and have a unity build. Note that
// these are not equivalent, creating a unity build defines functions that
// GResource expected to be private, the only public methods are supposed to be
// the ones in gresources.h. We do this so that we can still compile with the
// same command as above.
#include "gresources.c"

int main (void)
{
    GResource *resource = foo_get_resource ();
    if (resource == NULL) {
        printf ("Error getting resource\n");
        return -1;
    }

    GError *err = NULL;
    GBytes *bytes = g_resource_lookup_data (resource, "/file.txt", G_RESOURCE_LOOKUP_FLAGS_NONE, &err);
    if (bytes == NULL) {
        printf ("Error: %s\n", err->message);
        return -1;
    }
    gsize len;
    gconstpointer s = g_bytes_get_data (bytes, &len);
    printf ("%s", (char*)s);
    return 0;
}

#endif
