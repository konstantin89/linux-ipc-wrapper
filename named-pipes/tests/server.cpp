#include <iostream>
#include <string.h>
#include "./../src/NamedPipeWrapper.h"
#include "testCommon.h"


int main()
{
    int returnCode = 0;
    NamedPipeWrapper namedPipe;
    
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


    Buffer<uint8_t*> buf;

    while(1)
    {   
        auto retVal = namedPipe.Read(buf);
        std::cout << "Read return value: " << retVal << std::endl;

        buf.PrintAsUnicode();
        break;
    }
    
    returnCode = namedPipe.Close();

    if(returnCode)
    {
        std::cout << "[X] Close failed with error " << returnCode << std::endl;
        return ERROR;
    }

    return SUCCESS;
}