#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

//default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.05f;
const float FOV = 45.0f;

class Camera
{
private:
	void update_camera_vectors();
public:
	//camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;
	//eular angles
	float yaw;
	float pitch;
	//camera options
	float movement_speed;
	float mouse_sensivity;
	float fov;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensivity(SENSITIVITY), fov(FOV) {
		this->position = position;
		this->world_up = up;
		this->yaw = yaw;
		this->pitch = pitch;
		update_camera_vectors();
	}

	Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensivity(SENSITIVITY), fov(FOV) {
		position = glm::vec3(pos_x, pos_y, pos_z);
		world_up = glm::vec3(up_x, up_y, up_z);
		this->yaw = yaw;
		this->pitch = pitch;
		update_camera_vectors();
	}

	glm::mat4 get_view_matrix();
	void process_keyboard(camera_movement direction, float delta_time);
	void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch = true);
	void process_mouse_scroll(float y_offset);
};

#endif // !CAMERA_H

