# Geyser M2M backend architecture

## MK3 design

### Intro
*Time is of the essence.*

We will develop the firmware based on the ETSI standard, and if there is time in
December, the code will be adapted to fit in with the oneM2M standard. In either
case the HTTP protocol will be used.

### Communication

**Start out by calculating if the device will be able to support XML**

ATxmega128A4-AU:

* Flash 128 kB. (Dual 64 kB)
* EEPROM 2 kB
* SRAM 8 kB
* Speed 32 MHz

### Architectural design options.

First of all **make sure to fully understand the REST API of OM2M**. From there
on I can think of a couple of possible architectures:

1. Full ETSI REST client.

    The client application is completely in charge of registering and managing
    itself at the SCL.

2. OSGi IPU plugin.

    Investigate if this solution will actually reduce any client load.

3. Application Point of Contact (aPoC) proxy.

    (Ensure that OM2M does in fact support this)
    A full custom RESTful web service will need to be developed except for the fact
    that it will register at the SCL at startup, and all requests will first go to
    the SCL application and then be redirected to the service using an aPoC. Any URI
    resource element AFTER the aPoC will map to the resources on the IPU.

    The client will therefore be talking directly to the SCL except that it will
    be using resource constraint representation content (simple JSON). The client
    will think that it is some weird hybrid of the ETSI M2M interface with custom
    URI's and content. The way to fix this (according to Actility.com), is to include
    a DESCRIPTOR container which contains all the *schemas* needed to understand the
    custom representation (for a start, we most likely won't be needing this.)

4. Custom subscribe/notify proxy (current NIP).

    Similar to case three except that this time the SCL will be unaware of the
    existence of the proxy and from its point of view, all geysers will look fully
    ETSI M2M compliant. The geyser clients in turn will be unaware of the SCL and
    will only think that it is talking to yet another *ad hoc* M2M solution.

5. Wakaama client.

    **First learn C better and understand function pointers.** Then evaluate the
    Wakaama client and decide whether it is feasible to use it as a client. This
    will include:
    * Figuring out how to implement the UDP-IP layer on the Atmel
    * Understanding how the LWM2M protocol works.
    * Understanding how LWM2M fits into the new oneM2M complaint OM2M platform.

    The following is still unclear to me about the LWM2M protocol.
    * How it fits into oneM2M
    * How DTSL and and security keys work (pre-shared and public keys).
    * What the objects structure look like and how they are serialized.
    * How objects get populated with measurement data.
    
### Embedded questions
* Is XML feasible?
* We plan on using AT+HTTP commands, which stores all results as files on the modem:
  * Would the firmware need to be base64 encoded or can a HEX file be transported
  over HTTP?
  * Can base64 decoding happen continuously? If not, how will the firmware be
  delivered to the client.
* Over the air firmware upgrades. What does the boot-loading design look like?

Do not consider removing the OM2M platform and replacing it with your own PHP
server. This will only look good on your masters if you are able to out perform
OM2M (which won't happen).

#### HTTP design aspects:
* AT command interface on modem. i.e. no IP-stack on controller for now (simple).
* POST data points every minute.
* GET command every minute.
* Calculate estimated transaction data consumption.
* The LEON modem does not support HTTPS. How should security be implemented?
* How does a binary file get sent over a GET request in order to perform firmware
updates?

#### JSON:
* Which parameters should be posted every minute?
* What should the command/response mechanism look like (for firmware updates, system info
  and control)?
* What should the JSON structure for DATA, COMMANDS and REPLY's look like?

#### Network registration boot.
* How should geyser ID's be handled?
* How should IMSI and IMEI number be managed, and how should tampering be prevented?

#### Proxy
In the case that the controller can not meet the XML requirements, a proxy will
need to be used. The proxy will be redesigned from the ground up using lessons
learned form the NIP. If time allows it, an OSGi IPU will be developed, but otherwise
a custom RESTful proxy will have to do (most likely using Symphony with JSON or
we can wait and see what the OM2M Java API looks like). The main point is though
that for MK3, AT+HTTP will still be used. It will only be MK4 where a better M2M
protocol will be used such as MQTT or CoAP, in which case the an IP stack will
be implemented on the controller.

Note that if the plan is to develop an OSGi plugin, then it means that you most
probably wont be able to use OM2M as a SAAS at a remote server. Unless you can
somehow give the IPU to the NSCL service provider or, you have to write it for
your GSCL.

First figure out exactly how the IPU's work. Because it would seem that there is
a difference between OM2M and Cocoon, but obviously there cannot be!

http://www.mysmartgeyser.co.za:8080/om2m/nscl/applications/geyser_1/containers/DATA/contentInstances/latest/content

### Data management
How can ALL data be retrieved from xSCL through using the OM2M API instead of querying
a database directly?

The tables of the database must be redesigned to include all the lessons learned.
This would include:

* User profile with an authentication and authorization mechanism.
* System information such as firmware version, watchdog count, uptime etc.

Does it make sense to redesign the database ApoC application to be PHP? Or should
we just stick to Java and hope that an improved API is on its way? PHP has
excellent support for XML, JSON and database interaction. And if it is combined
with Symphony it would be a good opportunity for me to learn.

### Reporting
* Currently using Latex with R script, and data is read directly from database.


### Security

Should the server be limited to only our devices? Yes. We are not at the moment
providing a generic M2M platform. Be careful of trying to re-implement what the
OM2M platform is already doing.  

How does Discovery and AccessRights on OM2M work?

## MK4 design

Communication protocol options:
* HTTP over AT commands (IP stack on modem)
* HTTP over PPP with TCP
* CoAP over PPP with TCP
* MQTT over PPP with TCP

1. Maybe. We could possibly develop a generic appliance platform that builds on top
of the OM2M platform but uses our Web and App interface.  

## Ideas

* QR codes on devices

* 3 containers
  * DATA  
  * COMMAND
  * REPLY

Each command must have a transaction ID and any replies will then have the same
ID in the REPLY container.
