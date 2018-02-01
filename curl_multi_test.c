//gcc -g -o curl_multi curl_multi_test.c -lcurl
/* 
 * Copiright (C) 2017 Santiago León O.
 */

#include <curl/curl.h>
#include <unistd.h>
#include <signal.h>

int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                      curl_off_t ultotal, curl_off_t ulnow)
{
    printf ("\rTotal: %lu, Downloaded: %lu", dltotal, dlnow);
    return 0;
}

#define SIGNAL_HANDLER(name) void name(int i)
typedef SIGNAL_HANDLER(sighandler_t);

sighandler_t *prev_handler;
void sigint_handler (int i)
{
    printf ("SIGINT received\n");
    signal (SIGINT, prev_handler);
    raise (SIGINT);
}

int main (void)
{
    prev_handler = signal (SIGINT, sigint_handler);

    CURLM *multi_h = curl_multi_init ();
    if (multi_h == NULL) {
        return 0;
    }

    CURL *easy_h = curl_easy_init ();
    if (easy_h == NULL) {
        return 0;
    }

    FILE *f = fopen ("512mb_file.zip", "w");
    curl_easy_setopt (easy_h, CURLOPT_URL, "http://ipv4.download.thinkbroadband.com/512MB.zip");
    curl_easy_setopt (easy_h, CURLOPT_WRITEDATA, f);
    curl_easy_setopt (easy_h, CURLOPT_WRITEFUNCTION, NULL); // Required on Windows
    curl_easy_setopt (easy_h, CURLOPT_NOPROGRESS, 0);
    //curl_easy_setopt (easy_h, CURLOPT_XFERINFOFUNCTION, progress_callback);

    curl_multi_add_handle (multi_h, easy_h);

    int still_running;
    CURLMcode rc;
    rc = curl_multi_perform (multi_h, &still_running);
    while (still_running) {
        if (rc != CURLE_OK) {
            printf ("Error: %s\n", curl_easy_strerror (rc));
            break;
        }
        //printf ("%i\n", still_running);
        curl_multi_perform (multi_h, &still_running);
    }

    curl_multi_remove_handle (multi_h, easy_h);
    curl_easy_cleanup (easy_h);
    curl_multi_cleanup (multi_h);
    fclose (f);
    return 0;
}
