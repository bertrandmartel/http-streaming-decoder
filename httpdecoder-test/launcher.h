/**
    launcher.h
    Launch test cases for driving HTTP decoder library
    Display some information about http parser result

    @author Bertrand Martel
    @version 1.0
*/
#ifndef MAIN_H
#define MAIN_H

#include <Httpconsumer.h>
#include "HttpDecoder.h"

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
    static void displayInfo(httpConsumer* consumer);

    static void testStandaloneHttpFrames(httpConsumer * consumer,HttpDecoder decoder);

    static void testStandaloneHttpSeparatedByCRLF(httpConsumer * consumer,HttpDecoder decoder);

    static void testOnCharChunkedHttpFrame(httpConsumer * consumer,HttpDecoder decoder);

    static void testMultipleFrames(httpConsumer * consumer,HttpDecoder decoder);

    static void testMultipleFramesWithError(httpConsumer * consumer,HttpDecoder decoder);

};


#endif // MAIN_H
