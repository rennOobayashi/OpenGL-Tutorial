#include "camera.h"

glm::mat4 Camera::get_view_matrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(camera_movement direction, float delta_time) {
	float speed = movement_speed * delta_time;

	if (direction == FORWARD) {
		position += front * speed;
	}
	if (direction == BACKWARD) {
		position -= front * speed;
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