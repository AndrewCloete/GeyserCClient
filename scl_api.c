#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h> //Replace with AT commands API
#include <string.h>
#include "scl_api.h"

#define BASIC_AUTH "admin:admin"

struct MemoryStruct {
  char *memory;
  size_t size;
};


WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int createGeyserApplication(const char *base_url, int geyser_id){

  long http_status_code = 0;
  char app_url[50];
  char app_xml[400];

  sprintf(app_url, "%sapplications/", base_url);
  sprintf(app_xml, "<om2m:application xmlns:om2m=\"http://uri.etsi.org/m2m\" appId=\"geyser_%d\">"
      "<om2m:searchStrings>"
          "<om2m:searchString>Type/geyser</om2m:searchString>"
       "</om2m:searchStrings>"
  "</om2m:application>", geyser_id);

  // Do POST request ----------------------------------------------------------
  CURL *curl;
  CURLcode res;

   curl_global_init(CURL_GLOBAL_ALL);
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
   /* First set the URL that is about to receive our POST. This URL can
      just as well be a https:// URL if that is what should receive the
      data. */
   curl_easy_setopt(curl, CURLOPT_URL, app_url);
   /* Now specify the POST data */
   curl_easy_setopt (curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
   curl_easy_setopt (curl, CURLOPT_USERPWD, BASIC_AUTH );
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, app_xml);
   //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));

   /* Perform the request, res will get the return code */
   res = curl_easy_perform(curl);
   curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status_code);

   /* Check for errors */
   if(res != CURLE_OK)
     fprintf(stderr, "curl_easy_perform() failed: %s\n",
             curl_easy_strerror(res));

   /* always cleanup */
   curl_easy_cleanup(curl);
  }
  curl_global_cleanup();


  return http_status_code; //HTTP STATUS CODE
}

int createGeyserContainer(const char *base_url, int geyser_id, const char *container_id){

  long http_status_code = 0;
  char container_url[100];
  char container_xml[200];

  sprintf(container_url, "%sapplications/geyser_%d/containers", base_url, geyser_id);
  sprintf(container_xml, "<om2m:container xmlns:om2m=\"http://uri.etsi.org/m2m\" om2m:id=\"%s\"></om2m:container>", container_id);

  // Do POST request ----------------------------------------------------------
  CURL *curl;
  CURLcode res;

   curl_global_init(CURL_GLOBAL_ALL);
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
   curl_easy_setopt(curl, CURLOPT_URL, container_url);
   /* Specify the POST data */
   curl_easy_setopt (curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
   curl_easy_setopt (curl, CURLOPT_USERPWD, BASIC_AUTH );
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, container_xml);
   //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));

   /* Perform the request, res will get the return code */
   res = curl_easy_perform(curl);
   curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status_code);

   /* Check for errors */
   if(res != CURLE_OK)
     fprintf(stderr, "curl_easy_perform() failed: %s\n",
             curl_easy_strerror(res));

   /* always cleanup */
   curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return http_status_code; //HTTP STATUS CODE
}

int createGeyserContentInstance(const char *base_url, int geyser_id, const char *container_id, char *data){

  long http_status_code = 0;
  char content_instance_url[200];

  sprintf(content_instance_url, "%sapplications/geyser_%d/containers/%s/contentInstances", base_url, geyser_id, container_id);

  // Do POST request ----------------------------------------------------------
  CURL *curl;
  CURLcode res;

   curl_global_init(CURL_GLOBAL_ALL);
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
   curl_easy_setopt(curl, CURLOPT_URL, content_instance_url);
   /* Specify the POST data */
   curl_easy_setopt (curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
   curl_easy_setopt (curl, CURLOPT_USERPWD, BASIC_AUTH );
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
   //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));

   /* Perform the request, res will get the return code */
   res = curl_easy_perform(curl);
   curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status_code);

   /* Check for errors */
   if(res != CURLE_OK)
     fprintf(stderr, "curl_easy_perform() failed: %s\n",
             curl_easy_strerror(res));

   /* always cleanup */
   curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  return http_status_code; //HTTP STATUS CODE
}

int readGeyserLatestContentInstance(char *reply, const char *base_url, int geyser_id, const char *container_id){

  long http_status_code = 0;
  char content_instance_url[300];

  // Do GET request ----------------------------------------------------------
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = malloc(1);  // will be grown as needed by the realloc above
  chunk.size = 0;    // no data at this point


  curl_global_init(CURL_GLOBAL_ALL); // get a curl handle
  curl = curl_easy_init();

  sprintf(content_instance_url, "%sapplications/geyser_%d/containers/%s/contentInstances/latest/content", base_url, geyser_id, container_id);

  if(curl) {

   curl_easy_setopt(curl, CURLOPT_URL, content_instance_url);
   curl_easy_setopt (curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
   curl_easy_setopt (curl, CURLOPT_USERPWD, BASIC_AUTH );
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

   // Perform the request, res will get the return code
   res = curl_easy_perform(curl);
   curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status_code);

   // Check for errors
   if(res != CURLE_OK){
     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
   }else{
      strcpy(reply, chunk.memory);
   }


   // always cleanup
   curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  //reply = xmlParse(Base64decode(return msg))

  return http_status_code;
}

int containerExists(const char *base_url, int geyser_id, const char *container_id){
  long http_status_code = 0;
  char container_url[100];

  sprintf(container_url, "%sapplications/geyser_%d/containers/%s", base_url, geyser_id, container_id);

  // Do GET request ----------------------------------------------------------
  CURL *curl;
  CURLcode res;

   curl_global_init(CURL_GLOBAL_ALL);
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
   curl_easy_setopt(curl, CURLOPT_URL, container_url);
   /* Specify the POST data */
   curl_easy_setopt (curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC );
   curl_easy_setopt (curl, CURLOPT_USERPWD, BASIC_AUTH );

   /* Perform the request, res will get the return code */
   res = curl_easy_perform(curl);
   curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_status_code);

   /* Check for errors */
   if(res != CURLE_OK)
     fprintf(stderr, "curl_easy_perform() failed: %s\n",
             curl_easy_strerror(res));

   /* always cleanup */
   curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  if(http_status_code==200)
    return 1;
  else
    return 0;
}
