#pragma once
#include "Transform.h"
#include <vector>

class Model;

class Object
{
private:
	std::vector<Object*> children;
	Transform transform;
	Model* model;
	Object* parent;

public:
	Object() = default;

	void Update(float delta);
	void Draw();

	void AddChild(Object* child);
};