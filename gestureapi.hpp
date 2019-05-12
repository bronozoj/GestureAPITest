#ifndef __UCL7_GESTURE_WRAPPER__
#define __UCL7_GESTURE_WRAPPER__

#ifdef __GESTURE_ERROR_CODES__
#define ERROR_PIPE_OPEN_FAILED 1
#define ERROR_PIPE_MESSAGEMODE_FAILED 2
#define ERROR_PIPE_TRANSACTION_FAILED 3
#define ERROR_API_RESPONSE_MISMATCH 4
#define ERROR_CLIENT_NOT_INITIALIZED 5
#endif

#include <windows.h>

namespace gestureapi{
    class GestureInput{
#ifdef _WIN32
        HANDLE inputPipe;
#endif
        //std::thread gestureService;
        char buffer[150];
        bool isActive = false;
        WORD errorCode = 0;
        
        public:
        bool begin();
        void end();
        int input();
        ~GestureInput();
    };
}

#endif