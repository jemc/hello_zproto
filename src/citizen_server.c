/*  =========================================================================
    citizen_server - Citizen server

    =========================================================================
*/

/*
@header
    Description of class for man page.
@discuss
    Detailed discussion of the class, if any.
@end
*/

#include "hello_zproto.h"
//  TODO: Change these to match your project's needs
#include "../include/citizen_msg.h"
#include "../include/citizen_server.h"

//  ---------------------------------------------------------------------------
//  Forward declarations for the two main classes we use here

typedef struct _server_t server_t;
typedef struct _client_t client_t;

//  This structure defines the context for each running server. Store
//  whatever properties and structures you need for the server.

struct _server_t {
    //  These properties must always be present in the server_t
    //  and are set by the generated engine; do not modify them!
    zsock_t *pipe;              //  Actor pipe back to caller
    zconfig_t *config;          //  Current loaded configuration
    
    int follower_count;
};

//  ---------------------------------------------------------------------------
//  This structure defines the state for each client connection. It will
//  be passed to each action in the 'self' argument.

struct _client_t {
    //  These properties must always be present in the client_t
    //  and are set by the generated engine; do not modify them!
    server_t *server;           //  Reference to parent server
    citizen_msg_t *request;     //  Last received request
    citizen_msg_t *reply;       //  Reply to send out, if any

    //  TODO: Add specific properties for your application
};

//  Include the generated server engine
#include "citizen_server_engine.inc"

//  Allocate properties and structures for a new server instance.
//  Return 0 if OK, or -1 if there was an error.

static int
server_initialize (server_t *self)
{
    //  Construct properties
    self->follower_count = 0;
    
    return 0;
}

//  Free properties and structures for a server instance

static void
server_terminate (server_t *self)
{
    //  Destroy properties here
}

//  Process server API method, return reply message if any

static zmsg_t *
server_method (server_t *self, const char *method, zmsg_t *msg)
{
    return NULL;
}


//  Allocate properties and structures for a new client connection and
//  optionally engine_set_next_event (). Return 0 if OK, or -1 on error.

static int
client_initialize (client_t *self)
{
    //  Construct properties here
    return 0;
}

//  Free properties and structures for a client connection

static void
client_terminate (client_t *self)
{
    //  Destroy properties here
}

//  ---------------------------------------------------------------------------
//  Selftest

void
citizen_server_test (bool verbose)
{
    printf (" * citizen_server: ");
    if (verbose)
        printf ("\n");
    
    //  @selftest
    zactor_t *server = zactor_new (citizen_server, "server");
    if (verbose)
        zstr_send (server, "VERBOSE");
    zstr_sendx (server, "BIND", "ipc://@/citizen_server", NULL);
    
    // Connect a client
    zsock_t *client = zsock_new (ZMQ_DEALER);
    assert (client);
    zsock_set_rcvtimeo (client, 2000);
    zsock_connect (client, "ipc://@/citizen_server");
    
    // Message holders
    citizen_msg_t *request, *reply;
    
    // Check initial server status
    request = citizen_msg_new (CITIZEN_MSG_HOWAREYOU);
    citizen_msg_send (&request, client);
    reply = citizen_msg_recv (client);
    assert (reply);
    assert (citizen_msg_id (reply) == CITIZEN_MSG_STATUS);
    assert (citizen_msg_follower_count (reply) == 0);
    citizen_msg_destroy (&reply);
    
    // Solicit a follow request
    request = citizen_msg_new (CITIZEN_MSG_IMLOST);
    citizen_msg_send (&request, client);
    reply = citizen_msg_recv (client);
    assert (reply);
    assert (citizen_msg_id (reply) == CITIZEN_MSG_FOLLOWME);
    citizen_msg_destroy (&reply);
    
    // Solicit a follow request
    request = citizen_msg_new (CITIZEN_MSG_LEADME);
    citizen_msg_send (&request, client);
    
    // Check that status now shows one follower
    request = citizen_msg_new (CITIZEN_MSG_HOWAREYOU);
    citizen_msg_send (&request, client);
    reply = citizen_msg_recv (client);
    assert (reply);
    assert (citizen_msg_id (reply) == CITIZEN_MSG_STATUS);
    assert (citizen_msg_follower_count (reply) == 1);
    citizen_msg_destroy (&reply);
    
    // Force an expiration of the client with very short timeout,
    // a message to reapply the timeout time, and a too-long sleep;
    // then reset timeout to default time of 60 seconds.
    zstr_sendx (server, "SET", "server/timeout", "10", NULL);
    citizen_msg_send_howareyou (client);
    reply = citizen_msg_recv (client);
    citizen_msg_destroy (&reply);
    zclock_sleep(20); // client connection should time out
    zstr_sendx (server, "SET", "server/timeout", "60000", NULL);
    
    // Check that status now shows zero followers
    request = citizen_msg_new (CITIZEN_MSG_HOWAREYOU);
    citizen_msg_send (&request, client);
    reply = citizen_msg_recv (client);
    assert (reply);
    assert (citizen_msg_id (reply) == CITIZEN_MSG_STATUS);
    assert (citizen_msg_follower_count (reply) == 0);
    citizen_msg_destroy (&reply);
    
    // Cleanup
    zsock_destroy (&client);
    zactor_destroy (&server);
    //  @end
    printf ("OK\n");
}


//  ---------------------------------------------------------------------------
//  compile_status
//

static void
compile_status (client_t *self)
{
    citizen_msg_set_follower_count (self->reply, self->server->follower_count);
}


//  ---------------------------------------------------------------------------
//  add_to_followers
//

static void
add_to_followers (client_t *self)
{
    self->server->follower_count++;
}


//  ---------------------------------------------------------------------------
//  remove_from_followers
//

static void
remove_from_followers (client_t *self)
{
    self->server->follower_count--;
}
