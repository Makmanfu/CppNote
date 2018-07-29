//*************************************************************************
//
//              ZMQ
//                                              @2018-07-29°¢¸Ê
//
//*************************************************************************
#ifndef STYZMQ_H
#define STYZMQ_H

#include <zmq.h>
#include <stdio.h>
//#include <unistd.h>
#include <string.h>
//#include <windows.h>

struct StyZMQ
{
    static int hwserver(int argc, char** argv)
    {
        printf("hwserver \n");
        void* context = zmq_ctx_new();
        // Socket to talk to clients
        void* responder = zmq_socket(context, ZMQ_REP);
        zmq_bind(responder, "tcp://*:5555");
        while (1)
        {
            // Wait for next request from client
            zmq_msg_t request;
            zmq_msg_init(&request);
            zmq_msg_recv(&request, responder, 0);
            printf("Received Hello\n");
            zmq_msg_close(&request);
            // Do some 'work'
            Sleep(1000);
            // Send reply back to client
            zmq_msg_t reply;
            zmq_msg_init_size(&reply, 5);
            memcpy(zmq_msg_data(&reply), "World", 5);
            zmq_msg_send(&reply, responder, 0);
            zmq_msg_close(&reply);
        }
        // We never get here but if we did, this would be how we end
        zmq_close(responder);
        zmq_ctx_destroy(context);
        return 0;
    };

    static int hwserver2(int argc, char** argv)
    {
        // Prepare our context and socket
        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_REP);
        socket.bind("tcp://*:5555");
        while (true)
        {
            zmq::message_t request;
            // Wait for next request from client
            socket.recv(&request);
            std::cout << "Received Hello" << std::endl;
            // Do some 'work'
            sleep(1);
            // Send reply back to client
            zmq::message_t reply(5);
            memcpy((void*)reply.data(), "World", 5);
            socket.send(reply);
        }
        return 0;
    };


};









#endif