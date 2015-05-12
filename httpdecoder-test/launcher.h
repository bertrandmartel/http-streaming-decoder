/**
    launcher.h
    Launch test cases for driving HTTP decoder library
    Display some information about http parser result

    @author Bertrand Martel
    @version 1.0
*/
#ifndef MAIN_H
#define MAIN_H

#include <protocol/inter/http/httpconsumer.h>
#include "protocol/http/httpdecoder.h"

class launcher
{
    public:

    /**
     * @brief displayInfo
     *          display informationabout http parser
     *
     * @param consumer
     *      consumer client
     */
    static void displayInfo(httpconsumer* consumer);

    static void testStandaloneHttpFrames(httpconsumer * consumer,httpdecoder decoder);

    static void testStandaloneHttpSeparatedByCRLF(httpconsumer * consumer,httpdecoder decoder);

    static void testOnCharChunkedHttpFrame(httpconsumer * consumer,httpdecoder decoder);

    static void testMultipleFrames(httpconsumer * consumer,httpdecoder decoder);

    static void testMultipleFramesWithError(httpconsumer * consumer,httpdecoder decoder);

};


#endif // MAIN_H
