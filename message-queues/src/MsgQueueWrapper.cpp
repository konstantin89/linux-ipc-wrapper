#include <cstring>
#include <errno.h>
#include <sys/stat.h>
       #include <signal.h>
#include "MsgQueueWrapper.h"

#define SUCCESS_CODE 0
#define ERROR_CODE -1
#define INVALID_MSG_QUEUE_DESC -1

#define DEFAULT_MAX_MSG_SIZE 1024
#define DEFAULT_MAX_MSG_NUM  128

/**
 * POSIX standard defines that message priority 
 * range should be between 0 and 31. 
 */
#define POSIX_PRIORITY_MIN  0
#define POSIX_PRIORITY_MED  15
#define POSIX_PRIORITY_HIGH 31


int MsgQueueWrapper::Remove(std::string aQueueName)
{
    return mq_unlink(aQueueName.c_str());
}

int MsgQueueWrapper::Create(
    std::string aQueueName,
    mode_t aMode, 
    uint64_t aMsgMaxSize,
    uint64_t aMaxMsgNumber)
{
    int lReturnCode = SUCCESS_CODE;

    QueueAttrType lAttr;
    lAttr.mq_maxmsg = aMaxMsgNumber;
    lAttr.mq_msgsize = aMsgMaxSize;
    lAttr.mq_curmsgs = 0;
    lAttr.mq_flags = 0;

    lReturnCode = mq_open(aQueueName.c_str(), O_CREAT, aMode, &lAttr);
    if(lReturnCode == ERROR_CODE)
    {
        return errno;
    }

    return SUCCESS_CODE;
}

 MsgQueueWrapper::MsgQueueWrapper()
 {
    mMsgQueueDesc = INVALID_MSG_QUEUE_DESC;
    mMessageNotificationCv = new std::condition_variable;
 }

 MsgQueueWrapper::~MsgQueueWrapper()
 {
     /* EMPTY */
 }

int MsgQueueWrapper::SetQueueAttributes(uint64_t aMaxSize, uint64_t aMsgSize)
{
    int returnCode = SUCCESS_CODE;

    auto lQueueAttr = std::make_shared<struct mq_attr>();
    
    lQueueAttr->mq_maxmsg = aMaxSize;
    lQueueAttr->mq_msgsize = aMsgSize;
        
    if(mMsgQueueDesc != INVALID_MSG_QUEUE_DESC)
    {
        returnCode = mq_setattr(mMsgQueueDesc, lQueueAttr.get(), NULL);
    }

    return returnCode;
}

int MsgQueueWrapper::OpenReader(std::string aQueueName)
{
    int lRetVal = SUCCESS_CODE;

    lRetVal = openQueue(aQueueName, O_NONBLOCK | O_RDONLY);

    if(lRetVal != SUCCESS_CODE)
    {
        return lRetVal;
    }

    return lRetVal;
}


int MsgQueueWrapper::OpenWriter(std::string aQueueName)
{
    return openQueue(aQueueName, O_WRONLY);
}

int MsgQueueWrapper::Close()
{
    auto closeRetVal = mq_close(mMsgQueueDesc);
    mMsgQueueDesc = INVALID_MSG_QUEUE_DESC;

    if(closeRetVal == ERROR_CODE)
    {
        return errno;
    }

    return SUCCESS_CODE;
}

int MsgQueueWrapper::Send(Message aMessage)
{
    int returnCode = SUCCESS_CODE;

    returnCode = mq_send(
        mMsgQueueDesc, 
        aMessage.content.Front(), 
        aMessage.content.Size(), 
        priorityToPosix(aMessage.priority));


    if(returnCode != SUCCESS_CODE)
    {
        returnCode = errno;
    }

    return returnCode;
}

int MsgQueueWrapper::ReceiveAsync(Message &aMessage)
{
    if(mMsgQueueDesc == INVALID_MSG_QUEUE_DESC)
    {
        return ERROR_CODE;
    }

    int returnCode = SUCCESS_CODE;
    uint32_t lPosixMsgPriority = 0;
    uint64_t lMaxMsgSize = 0;

    returnCode = GetQueueMsgMaxSize(lMaxMsgSize);

    if(returnCode != SUCCESS_CODE)
    {
        return errno;
    }

    auto lReceiveBuffer = new char[lMaxMsgSize];

    if(!lReceiveBuffer)
    {
        return ERROR_CODE;
    }

    auto lReceiveRetCode = mq_receive(
        mMsgQueueDesc, 
        lReceiveBuffer, 
        lMaxMsgSize,
        &lPosixMsgPriority);

    if(lReceiveRetCode == ERROR_CODE)
    {
        delete[] lReceiveBuffer;
        return errno;
    }

    aMessage.content.Insert(lReceiveBuffer, lReceiveRetCode);

    aMessage.priority = priorityFromPosix(lPosixMsgPriority);

    delete[] lReceiveBuffer;
    return SUCCESS_CODE;
}

