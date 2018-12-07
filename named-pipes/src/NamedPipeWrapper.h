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
    static int Create(std::string aPipeName);
    
    /**
     * @brief: Removes named pipe file.
     * @returns: 0 on success, errno otherwise.
     */
    static int Remove(std::string aPipeName);

public:

    NamedPipeWrapper();
    ~NamedPipeWrapper();
    
    /**
     * @brief: Open named pipe for reading
     * @returns: 0 on success, errno otherwise.
     */
    int OpenReader(std::string aPipeName);

    /**
     * @brief: Open named pipe for writing
     * @returns: 0 on success, errno otherwise.
     */
    int OpenWriter(std::string aPipeName);

    /**
     * @brief: Closes named pipe.
     * @returns: 0 on success, errno otherwise.
     */
    int Close();

    /**
     * @brief: Write buffer into named pipe.
     * @returns 0 on success, errno otherwise.
     */
    int Write(Buffer<BufferPtrType> &aBuffer);
    
    /**
     * @brief: Read buffer from named pipe.
     * @returns 0 on success, errno otherwise.
     */
    int Read(Buffer<BufferPtrType> &aBuffer);


private:

    int openPipe(std::string aPipeName, int aMode);

private:

    int mPipeFd;
    std::string mPipeName;
    
};

#endif





