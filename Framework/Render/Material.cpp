#include "Framework.h"

Material::Material()
	: shader(nullptr), diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	buffer = new MaterialBuffer();
}

Material::Material(wstring shaderFile)
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	shader = Shader::Add(shaderFile);
	buffer = new MaterialBuffer();
}

Material::~Material()
{
	delete buffer;
}

void Material::Set()
{
	buffer->SetPSBuffer(1);

	if (diffuseMap != nullptr)
		diffuseMap->Set(0);

	if (specularMap != nullptr)
		specularMap->Set(1);

	if (normalMap != nullptr)
		normalMap->Set(2);

	if (shader != nullptr)
		shader->Set();
}
