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
