#include "main.hpp"
//#include "glad.c"

// Main function
int main(int argc, char const *argv[]){
	init(argc, argv);
	mainloop();
	return 0;
}

int mainloop(){
	// Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	while(!glfwWindowShouldClose(window)){

		//Run simulation and copy the new points to VBO
		runSim();

		// to set the color whenever glClear is called
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_POINTS, N, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
	}
	// Unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// Normal velocity update
void runSim(){
	float center[3*N];
	float velcenter[3*N];
	float repel[3*N];
	float velocity1[3*N];
	float velocity2[3*N];
	for(int i=0;i<N;i++){
		//std::cout<<"vertex-1 "<<i<<": "<<vertices[0]<<", "<<vertices[1]<<", "<<vertices[2]<<std::endl;
		//std::cout<<"vertex-2 "<<i<<": "<<vertices[3]<<", "<<vertices[4]<<", "<<vertices[5]<<std::endl;
		//Initialising to 0
		center[3*i+0]=0.0f;
		center[3*i+1]=0.0f;
		center[3*i+2]=0.0f;
		velcenter[3*i+0]=0.0f;
		velcenter[3*i+1]=0.0f;
		velcenter[3*i+2]=0.0f;
		repel[3*i+0]=0.0f;
		repel[3*i+1]=0.0f;
		repel[3*i+2]=0.0f;
		for(int j=0;j<N;j++){
			if(j!=i){
				center[3*i+0]+=vertices[3*j+0];
				center[3*i+1]+=vertices[3*j+1];
				// Velocity average
				velcenter[3*i+0]+=velocities[3*j+0];
				velcenter[3*i+1]+=velocities[3*j+1];
				if(compute_dist(vertices[3*i+0], vertices[3*i+1], vertices[3*j+0], vertices[3*j+1]) < dist_thresh){
					//repel[3*i+0] += vertices[3*i+0]-vertices[3*j+0];
					//repel[3*i+1] += vertices[3*i+1]-vertices[3*j+1];
					//Making the repulsion equivalent to a spring force
					repel[3*i+0] += sign_of(vertices[3*i+0]-vertices[3*j+0])*(dist_thresh-fabs(vertices[3*i+0]-vertices[3*j+0]));
					repel[3*i+1] += sign_of(vertices[3*i+1]-vertices[3*j+1])*(dist_thresh-fabs(vertices[3*i+1]-vertices[3*j+1]));
				}
			}
		}
		//Observations
		//The more stronger the central factor is more of them move together
		//The more stronger the common velocity factor the more natural it looks as if theyre moving together
		//Without repulsion and boundary the boids have infinite space, hence theyll simply reach an equilibrium and keep moving
		//Current idea of distance doesnt make sense as they do not see any repulsion if they are initiated on top of each other
		center[3*i+0]=center[3*i+0]/(float)(N-1);
		center[3*i+1]=center[3*i+1]/(float)(N-1);
		velcenter[3*i+0]=velcenter[3*i+0]/(float)(N-1);
		velcenter[3*i+1]=velcenter[3*i+1]/(float)(N-1);
		//std::cout<<"center "<<i<<": "<<center[3*i+0]<<", "<<center[3*i+1]<<", "<<center[3*i+2]<<std::endl;
		velocity1[3*i+0] = (center[3*i+0]-vertices[3*i+0])/100.0f;
		velocity1[3*i+1] = (center[3*i+1]-vertices[3*i+1])/100.0f;
		velocity1[3*i+2]=0.0f;
		velocity2[3*i+0] = (velcenter[3*i+0]-velocities[3*i+0])/8.0f;
		velocity2[3*i+1] = (velcenter[3*i+1]-velocities[3*i+1])/8.0f;
		velocity2[3*i+2]=0.0f;
	}

	for(int i=0;i<N;i++){
		//Update all the new vertices
		//vertices[3*i+0]+=(velocities[3*i+0]/3.0f+velocity1[3*i+0]+velocity2[3*i+0]+repel[3*i+0])*step;
		//vertices[3*i+1]+=(velocities[3*i+1]/3.0f+velocity1[3*i+1]+velocity2[3*i+1]+repel[3*i+1])*step;
		vertices[3*i+0]+=velocities[3*i+0]*step;
		vertices[3*i+1]+=velocities[3*i+1]*step;
		//Update all the new velocities
		velocities[3*i+0] += velocity1[3*i+0]+velocity2[3*i+0]+repel[3*i+0];
		velocities[3*i+1] += velocity1[3*i+1]+velocity2[3*i+1]+repel[3*i+1];
		// Bounding visual to the viewable space. Changing it to a elastic wall
		if(vertices[3*i+0]>1){
			velocities[3*i+0]=-penal_vel;
		}
		if(vertices[3*i+0]<-1){
			velocities[3*i+0]=penal_vel;
		}
		if(vertices[3*i+1]<-1){
			velocities[3*i+1]=penal_vel;
		}
		if(vertices[3*i+1]>1){
			velocities[3*i+1]=-penal_vel;
		}
		//if(vertices[3*i+1]>1 or vertices[3*i+1]<-1){
		//velocities[3*i+1]*=-2;
		//}
		//if(vertices[3*i+0]>1 or vertices[3*i+0]<-1){
		//velocities[3*i+0]*=-2;
		//}
	}
	//Once the value is returned from all the rules check if they are within the screen

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}

//float rule1_velocity(){
	//Move rule 1 stuff into this function and return pointer for velocity1
//}

int sign_of(float val){
	if(val>0) return 1;
	if(val<0) return -1;
	return 0;
}

float compute_dist(float b1x, float b1y, float b2x, float b2y){
	float dist = sqrt(pow((b1x-b2x),2)+pow((b1y-b2y),2));
	return dist;
}

bool init(int argc, char const *argv[]){
	// Initialise all the openGL settings
	glfwInit();
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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	// Making all the extensions valid
	glewExperimental = GL_TRUE;
	glewInit();

	if(glewInit() !=GLEW_OK){
		std::cout
		<< "Error initialising GLEW\n"
		<< "Perhaps the window is not set to the current context?"
		<<std::endl;
		return false;
	}
	// Initialise shaders
	initShaders();
	//Initialise drawing state
	initVAO();
	return true;
}

void initVAO(){
	// Declaring the vertices randomly
	for(int i=0;i<N;i++){
	vertices[3*i+0]= (float)(rand()%10)/20.0f;
	vertices[3*i+1]= (float)(rand()%10)/20.0f;
	vertices[3*i+2]= 0.0f;
	indices[i]=i;

	velocities[3*i+0]= (float)(rand()%10)/20.0f;
	velocities[3*i+1]= (float)(rand()%10)/20.0f;
	velocities[3*i+2]= 0.0f;

	//std::cout<<"vertex: "<<vertices[3*i+0]<<", "<<vertices[3*i+1]<<", "<<vertices[3*i+2]<<" : "<<indices[i]<<std::endl;
	//std::cout<<"velocity: "<<velocities[3*i+0]<<", "<<velocities[3*i+1]<<", "<<velocities[3*i+2]<<std::endl;
	}
		std::cout<<"testing"<<std::endl;
		glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
    glBindVertexArray(0);
}

void initShaders(){
	glPointSize(pointSize);
	// vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
