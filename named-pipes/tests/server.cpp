/**
 * @brief: Server side program for testing named pipes IPC.
 */

#include <iostream>
#include <string.h>
#include "./../src/NamedPipeWrapper.h"
#include "testCommon.h"

int runTestServer()
{
    int returnCode = 0;
    NamedPipeWrapper namedPipe;
    Buffer<uint8_t*> readBuffer;
    
    returnCode = NamedPipeWrapper::Create(TEST_PIPE_NAME);

    if(returnCode)
    {
        std::cout << "[X] Create failed with error " << returnCode << std::endl;
        return ERROR;
    }

    returnCode = namedPipe.OpenReader(TEST_PIPE_NAME);

    if(returnCode)
    {
        std::cout << "[X] Open failed with error " << std::string(strerror(returnCode)) << std::endl;
        return ERROR;
    }

    while(1)
    {   
        readBuffer.Clear();
        
        auto retVal = namedPipe.Read(readBuffer);
        if(retVal)
        {
            std::cout << "[X] Read failed with error " << retVal << std::endl;
            continue;
        }

        std::cout << "[ ] Incoming message : ";
        readBuffer.PrintAsUnicode();
    }
    
    returnCode = namedPipe.Close();
    if(returnCode)
    {
        std::cout << "[X] Close failed with error " << returnCode << std::endl;
        return ERROR;
    }

    return SUCCESS;
}

int main()
{
    return runTestServer();
}