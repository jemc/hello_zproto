/*  =========================================================================
    citizen_msg - Citizen interaction protocol

    Codec class for citizen_msg.

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

     * The XML model used for this code generation: citizen_msg.xml, or
     * The code generation script that built this file: zproto_codec_c
    ************************************************************************
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
    =========================================================================
*/

/*
@header
    citizen_msg - Citizen interaction protocol
@discuss
@end
*/

#include "hello_zproto.h"
#include "../include/citizen_msg.h"

//  Structure of our class

struct _citizen_msg_t {
    zframe_t *routing_id;               //  Routing_id from ROUTER, if any
    int id;                             //  citizen_msg message ID
    byte *needle;                       //  Read/write pointer for serialization
    byte *ceiling;                      //  Valid upper limit for read pointer
    uint16_t follower_count;            //  
};

//  --------------------------------------------------------------------------
//  Network data encoding macros

//  Put a block of octets to the frame
#define PUT_OCTETS(host,size) { \
    memcpy (self->needle, (host), size); \
    self->needle += size; \
}

//  Get a block of octets from the frame
#define GET_OCTETS(host,size) { \
    if (self->needle + size > self->ceiling) \
        goto malformed; \
    memcpy ((host), self->needle, size); \
    self->needle += size; \
}

//  Put a 1-byte number to the frame
#define PUT_NUMBER1(host) { \
    *(byte *) self->needle = (host); \
    self->needle++; \
}

//  Put a 2-byte number to the frame
#define PUT_NUMBER2(host) { \
    self->needle [0] = (byte) (((host) >> 8)  & 255); \
    self->needle [1] = (byte) (((host))       & 255); \
    self->needle += 2; \
}

//  Put a 4-byte number to the frame
#define PUT_NUMBER4(host) { \
    self->needle [0] = (byte) (((host) >> 24) & 255); \
    self->needle [1] = (byte) (((host) >> 16) & 255); \
    self->needle [2] = (byte) (((host) >> 8)  & 255); \
    self->needle [3] = (byte) (((host))       & 255); \
    self->needle += 4; \
}

//  Put a 8-byte number to the frame
#define PUT_NUMBER8(host) { \
    self->needle [0] = (byte) (((host) >> 56) & 255); \
    self->needle [1] = (byte) (((host) >> 48) & 255); \
    self->needle [2] = (byte) (((host) >> 40) & 255); \
    self->needle [3] = (byte) (((host) >> 32) & 255); \
    self->needle [4] = (byte) (((host) >> 24) & 255); \
    self->needle [5] = (byte) (((host) >> 16) & 255); \
    self->needle [6] = (byte) (((host) >> 8)  & 255); \
    self->needle [7] = (byte) (((host))       & 255); \
    self->needle += 8; \
}

//  Get a 1-byte number from the frame
#define GET_NUMBER1(host) { \
    if (self->needle + 1 > self->ceiling) \
        goto malformed; \
    (host) = *(byte *) self->needle; \
    self->needle++; \
}

//  Get a 2-byte number from the frame
#define GET_NUMBER2(host) { \
    if (self->needle + 2 > self->ceiling) \
        goto malformed; \
    (host) = ((uint16_t) (self->needle [0]) << 8) \
           +  (uint16_t) (self->needle [1]); \
    self->needle += 2; \
}

//  Get a 4-byte number from the frame
#define GET_NUMBER4(host) { \
    if (self->needle + 4 > self->ceiling) \
        goto malformed; \
    (host) = ((uint32_t) (self->needle [0]) << 24) \
           + ((uint32_t) (self->needle [1]) << 16) \
           + ((uint32_t) (self->needle [2]) << 8) \
           +  (uint32_t) (self->needle [3]); \
    self->needle += 4; \
}

//  Get a 8-byte number from the frame
#define GET_NUMBER8(host) { \
    if (self->needle + 8 > self->ceiling) \
        goto malformed; \
    (host) = ((uint64_t) (self->needle [0]) << 56) \
           + ((uint64_t) (self->needle [1]) << 48) \
           + ((uint64_t) (self->needle [2]) << 40) \
           + ((uint64_t) (self->needle [3]) << 32) \
           + ((uint64_t) (self->needle [4]) << 24) \
           + ((uint64_t) (self->needle [5]) << 16) \
           + ((uint64_t) (self->needle [6]) << 8) \
           +  (uint64_t) (self->needle [7]); \
    self->needle += 8; \
}

