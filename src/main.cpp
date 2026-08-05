#include "bwgl/bwgl.hpp"

void loadShaders() {
	bwgl::shader_cache.load(
		"texture",
		"..\\..\\..\\assets\\shaders\\texture.vert",
		"..\\..\\..\\assets\\shaders\\texture.frag"
	);
}

void loadTextures() {
	bwgl::texture_cache.load(
		"background",
		"..\\..\\..\\assets\\textures\\bg.png"
	);
}

int main() {
	if (!bwgl::window.create(1920, 1080, "bwgl")) {
		return 1;
	}

	bwgl::window.setFullscreenMode(bwgl::FullscreenMode::Borderless);
	bwgl::window.setResizable(false);
	bwgl::window.setVsync(false);

	loadShaders();
	loadTextures();

	bwgl::TestMesh plane;

	while (!bwgl::window.shouldClose()) {	
		bwgl::window.pollEvents();

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		bwgl::shader_cache.use("texture");
		bwgl::texture_cache.use("background");

		plane.draw();

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