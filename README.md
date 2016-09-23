# HTTP streaming decoder library#

[![Build Status](https://travis-ci.org/akinaru/http-streaming-decoder.svg?branch=master)](https://travis-ci.org/akinaru/http-streaming-decoder)
[![License](http://img.shields.io/:license-mit-blue.svg)](LICENSE.md)

http://akinaru.github.io/http-streaming-decoder

C++ HTTP streaming decoder library for Qt4/Qt5

* parse HTTP data streaming
* non-blocking process
* compatible with non-blocking or blocking socket architecture

## Usage

Instanciate `httpdecoder` & `httpconsumer` objects :

```
#include "protocol/http/httpdecoder.h"
#include "protocol/inter/http/httpconsumer.h"

....

httpdecoder decoder;
httpconsumer *consumer = new httpconsumer;
```

* `httpconsumer` will monitor your data streaming decoding and will contain decoded frame(s)

Decode a `QByteArray*` http stream :

```
QByteArray *httpframe = new QByteArray("POST /rest/help/todo HTTP/1.1\r\n"
                                       "headers1:  value1\r\n"
                                       "headers2:  value2\r\n"
                                       "Content-Length:  15\r\n\r\n"
                                       "bodyTobeWritten\r\n");

decoder.httpdecode(consumer, httpFrame);
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

* Note : You must delete your consumer when you are done with it (socket destroyed / destructor ...)

Complete example in [httpdecoder-test/launcher.cpp](httpdecoder-test/launcher.cpp)

## Example

```
httpdecoder decoder;

httpconsumer *consumer = new httpconsumer;

QByteArray *httpframe = new QByteArray("POST /rest/help/todo HTTP/1.1\r\n"
                                       "headers1:  value1\r\n"
                                       "headers2:  value2\r\n"
                                       "Content-Length:  15\r\n\r\n"
                                       "bodyTobeWritten\r\n");

decoder.httpdecode(consumer, httpframe);
```

## Integrate in your project

* from git submodule

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
INCLUDEPATH += $$PWD/../http-streaming-decoder/httpdecoder/release
LIBS += -L$$PWD/../http-streaming-decoder/httpdecoder/release -lhttpdecoder
DEPENDPATH += $$PWD/../http-streaming-decoder/httpdecoder/release
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