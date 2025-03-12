#pragma once
#include <glm/mat4x4.hpp>

class Model;
class Camera;
class Scene;
struct Light;
namespace aie { class ShaderProgram; }

class Instance
{
public:
	Instance(glm::mat4 transform, Model* mesh, aie::ShaderProgram* shader);
	Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, Model* mesh, aie::ShaderProgram* shader);

	void Update(float delta);
	void Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3& ambientLight, Light* light);
	void Draw(Scene* scene);

	glm::mat4 MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);

protected:
	glm::mat4 transform;
	Model* mesh;
	aie::ShaderProgram* shader;
};