#pragma once
#include "tga2d/Engine.h"
#include <functional>
#include <vector>

namespace DX2D
{
    #define ERROR_AUTO_PRINT(aFormat, ...) DX2D::CEngine::GetInstance()->GetErrorManager().ErrorPrint(__FILE__, __LINE__, aFormat, ##__VA_ARGS__);
    #define INFO_PRINT(aFormat, ...) DX2D::CEngine::GetInstance()->GetErrorManager().InfoPrint(aFormat, ##__VA_ARGS__);

    class CErrorManager
    {
    public:
        typedef std::function<void(std::string)> callback_function_log;
		typedef std::function<void(std::string)> callback_function_error;
        CErrorManager();
        ~CErrorManager(void);
		void AddLogListener(callback_function_log aFunctionToCall, callback_function_error aFunctionToCallOnError);
        void Destroy();
        void ErrorPrint(const char* aFile, int aline, const char* aFormat, ...);
        void InfoPrint(const char* aFormat, ...);
    private:
		std::vector<callback_function_log> myLogFunctions;
		std::vector<callback_function_error> myErrorFunctions;
        
    };
}