#include "camera.h"

glm::mat4 Camera::get_view_matrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(camera_movement direction, float delta_time) {
	float speed = movement_speed * delta_time;

	if (front.x < 0.01f && front.x > 0) {
		front.x = 0.01f;
	}
	else if (front.x > -0.01f && front.x < 0) {
		front.x = -0.01f;
	}

	if (front.z < 0.01f && front.z > 0) {
		front.x = 0.01f;
	}
	else if (front.z > -0.01f && front.z < 0) {
		front.z = -0.01f;
	}

	std::cout << front.x << " " << front.y << " " << front.z << std::endl;

	if (direction == FORWARD) {
		position.x += front.x * speed;
		position.z += front.z * speed;
	}
	if (direction == BACKWARD) {
		position.x -= front.x * speed;
		position.z -= front.z * speed;
	}
	if (direction == LEFT) {
		position -= right * speed;
	}
	if (direction == RIGHT) {
		position += right * speed;
	}
	if (direction == UP) {
		position += up * speed;
	}
	if (direction == DOWN) {
		position -= up * speed;
	}
}

void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch) {
	x_offset *= mouse_sensivity;
	y_offset *= mouse_sensivity;

	yaw += x_offset;
	pitch += y_offset;

	if (constrain_pitch) {
		if (pitch > 90.0f) {
			pitch = 90.0f;
		}
		else if (pitch < -90.0f) {
			pitch = -90.0f;
		}
	}

	update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {
	fov -= (float)y_offset;

	if (fov < 1.0f) {
		fov = 1.0f;
	}
	else if (fov > 45.0f) {
		fov = 45.0f;
	}
}

void Camera::update_camera_vectors() {
	glm::vec3 front;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);

	right = glm::normalize(glm::cross(this->front, world_up));
	up = glm::normalize(glm::cross(right, this->front));
}