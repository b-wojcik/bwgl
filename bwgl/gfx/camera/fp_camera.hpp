#pragma once

#include "camera.hpp"

namespace bwgl {
	class FPCamera {
	public:
		// Does nothing if the first person camera has already been created.
		void create(float fov, float sensitivity, float speed) {
			if (created) {
				return;
			}

			FPCamera::sensitivity = sensitivity / 10.0f;
			FPCamera::speed = speed;
			
			camera.createPerspective(fov, 0.1f, 100.0f);
			window.hideCursor();

			created = true;
		}

		// Calculates look direction and handles movement.
		// Does nothing if the first person camera has not been created.
		void update(float deltaTime) {
			if (!created) {
				if (!warningShown) {
					BWGL_WARNING(
						"bwgl::FPCamera::update(): camera was never been created"
					);
					warningShown = true;
				}

				return;
			}

			float xpos = input.cursorX();
			float ypos = input.cursorY();

			if (firstMouse) {
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			// Calculate how much the mouse moved
			float xOffset = (xpos - lastX) * sensitivity;
			float yOffset = (ypos - lastY) * sensitivity;

			lastX = xpos;
			lastY = ypos;

			// Update euler angles
			yaw += xOffset;
			pitch += yOffset;

			// Pitch restricted to [-89.0f ; 89.0f]
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			else if (pitch < -89.0f) {
				pitch = -89.0f;
			}

			// Yaw, pitch in radians
			float r_yaw = glm::radians(yaw);
			float r_pitch = glm::radians(pitch);

			// Update camera direction
			camera.direction.x = cos(r_yaw) * cos(r_pitch);
			camera.direction.y = sin(r_pitch);
			camera.direction.z = sin(r_yaw) * cos(r_pitch);
			camera.direction = glm::normalize(camera.direction);

			// Update camera 3d navigation vectors
			camera.right = glm::normalize(
				glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -camera.direction)
			);

			camera.up = glm::normalize(
				glm::cross(-camera.direction, camera.right)
			);

			// Handle movement
			if (input.isKeyDown(KeyCode::W)) {
				camera.position += camera.direction * speed * deltaTime;
			}
			if (input.isKeyDown(KeyCode::S)) {
				camera.position -= camera.direction * speed * deltaTime;
			}
			if (input.isKeyDown(KeyCode::A)) {
				camera.position -= camera.right * speed * deltaTime;
			}
			if (input.isKeyDown(KeyCode::D)) {
				camera.position += camera.right * speed * deltaTime;
			}

			// Up / down movement 
			if (input.isKeyDown(KeyCode::LeftShift)) {
				camera.position.y -= speed * deltaTime;
			}
			if (input.isKeyDown(KeyCode::Space)) {
				camera.position.y += speed * deltaTime;
			}

			camera.updateView();
		}

		const glm::vec3& getPosition() const {
			return camera.position;
		}

		const glm::mat4& getView() const {
			return camera.view;
		}

		const glm::mat4& getProjection() const {
			return camera.projection;
		}

		FPCamera() = default;

		// Delete copy and move constructors
		FPCamera(const FPCamera&) = delete;
		FPCamera(FPCamera&&) = delete;
	private:
		Camera camera;

		// Safety checks
		bool created = false;
		bool warningShown = false;

		// Movement
		float sensitivity = 0.0f;
		float speed = 0.0f;

		// Euler angles
		float yaw = -90.0f;
		float pitch = 0.0f;

		// Cursor
		float lastX = 0.0f;
		float lastY = 0.0f;
		bool firstMouse = true;
	};
}