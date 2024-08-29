#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H

#include <GLFW/glfw3.h>

class ImGuiManager {
public:
    void setup(GLFWwindow* window);
    void cleanup();
};

#endif // IMGUIMANAGER_H
