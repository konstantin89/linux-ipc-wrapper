#ifndef __NAMED_PIPE_SERVER_H
#define __NAMED_PIPE_SERVER_H

#include <string>
#include <memory>

#include "./../../common/Buffer.h"

class NamedPipeWrapper
{

public:

    using BufferType = uint8_t;
    using BufferPtrType = BufferType*;

    /**
     * @brief: Creates named pipe
     * @returns: 0 on success, errno otherwise.
     */
    static int Create(std::string pipeName);
    
    /**
     * @brief: Removes named pipe file.
     * @returns: 0 on success, errno otherwise.
     */
    static int Remove(std::string pipeName);

public:

    NamedPipeWrapper();
    ~NamedPipeWrapper();
    
    /**
     * @brief: Open named pipe for reading
     * @returns: 0 on success, errno otherwise.
     */
    int OpenReader(std::string pipeName);

    /**
     * @brief: Open named pipe for writing
     * @returns: 0 on success, errno otherwise.
     */
    int OpenWriter(std::string pipeName);

    /**
     * @brief: Closes named pipe.
     * @returns: 0 on success, errno otherwise.
     */
    int Close();

    int Write(Buffer<BufferPtrType> &buffer);
    int Read(Buffer<BufferPtrType> &buffer);


private:

    int openPipe(std::string pipeName, int mode);

private:

    int mPipeFd;
    std::string mPipeName;
    
};

#endif





