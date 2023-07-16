#version 440 core



layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 1) in vec3 aNormal;

uniform mat4 view_projection;
uniform mat4 model;
uniform mat3 NormalMatrix;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = view_projection * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = aTexCoord;

	//Posição do vertice em coordenadas do olho
	FragPos = vec3(model * vec4(aPos, 1.0));
	
	//Transformar a normal do vertice
	Normal = NormalMatrix * aNormal;
}