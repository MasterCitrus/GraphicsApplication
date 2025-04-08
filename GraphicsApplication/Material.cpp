#include "Material.h"
#include "Texture.h"

Material::Material() : Kd({ 0.0f, 0.0f, 0.0f }), Ka({ 0.0f, 0.0f, 0.0f }), Ks({ 0.0f, 0.0f, 0.0f }), shininess(0.0f)
{
}

Material::~Material()
{

}
