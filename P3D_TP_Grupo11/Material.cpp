#include "Material.h"

//construtor
Material::Material(string file_name) {
	this->file_name = file_name;
}

void Material::LoadMaterial() {

	string string1;
	vector<string> string_v;
	ifstream file1;

	file1.open(this->file_name);

	//verifica se o ficheiro esta aberto
	if (file1.is_open()) {

		//ate ao fim do ficheiro
		while (!file1.eof())
		{
			getline(file1, string1);

			if (string1[0] == 'N' && string1[1] == 's') {

				string1.erase(string1.begin(), string1.begin() + 2);
				
				ns = stof(string1);
			}

			if (string1[0] == 'K' && string1[1] == 'a') {
				
				string_v = GetStringData(string1);

				ka.x = stof(string_v[0]);
				ka.y = stof(string_v[1]);
				ka.z = stof(string_v[2]);

			}

			if (string1[0] == 'K' && string1[1] == 'd') {
			
				string_v = GetStringData(string1);

				kd.x = stof(string_v[0]);
				kd.y = stof(string_v[1]);
				kd.z = stof(string_v[2]);
			}

			if (string1[0] == 'K' && string1[1] == 's') {

				string_v = GetStringData(string1);

				ks.x = stof(string_v[0]);
				ks.y = stof(string_v[1]);
				ks.z = stof(string_v[2]);
			}

			if (string1[0] == 'N' && string1[1] == 'i') {

				string1.erase(string1.begin(), string1.begin() + 2);
				
				ni = stof(string1);
			}
		}
	}
}

vector<string> Material::GetStringData(string string1) {

	vector<string> string_v;

	string a;

	stringstream sstring(string1);

	while (sstring >> a)
	{
		string_v.push_back(a);
	}

	//elimina o primeiro elemento (v, vt, f)
	string_v.erase(string_v.begin(), string_v.begin() + 1);

	return string_v;
}

void Material::Init(GLuint programa) {

	//posiçao da luz
	glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "lightPos"), 1, glm::value_ptr(lightPos));

	//luz ambiente
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "ambient_light"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "ambientStrength"), ka.x);
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(ka));

	//luz difusa
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(kd));

	//luz especular
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.shininess"), ns);
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(ks));
}