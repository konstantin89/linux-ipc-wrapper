#include <iostream>
#include "./../src/NamedPipeWrapper.h"
#include "testCommon.h"


int main()
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

    std::cout << "Write return code: " << namedPipe.Write(buf) << std::endl;

    retVal = namedPipe.Close();
    if(retVal)
    {
        std::cout << "[X] Close failed with error " << retVal << std::endl;
    }

    return 0;
}