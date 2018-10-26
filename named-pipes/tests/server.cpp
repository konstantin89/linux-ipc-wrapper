#include <iostream>
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

    returnCode = namedPipe.Open(TEST_PIPE_NAME);

    if(returnCode)
    {
        std::cout << "[X] Open failed with error " << returnCode << std::endl;
        return ERROR;
    }


    returnCode = namedPipe.Close();

    if(returnCode)
    {
        std::cout << "[X] Close failed with error " << returnCode << std::endl;
        return ERROR;
    }

    return SUCCESS;
}