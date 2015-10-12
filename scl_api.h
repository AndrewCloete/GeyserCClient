#ifndef _SCLAPI_H_
#define _SCLAPI_H_

int createGeyserApplication(const char *base_url, int geyser_id);
int containerExists(const char *base_url, int geyser_id, const char *container_id);
int createGeyserContainer(const char *base_url, int geyser_id, const char *container_id);
int createGeyserContentInstance(const char *base_url, int geyser_id, const char *container_id, char *data);
int readGeyserLatestContentInstance(char *reply, const char *base_url, int geyser_id, const char *container_id);


#endif //_SCLAPI_H_
