# HTTP streaming decoder in C++ #

http://akinaru.github.io/http-streaming-decoder-cpp/

<i>Last update 10/05/2015</i>

HTTP protocol decoder

* parse any HTTP data streaming
* non-blocking process
* compatible with non-blocking or blocking socket architecture
* http decoding monitoring

<hr/>

<b>Generality</b>

This repo features source code and test project for http decoder library.

Library is generated as a shared library (.dll or .so)

This project uses QtCore and is build with qmake utility (you can use QtCreator to build it)

<hr/>

<b>PROGRAM SYNTAX for decoding HTTP frames</b>

First you have to declare the ``HttpDecoder`` object

``HttpDecoder decoder;``

Then instantiate a new ``httpConsumer`` : this object will monitor your data streaming decoding and will contain decoded frame(s)

``httpConsumer *consumer = new httpConsumer;``

For both of them you'll need these import : <br/>
``#include "HttpDecoder.h"``<br/>
``#include "Httpconsumer.h"``<br/>

Your data streaming will be put to a ``QByteArray *`` to be sent to http decoder :<br/>
``QByteArray *httpFrame = new QByteArray("POST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n")``

Eventually decode with : 

``decoder.httpDecode(consumer,httpFrame);``

Result of decoding will be in your pointer to consumer object you have just created

You can access to decoded frame with ``consumer->getHttpFrameList()`` which is a ``vector<httpConsumer*>`` you can iterate

Then you can remove frame you have treated and re-loop to decode again with the same object if you want to

Complete exemple in ./http-decoder-test/launcher.cpp

<hr/>

<b>How to build</b>

This is composed of 2 projects:
* http decoder library project
* http decoder test 

You can open .pro file of both with qtcreator and build project

<hr/>

* Project is Qt4 compliant
* You can build it with qmake
* Development on QtCreator
* Specification from https://www.ietf.org/rfc/rfc2616.txt

TODO : plan to remove QtCore dependencies for a more portable solution
