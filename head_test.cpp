#include<iostream>
#include<GL/glew.h>
#include<stdio.h>
//#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stdlib.h>
#include<memory>
#include<unistd.h>
#include<cmath>

const unsigned int width = 800;
const unsigned int height = 600;
//const unsigned int width = 200;
//const unsigned int height = 200;
GLFWwindow* window;

//Changeable parameters
//boids
const int N = 200;
const float pointSize = 4.0f;
//float step = 0.005;
float step = 0.45;
float dist_thresh = 0.025;
float penal_vel=0.25;
//Initiate all the vertices here
float vertices[3*N];
float velocities[3*N];
unsigned int indices[N];


// openGL variables
unsigned int VBO, VAO, EBO;
int shaderProgram;

// Shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// Functions
bool init(int argc, char const *argv[]);
void initShaders();
void errorCallback(int error, const char *description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initVAO();
void updateVAO();
int mainloop();
float compute_dist(float b1x, float b1y, float b2x, float b2y);
void runSim();
int sign_of(float val);


int main(int argc, char const *argv[]) {
  printf("Just testing");
  return 0;
}
