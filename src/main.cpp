#include "bwgl/bwgl.hpp"

int main() {
	if (!bwgl::window.create(1024, 512, "Window")) {
		return 1;
	}

	bwgl::window.setFullscreenMode(bwgl::FullscreenMode::Borderless);
	bwgl::window.setResizable(false);
	bwgl::window.setVsync(true);

	while (!bwgl::window.shouldClose()) {	
		bwgl::window.pollEvents();

		glClearColor(0.25f, 0.30f, 0.35f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Closing
		if (bwgl::input.didKeyClick(bwgl::KeyCode::Escape)) {
			bwgl::window.close();
		}
		// Fullscreen
		if (bwgl::input.didKeyClick(bwgl::KeyCode::F11)) {
			bwgl::window.setFullscreen(!bwgl::window.isFullscreen());
		}

		bwgl::window.present();
	}
}