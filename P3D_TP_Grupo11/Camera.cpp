#pragma once

#include "Camera.h"
#include <iostream>
#include <GLFW/glfw3.h>

using namespace std;

void Camera::update() {

	position[2] = ZOOM;
	
	// View
	view = glm::lookAt(
		position,	// Posição da câmara no mundo
		glm::normalize(target + position),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	view = glm::rotate(view, angle, glm::vec3(0, 1, 0));

	// MVP
	mvp = projection * view * model;
}

//construtor
Camera::Camera(glm::vec3 position, glm::vec3 target) {

	this->target = target;
	this->position = position;

	view = glm::lookAt(
		position,	// Posição da câmara no mundo
		position + target,	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
	);

	mvp = projection * view * model;
}

//devolve a mvp
glm::mat4 Camera::getMVP() {
	update();
	return mvp;
}

//devolve a viewProjection
glm::mat4 Camera::getVP() {
	update();
	return projection * view;
}

//devolve a ModelView
glm::mat4 Camera::getMV() {
	update();
	return view * model;
}

//atualiza o zoom
void Camera::SetZoom(double yoffset) {

	//zoom in
	if (yoffset == 1) {
		ZOOM += fabs(ZOOM) * 0.15f;

	}
	// zoom out
	else if (yoffset == -1) {
		ZOOM -= fabs(ZOOM) * 0.15f;
	}

	update();
}