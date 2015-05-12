/**
    httpconsumer.cpp
    HTTP Consumer client
    This object permits to store http life cycle into your own design and monitor your parsing in a blocking or non-blocking socket configuration

    @author Bertrand Martel
    @version 1.0
*/
#include "httpconsumer.h"
#include <iostream>
#include "protocol/http/httpparser.h"
#include <QString>
#include "protocol/http/httpstates.h"

using namespace std;

/**
 * @brief httpConsumer::httpConsumer
 *      Consumer client of http decoder<br/>
 *      This object permits to store http life cycle into your own design and monitor your parsing in a blocking or non-blocking socket configuration
 */
httpconsumer::httpconsumer()
{
    QByteArray bufferVal("");
    this->buffer=bufferVal;
    httpState = HTTP_STATE_INIT;
    bodyProcess=false;
    bodyLength=0;
    bodyIndex=0;
    finishedProcessing=false;
    debug=false;
}

/**
 * @brief httpConsumer::~httpConsumer
 *      destruct => clean ptrs
 */
httpconsumer::~httpconsumer()
{
}

/**
 * @brief httpConsumer::clearBuffer
 *      clear current buffer
 */
void httpconsumer::clearBuffer()
{
    buffer.clear();
}

/**
 * @brief httpConsumer::appendToBuffer
 *      append some data to buffer
 *@param data
 *      data to be appended
 * @return
 *      pointer to data
 */
int httpconsumer::appendToBuffer(QByteArray* data)
{
    return buffer.append(data->data()).size();
}

/**
 * @brief httpConsumer::getProcessState
 *      retrieve true if all data has been processed in current buffer
 * @return
 *
 */
bool httpconsumer::isFinishedProcessing()
{
    return finishedProcessing;
}

/**
 * @brief httpConsumer::getBuffer
 *      retrieve current buffer
 *@return
 */
QByteArray httpconsumer::getBuffer()
{
    return buffer;
}

/**
 * @brief setFinishedProcessing
 *      set processing frame state
 * @param processing
 */
void httpconsumer::setFinishedProcessing(bool processing)
{
    finishedProcessing=processing;
}

/**
 * @brief setDebug
 *      set debug mode for consumer
 * @param debug
 */
void httpconsumer::setDebug(bool debugArg)
{
    debug=debugArg;
}

/**
 * @brief bodyIndex
 *      index of body in buffer element
 */
int httpconsumer::getBodyIndex()
{
    return bodyIndex;
}

/**
 * @brief setBodyIndex
 *      set body index
 * @param bodyIndexArg
 */
void httpconsumer::setBodyIndex(int bodyIndexArg)
{
    bodyIndex=bodyIndexArg;
}

/**
 * @brief bodyProcess
 *      get to know if HTTP body is to be parsed or not
 */
bool httpconsumer::getBodyProcess()
{
    return bodyProcess;
}

/**
 * @brief setBodyProcess
 *      set HTTP body parsing state
 * @param processArg
 */
void httpconsumer::setBodyProcess(bool processArg)
{
    bodyProcess=processArg;
}

/**
 * @brief bodyLength
 *      length of body
 */
int  httpconsumer::getBodyLength()
{
    return bodyLength;
}

/**
 * @brief setBodyLength
 *      set body length
 * @param length
 */
void httpconsumer::setBodyLength(int length)
{
    bodyLength=length;
}

/**
 * @brief httpState
 *      http parser current state
 */
int  httpconsumer::getHttpState()
{
    return httpState;
}

/**
 * @brief setHttpState
 *      set http parser state
 * @param http_state
 */
void httpconsumer::setHttpState(int http_state)
{
    httpState=http_state;
}

/**
 * @brief isDebug
 *      debug state
 * @return
 */
bool httpconsumer::isDebug()
{
    return debug;
}

/**
 * @brief getHttpFrameList
 *      retrieve http frame object list
 * @return
 */
std::vector<Ihttpframe*> httpconsumer::getHttpFrameList()
{
    return httpFrameList;
}

/**
 * @brief httpConsumer::addNewHttpFrame
 *      add a new http frame in the list
 * @param frame
 */
void httpconsumer::addNewHttpFrame(Ihttpframe * frame)
{
    httpFrameList.push_back(frame);
}

/**
 * @brief getCurrentHttpFrame
 *      retrieve current http frame object
 * @return
 */
Ihttpframe* httpconsumer::getCurrentHttpFrame()
{
    if (httpFrameList.size()>0)
        return httpFrameList.at(httpFrameList.size()-1);
    else
        return 0;
}
