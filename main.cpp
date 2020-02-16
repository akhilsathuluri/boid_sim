/*
My own version of boid simulation
This is a CPU only version using OpenGL
*/
#include "main.hpp"
// ============
// OpenGL configuration
// ============
const int N_boid = 5000;

int main(int argc, char const *argv[]) {
  if(init(argc, argv)){
    std::cout<<"Things are working fine in init"<<std::endl;
    }
    return 0;
  }


// ============
// Initialising GLFW
// ============

bool init(int argc, char const *argv[]){
// Window setup
  glfwSetErrorCallback(errorCallback);

  if(!glfwInit()){
    std::cout
    << "Error: Could not initialise GLFW"
    << "Is OpenGL 3.3 available?"
    << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_CONPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, "SimSpace", NULL, NULL);
  if(!window){
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetCursorPosCallback(window, mousePositionCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);

  glewExperimental = GL_TRUE;
  if(glewInit() !=GLEW_OK){
    std::cout
    << "Error initialising GLEW\n"
    << "Perhaps the window is not set to the current context?"
    <<std::endl;
    return false;
  }
}

// =============
// Setting callback functions
// =============

void errorCallback(int error, const char *description){
  fprintf(stderr, "error %d: %s\n", error, description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
  leftMousePressed = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
  rightMousePressed = (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
}

void mousePositionCallback(GLFWwindow* window, double xpos, double ypos){
  if(leftMousePressed){
    // Change viewpoint
    phi += (xpos-lastX)/width;
    theta -= (ypos-lastY)/height;
    theta = std::fmax(0.01f, std::fmin(theta, 3.14f));
  }
  else if(rightMousePressed){
    // Change zoom
    zoom += (ypos-lastY)/height;
    zoom = std::fmax(0.1f, std::fmin(zoom, 5.0f));
    updateCamera();
  }

  lastX = xpos;
  lastY = ypos;
}

void updateCamera(){
  cameraPosition.x = zoom * sin(phi) * sin(theta);
  cameraPosition.y = zoom * cos(theta);
  cameraPosition.z = zoom * cos(phi) * sin(theta);
  cameraPosition += lookAt;

  projection = glm::perspective(fovy, float(width)/float(height), zNear, zFar);
  glm::mat4 view = glm::lookAt(cameraPosition, lookAt, glm::vec3(0,0,1));
  projection = projection * view;

  GLint location;
//See meanng of ths statement
// https://github.com/CIS565-Fall-2018/Project1-CUDA-Flocking/blob/master/src/main.cpp
//https://github.com/CIS565-Fall-2018/Project1-CUDA-Flocking/blob/master/src/main.hpp
//https://learnopengl.com/Getting-started/Shaders
  glUseProgram(program[])
}
