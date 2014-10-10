/*  =========================================================================
    hello_zproto.h - protocol framework for ZeroMQ

    Copyright (C) 2014 the Authors

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    =========================================================================*/

#ifndef __HELLO_ZPROTO_H_INCLUDED__
#define __HELLO_ZPROTO_H_INCLUDED__

//  HELLO_ZPROTO version macros for compile-time API detection

#define HELLO_ZPROTO_VERSION_MAJOR 0
#define HELLO_ZPROTO_VERSION_MINOR 0
#define HELLO_ZPROTO_VERSION_PATCH 1

#define HELLO_ZPROTO_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define HELLO_ZPROTO_VERSION \
    HELLO_ZPROTO_MAKE_VERSION(HELLO_ZPROTO_VERSION_MAJOR, HELLO_ZPROTO_VERSION_MINOR, HELLO_ZPROTO_VERSION_PATCH)

#include <czmq.h>
#if CZMQ_VERSION < 20200
#   error "HELLO_ZPROTO needs CZMQ/2.2.0 or later"
#endif

#include "hello_zproto_msg.h"
#include "citizen_msg.h"

#endif
