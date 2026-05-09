#pragma once
#include <assert.h>
#include <stdio.h>

#include <renderdoc_app.h>
#include <glad/glad.h>

#ifdef _WIN32
        #include <windows.h>
#else
        #include <dlfcn.h>
#endif

class RenderDocIntegration
{
public:
        static void Init()
        {
#if defined(_WIN32)
                HMODULE module = LoadLibraryA("renderdoc.dll");
                if (!module)
                {
                        // Try the default install path
                        module = LoadLibraryA("C:/Program Files/RenderDoc/renderdoc.dll");
                }
                if (module)
                {
                        pRENDERDOC_GetAPI getApi =
                        (pRENDERDOC_GetAPI)GetProcAddress(module, "RENDERDOC_GetAPI");
                        int ret = getApi(eRENDERDOC_API_Version_1_7_0, (void**)&s_api);
                        assert(ret == 1);
                        printf("RendeDocIntegration::renderdoc.dll loaded!");
                }
#elif defined(__linux__)
                void* module = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD);
                if (module)
                {
                        pRENDERDOC_GetAPI getApi =
                                (pRENDERDOC_GetAPI)dlsym(module, "RENDERDOC_GetAPI");
                        int ret = getApi(eRENDERDOC_API_Version_1_6_0, (void**)&s_api);
                        assert(ret == 1);
                        printf("RendeDocIntegration::librenderdoc.so loaded!");
                }
#endif
                if (s_api)
                {
                        s_api->SetCaptureOptionU32(eRENDERDOC_Option_CaptureCallstacks, 1);
                        s_api->SetCaptureOptionU32(eRENDERDOC_Option_CaptureAllCmdLists, 1);
                        s_api->SetCaptureOptionU32(eRENDERDOC_Option_APIValidation, 1);
                        // Suppress the in-game overlay
                        s_api->MaskOverlayBits(eRENDERDOC_Overlay_None, eRENDERDOC_Overlay_None);
                }
        }

        /** Capture next frame */
        static void TriggerCapture()
        {
                if (s_api) s_api->TriggerCapture();
        }

        // Scope captures
        static void StartCapture(void* deviceHandle, void* windowHandle)
        {
                if (s_api) s_api->StartFrameCapture(deviceHandle, windowHandle);
        }

        static void EndCapture(void* deviceHandle, void* windowHandle)
        {
                if (s_api) s_api->EndFrameCapture(deviceHandle, windowHandle);
        }

        static RENDERDOC_API_1_7_0* Get() { return s_api; }

private:
        inline static RENDERDOC_API_1_7_0* s_api = nullptr;
};

struct GLDebugScope
{
        GLDebugScope(const char* name)
        {
                // GL_DEBUG_SOURCE_APPLICATION tells the driver this came from app code.
                // The id can be anything you use to categorize; 0 is fine.
                glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, (GLsizei)strlen(name), name);
        }

        ~GLDebugScope()
        {
                glPopDebugGroup();
        }

        GLDebugScope(const GLDebugScope&) = delete;
        GLDebugScope& operator=(const GLDebugScope&) = delete;
};

#define PROFILER_SCOPE(name, ...) \
        GLDebugScope _prof_scope_##__LINE__((name))
#define PROFILER_SCOPE_COLOR(name, ...) PROFILER_SCOPE(name)
