#pragma once
#include "Transform.h"
#include <vector>

class Model;
class Scene;

namespace aie { class ShaderProgram; }

class Object
{
public:
	Object() = default;
	Object(Model* model, aie::ShaderProgram* shader);
	~Object();

	void Update(float delta);
	void Draw(Scene* scene);

	Object* GetParent() { return parent; }
	Model* GetModel() { return model; }
	Transform& GetTransform() { return transform; }

	void AddChild(Object* child);
	void SetParent(Object* parent) { this->parent = parent; }
private:
	std::vector<Object*> children;
	Transform transform;
	Model* model = nullptr;
	Object* parent = nullptr;
	aie::ShaderProgram* shader;
};