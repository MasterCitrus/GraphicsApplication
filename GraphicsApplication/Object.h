#pragma once
#include "Transform.h"

class Model;

class Object
{
private:
	Transform transform;
	Model* model;

public:
	Object() = default;


	void Update(float delta);
	void Draw();
};