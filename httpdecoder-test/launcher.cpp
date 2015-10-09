/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Bertrand Martel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @mainpage  HTTP Decoder used with QtCore
 *
 * <ul>
 *  <li>parse any HTTP data streaming</li>
 *  <li>non-blocking decoding process - decoding state and data are stored</li>
 *  <li>compatible with non-blocking or blocking socket architecture</li>
 *  <li>can manage multiple data streaming decoding process for the same instance</li>
 * </ul>
 *
 */

/**
    launcher.cpp
    Launch test cases for driving HTTP decoder library

    @author Bertrand Martel
    @version 1.0
*/
#include "protocol/http/httpdecoder.h"
#include "protocol/inter/http/httpconsumer.h"
#include <QString>
#include <QByteArray>
#include "iostream"

#include "launcher.h"
#include "string"

using namespace std;

//[TEST OK] one standalone http frame
const char * data1 = "POST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";
const char * data2 = "GET /rest/help/todo HTTP/1.1\r\nheaders3:  value3\r\nheaders4:  value4\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";
const char * data3 = "HTTP/1.1 200 OK\r\nheaders5:  value5\r\nheaders6:  value6\r\n\r\n";
const char * data4 = "HTTP/1.1 200 OK\r\n\r\n";
const char * data5 = "POST /rest/help/todo HTTP/1.1\r\nheaders7:  value7\r\nheaders8:  value8\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";

//[TEST OK] one standalone http frame chunked according to carriage return
const char * chunk1 = "HTTP/1.1 200 OK\r\n";
const char * chunk2 = "headers1:  value1\r\n";
const char * chunk3 = "headers2:  value2\r\n";
const char * chunk4 = "Content-Length:  15\r\n\r\n";
const char * chunk5 = "bodyTobeWritten\r\n";

//[TEST OK] one-char-chunked http frame
const string chunkedChar1="POST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";
const string chunkedChar2="GET /rest/help/todo HTTP/1.1\r\nheaders3:  value3\r\nheaders4:  value4\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";
const string chunkedChar3="HTTP/1.1 200 OK\r\nheaders5:  value5\r\nheaders6:  value6\r\n\r\n";
const string chunkedChar4="HTTP/1.1 200 OK\r\n\r\n";
const string chunkedChar5="POST /rest/help/todo HTTP/1.1\r\nheaders7:  value7\r\nheaders8:  value8\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";

//[TEST OK] multiple complete http frames input
const char * multipleFrame1 = "HTTP/1.1 200 OK\r\nheaders5:  value5\r\nheaders6:  value6\r\n\r\nHTTP/1.1 200 OK\r\n\r\n";
const char * multipleFrame2 = "POST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\nHTTP/1.1 200 OK\r\n\r\n";

//[TEST OK] multiple http frames input with error in between frames
const char * multipleFrameWithError1="bodyTobeWritten\r\nHTTP/1.1 200 OK\r\n\r\nPOST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";
const char * multipleFrameWithError2="HTTP/1.1 200 OK\r\n\r\nqsdqsfefneifniznginzieg+1+2+121422°KEF0KEffpskdpfkHr\nPOST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";
const char * multipleFrameWithError3="HTTP/1.1 200 OK\r\n\r\nqsdqsfefneifniznginzieg+1+2+1214\r\n\r\n22°KEF0KEffpskdpfkHr\r\n\r\n\r\n\r\nPOST /rest/help/todo HTTP/1.1\r\nheaders1:  value1\r\nheaders2:  value2\r\nContent-Length:  15\r\n\r\nbodyTobeWritten\r\n";

/**
 * Test cases for http decoder
 *
 * @param args
 */
int main(int argc, char *argv[]) {

    httpdecoder decoder; //must be called only once (TODO: memory leak on multiple call)
    httpconsumer *consumer = new httpconsumer;

    //debug your consumer here
    //consumer->setDebug(true);

    launcher::testStandaloneHttpFrames(consumer,decoder);
    launcher::testStandaloneHttpSeparatedByCRLF(consumer,decoder);
    launcher::testOnCharChunkedHttpFrame(consumer,decoder);
    launcher::testMultipleFrames(consumer,decoder);
    launcher::testMultipleFramesWithError(consumer,decoder);

    launcher::displayInfo(consumer);

    delete consumer;
    consumer=0;

    return 0;
}

/**
 * @brief launcher::testStandaloneHttpFrames
 *      Test for one standalone http frame
 */
