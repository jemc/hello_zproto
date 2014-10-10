/*  =========================================================================
    citizen_server - Citizen server

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

     * The XML model used for this code generation: citizen_server.xml, or
     * The code generation script that built this file: zproto_server_c
    ************************************************************************
    =========================================================================
*/

#ifndef __CITIZEN_SERVER_H_INCLUDED__
#define __CITIZEN_SERVER_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  To work with citizen_server, use the CZMQ zactor API:
//
//  Create new citizen server instance, passing logging prefix:
//
//      zactor_t *citizen_server = zactor_new (citizen_server, "myname");
//  
//  Destroy citizen server instance
//
//      zactor_destroy (&citizen_server);
//  
//  Enable verbose logging of commands and activity:
//
//      zstr_send (citizen_server, "VERBOSE");
//
//  Bind citizen server to specified endpoint. TCP endpoints may specify
//  the port number as "*" to aquire an ephemeral port:
//
//      zstr_sendx (citizen_server, "BIND", endpoint, NULL);
//
//  Return assigned port number, specifically when BIND was done using an
//  an ephemeral port:
//
//      zstr_sendx (citizen_server, "PORT", NULL);
//      char *command, *port_str;
//      zstr_recvx (citizen_server, &command, &port_str, NULL);
//      assert (streq (command, "PORT"));
//
//  Specify configuration file to load, overwriting any previous loaded
//  configuration file or options:
//
//      zstr_sendx (citizen_server, "CONFIGURE", filename, NULL);
//
//  Set configuration path value:
//
//      zstr_sendx (citizen_server, "SET", path, value, NULL);
//    
//  Send zmsg_t instance to citizen server:
//
//      zactor_send (citizen_server, &msg);
//
//  Receive zmsg_t instance from citizen server:
//
//      zmsg_t *msg = zactor_recv (citizen_server);
//
//  This is the citizen_server constructor as a zactor_fn:
//
void
    citizen_server (zsock_t *pipe, void *args);

//  Self test of this class
void
    citizen_server_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