//  Put a string to the frame
#define PUT_STRING(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER1 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a string from the frame
#define GET_STRING(host) { \
    size_t string_size; \
    GET_NUMBER1 (string_size); \
    if (self->needle + string_size > (self->ceiling)) \
        goto malformed; \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}

//  Put a long string to the frame
#define PUT_LONGSTR(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER4 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a long string from the frame
#define GET_LONGSTR(host) { \
    size_t string_size; \
    GET_NUMBER4 (string_size); \
    if (self->needle + string_size > (self->ceiling)) \
        goto malformed; \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}


//  --------------------------------------------------------------------------
//  Create a new citizen_msg

citizen_msg_t *
citizen_msg_new (int id)
{
    citizen_msg_t *self = (citizen_msg_t *) zmalloc (sizeof (citizen_msg_t));
    self->id = id;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the citizen_msg

void
citizen_msg_destroy (citizen_msg_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        citizen_msg_t *self = *self_p;

        //  Free class properties
        zframe_destroy (&self->routing_id);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Parse a citizen_msg from zmsg_t. Returns a new object, or NULL if
//  the message could not be parsed, or was NULL. Destroys msg and 
//  nullifies the msg reference.

citizen_msg_t *
citizen_msg_decode (zmsg_t **msg_p)
{
    assert (msg_p);
    zmsg_t *msg = *msg_p;
    if (msg == NULL)
        return NULL;
        
    citizen_msg_t *self = citizen_msg_new (0);
    //  Read and parse command in frame
    zframe_t *frame = zmsg_pop (msg);
    if (!frame) 
        goto empty;             //  Malformed or empty

    //  Get and check protocol signature
    self->needle = zframe_data (frame);
    self->ceiling = self->needle + zframe_size (frame);
    uint16_t signature;
    GET_NUMBER2 (signature);
    if (signature != (0xAAA0 | 0))
        goto empty;             //  Invalid signature

    //  Get message id and parse per message type
    GET_NUMBER1 (self->id);

    switch (self->id) {
        case CITIZEN_MSG_HOWAREYOU:
            break;

        case CITIZEN_MSG_STATUS:
            GET_NUMBER2 (self->follower_count);
            break;

        case CITIZEN_MSG_IMLOST:
            break;

        case CITIZEN_MSG_FOLLOWME:
            break;

        case CITIZEN_MSG_LEADME:
            break;

        case CITIZEN_MSG_NOTHANKS:
            break;

        default:
            goto malformed;
    }
    //  Successful return
    zframe_destroy (&frame);
    zmsg_destroy (msg_p);
    return self;

    //  Error returns
    malformed:
        zsys_error ("malformed message '%d'\n", self->id);
    empty:
        zframe_destroy (&frame);
        zmsg_destroy (msg_p);
        citizen_msg_destroy (&self);
        return (NULL);
}


//  --------------------------------------------------------------------------
//  Encode citizen_msg into zmsg and destroy it. Returns a newly created
//  object or NULL if error. Use when not in control of sending the message.

zmsg_t *
citizen_msg_encode (citizen_msg_t **self_p)
{
    assert (self_p);
    assert (*self_p);
    
    citizen_msg_t *self = *self_p;
    zmsg_t *msg = zmsg_new ();

    size_t frame_size = 2 + 1;          //  Signature and message ID
    switch (self->id) {
        case CITIZEN_MSG_HOWAREYOU:
            break;
            
        case CITIZEN_MSG_STATUS:
            //  follower_count is a 2-byte integer
            frame_size += 2;
            break;
            
        case CITIZEN_MSG_IMLOST:
            break;
            
        case CITIZEN_MSG_FOLLOWME:
            break;
            
        case CITIZEN_MSG_LEADME:
            break;
            
        case CITIZEN_MSG_NOTHANKS:
            break;
            
        default:
            zsys_error ("bad message type '%d', not sent\n", self->id);
            //  No recovery, this is a fatal application error
            assert (false);
    }
    //  Now serialize message into the frame
    zframe_t *frame = zframe_new (NULL, frame_size);
    self->needle = zframe_data (frame);
    PUT_NUMBER2 (0xAAA0 | 0);
    PUT_NUMBER1 (self->id);

    switch (self->id) {
        case CITIZEN_MSG_HOWAREYOU:
            break;

        case CITIZEN_MSG_STATUS:
            PUT_NUMBER2 (self->follower_count);
            break;

        case CITIZEN_MSG_IMLOST:
            break;

        case CITIZEN_MSG_FOLLOWME:
            break;

        case CITIZEN_MSG_LEADME:
            break;

        case CITIZEN_MSG_NOTHANKS:
            break;

    }
    //  Now send the data frame
    if (zmsg_append (msg, &frame)) {
        zmsg_destroy (&msg);
        citizen_msg_destroy (self_p);
        return NULL;
    }
    //  Destroy citizen_msg object
    citizen_msg_destroy (self_p);
    return msg;
}


//  --------------------------------------------------------------------------
//  Receive and parse a citizen_msg from the socket. Returns new object or
//  NULL if error. Will block if there's no message waiting.

citizen_msg_t *
citizen_msg_recv (void *input)
{
    assert (input);
    zmsg_t *msg = zmsg_recv (input);
    if (!msg)
        return NULL;            //  Interrupted
    //  If message came from a router socket, first frame is routing_id
    zframe_t *routing_id = NULL;
    if (zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER) {
        routing_id = zmsg_pop (msg);
        //  If message was not valid, forget about it
        if (!routing_id || !zmsg_next (msg))
            return NULL;        //  Malformed or empty
    }
    citizen_msg_t *citizen_msg = citizen_msg_decode (&msg);
    if (citizen_msg && zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER)
        citizen_msg->routing_id = routing_id;

    return citizen_msg;
}


//  --------------------------------------------------------------------------
//  Receive and parse a citizen_msg from the socket. Returns new object,
//  or NULL either if there was no input waiting, or the recv was interrupted.

citizen_msg_t *
citizen_msg_recv_nowait (void *input)
{
    assert (input);
    zmsg_t *msg = zmsg_recv_nowait (input);
    if (!msg)
        return NULL;            //  Interrupted
    //  If message came from a router socket, first frame is routing_id
    zframe_t *routing_id = NULL;
    if (zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER) {
        routing_id = zmsg_pop (msg);
        //  If message was not valid, forget about it
        if (!routing_id || !zmsg_next (msg))
            return NULL;        //  Malformed or empty
    }
    citizen_msg_t *citizen_msg = citizen_msg_decode (&msg);
    if (citizen_msg && zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER)
        citizen_msg->routing_id = routing_id;

    return citizen_msg;
}


//  --------------------------------------------------------------------------
//  Send the citizen_msg to the socket, and destroy it
//  Returns 0 if OK, else -1

int
citizen_msg_send (citizen_msg_t **self_p, void *output)
{
    assert (self_p);
    assert (*self_p);
    assert (output);

    //  Save routing_id if any, as encode will destroy it
    citizen_msg_t *self = *self_p;
    zframe_t *routing_id = self->routing_id;
    self->routing_id = NULL;

    //  Encode citizen_msg message to a single zmsg
    zmsg_t *msg = citizen_msg_encode (self_p);
    
    //  If we're sending to a ROUTER, send the routing_id first
    if (zsocket_type (zsock_resolve (output)) == ZMQ_ROUTER) {
        assert (routing_id);
        zmsg_prepend (msg, &routing_id);
    }
    else
        zframe_destroy (&routing_id);
        
    if (msg && zmsg_send (&msg, output) == 0)
        return 0;
    else
        return -1;              //  Failed to encode, or send
}


//  --------------------------------------------------------------------------
//  Send the citizen_msg to the output, and do not destroy it

int
citizen_msg_send_again (citizen_msg_t *self, void *output)
{
    assert (self);
    assert (output);
    self = citizen_msg_dup (self);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Encode HOWAREYOU message

zmsg_t * 
citizen_msg_encode_howareyou (
)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_HOWAREYOU);
    return citizen_msg_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode STATUS message

zmsg_t * 
citizen_msg_encode_status (
    uint16_t follower_count)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_STATUS);
    citizen_msg_set_follower_count (self, follower_count);
    return citizen_msg_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode IMLOST message

zmsg_t * 
citizen_msg_encode_imlost (
)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_IMLOST);
    return citizen_msg_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode FOLLOWME message

