#include<iostream>
#include<fstream>
#include<Eigen/Dense>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

using namespace Eigen;

const float fovy = (float) PI/4;
const float zNear = 0.1f;
const float zFar = 10.0f;

int width = 1280;
int height = 720;

bool leftMousePressed = false;
bool rightMousePressed = false;
double lastX;
double lastY;
float theta = 1.22f;
float phi = -0.70f;
float zoom = 4.0f;
glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPosition;

glm::vec4 projection;

bool init(int argc, char const *argv[]);
void errorCallback(int error, const char *description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void updateCamera();
