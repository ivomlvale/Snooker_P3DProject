#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iomanip>

#include "stb_image.h"

using namespace std;

class Ball
{
private:
	
	//variaveis
	unsigned int texture;
	unsigned int VAO;

	string file_name;
	string mtl_file_name;
	string texture_name;

	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	GLfloat ns;
	GLfloat ni;


	vector<float> vertices;
	vector<float> vertices_textura;
	vector<float> normais;
	
	vector<int> face_vertices;
	vector<int> face_vertices_textura;
	vector<int> face_normais;

	vector<glm::vec3> vec3_vertices;
	vector<glm::vec3> vec3_vertices_normal;

	GLint is_table;

	//métodos
	vector<string> GetStringData(string string1);

	void AddVertices(vector<glm::vec3>& vertex, vector<string> string_v);

	void AddTextureVertices(vector<string> string_v);

	void AddFaces(string string_v);

	vector<float> Organize(vector<float> vector1, vector<int> index);

	vector<float> OrganizeVT(vector<float> vector1, vector<int> index);
	
	void Textura();

public:

	glm::mat4 model_matrix;

	Ball(string file_name);

	void Read();
	
	void Send(GLuint programa, glm::mat4 view_projection);

	void Draw(GLuint programa, glm::mat4 view_projection);

	//~Ball();
};
