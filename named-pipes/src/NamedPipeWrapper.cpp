#include "NamedPipeWrapper.h"

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <memory>

#define SUCCESS_CODE 0;

NamedPipeWrapper::NamedPipeWrapper()
{
    mPipeFd = 0;
    mBuffer = std::make_unique<Buffer<char*>>(_initialBufferSize);
}

NamedPipeWrapper::~NamedPipeWrapper()
{
    if(mPipeFd)
    {
        close(mPipeFd);
    }
}

int NamedPipeWrapper::Create(std::string pipeName)
{
    umask(0);
    if(!mkfifo(pipeName.c_str(), 0666))
    {
        return errno;
    }
    
    return SUCCESS_CODE;
}

int NamedPipeWrapper::Remove(std::string pipeName)
{
    return remove(pipeName.c_str());
}

int NamedPipeWrapper::Open(std::string pipeName)
{
    if(!mPipeFd)
    {
        close(mPipeFd);
    }

    mPipeFd = open(pipeName.c_str(), O_WRONLY);

    if(mPipeFd == -1)
    {
        mPipeFd = 0;
        return errno;
    }

    mPipeName = pipeName;
}

int NamedPipeWrapper::Close()
{
    if(!mPipeFd)
    {
        return 0;
    }

    mPipeName = "";
    if(close(mPipeFd))
    {
        return errno;
    }

    return 0;
}

