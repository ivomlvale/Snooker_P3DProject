#pragma once

#include <GL\glew.h>
#include <GL\gl.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class Material
{
private:
	
	string file_name;
	
	GLfloat ns;	  //brilho da luz especular
	glm::vec3 ka; //ambiente
	glm::vec3 kd; //difusa
	glm::vec3 ks; //especular
	GLfloat ni;

	vector<string> GetStringData(string string1);

public:

	void Init(GLuint programa);

	Material(string file_name);

	void LoadMaterial();
};
