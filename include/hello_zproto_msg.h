/*  =========================================================================
    hello_zproto_msg - hello_zproto example protocol
    
    Generated codec header for hello_zproto_msg
    -------------------------------------------------------------------------
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

#ifndef __HELLO_ZPROTO_MSG_H_INCLUDED__
#define __HELLO_ZPROTO_MSG_H_INCLUDED__

/*  These are the hello_zproto_msg messages:

    LOG - Log an event.
        sequence            number 2    
        level               number 1    Log severity level
        event               number 1    Type of event
        node                number 2    Sending node
        peer                number 2    Refers to this peer
        time                number 8    Log date/time
        data                string      Actual log message

    STRUCTURES - This message contains a list and a hash.
        sequence            number 2    
        aliases             strings     List of strings
        headers             dictionary  Other random properties

    BINARY - Deliver a multi-part message.
        sequence            number 2    
        flags               octets [4]  A set of flags
        address             frame       Return address as frame
        content             msg         Message to be delivered
*/

#define HELLO_ZPROTO_MSG_VERSION                  1

#define HELLO_ZPROTO_MSG_LOG                      1
#define HELLO_ZPROTO_MSG_STRUCTURES               2
#define HELLO_ZPROTO_MSG_BINARY                   3
#define HELLO_ZPROTO_MSG_FLAGS_SIZE               4

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _hello_zproto_msg_t hello_zproto_msg_t;

//  @interface
//  Create a new hello_zproto_msg
hello_zproto_msg_t *
    hello_zproto_msg_new (int id);

//  Destroy the hello_zproto_msg
void
    hello_zproto_msg_destroy (hello_zproto_msg_t **self_p);

//  Receive and parse a hello_zproto_msg from the input
hello_zproto_msg_t *
    hello_zproto_msg_recv (void *input);

//  Send the hello_zproto_msg to the output, and destroy it
int
    hello_zproto_msg_send (hello_zproto_msg_t **self_p, void *output);

//  Send the hello_zproto_msg to the output, and do not destroy it
int
    hello_zproto_msg_send_again (hello_zproto_msg_t *self, void *output);

//  Send the LOG to the output in one step
int
    hello_zproto_msg_send_log (void *output,
        uint16_t sequence,
        byte level,
        byte event,
        uint16_t node,
        uint16_t peer,
        uint64_t time,
        char *data);
    
//  Send the STRUCTURES to the output in one step
int
    hello_zproto_msg_send_structures (void *output,
        uint16_t sequence,
        zlist_t *aliases,
        zhash_t *headers);
    
//  Send the BINARY to the output in one step
int
    hello_zproto_msg_send_binary (void *output,
        uint16_t sequence,
        byte *flags,
        zframe_t *address,
        zmsg_t *content);
    
//  Duplicate the hello_zproto_msg message
hello_zproto_msg_t *
    hello_zproto_msg_dup (hello_zproto_msg_t *self);

//  Print contents of message to stdout
void
    hello_zproto_msg_dump (hello_zproto_msg_t *self);

//  Get/set the message routing id
zframe_t *
    hello_zproto_msg_routing_id (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_routing_id (hello_zproto_msg_t *self, zframe_t *routing_id);

//  Get the hello_zproto_msg id and printable command
int
    hello_zproto_msg_id (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_id (hello_zproto_msg_t *self, int id);
char *
    hello_zproto_msg_command (hello_zproto_msg_t *self);

//  Get/set the sequence field
uint16_t
    hello_zproto_msg_sequence (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_sequence (hello_zproto_msg_t *self, uint16_t sequence);

//  Get/set the level field
byte
    hello_zproto_msg_level (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_level (hello_zproto_msg_t *self, byte level);

//  Get/set the event field
byte
    hello_zproto_msg_event (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_event (hello_zproto_msg_t *self, byte event);

//  Get/set the node field
uint16_t
    hello_zproto_msg_node (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_node (hello_zproto_msg_t *self, uint16_t node);

//  Get/set the peer field
uint16_t
    hello_zproto_msg_peer (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_peer (hello_zproto_msg_t *self, uint16_t peer);

//  Get/set the time field
uint64_t
    hello_zproto_msg_time (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_time (hello_zproto_msg_t *self, uint64_t time);

//  Get/set the data field
char *
    hello_zproto_msg_data (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_data (hello_zproto_msg_t *self, char *format, ...);

//  Get/set the aliases field
zlist_t *
    hello_zproto_msg_aliases (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_aliases (hello_zproto_msg_t *self, zlist_t *aliases);

//  Iterate through the aliases field, and append a aliases value
char *
    hello_zproto_msg_aliases_first (hello_zproto_msg_t *self);
char *
    hello_zproto_msg_aliases_next (hello_zproto_msg_t *self);
void
    hello_zproto_msg_aliases_append (hello_zproto_msg_t *self, char *format, ...);
size_t
    hello_zproto_msg_aliases_size (hello_zproto_msg_t *self);

//  Get/set the headers field
zhash_t *
    hello_zproto_msg_headers (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_headers (hello_zproto_msg_t *self, zhash_t *headers);
    
//  Get/set a value in the headers dictionary
char *
    hello_zproto_msg_headers_string (hello_zproto_msg_t *self, char *key, char *default_value);
uint64_t
    hello_zproto_msg_headers_number (hello_zproto_msg_t *self, char *key, uint64_t default_value);
void
    hello_zproto_msg_headers_insert (hello_zproto_msg_t *self, char *key, char *format, ...);
size_t
    hello_zproto_msg_headers_size (hello_zproto_msg_t *self);

//  Get/set the flags field
byte *
    hello_zproto_msg_flags (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_flags (hello_zproto_msg_t *self, byte *flags);

//  Get/set the address field
zframe_t *
    hello_zproto_msg_address (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_address (hello_zproto_msg_t *self, zframe_t *frame);

//  Get/set the content field
zmsg_t *
    hello_zproto_msg_content (hello_zproto_msg_t *self);
void
    hello_zproto_msg_set_content (hello_zproto_msg_t *self, zmsg_t *msg);

//  Self test of this class
int
    hello_zproto_msg_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
