#include "Light.h"
#include "Gizmos.h"
#include <glm/vec4.hpp>

Light::Light(glm::vec3 position, glm::vec3 colour, float intesity) : direction(position), colour(colour), intensity(intesity)
{

}

void Light::Draw()
{
	if (debug)
	{
		aie::Gizmos::addSphere(direction, intensity, 16, 16, { colour.x, colour.y, colour.z, 0.0f });
	}
}
