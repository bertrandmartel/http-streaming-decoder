/**
    HttpFrame.h
    HTTP frame
    Regroup all http metadata for one http frame

    @author Bertrand Martel
    @version 1.0
*/
#include "Httpframe.h"
#include "string"

using namespace std;

HttpFrame::HttpFrame()
{
    headers=new map<string,string>();
    statusCode=0;
    finishedProcessing=false;
    uri="";
    method="";
    body="";
}

/**
 * @brief HttpFrame::~HttpFrame
 *      destruct => clean ptrs
 */
HttpFrame::~HttpFrame()
{
    delete headers;
    headers=0;
}

/**
 * @brief getUri
 *      retrieve HTTP uri
 * @return
 */
string HttpFrame::getUri()
{
    return uri;
}

/**
 * @brief getMethod
 *      retrieve HTTP method
 * @return
 */
string HttpFrame::getMethod()
{
    return method;
}

/**
 * @brief getBodyLength
 *      retrieve length of body
 * @return
 */
string HttpFrame::getBody()
{
    return body;
}

std::string HttpFrame::getQueryString()
{
    return querystring;
}

int HttpFrame::getStatusCode()
{
    return statusCode;
}

/**
 * @brief getHeaders
 *      retrieve http headers
 * @return
 */
map< std::string,std::string >* HttpFrame::getHeaders()
{
    return headers;
}

/**
 * @brief setFinishedProcessing
 *      set processing frame state
 * @param processing
 */
void HttpFrame::setFinishedProcessing(bool processing)
{
    finishedProcessing=processing;
}
/**
 * @brief setUri
 *      set http uri value
 * @param uri
 */
void HttpFrame::setUri(std::string uriArg)
{
    uri=uriArg;
}

/**
 * @brief setMethod
 *      set http method
 * @param method
 */
void HttpFrame::setMethod(std::string methodArg)
{
    method=methodArg;
}

/**
 * @brief setBody
 *      set http body value
 * @param body
 */
void HttpFrame::setBody(std::string bodyArg)
{
    body=bodyArg;
}

/**
 * @brief setQueryString
 *          set http query string value
 * @param querystring
 */
void HttpFrame::setQueryString(std::string querystringArg)
{
    querystring=querystringArg;
}

/**
 * @brief setStatusCode
 *      set status code value
 * @param statusCode
 */
void HttpFrame::setStatusCode(int statusCodeArg)
{
    statusCode=statusCodeArg;
}
/**
 * @brief httpConsumer::getProcessState
 *      retrieve true if all data has been processed in current buffer
 * @return
 *
 */
bool HttpFrame::isFinishedProcessing()
{
    return finishedProcessing;
}
