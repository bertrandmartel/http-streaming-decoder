/**
    HttpDecoder.cpp
    HTTP Decoder main class
    Will decode according to CRLF presence or add data to http consumer buffer

    @author Bertrand Martel
    @version 1.0
*/
#include "httpdecoder.h"
#include "httpconstants.h"
#include "httpparser.h"
#include "QString"

using namespace std;

httpdecoder::httpdecoder()
{
    httpconstants::static_init();
}

/**
 * @brief httpDecode
 *      http decoder
 *
 * @param consumer
 *      http client consumer featuring connection monitoring
 * @param data
 *      non-blocking or blocking data stream
 */
void httpdecoder::httpdecode(httpconsumer * consumer,QByteArray* data)
{
    QByteArray value = *data;
    httpparser parser;

    QByteArray currentLine="";

    if (consumer->getBuffer().length()!=0)
    {
        currentLine=consumer->getBuffer();
    }
    currentLine.append(value);

    int indexOfCarriage= currentLine.indexOf("\r\n");

    if (indexOfCarriage!=-1)
    {
        while (indexOfCarriage!=-1)
        {
            currentLine="";

            // parse header
            if (consumer->getBuffer().length()!=0)
            {
                currentLine=consumer->getBuffer();
                consumer->clearBuffer();
            }

            currentLine.append(value);

            indexOfCarriage= currentLine.indexOf("\r\n");

            QByteArray temp = QByteArray(QString(currentLine).toStdString().substr(0,indexOfCarriage).data());

            parser.parseHttp(&temp,consumer);

            if (currentLine.length()>(indexOfCarriage+2))
            {
                value = QString(currentLine).toStdString().substr(indexOfCarriage+2,currentLine.length()).data();

                if (!consumer->getBodyProcess())
                {
                    indexOfCarriage=value.indexOf("\r\n");
                }
                else
                {
                    if (value.length()>=consumer->getBodyLength())
                    {
                        indexOfCarriage=consumer->getBodyLength();
                    }
                    else
                    {
                        indexOfCarriage=value.length();
                    }
                }
            }
            else
            {
                indexOfCarriage=-1;
            }
        }
    }
    else
    {
        // bufferize data
        consumer->appendToBuffer(&value);
    }
}
