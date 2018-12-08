#include <cstdio>
#include "./../src/MsgQueueWrapper.h"
#include "common.h"

#define MAX_MSG_SIZE 1024
#define MAX_MSG_NUMBER 10 
#define MSG_QUEUE_MODE 0644

void printReceivedMessage(MsgQueueWrapper::Message& aMessage)
{
    char lFormattedMessage[2*MAX_MSG_SIZE];

    sprintf(lFormattedMessage, "[V] message received: size:[%lu], priority:[%d], content:[%s],",
        aMessage.content.Size(), 
        aMessage.priority, 
        aMessage.content.GetBufferAsString().c_str());

    std::cout << lFormattedMessage << std::endl;
}

int runMqueueAsyncServerTest()
{
    int returnCode = SUCCESS_CODE;

    MsgQueueWrapper lMsgQueue;

    MsgQueueWrapper::Message lMessage;

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

    while(1)
    {
        lMsgQueue.ReceiveSync(lMessage);
        printReceivedMessage(lMessage);
    }


    return returnCode;
}

int main()
{
    return runMqueueAsyncServerTest();
}