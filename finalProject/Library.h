#ifndef APPLICATION_H
#define APPLICATION_H

#include "Manager.h"
#include "UI.h"
#include "UIManager.h"

class Application {
public:
    void run();

private:
    void mainLoop(GLFWwindow* window);
};

#endif // APPLICATION_H
