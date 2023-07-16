#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Table
{
private:

	//variaveis
	unsigned int VAO;

	GLint is_table;

public:

	//métodos
	Table();

	void Init(GLuint programa, glm::mat4 view_projection);

	void Draw(GLuint programa, glm::mat4 view_projection);

	glm::mat4 model_matrix;
	//~Table();
};

