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
    httpparser.cpp
    HTTP Parser
    Will parse streaming data through parseHttp() method

    @author Bertrand Martel
    @version 1.0
*/
#include <cctype>

#include "httpparser.h"
#include <vector>
#include <sstream>
#include <QString>
#include <iostream>
#include "utils/stringutils.h"
#include "httpconstants.h"
#include "httpstates.h"
#include "protocol/inter/http/httpconsumer.h"
#include "protocol/inter/http/httpframe.h"

using namespace std;

std::vector<httpconstants::statusCodeStruct> httpconstants::http_status_code_list;

/**
 * @brief The HttpParser class
 *      Main HTTP Parser<br/>
 *      Will parse streaming data
 */
httpparser::httpparser()
{
    debug=false;
}

/**
 * @brief setDebug
 *      set debug mode for http parser
 * @param debug
 */
void httpparser::setDebug(bool debugArg)
{
    debug=debugArg;
}

/**
 * @brief parseHttp
 *      parse streaming data
 *
 * @param data
 *      streaming data
 */
void httpparser::parseHttp(QByteArray* data,httpconsumer *consumer)
{
    if (debug)
        cout << "http state init : " << consumer->getHttpState() << endl;

    switch (consumer->getHttpState())
    {
        case HTTP_STATE_INIT:
        {
            httpframe frame;

            consumer->addNewHttpFrame(frame);
            consumer->setBodyProcess(false);

            consumer->getCurrentHttpFrame()->setBody("");

            consumer->getCurrentHttpFrame()->getHeaders().clear();

            consumer->setBodyLength(0);
            consumer->setBodyIndex(0);
            consumer->getCurrentHttpFrame()->setFinishedProcessing(false);
            consumer->setFinishedProcessing(false);

            if (debug)
                cout << "HTTP STATE INIT" << endl;
        }
        case HTTP_STATE_VERSION:
        {
            if (debug)
                cout << "HTTP STATE VERSION" << endl;

            std::vector<std::string>  initStateLine =  stringutils::split(QString(data->data()).toStdString(), ' ');

            if (initStateLine.size()>2) {

                QString firstElement(initStateLine.at(0).data());

                QString thirdElement(initStateLine.at(2).data());

                if (firstElement.indexOf("HTTP/")!=-1) {

                    bool isMethodVal  = false;
                    bool isStatusNumVal = false;

                    consumer->getCurrentHttpFrame()->setMethod("");
                    consumer->getCurrentHttpFrame()->setStatusCode(0);
                    consumer->getCurrentHttpFrame()->setUri("");
                    consumer->getCurrentHttpFrame()->setMethod("");
                    consumer->getCurrentHttpFrame()->setQueryString("");

                    isMethodVal=isMethod(initStateLine.at(1));

                    if (isMethodVal) {

                        consumer->getCurrentHttpFrame()->setMethod(initStateLine.at(1));
                        consumer->getCurrentHttpFrame()->setUri(initStateLine.at(2));
                    }
                    else {

                        isStatusNumVal=isStatusNum(initStateLine.at(1));

                        if (isStatusNumVal) {

                            consumer->getCurrentHttpFrame()->setStatusCode(atoi(initStateLine.at(1).data()));
                            consumer->getCurrentHttpFrame()->setQueryString(initStateLine.at(2).data());
                        }
                        else {

                            if (debug)
                                cout  << "Http parse error occured. No http status number or method found." << endl;
                        }
                    }

                    consumer->getCurrentHttpFrame()->getHeaders().clear();

                    consumer->setHttpState(HTTP_STATE_HEADERS);
                }
                else if (thirdElement.indexOf("HTTP/")!=-1) {

                    bool isMethodVal  = false;

                    consumer->getCurrentHttpFrame()->setUri("");
                    consumer->getCurrentHttpFrame()->setMethod("");

                    isMethodVal=isMethod(initStateLine.at(0));

                    if (isMethodVal) {

                        consumer->getCurrentHttpFrame()->setUri(initStateLine.at(1));
                        consumer->getCurrentHttpFrame()->setMethod(initStateLine.at(0));
                    }
                    else {

                       if (debug)
                            cout  << "Http parse error occured. No method found." << endl;
                    }

                    consumer->getCurrentHttpFrame()->getHeaders().clear();

                    consumer->setHttpState(HTTP_STATE_HEADERS);
                }
                else {

                    if (debug)
                        cout  << "Http parse error occured. No http version was specified in header." << endl;
                }
            }
            else {

                if (debug)
                    cout  << "Http parse error occured. Http version header is undefined." << endl;
            }
            break;
        }
        case HTTP_STATE_HEADERS:
        {
            if (debug)
                cout << "HTTP STATE HEADERS" << endl;

            int indexOfPoint = data->indexOf(":");
            if (indexOfPoint!=-1)
            {
               string currentHeader(QString(data->data()).trimmed().toStdString());

               httpframe frame = *consumer->getCurrentHttpFrame();
               map<string,string> headers = frame.getHeaders();

               headers[currentHeader.substr(0,indexOfPoint)]=QString(currentHeader.substr(indexOfPoint+1,currentHeader.length()).data()).trimmed().toStdString();

               consumer->getCurrentHttpFrame()->setHeaders(headers);

                if (debug)
                    cout << currentHeader.substr(0,indexOfPoint).data() << " => " <<currentHeader.substr(indexOfPoint+1,currentHeader.length()).data() <<endl;
            }
            else
            {

                map<string,string> headers =consumer->getCurrentHttpFrame()->getHeaders();

                if (headers.find(HTTP_HEADERS_CONTENT_LENGTH)==headers.end())
                {
                    if (debug)
                        cout << "return to HTTP_INIT state" << endl;
                    consumer->setBodyLength(0);
                    consumer->getCurrentHttpFrame()->setFinishedProcessing(true);
                    consumer->setFinishedProcessing(true);

                    consumer->setHttpState(HTTP_STATE_INIT);
                }
                else
                {
                    if (debug)
                        cout << "continue to HTTP_BODY state" << endl;
                    consumer->setBodyLength(atoi(consumer->getCurrentHttpFrame()->getHeaders()[HTTP_HEADERS_CONTENT_LENGTH].data()));
                    consumer->setHttpState(HTTP_STATE_BODY);
                    consumer->setBodyProcess(true);
                }
            }
            break;
        }
        case HTTP_STATE_BODY:
        {
            if (debug)
                cout << "HTTP STATE BODY" << endl;
            if (consumer->getBodyLength()==0)
            {
                if (debug)
                    cout << "no body to read" << endl;
                consumer->setBodyProcess(false);
                consumer->setBodyLength(0);
                consumer->getCurrentHttpFrame()->setFinishedProcessing(true);
                consumer->setFinishedProcessing(true);
                consumer->setHttpState(HTTP_STATE_INIT);
            }
            else if (data->length()>=consumer->getBodyLength() && consumer->getBodyIndex()==0)
            {
                consumer->getCurrentHttpFrame()->setBody(QString(data->data()).toStdString().substr(0,consumer->getBodyLength()));

                consumer->setBodyProcess(false);
                consumer->setBodyLength(0);
                consumer->setBodyIndex(0);

                consumer->getCurrentHttpFrame()->setFinishedProcessing(true);
                consumer->setFinishedProcessing(true);
                consumer->setHttpState(HTTP_STATE_INIT);
            }
            else
            {
                if (debug)
                    cout << "not all body can be read";
                if (consumer->getBodyIndex()==0 && data->length()>=consumer->getBodyLength())
                {
                    consumer->getCurrentHttpFrame()->setBody(QString(data->data()).toStdString().substr(0,data->length()));

                    consumer->setBodyProcess(false);
                    consumer->setBodyLength(0);
                    consumer->setBodyIndex(0);

                    consumer->getCurrentHttpFrame()->setFinishedProcessing(true);
                    consumer->setFinishedProcessing(true);
                    consumer->setHttpState(HTTP_STATE_INIT);
                }
                else
                {
                    if (data->length()>=(consumer->getBodyLength()-consumer->getBodyIndex()))
                    {
                        std::string bodyTemp = consumer->getCurrentHttpFrame()->getBody();

                        bodyTemp+=QString(data->data()).toStdString().substr(0,consumer->getBodyLength()-consumer->getBodyIndex());

                        consumer->getCurrentHttpFrame()->setBody(bodyTemp);

                        consumer->setBodyProcess(false);
                        consumer->setBodyLength(0);
                        consumer->setBodyIndex(0);

                        consumer->getCurrentHttpFrame()->setFinishedProcessing(true);
                        consumer->setFinishedProcessing(true);
                        consumer->setHttpState(HTTP_STATE_INIT);
                    }
                    else
                    {
                        std::string bodyTemp = consumer->getCurrentHttpFrame()->getBody();
                        bodyTemp+=QString(data->data()).toStdString().substr(0,data->length());
                        consumer->getCurrentHttpFrame()->setBody(bodyTemp);

                        int bodyIndexTemp = consumer->getBodyIndex();
                        bodyIndexTemp+=data->length();
                        consumer->setBodyIndex(bodyIndexTemp);
                    }
                }
            }
            break;
        }
    }
}

/**
 * @brief isMethod
 *      check if input data is a valid HTTP rest method
 *
 * @param data
 *      data to be tested
 * @return
 *      true if data is valid REST HTTP method
 */
bool httpparser::isMethod(std::string data) {

    if (strcmp(data.data(),HTTP_METHOD_GET)==0 || strcmp(data.data(),HTTP_METHOD_POST)==0 || strcmp(data.data(),HTTP_METHOD_PUT)==0 || strcmp(data.data(),HTTP_METHOD_DELETE)==0) {

        return true;

    }
    return false;
}

/**
 * @brief isStatusNum
 *     check if input data is a valid HTTP status code
 * @param data
 *      data to be tested
 * @return
 *      true if data is valid HTTP status code
 */
bool httpparser::isStatusNum(std::string data) {

    if (stringutils::isNum((char*)data.data())) {

        int code = atoi(data.data());

        for(std::vector<httpconstants::statusCodeStruct>::iterator it = httpconstants::http_status_code_list.begin(); it != httpconstants::http_status_code_list.end(); ++it) {

            if (code==(*it).code_value)
                return true;
        }
    }
    return false;
}
