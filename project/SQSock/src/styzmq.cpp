
#include "stdafx.h"
#include "styzmq.h"


int StyZMQ::zmq_hwserverc(int argc, char** argv)
{
    void* context = zmq_ctx_new();
    void* responder = zmq_socket(context, ZMQ_REP);
    zmq_bind(responder, "tcp://*:5555");
    while (true)
    {
        zmq_msg_t request;
        zmq_msg_init(&request);
        //�����ȴ��ͻ�������
        zmq_msg_recv(&request, responder, 0);
        printf("Received Hello\n");
        zmq_msg_close(&request);
        // Do some 'work'
        Sleep(1000);
        //�����������
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
}

int StyZMQ::zmq_hwservercpp(int argc, char** argv)
{
    //��socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");
    while (true)
    {
        zmq::message_t request;
        //�����ȴ��ͻ�������
        socket.recv(&request);
        std::cout << "Received Hello" << std::endl;
        // Do some 'work'
        Sleep(1000);
        //�����������
        zmq::message_t reply(5);
        memcpy((void*)reply.data(), "World", 5);
        socket.send(reply);
    }
    return 0;
}

int StyZMQ::zmq_hwclient(int argc, char** argv)
{
    void* context = zmq_ctx_new();
    //����socket
    printf("Connecting to hello world server...\n");
    void* requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");
    //��������˷�������
    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        zmq_msg_t request;
        zmq_msg_init_size(&request, 5);
        memcpy(zmq_msg_data(&request), "Hello", 5);
        printf("Sending Hello %d...\n", request_nbr);
        zmq_msg_send(&request, requester, 0);
        zmq_msg_close(&request);
        zmq_msg_t reply;
        zmq_msg_init(&reply);
        zmq_msg_recv(&reply, requester, 0);
        printf("Received World %d\n", request_nbr);
        zmq_msg_close(&reply);
    }
    Sleep(2000);
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}

int StyZMQ::zmq_wuserverPub(int argc, char** argv)
{
    void* context = zmq_init(1);
    void* publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://*:5556");
    zmq_bind(publisher, "ipc://weather.ipc");
    srandom((unsigned)time(NULL));      //��ʼ�������������
    printf("������Ϣ...\n");
    while (true)
    {
        //��������
        int zipcode, temperature, relhumidity;
        zipcode = randof(100000);
        temperature = randof(215) - 80;
        relhumidity = randof(50) + 10;
        //�����ж����߷�����Ϣ
        char update[20];
        sprintf(update, "%05d %d %d", 10001/*zipcode*/, temperature, relhumidity);
        printf(update);
        printf("\n");
        s_send(publisher, update);
    }
    zmq_close(publisher);
    zmq_ctx_destroy(context);
    return 0;
}

int StyZMQ::zmq_wuclientSub(int argc, char** argv)
{
    void* context = zmq_init(1);
    //��������������˵��׽���
    printf("�����ռ�������Ϣ...\n");
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5556");
    //���ö�����Ϣ��Ĭ��ΪŦԼ���ʱ�10001
    char* filter = (argc > 1) ? argv[1] : "10001";
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
    //����100��������Ϣ
    int update_nbr;
    long total_temp = 0;
    for (update_nbr = 0; update_nbr < 100; update_nbr++)
    {
        char* str = s_recv(subscriber);
        int zipcode, temperature, relhumidity;
        sscanf(str, "%d %d %d", &zipcode, &temperature, &relhumidity);
        total_temp += temperature;
        free(str);
    }
    printf("�����ʱ� '%s' ��ƽ���¶�Ϊ %dF\n",
           filter, (int)(total_temp / update_nbr));
    zmq_close(subscriber);
    zmq_term(context);
    return 0;
}









#ifdef _WIN64
#ifdef ZMQ_STATIC
    #pragma comment(lib,"libzmqs64.lib")
    #pragma comment(lib,"IPHlpApi.lib")
#else
    #pragma comment(lib,"libzmq64.lib")
#endif
#else
#ifdef ZMQ_STATIC
    #pragma comment(lib,"libzmqs.lib")
    #pragma comment(lib,"IPHlpApi.lib")
#else
    #pragma comment(lib,"libzmq.lib")
#endif
#endif