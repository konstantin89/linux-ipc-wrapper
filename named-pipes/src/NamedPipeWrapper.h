#ifndef __NAMED_PIPE_SERVER_H
#define __NAMED_PIPE_SERVER_H

#include <string>
#include <memory>

#include "./../../common/Buffer.h"

class NamedPipeWrapper
{

public:

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
     * @brief: Open named pipe
     * @returns: 0 on success, errno otherwise.
     */
    int Open(std::string pipeName);

    /**
     * @brief: Closes named pipe.
     * @returns: 0 on success, errno otherwise.
     */
    int Close();

private:

    int mPipeFd;
    std::string mPipeName;
    
    std::unique_ptr<Buffer<char*>> mBuffer;

private:

    const uint32_t _initialBufferSize = 1024;

};

#endif





