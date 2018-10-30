/**
 * @brief: Client side program for testing named pipes IPC.
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "./../src/NamedPipeWrapper.h"
#include "testCommon.h"

// Delay in seconds
#define DELAY_BETWEEN_MESSAGES 1

int sendTestMessage()
{
    int retVal = 0;
    NamedPipeWrapper namedPipe;
    
    retVal = namedPipe.OpenWriter(TEST_PIPE_NAME);
    if(retVal)
    {
        std::cout << "[X] Open failed with error " << retVal << std::endl;
        return ERROR;
    }

    Buffer<uint8_t*> buf;
    uint8_t message[] = {'M','E','S','S','A','G','E'};

    buf.Insert(message, 7);

    while(1)
    {
        retVal = namedPipe.Write(buf);
        if(retVal)
        {
            std::cout << "[X] Write failed with error " << retVal << std::endl;
            return ERROR;
        }

        std::this_thread::sleep_for (std::chrono::seconds(DELAY_BETWEEN_MESSAGES));
    }


    retVal = namedPipe.Close();
    if(retVal)
    {
        std::cout << "[X] Close failed with error " << retVal << std::endl;
        return ERROR;
    }

    return SUCCESS;
}


int main()
{
    return sendTestMessage();
}