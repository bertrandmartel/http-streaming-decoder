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
