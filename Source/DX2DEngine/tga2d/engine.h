/*
Engine
This class is the interface between game logic and rendering. 
It takes care of creating everything necessary for window handling and rendering
*/

#pragma once
#include "math/Vector2.h"
#define _X86_
#include <windef.h>

#include <functional>
#include "math/Color.h"
#include <chrono>

// Forward declarations
//
namespace DX2D
{
    class CWindowsWindow;
    class CDirectEngine;
    class CRenderer;
    class CTextureManager;
    class CDebugDrawer;
    class CInputManager;
    class CTextService;
    class CLightManager;
    class CErrorManager;
    class CFileWatcher;
	//class CFbxLoader;
}


namespace DX2D
{
    typedef std::function<void()> callback_function;
	typedef std::function<void()> callback_function_update;
    typedef std::function<void(std::string)> callback_function_log;
	typedef std::function<void(std::string)> callback_function_error;

	enum EWindowSetting
	{
		EWindowSetting_Overlapped,
		EWindowSetting_Borderless,
	};
    struct SEngineCreateParameters
    {
		SEngineCreateParameters()
		{ 
			myHwnd = nullptr; 
			myHInstance = nullptr;  
			myWindowWidth = 800; 
			myWindowHeight = 600; 
			myEnableVSync = false; 
			myMaxRenderedObjectsPerFrame = 100000;  // Legacy, not used
			myRenderWidth = myWindowWidth; 
			myRenderHeight = myWindowHeight; 
			myErrorFunction = nullptr; 
			myStartInFullScreen = false;
			myWindowSetting = EWindowSetting_Overlapped;
		}
        callback_function myInitFunctionToCall;
        callback_function_update myUpdateFunctionToCall;
        callback_function_log myLogFunction;
		callback_function_error myErrorFunction;
        unsigned short myWindowWidth;
        unsigned short myWindowHeight;
		unsigned short myRenderWidth;
		unsigned short myRenderHeight;
        /* Will show the FPS and memory text*/
        bool myActivateDebugSystems;
        CColor myClearColor;
        HWND *myHwnd;
        HINSTANCE myHInstance;
        std::wstring myApplicationName;
        bool myEnableVSync;
		bool myStartInFullScreen;
        int myMaxRenderedObjectsPerFrame; // Legacy, not used
		EWindowSetting myWindowSetting;
    };


    class CEngine
    {
        friend class CTextureManager;
        friend class CRenderer;
        friend class CShader;
        friend class CErrorManager;
		friend class CDirectEngine;
    public:
        CEngine &operator =( const CEngine &anOther ) = delete;
        static void CreateInstance( const SEngineCreateParameters& aCreateParameters);
        static CEngine* GetInstance() {return myInstance;}
        static void DestroyInstance();

        bool Start();
		void Shutdown();
        
        CWindowsWindow& GetWindow() const {return *myWindow;}
        CRenderer& GetRenderer() const {return *myRenderer;}
        CTextureManager& GetTextureManager() const {return *myTextureManager;}

        CDebugDrawer& GetDebugDrawer() const {return *myDebugDrawer;}
        CInputManager& GetInputManager() const { return *myInputManager; }

        CTextService& GetTextService() const { return *myTextService; }
        CLightManager& GetLightManager() const { return *myLightManager; }

        CErrorManager& GetErrorManager() const { return *myErrorManager; }

        void SetWorldMatrixPosition( const Vector2f &aPosition);
        void SetWorldMatrixZoom(const float aZoom);
        
        Vector2<unsigned int> GetWindowSize() const {return myWindowSize;}
		Vector2<unsigned int> GetRenderSize() const { return myRenderSize; }
        float GetWindowRatio() const;
		float GetWindowRatioInversed() const;
        CDirectEngine& GetDirect3D() const { return *myDirect3D; }

        void SetResolution(const DX2D::Vector2<unsigned int> &aResolution, bool aAlsoSetWindowSize = true);

        HWND* GetHWND() const;
        HINSTANCE GetHInstance() const;

        CFileWatcher* GetFileWatcher(){ return myFileWatcher; }

        bool GetIsDebugSystemsActive() const { return myCreateParameters.myActivateDebugSystems; }
		void SetViewPort(float aTopLeftX, float aTopRightY, float aWidth, float aHeight, float aMinDepth = 0.0f, float aMaxDepth = 1.0f);

		//CFbxLoader* GetFBXLoader() const { return myFBXLoader; }

		void SetFullScreen(bool aFullScreen);
    private:
        CEngine(const SEngineCreateParameters& aCreateParameters);
        ~CEngine();

        void StartStep();
        void DoStep();
		void CalculateRatios();

        static CEngine* myInstance;

        callback_function_update myUpdateFunctionToCall;
        callback_function myInitFunctionToCall;
        CWindowsWindow* myWindow;
        CDirectEngine* myDirect3D;
        CRenderer* myRenderer;
        CTextureManager* myTextureManager;
        CDebugDrawer* myDebugDrawer;
        CInputManager* myInputManager;
        CTextService* myTextService;
        Vector2<unsigned int> myWindowSize;
		Vector2<unsigned int> myRenderSize;
        Vector2<unsigned int> myNativeRenderSize;
        HWND *myHwnd;
		//CFbxLoader* myFBXLoader;
        HINSTANCE myHInstance;
        CColor myClearColor;
        SEngineCreateParameters myCreateParameters;
        CLightManager* myLightManager;
        CErrorManager* myErrorManager;
        CFileWatcher* myFileWatcher;

		bool myRunEngine;
		float myWindowRatio;
		float myWindowRatioInversed;

		std::chrono::system_clock::time_point start;
    };
}
