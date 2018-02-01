//gcc -o curl_test curl_test.c -lcurl
/* 
 * Copiright (C) 2017 Santiago León O.
 */

#include <curl/curl.h>

int main (void)
{
    CURL *h = curl_easy_init ();
    if (h) {
        FILE *f = fopen ("otypes04.b08", "w");
        curl_easy_setopt (h, CURLOPT_URL, "http://www.ist.tugraz.at/aichholzer/research/rp/triangulations/ordertypes/data/otypes04.b08");
        curl_easy_setopt (h, CURLOPT_WRITEDATA, f);
        curl_easy_setopt (h, CURLOPT_WRITEFUNCTION, NULL); // Required on Windows
        curl_easy_perform (h);
        curl_easy_cleanup (h);
        fclose (f);
    }
    return 0;
}
