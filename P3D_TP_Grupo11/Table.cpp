#include "Table.h"

//construtor
Table::Table() {
	
	model_matrix = glm::mat4(1.0f);
}

void Table::Init(GLuint programa, glm::mat4 view_projection) {

	//vertices
	vector<float> verticesCube = {
	-1.0f,-1.0f,-1.0f,	//1
	-1.0f,-1.0f, 1.0f,	//2
	-1.0f, 1.0f, 1.0f,	//3
	1.0f, 1.0f,-1.0f,	//4
	-1.0f,-1.0f,-1.0f,	//1
	-1.0f, 1.0f,-1.0f,	//4
	1.0f,-1.0f, 1.0f,	//5
	-1.0f,-1.0f,-1.0f,	//1
	1.0f,-1.0f,-1.0f,	//6
	1.0f, 1.0f,-1.0f,	//4
	1.0f,-1.0f,-1.0f,	//6
	-1.0f,-1.0f,-1.0f,	//1
	-1.0f,-1.0f,-1.0f,	//1
	-1.0f, 1.0f, 1.0f,	//3
	-1.0f, 1.0f,-1.0f,	//4
	1.0f,-1.0f, 1.0f,	//5
	-1.0f,-1.0f, 1.0f,	//2
	-1.0f,-1.0f,-1.0f,	//1
	-1.0f, 1.0f, 1.0f,	//3
	-1.0f,-1.0f, 1.0f,	//2
	1.0f,-1.0f, 1.0f,	//5
	1.0f, 1.0f, 1.0f,	//7
	1.0f,-1.0f,-1.0f,	//6
	1.0f, 1.0f,-1.0f,	//4
	1.0f,-1.0f,-1.0f,	//6
	1.0f, 1.0f, 1.0f,	//7
	1.0f,-1.0f, 1.0f,	//5
	1.0f, 1.0f, 1.0f,	//7
	1.0f, 1.0f,-1.0f,	//4
	-1.0f, 1.0f,-1.0f,	//4
	1.0f, 1.0f, 1.0f,	//7
	-1.0f, 1.0f,-1.0f,	//4
	-1.0f, 1.0f, 1.0f,	//3	
	1.0f, 1.0f, 1.0f,	//7
	-1.0f, 1.0f, 1.0f,	//3
	1.0f,-1.0f, 1.0f
	};

	//vbo
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesCube.size() * sizeof(float), &verticesCube.front(), GL_STATIC_DRAW);

	//transformacoes
	model_matrix = glm::scale(model_matrix, glm::vec3(6, 0.5, 11));
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.6f, 0.0f));

	//uniform viewProjection
	glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "view_projection"), 1, GL_FALSE, glm::value_ptr(view_projection * model_matrix));

	//vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
}

void Table::Draw(GLuint programa, glm::mat4 view_projection) {
	
	//seleciona vao
	glBindVertexArray(VAO);
	
	is_table = glGetUniformLocation(programa, "is_table");
	glUniform1i(is_table, true);

	//uniform viewProjection
	glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "view_projection"), 1, GL_FALSE, glm::value_ptr(view_projection));
	
	glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	//desenha
	glDrawArrays(GL_TRIANGLES, 0, 35);
}