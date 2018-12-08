#ifndef __MSG_QUEUE_WRAPPER_H
#define __MSG_QUEUE_WRAPPER_H

#include <string>
#include <memory>
#include <mqueue.h>
#include <condition_variable>

#include "./../../common/Buffer.h"

class MsgQueueWrapper
{

public:

    using BufferType = char;

    using BufferPtrType = BufferType*;

    using QueueAttrType = struct mq_attr;

    using PConditionalVar = std::condition_variable*;

    typedef enum{
        Low,
        Med,
        High
    } MsgPriority;

    typedef struct{
        MsgPriority priority;
        Buffer<BufferPtrType> content;
    } Message;

    /**
     * @brief: Create message queue.
     * 
     * @param aQueueName - Message queue name. Note that there is
     *                     Linux naming convention for message queues.
     *                     
     * @param aMode - Permissions for the queue. For example 0644.
     * 
     * @param aMaxMsgNumber - Maximum number of messages
     *                        in queue. Note that this value
     *                        cannot be larger than the system value
     *                        specified in /proc/sys/fs/mqueue/msg_max.
     * 
     * @param aMsgMaxSize - Maximum number of messages
     *                      in queue. Note that this value
     *                      cannot be larger than the system value
     *                      specified in /proc/sys/fs/mqueue/msgsize_max.
     * 
     */
    static int Create(
        std::string aQueueName, 
        mode_t aMode,
        uint64_t aMsgMaxSize,
        uint64_t aMaxMsgNumber);

    static int Remove(std::string aQueueName);
    

public:

    /**
     * @brief: Send a message to message queue.
     * 
     * @param: aMessage - Message to send.
     * 
     * @returns: ERROR_SUCCESS - iff call ended successfully.
     *           errno - On error
     */
    int Send(Message aMessage);

    /**
     * @brief: Receive message from linux message queue.
     *         The call with blocks until message is available for reading 
     * 
     * @param: aMessage - The message that has been read from queue.
     * 
     * @returns: ERROR_SUCCESS - iff call ended successfully.
     *           errno - On error
     */
    int ReceiveSync(Message &aMessage);

    /**
     * @brief: Receive message from linux message queue.
     *         The call returns immidiatly, even if no message is 
     *         available for reading in the queue.
     * 
     * @param: aMessage - The message that has been read from queue.
     * 
     * @returns: ERROR_SUCCESS - On success returns.
     *           EAGAIN - No message available in queue.
     *           errno - If other error occures.
     */
    int ReceiveAsync(Message &aMessage);

    MsgQueueWrapper();

    ~MsgQueueWrapper();

    int OpenReader(std::string aQueueName);

    int OpenWriter(std::string aQueueName);

    int Close();

    int SetQueueAttributes(uint64_t aMaxSize, uint64_t aMsgSize);

    int GetQueueMsgMaxSize(uint64_t &aMsgMaxSize);

    long GetCurrentMsgCount();

private:

    std::string mQueueName;
    mqd_t mMsgQueueDesc;

    PConditionalVar mMessageNotificationCv;
    std::mutex mCondVarMutex;


private:

    int openQueue(std::string aQueueName,int aMode);

    uint32_t priorityToPosix(MsgPriority aPriorityEnum);
 
    MsgPriority priorityFromPosix(uint32_t aPosixPriority);

    int getQueueAttributes(QueueAttrType &aAttributes);

    static void receiveAsyncMessageAndNotify(union sigval sv);

    int setMessageNotification();

};

#endif