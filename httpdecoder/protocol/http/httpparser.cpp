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
            consumer->addNewHttpFrame(new httpframe());
            consumer->setBodyProcess(false);

            consumer->getCurrentHttpFrame()->setBody("");

            //TODO:test if pointer null here

            (*consumer->getCurrentHttpFrame()->getHeaders()).clear();

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

            if (initStateLine.size()>2)
            {
                QString firstElement(initStateLine.at(0).data());

                QString thirdElement(initStateLine.at(2).data());

                if (firstElement.indexOf("HTTP/")!=-1)
                {
                    bool isMethodVal  = false;
                    bool isStatusNumVal = false;

                    consumer->getCurrentHttpFrame()->setMethod("");
                    consumer->getCurrentHttpFrame()->setStatusCode(0);
                    consumer->getCurrentHttpFrame()->setUri("");
                    consumer->getCurrentHttpFrame()->setMethod("");
                    consumer->getCurrentHttpFrame()->setQueryString("");

                    QString secondElement(initStateLine.at(1).data());
                    isMethodVal=isMethod(secondElement.toLocal8Bit().data());
                    if (isMethodVal)
                    {
                        consumer->getCurrentHttpFrame()->setMethod(secondElement.toStdString());
                        consumer->getCurrentHttpFrame()->setUri(initStateLine.at(2));
                    }
                    else
                    {
                        isStatusNumVal=isStatusNum(secondElement.toLocal8Bit().data());

                        if (isStatusNumVal)
                        {
                            consumer->getCurrentHttpFrame()->setStatusCode(atoi(secondElement.toLocal8Bit().data()));
                            consumer->getCurrentHttpFrame()->setQueryString(initStateLine.at(2).data());
                        }
                        else
                        {
                            if (debug)
                                cout  << "Http parse error occured. No http status number or method found." << endl;
                        }
                    }

                    (*consumer->getCurrentHttpFrame()->getHeaders()).clear();

                    consumer->setHttpState(HTTP_STATE_HEADERS);
                }
                else if (thirdElement.indexOf("HTTP/")!=-1)
                {
                    bool isMethodVal  = false;

                    consumer->getCurrentHttpFrame()->setUri("");
                    consumer->getCurrentHttpFrame()->setMethod("");

                    QString firstElement(initStateLine.at(0).data());
                    isMethodVal=isMethod(firstElement.toLocal8Bit().data());

                    if (isMethodVal)
                    {
                        consumer->getCurrentHttpFrame()->setUri(initStateLine.at(1));
                        consumer->getCurrentHttpFrame()->setMethod(firstElement.toStdString());
                    }
                    else
                    {
                       if (debug)
                            cout  << "Http parse error occured. No method found." << endl;
                    }

                    (*consumer->getCurrentHttpFrame()->getHeaders()).clear();

                    consumer->setHttpState(HTTP_STATE_HEADERS);
                }
                else
                {
                    if (debug)
                        cout  << "Http parse error occured. No http version was specified in header." << endl;
                }
            }
            else
            {
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
               string currentHeader(QString(data->data()).toStdString());

                (*consumer->getCurrentHttpFrame()->getHeaders())[currentHeader.substr(0,indexOfPoint)]=currentHeader.substr(indexOfPoint+1,currentHeader.length());
                if (debug)
                    cout << currentHeader.substr(0,indexOfPoint).data() << " => " <<currentHeader.substr(indexOfPoint+1,currentHeader.length()).data() <<endl;
            }
            else
            {
                if ((*consumer->getCurrentHttpFrame()->getHeaders()).find(HTTP_HEADERS_CONTENT_LENGTH)==(*consumer->getCurrentHttpFrame()->getHeaders()).end())
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
                    consumer->setBodyLength(atoi((*consumer->getCurrentHttpFrame()->getHeaders())[HTTP_HEADERS_CONTENT_LENGTH].data()));
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
bool httpparser::isMethod(char* data)
{
    if (strcmp(data,HTTP_METHOD_GET)==0 || strcmp(data,HTTP_METHOD_POST)==0 || strcmp(data,HTTP_METHOD_PUT)==0 || strcmp(data,HTTP_METHOD_DELETE)==0)
    {
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
bool httpparser::isStatusNum(char * data)
{

    if (stringutils::isNum(data))
    {
        int code = atoi(data);
        for(std::vector<httpconstants::statusCodeStruct>::iterator it = httpconstants::http_status_code_list.begin(); it != httpconstants::http_status_code_list.end(); ++it) {
            if (code==(*it).code_value)
                return true;
        }
    }
    return false;
}
