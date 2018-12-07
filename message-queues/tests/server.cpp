#include "./../src/MsgQueueWrapper.h"
#include "common.h"

#define MAX_MSG_SIZE 1024
#define MAX_MSG_NUMBER 10 
#define MSG_QUEUE_MODE 0644

int main()
{
    int returnCode = SUCCESS_CODE;

    MsgQueueWrapper lMsgQueue;

    MsgQueueWrapper::Remove(MSG_QUEUE_NAME);

    returnCode = MsgQueueWrapper::Create(
        MSG_QUEUE_NAME, 
        MSG_QUEUE_MODE, 
        MAX_MSG_SIZE, 
        MAX_MSG_NUMBER);

    if(returnCode != SUCCESS_CODE)
    {
       printError(returnCode, "Create failed");
       return returnCode;

    }

    returnCode = lMsgQueue.OpenReader(MSG_QUEUE_NAME);

    if(returnCode != SUCCESS_CODE)
    {
       printError(returnCode, "OpenReader failed"); 
       return returnCode;
    }

    MsgQueueWrapper::Message lMessage;

    while(1)
    {
        returnCode = lMsgQueue.Receive(lMessage);

        if(returnCode != SUCCESS_CODE)
        {
            printError(returnCode, "Receive failed");
            //break;
        }
        else
        {
            std::cout << "Message received" << std::endl;
        }
    }

    return returnCode;
}