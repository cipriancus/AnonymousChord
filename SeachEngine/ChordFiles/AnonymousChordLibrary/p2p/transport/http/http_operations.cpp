#include "http_operations.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <curl/curl.h>
#include <stdio.h>
#include <sstream>

char* process_http(char *host, char *page, char *poststr) {

}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
    ((std::string*) stream)->append((char*) ptr, 0, size * count);
    return size*count;
}

char* sendPost(char *hostName, int port, char *page, char *postString) {

    signal(SIGPIPE, SIG_IGN);

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        std::ostringstream oss;
        oss << "https://" << hostName << page;

        std::string request = oss.str();

        curl_easy_setopt(curl, CURLOPT_URL, oss.str().c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return (char*) response.c_str();
    }
}

char* getLocalIp() {
    //First we get the host-name
    char hostName[255];
    gethostname(hostName, 255);

    //We need to fill a hostent struct
    struct hostent *host_entry;
    host_entry = gethostbyname(hostName);

    //From the hostent struct we obtain our local IP.
    char * hostIP = NULL;

    if (host_entry != NULL)
        hostIP = inet_ntoa(*(struct in_addr *) *host_entry->h_addr_list);

    return hostIP;
}

/*
 * This function assumes the string passed is in the format AAAAAA,POSTMESAGE.
 * so just extracts the second part of the comma separated string and returns the
 * (in this case) ChordTransportCode number according to the message.
 */

int getPostMessage(char *message) {
    int a = -1;
    char *postMessage = NULL;

    postMessage = strtok(message, ",");

    while (postMessage != NULL) {
        postMessage = strtok(NULL, ",");
        if (postMessage != NULL)
            a = atoi(postMessage); //convert the number in "string" format to an int.
    }

    return a;
}
