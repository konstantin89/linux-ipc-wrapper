#include "./../src/MsgQueueWrapper.h"
#include "common.h"

#include <chrono>
#include <thread>

int main()
{
    std::string lTestMsg("TEST_MESSAGE");

    int returnCode = SUCCESS_CODE;

    MsgQueueWrapper lMsgQueue;

    returnCode = lMsgQueue.OpenWriter(MSG_QUEUE_NAME);

    if(returnCode != SUCCESS_CODE)
    {
       printError(returnCode, "OpenReader failed"); 
    }

    MsgQueueWrapper::Message lMessage;

    lMessage.content.Insert(lTestMsg.data(), lTestMsg.size());
    lMessage.priority = MsgQueueWrapper::Low;

    while(1)
    {
        returnCode = lMsgQueue.Send(lMessage);

        if(returnCode != SUCCESS_CODE)
        {
            printError(returnCode, "Send failed");
            break;
        }
        else
        {
            lMessage.content.PrintAsUnicode();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return returnCode;
}