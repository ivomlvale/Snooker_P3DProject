#include "Ball.h"

//construtor
Ball::Ball(string file_name) {

	//nome do ficheiro obj
	this->file_name = file_name;

	//nome ficheiro mtl
	mtl_file_name = file_name;
	mtl_file_name.erase(mtl_file_name.end() - 3, mtl_file_name.end());
	mtl_file_name += "mtl";

	model_matrix = glm::mat4(1.0f);

}

void Ball::Read() {

	string string1;

	ifstream file1;

	//abre o ficheiro
	file1.open(this->file_name);

	//verifica se o ficheiro esta aberto
	if (file1.is_open()) {

		//ate ao fim do ficheiro
		while (!file1.eof())
		{
			//le uma linha
			getline(file1, string1);
			// se for um vertice
			if (string1[0] == 'v' && string1[1] == ' ')
			{
				AddVertices(vec3_vertices, GetStringData(string1));
			} //se for um face
			else if (string1[0] == 'f' && string1[1] == ' ')
			{
				AddFaces(string1);
			} //se for um vertice de textura
			else if (string1[0] == 'v' && string1[1] == 't')
			{
				AddTextureVertices(GetStringData(string1));
			} // se for uma normal
			else if (string1[0] == 'v' && string1[1] == 'n') // ?
			{
				AddVertices(vec3_vertices_normal, GetStringData(string1));
			}
		}
		//fecha o ficheiro
		file1.close();
	}

	// ler o ficheiro mtl
	file1.open(this->mtl_file_name);

	//verifica se o ficheiro esta aberto
	if (file1.is_open()) {

		//ate ao fim do ficheiro
		while (!file1.eof())
		{	
			getline(file1, string1);
			
			//nome da textura
			if (string1[0] == 'm' && string1[1] == 'a')
			{
				//apaga o que esta a mais na string para sobrar apenas o nome da textura
				string1.erase(string1.begin(), string1.begin() + 7);
				texture_name = string1;
			}
		}
		//fecha o ficheiro
		file1.close();
	}
	
	//organiza os vertices
	vertices = Organize(vertices, face_vertices);

	//organiza as normais
	normais = Organize(normais, face_normais);
	
	//organiza os vertices de textura
	vertices_textura = OrganizeVT(vertices_textura, face_vertices_textura);
}

//separa a linha em 3 strings com os dados
vector<string> Ball::GetStringData(string string1) {

	vector<string> string_v;

	string a;

	stringstream sstring(string1);

	while (sstring >> a)
	{
		string_v.push_back(a);
	}

	//elimina o primeiro elemento (v, vt, f)
	string_v.erase(string_v.begin());

	return string_v;
}

//adiciona ao array um novo vertice(vec3)
void Ball::AddVertices(vector<glm::vec3>& vertex, vector<string> string_v) {
		

	//						X						Y				Z
	glm::vec3 vec = glm::vec3(stof(string_v[0]), stof(string_v[1]), stof(string_v[2]));
	
	//adiciona um novo vec3
	vertex.push_back(vec);

	return;
}

//adiciona ao array novas cordenadas de textura
void Ball::AddTextureVertices(vector<string> string_v) {
	
	vertices_textura.push_back(stof(string_v[0]));
	vertices_textura.push_back(stof(string_v[1]));

	return;
}

//preenche os array de indice (v, vt, n) 
void Ball::AddFaces(string string_f) {

	std::size_t delimiter_pos;
	char delimiter = '/';

	//elimina o primeiro elemento 
	string_f.erase(string_f.begin());

	for (size_t i = 0; i < 3; i++)
	{
		delimiter_pos = string_f.find(delimiter);

		//primeiro elemento: vertice
		face_vertices.push_back(stof(string_f.substr(0, delimiter_pos)));
		//apaga a informçao lida
		string_f.erase(string_f.begin(), string_f.begin() + static_cast<int>(delimiter_pos) + 1);

		delimiter_pos = string_f.find(delimiter);

		//segundo elemento: cordenadas textura
		face_vertices_textura.push_back(stof(string_f.substr(0, delimiter_pos)));
		//apaga a informçao lida
		string_f.erase(string_f.begin(), string_f.begin() + static_cast<int>(delimiter_pos) + 1);

		delimiter_pos = string_f.find(" ");

		//terceiro elemento: normal
		face_normais.push_back(stof(string_f.substr(0, delimiter_pos)));
		//apaga a informçao lida
		string_f.erase(string_f.begin(), string_f.begin() + static_cast<int>(delimiter_pos) + 1);
	}
}

//										8446				24192
//organiza os vertices textura
vector<float> Ball::OrganizeVT(vector<float> vector1, vector<int> index) {
	
	vector<float> vertexF;

	for (size_t i = 0; i < index.size(); i++)
	{
		vertexF.push_back(vector1[index[i] * 2 - 2]);
		vertexF.push_back(vector1[index[i] * 2 - 1]);
	}
	
	return vertexF;
}

//organiza os vertices em um vector de floats
vector<float> Ball::Organize(vector<float> vector1, vector<int> index) {
	
	vector<float> verticesF;
		
	for (size_t i = 0; i < index.size(); i++)
	{
		verticesF.push_back(vec3_vertices[index[i] - 1].x);
		verticesF.push_back(vec3_vertices[index[i] - 1].y);
		verticesF.push_back(vec3_vertices[index[i] - 1].z);
	}
		
	return verticesF;
}

//carrega a textura
void Ball::Textura() {
	
	//propriedades da textura
	int width, height, nrChannels;
	unsigned char* data;

	//inverte a imagem 
	stbi_set_flip_vertically_on_load(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// define as parametros necessarias para carregar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load(texture_name.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


void Ball::Send(GLuint programa, glm::mat4 view_projection) {

	//vertices
	GLuint vertexBuffer, textureBuffer, normalBuffer;
	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

	//coordenadas textura
	glGenBuffers(1, &textureBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

	glBufferData(GL_ARRAY_BUFFER, vertices_textura.size() * sizeof(float), &vertices_textura.front(), GL_STATIC_DRAW);

	//vertices normais
	glGenBuffers(1, &normalBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);

	glBufferData(GL_ARRAY_BUFFER, normais.size() * sizeof(float), &normais.front(), GL_STATIC_DRAW);

	//uniform viewProjection
	glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "view_projection"), 1, GL_FALSE, glm::value_ptr(view_projection * model_matrix));

	//seleciona a textura
	glBindTexture(GL_TEXTURE_2D, texture);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertices 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// coordenadas de textura
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	
	//vertices normais
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	//reduz o tamanho
	model_matrix =  glm::scale(model_matrix, glm::vec3(0.5, 0.5, 0.5));

	//posiciona na altura certa
	model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 2.5f, 0.0f));

	//Importa a textura
	Textura();
}

void Ball::Draw(GLuint programa, glm::mat4 view_projection) {

	//seleciona o vao
	glBindVertexArray(VAO);
	
	//seleciona a textura
	glBindTexture(GL_TEXTURE_2D, texture);

	//verifica se é uma mesa
	is_table = glGetUniformLocation(programa, "is_table");
	glUniform1i(is_table, false);

	//atualiza a viewProjection
	glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "view_projection"), 1, GL_FALSE, glm::value_ptr(view_projection));
	glProgramUniformMatrix4fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "model"), 1, GL_FALSE, glm::value_ptr( model_matrix));

	//desenha
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

