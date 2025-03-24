#include "Object.h"
#include "Scene.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

Object::Object(Model* model, aie::ShaderProgram* shader) : model(model), shader(shader)
{
	transform.ComputeTransform();
}

Object::~Object()
{
	for( auto child : children ) delete child;
	delete model;
}

void Object::Update(float delta)
{
	transform.ComputeTransform();
}

void Object::Draw(Scene* scene)
{
	shader->bind();

	auto pvm = scene->GetCamera()->GetProjectionMatrix(scene->GetWindowSize().x, scene->GetWindowSize().y) * scene->GetCamera()->GetViewMatrix() * transform.GetLocalMatrix();

	shader->bindUniform("ProjectionViewModel", pvm);

	shader->bindUniform("ModelMatrix", transform.GetLocalMatrix());
	shader->bindUniform("AmbientColour", scene->GetAmbientLight());
	shader->bindUniform("LightColour", scene->GetLight().colour * scene->GetLight().intensity);
	shader->bindUniform("LightDirection", scene->GetLight().direction);

	shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

	int numLights = scene->GetNumLights();
	shader->bindUniform("numLights", numLights);
	shader->bindUniform("PointLightPosition", numLights, scene->GetLightPositions());
	shader->bindUniform("PointLightColour", numLights, scene->GetLightColours());

	model->Draw(shader);
}

void Object::AddChild(Object* child)
{
	child->SetParent(this);
	children.push_back(child);
}
