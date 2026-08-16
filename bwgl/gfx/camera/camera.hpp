#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace bwgl {
	struct Camera {
		glm::vec3 direction;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 position;

		glm::mat4 view;
		glm::mat4 projection;

		// Creates a perspective camera looking in the -Z direction.
		void createPerspective(float fov, float near, float far, float aspectRatio) {
			position = glm::vec3(0.0f);

			// Initialize 3d navigation vectors 
			direction = glm::vec3(0.0f, 0.0f, -1.0f);

			right = glm::normalize(
				glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -direction)
			);

			up = glm::normalize(
				glm::cross(-direction, right)
			);

			// Initialize matrices
			updateView();
			projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		}

		// Creates a perspective camera looking in the -Z direction.
		// The aspect ratio is set by default.
		void createPerspective(float fov, float near, float far) {
			const auto& windowState = bwgl::window.getState();
			createPerspective(fov, near, far, float(windowState.width) / windowState.height);
		}

		// Creates an orthographic camera looking in the -Z direction.
		void createOrthographic(
			float left, 
			float right, 
			float bottom, 
			float top,
			float near = 0.1f,
			float far = 10.0f
		) {
			position = glm::vec3(0.0f);
		
			// Initialize 3d navigation vectors 
			direction = glm::vec3(0.0f, 0.0f, -1.0f);

			Camera::right = glm::vec3(1.0f, 0.0f, 0.0f);
			Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);

			// Initialize matrices
			updateView();
			projection = glm::ortho(left, right, bottom, top, near, far);
		}

		// Should be called after any transformations.
		void updateView() {
			view = glm::lookAt(
				position, 
				position + direction, 
				up
			);
		}

		Camera() = default;
	};
}