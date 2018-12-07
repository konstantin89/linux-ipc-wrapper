#ifndef __COMMON_H
#define __COMMON_H

#include <string>
#include <iostream>

#define MSG_QUEUE_NAME "/test_queue_1"

#define SUCCESS_CODE 0

#define printError(errCode, errMsg)\
    do{\
        std::cout << "[X] " << errMsg;\
        std::cout << " [" << errCode << "]" <<std::endl;\
    }while(0)


#endif