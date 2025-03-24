#pragma once

#include "Cubemap.h"

class Mesh;
class Camera;

namespace aie { class ShaderProgram; }

class Skybox
{
public:
	Skybox() = default;
	Skybox(const std::string& path, aie::ShaderProgram* shader, Camera* camera);
	~Skybox();

	void Draw();
private:
	Cubemap* skyboxTexture;
	aie::ShaderProgram* shader;
	Camera* camera;

	unsigned int vao, vbo;
};