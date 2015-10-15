geyserclient_make: client.c scl_api.c
					gcc -o client client.c scl_api.c -I. -lcurl 
