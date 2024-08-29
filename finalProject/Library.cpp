#include "Library.h"
#include "Manager.h"
#include "UI.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Application::run() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Library Management App", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGuiManager imguiManager;
    imguiManager.setup(window);

    mainLoop(window);

    imguiManager.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::mainLoop(GLFWwindow* window) {
    BookManager bookManager;  // Create an instance of BookManager
    BookUI bookUI(bookManager);  // Pass the BookManager instance to BookUI

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bookUI.render();  // Render the UI

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

int main() {
    Application app;
    app.run();
    return 0;
}