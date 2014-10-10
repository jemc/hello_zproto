/*  =========================================================================
    citizen_msg - Citizen interaction protocol
    
    Codec header for citizen_msg.

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

#ifndef __CITIZEN_MSG_H_INCLUDED__
#define __CITIZEN_MSG_H_INCLUDED__

/*  These are the citizen_msg messages:

    IMLOST - Let another Citizen know that you're in need of direction.

    FOLLOWME - Request the devotion of another Citizen,
taking responsibility for giving them direction.

    LEADME - Pledge your sole devotion to a single Citizen,
agreeing to follow their direction.

    NOTHANKS - Your devotion is already pledged to another.
*/

#define CITIZEN_MSG_VERSION                 1

#define CITIZEN_MSG_IMLOST                  1
#define CITIZEN_MSG_FOLLOWME                2
#define CITIZEN_MSG_LEADME                  3
#define CITIZEN_MSG_NOTHANKS                4

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _citizen_msg_t citizen_msg_t;

//  @interface
//  Create a new citizen_msg
citizen_msg_t *
    citizen_msg_new (int id);

//  Destroy the citizen_msg
void
    citizen_msg_destroy (citizen_msg_t **self_p);

//  Parse a citizen_msg from zmsg_t. Returns a new object, or NULL if
//  the message could not be parsed, or was NULL. Destroys msg and 
//  nullifies the msg reference.
citizen_msg_t *
    citizen_msg_decode (zmsg_t **msg_p);

//  Encode citizen_msg into zmsg and destroy it. Returns a newly created
//  object or NULL if error. Use when not in control of sending the message.
zmsg_t *
    citizen_msg_encode (citizen_msg_t **self_p);

//  Receive and parse a citizen_msg from the socket. Returns new object, 
//  or NULL if error. Will block if there's no message waiting.
citizen_msg_t *
    citizen_msg_recv (void *input);

//  Receive and parse a citizen_msg from the socket. Returns new object, 
//  or NULL either if there was no input waiting, or the recv was interrupted.
citizen_msg_t *
    citizen_msg_recv_nowait (void *input);

//  Send the citizen_msg to the output, and destroy it
int
    citizen_msg_send (citizen_msg_t **self_p, void *output);

//  Send the citizen_msg to the output, and do not destroy it
int
    citizen_msg_send_again (citizen_msg_t *self, void *output);

//  Encode the IMLOST 
zmsg_t *
    citizen_msg_encode_imlost (
);

//  Encode the FOLLOWME 
zmsg_t *
    citizen_msg_encode_followme (
);

//  Encode the LEADME 
zmsg_t *
    citizen_msg_encode_leadme (
);

//  Encode the NOTHANKS 
zmsg_t *
    citizen_msg_encode_nothanks (
);


//  Send the IMLOST to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
int
    citizen_msg_send_imlost (void *output);
    
//  Send the FOLLOWME to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
int
    citizen_msg_send_followme (void *output);
    
//  Send the LEADME to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
int
    citizen_msg_send_leadme (void *output);
    
//  Send the NOTHANKS to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
int
    citizen_msg_send_nothanks (void *output);
    
//  Duplicate the citizen_msg message
citizen_msg_t *
    citizen_msg_dup (citizen_msg_t *self);

//  Print contents of message to stdout
void
    citizen_msg_print (citizen_msg_t *self);

//  Get/set the message routing id
zframe_t *
    citizen_msg_routing_id (citizen_msg_t *self);
void
    citizen_msg_set_routing_id (citizen_msg_t *self, zframe_t *routing_id);

//  Get the citizen_msg id and printable command
int
    citizen_msg_id (citizen_msg_t *self);
void
    citizen_msg_set_id (citizen_msg_t *self, int id);
const char *
    citizen_msg_command (citizen_msg_t *self);

//  Self test of this class
int
    citizen_msg_test (bool verbose);
//  @end

//  For backwards compatibility with old codecs
#define citizen_msg_dump    citizen_msg_print

#ifdef __cplusplus
}
#endif

#endif
