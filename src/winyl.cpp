#ifndef NDEBUG
#define RGFW_DEBUG
#endif

#define RGFW_IMPLEMENTATION
#define RGFW_IMGUI_IMPLEMENTATION
#include "imgui_impl_rgfw.h"
#include "backends/imgui_impl_opengl3.h"
#undef RGFW_IMPLEMENTATION

#include "winyl.hpp"

#include <print>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <shlwapi.h>

#include "station.hpp"

#pragma comment(lib, "Ws2_32.lib")

namespace Winyl
{

App::App(const std::string &title)
{
    RGFW_setHint_OpenGL(RGFW_glMajor, 4);
    RGFW_setHint_OpenGL(RGFW_glMinor, 6);
    RGFW_setHint_OpenGL(RGFW_glProfile, RGFW_glCore);

    this->window = RGFW_createWindow(title.data(), RGFW_RECT(0, 0, 400, 500),
                                     this->rgfwWindowFlags);
    if (this->window == nullptr)
    {
        App::Error("Failed to create a window");
    }

    RGFW_window_makeCurrentContext_OpenGL(this->window);
    this->monitor = RGFW_window_getMonitor(this->window);

#ifndef NDEBUG
    auto gl_version = glGetString(GL_VERSION);
    printf("%s\n", gl_version);
#endif

    this->getStations();
}

void App::Run()
{
    glClearColor(this->openGLClearColor.x, this->openGLClearColor.y,
                 this->openGLClearColor.z, this->openGLClearColor.w);
    glViewport(
        0, 0,
        static_cast<GLsizei>(this->window->r.w * this->monitor.pixelRatio),
        static_cast<GLsizei>(this->window->r.h * this->monitor.pixelRatio));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(this->window->r.w),
                                        static_cast<float>(this->window->r.h));
    ImGui::GetIO().DisplayFramebufferScale =
        ImVec2(this->monitor.scaleX, this->monitor.scaleY);
    ImGui::GetIO().IniFilename = "";
    mainFont                   = ImGui::GetIO().Fonts->AddFontFromFileTTF(
        "C:\\WINDOWS\\FONTS\\SEGOEUI.TTF", 18.0f);

    ImGui_ImplRgfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init();

    while (RGFW_window_shouldClose(this->window) == RGFW_FALSE)
    {
        for (RGFW_event event{}; RGFW_window_checkEvent(this->window, &event);)
        {
            switch (event.type)
            {
            case RGFW_windowMinimized:
                isMinimized = true;
                break;
            case RGFW_windowRefresh:
                isMinimized = false;
                break;
            }
        }

        if (isMinimized)
            continue;

        ImGui::NewFrame();
        ImGui_ImplRgfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("MainWindow", nullptr, this->imguiWindowFlags);

        ImGui::PushFont(mainFont, 32.0f);
        ImGui::Text("Stations List");
        ImGui::PopFont();

        ImGui::PushFont(mainFont, 18.0f);

        int i = 0;
        for (Winyl::Station &s : stations)
        {
            ImGui::PushID(i++);
            if (ImGui::CollapsingHeader(s.Name.c_str()))
            {
                static char buffer[MAX_PATH]{};

                std::memset(buffer, '\0', MAX_PATH);
                s.Url.copy(buffer, MAX_PATH - 1);

                ImGui::BeginGroup();
                /**/ ImGui::AlignTextToFramePadding();
                /**/ ImGui::Text("URL");
                /**/ ImGui::SameLine();
                /**/ ImGui::InputText("##station_url", buffer, MAX_PATH);
                /**/ ImGui::SameLine();
                if (ImGui::Button("Play"))
                    s.ChangeState();
                ImGui::EndGroup();
            }
            ImGui::PopID();
        }
        ImGui::PopFont();

        ImGui::End();
        ImGui::PopStyleVar();

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        this->dd = ImGui::GetDrawData();
        ImGui_ImplOpenGL3_RenderDrawData(this->dd);

        RGFW_window_swapBuffers_OpenGL(this->window);
    }
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplRgfw_Shutdown();
    ImGui::DestroyContext();

    RGFW_window_close(this->window);
}

void App::getStations()
{
    char pathBuffer[MAX_PATH];
    std::string cfgPath{};
    std::fstream cfg{};

    GetModuleFileName(nullptr, pathBuffer, MAX_PATH);
    PathRemoveFileSpec(pathBuffer);
    cfgPath = pathBuffer;
    cfgPath += "\\winyl.cfg";

    // Fill with default data
    if (!std::filesystem::exists(cfgPath))
    {
        cfg.open(cfgPath, std::fstream::out | std::fstream::trunc);
        cfg << "#Lofi Hunter" << std::endl;
        cfg << "https://live.hunter.fm/lofi_high" << std::endl;
        cfg.close();
    }

    cfg.open(cfgPath, std::fstream::in);
    for (std::string line{}; std::getline(cfg, line);)
    {
        Winyl::Station s{};
        if (!line.starts_with('#'))
        {
            App::Error("\"#\" expected in the start of the line");
        }
        s.Name = line.erase(0, 1);

        std::getline(cfg, line);
        if (line == "")
        {
            App::Error("URL expected");
        }
        else if (line.size() > MAX_PATH)
        {
            App::Error("URL has more than 260 characters");
        }

        s.Url = line;
        this->stations.push_back(s);
    }
    cfg.close();
}

void App::Error(const std::string &msg)
{
    std::println(std::cerr, "ERROR:\t{}", msg);
    std::terminate();
}
} // namespace Winyl
