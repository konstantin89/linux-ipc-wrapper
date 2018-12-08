#include "./../src/MsgQueueWrapper.h"
#include "common.h"


std::string gTestMessageStr("TEST_MESSAGE");

int runMqueueClientTest()
{
    int lReturnCode = SUCCESS_CODE;

    MsgQueueWrapper lMsgQueue;

    lReturnCode = lMsgQueue.OpenWriter(MSG_QUEUE_NAME);

    if(lReturnCode != SUCCESS_CODE)
    {
       printError(lReturnCode, "OpenReader failed"); 
    }

    MsgQueueWrapper::Message lMessage;

    lMessage.content.Insert(gTestMessageStr.data(), gTestMessageStr.size());
    lMessage.priority = MsgQueueWrapper::Low;

    while(1)
    {
        lReturnCode = lMsgQueue.Send(lMessage);

        if(lReturnCode != SUCCESS_CODE)
        {
            printError(lReturnCode, "Send failed");
            break;
        }
        else
        {
            std::cout << "message sent: " 
                << lMessage.content.GetBufferAsString().c_str()
                << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    return lReturnCode;
}

int main()
{
    return runMqueueClientTest();
}