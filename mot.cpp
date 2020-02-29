// This file implements the boids rules
// There are three main rules: (https://www.red3d.com/cwr/boids/)
// Rule-1: Cohesion: Average heading to local neighbours; try to move towards the center of the neighbours
// Rule-2: Alignment: Maintain the speed of the neighbour; try to match speed and direction of the neighbours
// Rule-3: Seperation: Avoid local crowding; try to be at a distance d from the neighbours

// Update based on random velocity
// Using Explicit Euler integration scheme

// Limit the speed and position before drawing

#include<numeric>
#include<iostream>
#include<unistd.h>
#include<cmath>

int main(){
	// Initialise positions
	float sum = 0.0f;
	const int N = 1;
	float vertices[3*N], velocities[3*N];
	for(int i=0;i<N;i++){
	vertices[3*i+0]= (float)(rand()%10)/20.0f;
	vertices[3*i+1]= (float)(rand()%10)/20.0f;
	vertices[3*i+2]= 0.0f;
	
	velocities[3*i+0]= (float)(rand()%10)/20.0f;
	velocities[3*i+1]= (float)(rand()%10)/20.0f;
	velocities[3*i+2]= 0.0f;
	}
	
	//Limit the speed and position before drawing
	std::cout<<center<<std::endl;
	return 0;
}

float compute_dist(float b1x, float b1y, float b2x, float b2y){
	float dist = sqrt((b1x-b2x)^2+(b1y-b2y)^2);
	return dist;
}

float Cohesion(){
	float sum=0.0f;
	float vel1[3*N];
	float center[3]={0,0,0};
	float velcenter[3]={0,0,0};
	float vel2[3*N];
	float nbd = 10.0f;
	float c;
	float dist;
	int j=0;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(j!=i){
				center[0]+=vertices[3*j+0];
				center[1]+=vertices[3*j+1];
				
				velcenter[0]+=velocities[3*j+0];
				velcenter[1]+=velocities[3*j+1];
				
				//check distance less than threshold
				dist = compute_dist(vertex[3*i+0],vertex[3*i+1],vertex[3*j+0],vertex[3*j+1])
				if(dist<=nbd){
					c-=dist;
				}
				
			}
			//Division should be between floats
			center[0]=center[0]/(N-1);
			center[1]=center[1]/(N-1);
			center[2]=0.0f;
			
			velcenter[0]=velcenter[0]/(N-1);
			velcenter[1]=velcenter[1]/(N-1);
			velcenter[2]=0.0f;
			
			vel1[3*i+0] = center[0];
			vel1[3*i+1] = center[1];
			vel1[3*i+2] = center[2];
			
			vel2[3*i+0] = velcenter[0];
			vel2[3*i+1] = velcenter[1];
			vel2[3*i+2] = velcenter[2];
		}
	}
//Returns average center position for each boid
return vel1;
return vel2;
return c;
}