void launcher::testStandaloneHttpFrames(httpconsumer * consumer,httpdecoder decoder) {

    QByteArray *httpFrame=0;

    //[TEST OK] one standalone http frame
    cout << "####################################" << endl;
    cout << "HTTP request POST test " << endl;

    httpFrame = new QByteArray(data1);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    cout << "####################################" << endl;
    cout << "HTTP request GET test " << endl;


    httpFrame = new QByteArray(data2);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    cout << "####################################" << endl;
    cout << "HTTP response with body test " << endl;


    httpFrame = new QByteArray(data3);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    cout << "####################################" << endl;
    cout << "HTTP response without body test " << endl;

    httpFrame = new QByteArray(data4);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    cout << "####################################" << endl;
    cout << "HTTP request POST test " << endl;

    httpFrame = new QByteArray(data5);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    //launcher::displayInfo(consumer);

    cout << "####################################" << endl;
}

/**
 * @brief launcher::testStandaloneHttpSeparatedByCRLF
 *      Test for one standalone HTTP frame separated with CRLF (one line at the time)
 */
void launcher::testStandaloneHttpSeparatedByCRLF(httpconsumer * consumer,httpdecoder decoder) {

    QByteArray *httpFrame;

    //[TEST OK] one standalone http frame chunked according to carriage return

    httpFrame = new QByteArray(chunk1);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    httpFrame = new QByteArray(chunk2);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    httpFrame = new QByteArray(chunk3);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    httpFrame = new QByteArray(chunk4);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    httpFrame = new QByteArray(chunk5);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    //launcher::displayInfo(consumer);
    cout << "####################################" << endl;
}

/**
 * @brief launcher::testOnCharChunkedHttpFrame
 *          test for one char at the time
 */
void launcher::testOnCharChunkedHttpFrame(httpconsumer * consumer,httpdecoder decoder) {

    QByteArray *httpFrame;

    //[TEST OK] one-char-chunked http frame

    for (int i = 0; i<chunkedChar2.length() ;i++) {

        httpFrame = new QByteArray();
        httpFrame->append(chunkedChar2[i]);
        decoder.httpdecode(consumer,httpFrame);
        delete httpFrame;
        httpFrame=0;

    }
    //launcher::displayInfo(consumer);
}

/**
 * @brief launcher::testMultipleFrames
 *      test of multiple http frame on the row
 */
void launcher::testMultipleFrames(httpconsumer * consumer,httpdecoder decoder) {

    QByteArray *httpFrame;

    //[TEST OK] multiple complete http frames input

    httpFrame = new QByteArray(multipleFrame2);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    //launcher::displayInfo(consumer);
}

/**
 * @brief launcher::testMultipleFramesWithError
 *      test for error encapsulated in mutliple frames
 */
void launcher::testMultipleFramesWithError(httpconsumer * consumer,httpdecoder decoder) {

    QByteArray *httpFrame;

    //[TEST OK] multiple http frames input with error in between frames

    httpFrame = new QByteArray(multipleFrameWithError3);
    decoder.httpdecode(consumer,httpFrame);
    delete httpFrame;
    httpFrame=0;

    //launcher::displayInfo(consumer);
}

/**
 * @brief launcher::launcher::displayInfo
 *      display all frames decoded with http decoder and display all data from that
 * @param consumer
 */
void launcher::displayInfo(httpconsumer* consumer) {

    cout << "number of http frames detected : " << consumer->getHttpFrameList().size() << endl;

    cout << "####################################" << endl;


    for (int i = 0 ;i  < consumer->getHttpFrameList().size();i++) {

        if (consumer->getHttpFrameList().at(i).isFinishedProcessing()) {


            cout << "####################################" << endl;
            cout << "New HTTP Frame" << endl;
            cout << "uri         : " << consumer->getHttpFrameList().at(i).getUri()         << endl;

            cout << "method      : " << consumer->getHttpFrameList().at(i).getMethod()      << endl;
            cout << "body        : " << consumer->getHttpFrameList().at(i).getBody()        << endl;
            cout << "querystring : " << consumer->getHttpFrameList().at(i).getQueryString() << endl;
            cout << "status code : " << consumer->getHttpFrameList().at(i).getStatusCode()  << endl;
            cout << "headers : " << endl;

            if (consumer->getHttpFrameList().at(i).getHeaders().size()>0){

               typedef std::map<std::string,std::string>::iterator it_type;

               map<string,string> headers = consumer->getHttpFrameList().at(i).getHeaders();

               for (it_type it=headers.begin(); it!=headers.end(); ++it){
                    cout << "\t\t" << it->first << " => " << it->second << '\n';
               }

            }
            cout << "####################################" << endl;

        }
        else {
            cout << "Current HTTP frame has not been processed correctly." << endl;
        }

    }

}
