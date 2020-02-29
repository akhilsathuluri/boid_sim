#include "main.hpp"
//#include "glad.c"

// Main function
int main(int argc, char const *argv[]){
	srand (static_cast <unsigned> (time(0)));
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

	// for(int i=0;i<N;i++){
	// center[3*i+0]=0.0f;
	// center[3*i+1]=0.0f;
	// center[3*i+2]=0.0f;
	// velcenter[3*i+0]=0.0f;
	// velcenter[3*i+1]=0.0f;
	// velcenter[3*i+2]=0.0f;
	// repel[3*i+0]=0.0f;
	// repel[3*i+1]=0.0f;
	// repel[3*i+2]=0.0f;
	// }
	for(int i=0;i<N;i++){
		center[3*i+0]=0.0f;
		center[3*i+1]=0.0f;
		center[3*i+2]=0.0f;
		velcenter[3*i+0]=0.0f;
		velcenter[3*i+1]=0.0f;
		velcenter[3*i+2]=0.0f;
		repel[3*i+0]=0.0f;
		repel[3*i+1]=0.0f;
		repel[3*i+2]=0.0f;
		count=0;
		for(int j=0;j<N;j++){
			dist = compute_dist(vertices[3*i+0], vertices[3*i+1], vertices[3*j+0], vertices[3*j+1]);
			if(j!=i && dist < dist_thresh){
				// Alignment
				velcenter[3*i+0]+=velocities[3*j+0];
				velcenter[3*i+1]+=velocities[3*j+1];
				// Cohesion
				center[3*i+0]+=vertices[3*j+0];
				center[3*i+1]+=vertices[3*j+1];
				//Seperation
				repel[3*i+0] += (vertices[3*i+0]-vertices[3*j+0])*(dist_thresh-dist);
				repel[3*i+1] += (vertices[3*i+1]-vertices[3*j+1])*(dist_thresh-dist);
				count++;
			}
		}
		if (count!=0) {
			velcenter[3*i+0]=velcenter[3*i+0]/(float)count;
			velcenter[3*i+1]=velcenter[3*i+1]/(float)count;
			center[3*i+0]=center[3*i+0]/(float)count;
			center[3*i+1]=center[3*i+1]/(float)count;
			repel[3*i+0] = repel[3*i+0]/(float)count;
			repel[3*i+1] = repel[3*i+1]/(float)count;
			// Update velocities due to alignment and cohesion
			velocity2[3*i+0] = (velcenter[3*i+0]-velocities[3*i+0])/2.0f;
			velocity2[3*i+1] = (velcenter[3*i+1]-velocities[3*i+1])/2.0f;
			velocity1[3*i+0] = (center[3*i+0]-vertices[3*i+0])/100.0f;
			velocity1[3*i+1] = (center[3*i+1]-vertices[3*i+1])/100.0f;
		}
	}

	for(int i=0;i<N;i++){
		//Update all the new vertices
		// Since we are only updating the vertices, I have observed that the particles lose energy and tend to reach a slow moving state. To make it more exciting added a small random velocity to the motion.
		velocities[3*i+0] += velocity2[3*i+0]+velocity1[3*i+0]+0.2*repel[3*i+0]+(LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))))/30.0f;
		velocities[3*i+1] += velocity2[3*i+1]+velocity1[3*i+1]+0.2*repel[3*i+1]+(LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))))/30.0f;
			// if (fabs(velocities[3*i+0])>speed_limit) {
			// 	//velocities[3*i+0] += velocity1[3*i+0]+0*velocity2[3*i+0]+0*repel[3*i+0];
			// 	velocities[3*i+0] = sign_of(velocities[3*i+0])*speed_limit;
			// }
			// if (fabs(velocities[3*i+1])>speed_limit) {
			// 		//velocities[3*i+1] += velocity1[3*i+1]+0*velocity2[3*i+1]+0*repel[3*i+1];
			// 		velocities[3*i+1] = sign_of(velocities[3*i+1])*speed_limit;
			// }
		//Update vertices
		vertices[3*i+0]+=velocities[3*i+0]*step;
		vertices[3*i+1]+=velocities[3*i+1]*step;
		//std::cout<<"vertex: "<<vertices[3*i+0]<<", "<<vertices[3*i+1]<<", "<<vertices[3*i+2]<<" : "<<indices[i]<<std::endl;
		// Bounding visual to the viewable space. Changing it to a elastic wall
		if(vertices[3*i+0]>1){
			// velocities[3*i+0]=-penal_vel;
			vertices[3*i+0]=-1;
		}
		if(vertices[3*i+0]<-1){
			// velocities[3*i+0]=penal_vel;
			vertices[3*i+0]=1;
		}
		if(vertices[3*i+1]<-1){
			// velocities[3*i+1]=penal_vel;
			vertices[3*i+1]=1;
		}
		if(vertices[3*i+1]>1){
			// velocities[3*i+1]=-penal_vel;
			vertices[3*i+1]=-1;
		}
		// if(vertices[3*i+1]>1 or vertices[3*i+1]<-1){
		// velocities[3*i+1]*=-1;
		// }
		// if(vertices[3*i+0]>1 or vertices[3*i+0]<-1){
		// velocities[3*i+0]*=-1;
		// }
	}
	//Once the value is returned from all the rules check if they are within the screen

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}

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
	//vertices[3*i+0]= (float)(rand()%2);
	vertices[3*i+0]= LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	//vertices[3*i+0]= ;
	vertices[3*i+1]= LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	vertices[3*i+2]= 0.0f;
	indices[i]=i;
	//velocities[3*i+0]= (float)(rand()%10)/15.0f;
	velocities[3*i+0]= LO_v + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI_v-LO_v)));
	// velocities[3*i+1]= (float)(rand()%10)/15.0f;
	velocities[3*i+1]= LO_v + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI_v-LO_v)));
	velocities[3*i+2]= 0.0f;

	//std::cout<<"vertex: "<<vertices[3*i+0]<<", "<<vertices[3*i+1]<<", "<<vertices[3*i+2]<<" : "<<indices[i]<<std::endl;
	//std::cout<<"velocity: "<<velocities[3*i+0]<<", "<<velocities[3*i+1]<<", "<<velocities[3*i+2]<<std::endl;
	}
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
