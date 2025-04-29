#include "Material.h"
#include "Texture.h"

Material::Material() : Kd({ 0.0f, 0.0f, 0.0f }), Ka({ 0.0f, 0.0f, 0.0f }), Ks({ 0.0f, 0.0f, 0.0f }), shininess(0.0f)
{
	mapKd = new Texture();
	mapKs = new Texture();
	mapBump = new Texture();
	mapKd->load("./Working/defaultdiffuse.jpg");
	mapKs->load("./Working/defaultspecular.jpg");
	mapBump->load("./Working/defaultnormal.jpg");
}

Material::~Material()
{
	delete mapKd;
	delete mapKs;
	delete mapBump;
}