int MsgQueueWrapper::ReceiveSync(Message &aMessage)
{
    if(mMsgQueueDesc == INVALID_MSG_QUEUE_DESC)
    {
        return ERROR_CODE;
    }

    int returnCode = SUCCESS_CODE;
    uint32_t lPosixMsgPriority = 0;
    uint64_t lMaxMsgSize = 0;

    returnCode = GetQueueMsgMaxSize(lMaxMsgSize);

    if(returnCode != SUCCESS_CODE)
    {
        return errno;
    }

    auto lReceiveBuffer = new char[lMaxMsgSize];

    if(!lReceiveBuffer)
    {
        return ERROR_CODE;
    }

    auto lReceiveRetCode = mq_receive(
        mMsgQueueDesc, 
        lReceiveBuffer, 
        lMaxMsgSize,
        &lPosixMsgPriority);

    if(lReceiveRetCode == ERROR_CODE && errno != EAGAIN)
    {
        delete[] lReceiveBuffer;
        return errno;
    }

    if(lReceiveRetCode != ERROR_CODE)
    {
        aMessage.content.Insert(lReceiveBuffer, lReceiveRetCode);
        aMessage.priority = priorityFromPosix(lPosixMsgPriority);
        delete[] lReceiveBuffer;
        return SUCCESS_CODE;
    }
    setMessageNotification();

    std::unique_lock<std::mutex> lMutexLock(mCondVarMutex);
    mMessageNotificationCv->wait(lMutexLock);
    return ReceiveAsync(aMessage);

}



int MsgQueueWrapper::GetQueueMsgMaxSize(uint64_t &aMsgMaxSize)
{
    QueueAttrType lAttributes;
    int returnCode = SUCCESS_CODE;

    returnCode = getQueueAttributes(lAttributes);

    if(returnCode != SUCCESS_CODE)
    {
        return returnCode;
    }

    aMsgMaxSize = lAttributes.mq_msgsize;

    return SUCCESS_CODE;
}

 int MsgQueueWrapper::getQueueAttributes(QueueAttrType &aAttributes)
 {
     int returnCode = SUCCESS_CODE;

     if(mMsgQueueDesc != INVALID_MSG_QUEUE_DESC)
     {
         returnCode = mq_getattr(mMsgQueueDesc, &aAttributes);
         if(returnCode == ERROR_CODE)
         {
             returnCode = errno;
             return returnCode;
         }
     }
     else
     {
         returnCode = ERROR_CODE;
     }

     return returnCode;
 }

int MsgQueueWrapper::openQueue(std::string aQueueName, int aMode)
{
    if(mMsgQueueDesc == INVALID_MSG_QUEUE_DESC)
    {
        Close();
    }

    mMsgQueueDesc = mq_open(aQueueName.c_str(), aMode);
    if(mMsgQueueDesc == ERROR_CODE)
    {
        mMsgQueueDesc = INVALID_MSG_QUEUE_DESC;
        return errno;
    }

    return SUCCESS_CODE;
}

uint32_t MsgQueueWrapper::priorityToPosix(MsgPriority aPriorityEnum)
{
    switch(aPriorityEnum)
    {
        case Low:
            return POSIX_PRIORITY_MIN;
        
        case Med:
            return POSIX_PRIORITY_MED;

        case High:
            return POSIX_PRIORITY_HIGH;
        
        default:
            return POSIX_PRIORITY_MIN;
    }
}

MsgQueueWrapper::MsgPriority MsgQueueWrapper::priorityFromPosix(uint32_t aPosixPriority)
{
    switch(aPosixPriority)
    {
        case POSIX_PRIORITY_MIN:
            return Low;
        
        case POSIX_PRIORITY_MED:
            return Med;

        case POSIX_PRIORITY_HIGH:
            return High;
        
        default:
            return Low;
    }
}

int MsgQueueWrapper::setMessageNotification()
{
 
    struct sigevent lNotificationSignal;
    lNotificationSignal.sigev_notify = SIGEV_THREAD;
    lNotificationSignal.sigev_notify_function = receiveAsyncMessageAndNotify;
    lNotificationSignal.sigev_notify_attributes = NULL;
    lNotificationSignal.sigev_value.sival_ptr = mMessageNotificationCv;

    auto lRetVal = mq_notify(mMsgQueueDesc, &lNotificationSignal);
    if(lRetVal != SUCCESS_CODE)
    {
        return errno;
    }

    return SUCCESS_CODE;
}


void MsgQueueWrapper::receiveAsyncMessageAndNotify(union sigval sv)
{
    PConditionalVar lcondVar = (PConditionalVar)sv.sival_ptr;
    lcondVar->notify_one();
}