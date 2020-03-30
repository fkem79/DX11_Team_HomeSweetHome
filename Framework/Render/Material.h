#pragma once

class Material
{
public:
	class MaterialBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float4 diffuse;
			Float4 specular;
			Float4 ambient;
		}data;

		MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.diffuse = Float4(1, 1, 1, 1);
			data.specular = Float4(1, 1, 1, 1);
			data.ambient = Float4(1, 1, 1, 1);
		}
	};

	string name;
private:
	Shader* shader;	

	MaterialBuffer* buffer;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;
public:
	Material();
	Material(wstring shaderFile);
	~Material();

	void Set();

	MaterialBuffer* GetBuffer() { return buffer; }

	void SetShader(Shader* shader) { this->shader = shader; }
	void SetShader(wstring file) { shader = Shader::Add(file); }

	void SetDiffuseMap(wstring file) { diffuseMap = Texture::Add(file); }
	void SetSpecularMap(wstring file) { specularMap = Texture::Add(file); }
	void SetNormalMap(wstring file) { normalMap = Texture::Add(file); }

	Texture* GetDiffuseMap() { return diffuseMap; }
	Texture* GetSpecularMap() { return specularMap; }
	Texture* GetNormalMap() { return normalMap; }
};