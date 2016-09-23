# HTTP streaming decoder #

[![Build Status](https://travis-ci.org/akinaru/http-streaming-decoder.svg?branch=master)](https://travis-ci.org/akinaru/http-streaming-decoder)
[![License](http://img.shields.io/:license-mit-blue.svg)](LICENSE.md)

http://akinaru.github.io/http-streaming-decoder

C++ HTTP streaming decoder for Qt4/Qt5

* parse HTTP data streaming
* non-blocking process
* compatible with non-blocking or blocking socket architecture

## Usage

First you have to declare the `httpdecoder` object

```
httpdecoder decoder;
```

Then instantiate a new `httpconsumer` : this object will monitor your data streaming decoding and will contain decoded frame(s)

```
httpconsumer *consumer = new httpconsumer;
```

For both of them you'll need these import :
```
#include "protocol/http/httpdecoder.h"
#include "protocol/inter/http/httpconsumer.h"
```

Your data streaming will be put to a `QByteArray *` to be sent to http decoder :
```
QByteArray *httpframe = new QByteArray("POST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n")
```

Eventually decode with : 

```
decoder.httpdecode(consumer,httpFrame);
```

* Result of decoding will be in your pointer to consumer object you have just created

* You can access to decoded frame with `consumer->getHttpFrameList()` which is a `vector<httpconsumer*>` you can iterate

* Then you can remove frame you have treated and re-loop to decode again with the same object

* You must delete your consumer when you are done with it (socket destroyed / destructor ...)

Complete example in [httpdecoder-test/launcher.cpp](httpdecoder-test/launcher.cpp)

## Example

```
httpdecoder decoder;

httpconsumer *consumer = new httpconsumer;

QByteArray *httpframe = "POST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\nHTTP/1.1 200 OK\r\n\r\n";

httpframe = new QByteArray(data1);

decoder.httpdecode(consumer,httpframe);
```

From consumer object `consumer->getHttpFrameList()` you can extract those fields : 

|  method name       | return data type                  |  description  |  example                |
| -------------------| ----------------------------------|---------------|-------------------------|
| `getUri()`         | `std::string`                       | http uri           |  "/api/rest"              |
| `getMethod()`      | `std::string`                       | http method        |  "POST"                   |
| `getBody()`        | `std::string`                       | http body          |  "{\"data\":\"OK\"}"          |
| `getQueryString()` | `std::string`                       | http querystring   |  "Not Found"              |
| `getStatusCode()`  | `int`                               | http status code   |  404                    |
| `getHeaders()`     | `std::map<std::string,std::string>` | http headers       | ("Content-Length","15") |

## Integrate in your project

* from a submodule

```
git submodule add git://github.com/akinaru/http-streaming-decoder.git
```

and in your `project.pro` :

```
TEMPLATE = subdirs
SUBDIRS = http-streaming-decoder your-app
your-app.depends = http-streaming-decoder
```

with in `your-app.pro` :

```
TARGET = your-app
SOURCES = main.cpp
INCLUDEPATH += ../http-streaming-decoder/httpdecoder/release
LIBS += -L../http-streaming-decoder/httpdecoder/release -lhttpdecoder
```

## Build library

```
qmake
make
```

## Projects using this library

* https://github.com/akinaru/websocket-non-blocking

## Compatibility

* Qt4
* Qt5

## Specification

* https://www.ietf.org/rfc/rfc2616.txt