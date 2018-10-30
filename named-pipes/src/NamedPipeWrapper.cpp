#include "NamedPipeWrapper.h"

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <memory>

#define SUCCESS_CODE 0
#define ERROR_CODE -1
#define INVALID_FD -1

#define READ_BUFFER_SIZE 64

NamedPipeWrapper::NamedPipeWrapper()
{
    mPipeFd = INVALID_FD;
}

NamedPipeWrapper::~NamedPipeWrapper()
{
    if(mPipeFd != INVALID_FD)
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

int NamedPipeWrapper::OpenReader(std::string pipeName)
{
    return openPipe(pipeName, O_RDONLY);
}

int NamedPipeWrapper::OpenWriter(std::string pipeName)
{
    return openPipe(pipeName, O_WRONLY);
}

int NamedPipeWrapper::Close()
{
    if(mPipeFd == INVALID_FD)
    {
        return 0;
    }

    mPipeName = "";
    if(close(mPipeFd))
    {
        return errno;
    }

    return SUCCESS_CODE;
}

int NamedPipeWrapper::Write(Buffer<BufferPtrType> &buffer)
{
    if(mPipeFd == INVALID_FD)
    {
        return ERROR_CODE;
    }

    if(!buffer.Size())
    {
        return SUCCESS_CODE;
    }

    if(write(mPipeFd, buffer.Front(), buffer.Size()) == ERROR_CODE)
    {
        return errno;
    }

    return SUCCESS_CODE;
}

int NamedPipeWrapper::Read(Buffer<BufferPtrType> &buffer)
{
    if(mPipeFd == INVALID_FD)
    {
        return ERROR_CODE;
    }

    int readRetVal = 0;
    int bytesRead = 0;
 
    BufferType readBuf[READ_BUFFER_SIZE];

    do
    {
        bytesRead = read(mPipeFd, readBuf, READ_BUFFER_SIZE);
        if(bytesRead > 0)
        {
            buffer.Append(readBuf, bytesRead);
        }
        else
        {
            break;
        }

    }
    while(bytesRead > 0 && bytesRead == READ_BUFFER_SIZE);
    

    if(readRetVal == ERROR_CODE)
    {
        return errno;
    }

    return SUCCESS_CODE;
}

int NamedPipeWrapper::openPipe(std::string pipeName, int mode)
{
    if(mPipeFd != INVALID_FD)
    {
        close(mPipeFd);
    }

    mPipeFd = open(pipeName.c_str(), mode);

    if(mPipeFd == INVALID_FD)
    {
        return errno;
    }

    mPipeName = pipeName; 

    return SUCCESS_CODE;
}