zmsg_t * 
citizen_msg_encode_followme (
)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_FOLLOWME);
    return citizen_msg_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode LEADME message

zmsg_t * 
citizen_msg_encode_leadme (
)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_LEADME);
    return citizen_msg_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode NOTHANKS message

zmsg_t * 
citizen_msg_encode_nothanks (
)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_NOTHANKS);
    return citizen_msg_encode (&self);
}


//  --------------------------------------------------------------------------
//  Send the HOWAREYOU to the socket in one step

int
citizen_msg_send_howareyou (
    void *output)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_HOWAREYOU);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the STATUS to the socket in one step

int
citizen_msg_send_status (
    void *output,
    uint16_t follower_count)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_STATUS);
    citizen_msg_set_follower_count (self, follower_count);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the IMLOST to the socket in one step

int
citizen_msg_send_imlost (
    void *output)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_IMLOST);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the FOLLOWME to the socket in one step

int
citizen_msg_send_followme (
    void *output)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_FOLLOWME);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the LEADME to the socket in one step

int
citizen_msg_send_leadme (
    void *output)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_LEADME);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the NOTHANKS to the socket in one step

int
citizen_msg_send_nothanks (
    void *output)
{
    citizen_msg_t *self = citizen_msg_new (CITIZEN_MSG_NOTHANKS);
    return citizen_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Duplicate the citizen_msg message

citizen_msg_t *
citizen_msg_dup (citizen_msg_t *self)
{
    if (!self)
        return NULL;
        
    citizen_msg_t *copy = citizen_msg_new (self->id);
    if (self->routing_id)
        copy->routing_id = zframe_dup (self->routing_id);
    switch (self->id) {
        case CITIZEN_MSG_HOWAREYOU:
            break;

        case CITIZEN_MSG_STATUS:
            copy->follower_count = self->follower_count;
            break;

        case CITIZEN_MSG_IMLOST:
            break;

        case CITIZEN_MSG_FOLLOWME:
            break;

        case CITIZEN_MSG_LEADME:
            break;

        case CITIZEN_MSG_NOTHANKS:
            break;

    }
    return copy;
}


//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
citizen_msg_print (citizen_msg_t *self)
{
    assert (self);
    switch (self->id) {
        case CITIZEN_MSG_HOWAREYOU:
            zsys_debug ("CITIZEN_MSG_HOWAREYOU:");
            break;
            
        case CITIZEN_MSG_STATUS:
            zsys_debug ("CITIZEN_MSG_STATUS:");
            zsys_debug ("    follower_count=%ld", (long) self->follower_count);
            break;
            
        case CITIZEN_MSG_IMLOST:
            zsys_debug ("CITIZEN_MSG_IMLOST:");
            break;
            
        case CITIZEN_MSG_FOLLOWME:
            zsys_debug ("CITIZEN_MSG_FOLLOWME:");
            break;
            
        case CITIZEN_MSG_LEADME:
            zsys_debug ("CITIZEN_MSG_LEADME:");
            break;
            
        case CITIZEN_MSG_NOTHANKS:
            zsys_debug ("CITIZEN_MSG_NOTHANKS:");
            break;
            
    }
}


//  --------------------------------------------------------------------------
//  Get/set the message routing_id

zframe_t *
citizen_msg_routing_id (citizen_msg_t *self)
{
    assert (self);
    return self->routing_id;
}

void
citizen_msg_set_routing_id (citizen_msg_t *self, zframe_t *routing_id)
{
    if (self->routing_id)
        zframe_destroy (&self->routing_id);
    self->routing_id = zframe_dup (routing_id);
}


//  --------------------------------------------------------------------------
//  Get/set the citizen_msg id

int
citizen_msg_id (citizen_msg_t *self)
{
    assert (self);
    return self->id;
}

void
citizen_msg_set_id (citizen_msg_t *self, int id)
{
    self->id = id;
}

//  --------------------------------------------------------------------------
//  Return a printable command string

const char *
citizen_msg_command (citizen_msg_t *self)
{
    assert (self);
    switch (self->id) {
        case CITIZEN_MSG_HOWAREYOU:
            return ("HOWAREYOU");
            break;
        case CITIZEN_MSG_STATUS:
            return ("STATUS");
            break;
        case CITIZEN_MSG_IMLOST:
            return ("IMLOST");
            break;
        case CITIZEN_MSG_FOLLOWME:
            return ("FOLLOWME");
            break;
        case CITIZEN_MSG_LEADME:
            return ("LEADME");
            break;
        case CITIZEN_MSG_NOTHANKS:
            return ("NOTHANKS");
            break;
    }
    return "?";
}

//  --------------------------------------------------------------------------
//  Get/set the follower_count field

uint16_t
citizen_msg_follower_count (citizen_msg_t *self)
{
    assert (self);
    return self->follower_count;
}

void
citizen_msg_set_follower_count (citizen_msg_t *self, uint16_t follower_count)
{
    assert (self);
    self->follower_count = follower_count;
}



//  --------------------------------------------------------------------------
//  Selftest

int
citizen_msg_test (bool verbose)
{
    printf (" * citizen_msg: ");

    //  @selftest
    //  Simple create/destroy test
    citizen_msg_t *self = citizen_msg_new (0);
    assert (self);
    citizen_msg_destroy (&self);

    //  Create pair of sockets we can send through
    zsock_t *input = zsock_new (ZMQ_ROUTER);
    assert (input);
    zsock_connect (input, "inproc://selftest-citizen_msg");

    zsock_t *output = zsock_new (ZMQ_DEALER);
    assert (output);
    zsock_bind (output, "inproc://selftest-citizen_msg");

    //  Encode/send/decode and verify each message type
    int instance;
    citizen_msg_t *copy;
    self = citizen_msg_new (CITIZEN_MSG_HOWAREYOU);
    
    //  Check that _dup works on empty message
    copy = citizen_msg_dup (self);
    assert (copy);
    citizen_msg_destroy (&copy);

    //  Send twice from same object
    citizen_msg_send_again (self, output);
    citizen_msg_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = citizen_msg_recv (input);
        assert (self);
        assert (citizen_msg_routing_id (self));
        
        citizen_msg_destroy (&self);
    }
    self = citizen_msg_new (CITIZEN_MSG_STATUS);
    
    //  Check that _dup works on empty message
    copy = citizen_msg_dup (self);
    assert (copy);
    citizen_msg_destroy (&copy);

    citizen_msg_set_follower_count (self, 123);
    //  Send twice from same object
    citizen_msg_send_again (self, output);
    citizen_msg_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = citizen_msg_recv (input);
        assert (self);
        assert (citizen_msg_routing_id (self));
        
        assert (citizen_msg_follower_count (self) == 123);
        citizen_msg_destroy (&self);
    }
    self = citizen_msg_new (CITIZEN_MSG_IMLOST);
    
    //  Check that _dup works on empty message
    copy = citizen_msg_dup (self);
    assert (copy);
    citizen_msg_destroy (&copy);

    //  Send twice from same object
    citizen_msg_send_again (self, output);
    citizen_msg_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = citizen_msg_recv (input);
        assert (self);
        assert (citizen_msg_routing_id (self));
        
        citizen_msg_destroy (&self);
    }
    self = citizen_msg_new (CITIZEN_MSG_FOLLOWME);
    
    //  Check that _dup works on empty message
    copy = citizen_msg_dup (self);
    assert (copy);
    citizen_msg_destroy (&copy);

    //  Send twice from same object
    citizen_msg_send_again (self, output);
    citizen_msg_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = citizen_msg_recv (input);
        assert (self);
        assert (citizen_msg_routing_id (self));
        
        citizen_msg_destroy (&self);
    }
    self = citizen_msg_new (CITIZEN_MSG_LEADME);
    
    //  Check that _dup works on empty message
    copy = citizen_msg_dup (self);
    assert (copy);
    citizen_msg_destroy (&copy);

    //  Send twice from same object
    citizen_msg_send_again (self, output);
    citizen_msg_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = citizen_msg_recv (input);
        assert (self);
        assert (citizen_msg_routing_id (self));
        
        citizen_msg_destroy (&self);
    }
    self = citizen_msg_new (CITIZEN_MSG_NOTHANKS);
    
    //  Check that _dup works on empty message
    copy = citizen_msg_dup (self);
    assert (copy);
    citizen_msg_destroy (&copy);

    //  Send twice from same object
    citizen_msg_send_again (self, output);
    citizen_msg_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = citizen_msg_recv (input);
        assert (self);
        assert (citizen_msg_routing_id (self));
        
        citizen_msg_destroy (&self);
    }

    zsock_destroy (&input);
    zsock_destroy (&output);
    //  @end

    printf ("OK\n");
    return 0;
}
