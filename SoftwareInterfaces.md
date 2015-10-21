# Software building blocks and interfaces.

## Intro
The purpose of this document is to describe the separate building blocks of the
firmware, how they relate to each other and how they interface.

## Communication hierarchy
1. Data serializer (JSON, oBIX, binary, plain text, etc)
2. SCL API
3. HTTP API (later COaP or MQTT)
4. Modem interface.
5. Serial interface


## Data serializer
The purpose of this module is to serialize variables into a representation such
as JSON, oBIX, plain text, etc.

The variables of interest should be placed in a defined structure and the serializer
should then be able to serialize using a reference to this structure.

NOTE: This is NOT a serializer for the SCL interface. This is for the data blob
that is stored at the lowest level of the SCL resource tree.

## SCL API
This module provides the necessary functions to interact with an xSCL in a RESTful
manner. It must be able to manage SCL applications and containers. This module must
interface with the serializer to obtain the content representations. This module
must be unaware of the underlying transport protocol i.e. HTTP, COaP, MQTT.

## HTTP API
This module provides an HTTP interface the the SCL-API can use to make RESTful
requests to the xSCL. It uses the modem interface to execute the requests.

The outbound interface presented by this module must remain constant irrespective of the
underlying communication protocol (High level HTTP AT commands VS. Point-to-point)

Since the HTTP AT commands will be used initially, this module might seem
unnecessary, but it is needed to for future implementations. The underlying implementation
will change when the modem is switched to PPP.


## Modem interface
The module provides a programming interface to make and process the AT commands
required for this application.

The outbound interface CANNOT be generic since it is dependent on the communication
layer i.e. high level HTTP AT commands VS. Point-to-point

Later, when an IP stack is implemented, this module will updated to use PPP.

### Pseudo functions: General
```c
int init();
int turnEchoOff();
int modemInfo();
int networkAttach();
int getTime();
int readFile();
int sendSMS()
...
```
### Pseudo functions: AT+HTTP
```c
int doGet_request();
int doPut_request();
int doPost_request();
int doDelete_request();
```

### Pseudo functions: PPP
(No idea how this works yet)


## Serial interface
A set of high level serial functions that perform a variety of tasks.

### Pseudo functions

```c
int basicATcommand(char* reply, const char* command);
int extendedATcommand();
int debug(const char* debug_msg)
```


<img src="/home/andrew/Pictures/butler.jpg" alt="Drawing" style="height:200px;"/>
