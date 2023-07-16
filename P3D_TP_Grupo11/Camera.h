#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4, ...
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...
#include <glm/gtc/type_ptr.hpp> // value_ptr

using namespace std;

class Camera
{
private:
	//variaveis 
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(1920) / float(1080), 0.1f, 100.f);

	float ZOOM = 25.0f;

	glm::vec3 target;
	glm::vec3 direction;

	// MVP
	glm::mat4 mvp;

public:
	
	glm::vec3 position;

	float angle;

	// ModelMatrix
	glm::mat4 model = glm::mat4(1.0f);

	// ViewMatrixx
	glm::mat4 view;

	//Construtor
	Camera(glm::vec3 position, glm::vec3 target);

	void update();

	glm::mat4 getMVP();
	glm::mat4 getVP();
	glm::mat4 getMV();

	void SetZoom(double yoffset);
};
