#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"


#define GEYSER_ID 210

int main(void){

  const char *base_url = "http://localhost:8080/om2m/nscl/";
  int geyser_id = GEYSER_ID;

  long http_status_code = 0;
  int exists = 0;
  char received_content[2048];

  //Create application: 409 is conflict
  http_status_code = createGeyserApplication(base_url, geyser_id);
  printf("Created application: %ld\n", http_status_code);

  //Check if container exits. If not, create it
  if(!containerExists(base_url, geyser_id, "DATA")){
    http_status_code = createGeyserContainer(base_url, geyser_id, "DATA");
    printf("Created container: %ld\n", http_status_code);
  } else{
    printf("Container %s already exists\n", "DATA");
  }

  //Check if container exits. If not, create it
  if(!containerExists(base_url, geyser_id, "SETTINGS")){
    http_status_code = createGeyserContainer(base_url, geyser_id, "SETTINGS");
    printf("Created container: %ld\n", http_status_code);
  }else{
    printf("Container %s already exists\n", "SETTINGS");
  }

  createGeyserContentInstance(base_url, geyser_id, "DATA", "{\'Ver\':20,\'ID\':104,\'Vstate\':\'OPEN\',\'Rstate\':\'ON\',\'Gstate\':\'OK\',\'T1\':47,\'T2\':40,\'T3\':18,\'T4\':18,\'KW\':0.005545,\'KWH\':1.004138,\'HLmin\':0.000000,\'CLmin\':0.000000,\'HLtotal\':7.840000,\'CLtotal\':0.000000,\'Tstamp\':1444632273}");
  readGeyserLatestContentInstance(received_content, base_url, geyser_id, "DATA");
  printf("Recieved: \"%s\"\n", received_content);

  return 0;
}
