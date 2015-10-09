/**
    HttpFrame.h
    HTTP frame
    Regroup all http metadata for one http frame

    @author Bertrand Martel
    @version 1.0
*/
#include "httpframe.h"
#include "string"

using namespace std;

httpframe::httpframe()
{
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
httpframe::~httpframe()
{
}

/**
 * @brief getUri
 *      retrieve HTTP uri
 * @return
 */
string httpframe::getUri()
{
    return uri;
}

/**
 * @brief getMethod
 *      retrieve HTTP method
 * @return
 */
string httpframe::getMethod()
{
    return method;
}

/**
 * @brief getBodyLength
 *      retrieve length of body
 * @return
 */
string httpframe::getBody()
{
    return body;
}

std::string httpframe::getQueryString()
{
    return querystring;
}

int httpframe::getStatusCode()
{
    return statusCode;
}

/**
 * @brief getHeaders
 *      retrieve http headers
 * @return
 */
map< std::string,std::string > httpframe::getHeaders()
{
    return headers;
}

/**
 * @brief setFinishedProcessing
 *      set processing frame state
 * @param processing
 */
void httpframe::setFinishedProcessing(bool processing)
{
    finishedProcessing=processing;
}
/**
 * @brief setUri
 *      set http uri value
 * @param uri
 */
void httpframe::setUri(std::string uriArg)
{
    uri=uriArg;
}

/**
 * @brief setMethod
 *      set http method
 * @param method
 */
void httpframe::setMethod(std::string methodArg)
{
    method=methodArg;
}

/**
 * @brief setBody
 *      set http body value
 * @param body
 */
void httpframe::setBody(std::string bodyArg)
{
    body=bodyArg;
}

/**
 * @brief setQueryString
 *          set http query string value
 * @param querystring
 */
void httpframe::setQueryString(std::string querystringArg)
{
    querystring=querystringArg;
}

/**
 * @brief setStatusCode
 *      set status code value
 * @param statusCode
 */
void httpframe::setStatusCode(int statusCodeArg)
{
    statusCode=statusCodeArg;
}
/**
 * @brief httpConsumer::getProcessState
 *      retrieve true if all data has been processed in current buffer
 * @return
 *
 */
bool httpframe::isFinishedProcessing()
{
    return finishedProcessing;
}

void httpframe::setHeaders(std::map<std::string,std::string> header){
    this->headers=header;
}
