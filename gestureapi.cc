#define __GESTURE_ERROR_CODES__
#include "gestureapi.hpp"

namespace gestureapi{
    bool GestureInput::begin(){
        char buffer[100];
        DWORD longValue = PIPE_READMODE_MESSAGE;
        inputPipe = CreateFile(TEXT("\\\\.\\pipe\\wxImuApi"),
            GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(inputPipe == INVALID_HANDLE_VALUE){
            errorCode = ERROR_PIPE_OPEN_FAILED;
            return false;
        }

        if(!SetNamedPipeHandleState(inputPipe, &longValue, NULL, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_MESSAGEMODE_FAILED;
            return false;
        }

        if(!WriteFile(inputPipe, (void*) "imuapi1.0", 10, &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            return false;
        }

        if(!ReadFile(inputPipe, buffer, sizeof(buffer), &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            return false;
        }

        if(strcmp(buffer, "imuapi1.0")){
            !WriteFile(inputPipe, (void*) "apiexit", 8, &longValue, NULL);
            CloseHandle(inputPipe);
            errorCode = ERROR_API_RESPONSE_MISMATCH;
        }
        isActive = true;
        return true;
    }

    void GestureInput::end(){
        DWORD longValue;
        if(isActive){
            WriteFile(inputPipe, (void*) "apiexit", 8, &longValue, NULL);
            CloseHandle(inputPipe);
            isActive = false;
        }
    }

    int GestureInput::input(){
        DWORD longValue;
        char buffer[100];
        if(!isActive){
            errorCode = ERROR_CLIENT_NOT_INITIALIZED;
            return -1;
        }
        if(!WriteFile(inputPipe, (void*) "apidetect", 10, &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            isActive = false;
            return -1;
        }
        if(!ReadFile(inputPipe, buffer, sizeof(buffer), &longValue, NULL)){
            CloseHandle(inputPipe);
            errorCode = ERROR_PIPE_TRANSACTION_FAILED;
            isActive = false;
            return -1;
        }
        if(longValue != 1){
            CloseHandle(inputPipe);
            errorCode = ERROR_API_RESPONSE_MISMATCH;
            isActive = false;
            return -1;
        }

        return buffer[0];
    }

    GestureInput::~GestureInput(){
        DWORD longValue;
        if(isActive){
            WriteFile(inputPipe, (void*) "apiexit", 8, &longValue, NULL);
            CloseHandle(inputPipe);
            isActive = false;
        }
    }
}