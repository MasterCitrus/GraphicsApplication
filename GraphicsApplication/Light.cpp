#include "Light.h"
#include "Gizmos.h"
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

Light::Light(glm::vec3 position, glm::vec3 colour, float intesity) : direction(position), colour(colour), intensity(intesity)
{

}

void Light::Draw() const
{
	if (debug)
	{
		glm::vec4 colourNorm = {colour, 1.0f};
		glm::normalize(colourNorm);
		colourNorm.w = 0.25f;
		aie::Gizmos::addSphere(direction, intensity, 16, 16, colourNorm);
	}
}
