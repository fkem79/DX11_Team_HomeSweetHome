#include "Framework.h"

map<wstring, Texture*> Texture::totalTexture;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
	: srv(srv), image(move(image))
{
	
}

Texture::~Texture()
{
	srv->Release();	
}

Texture* Texture::Add(wstring file)
{
	if (totalTexture.count(file) > 0)
		return totalTexture[file];
	
	wstring extension = Path::GetExtension(file);

	ScratchImage image;

	if (extension == L"tga")
	{
		V(LoadFromTGAFile(file.c_str(), nullptr, image));
	}
	else if (extension == L"dds")
	{
		V(LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image));
	}
	else
	{
		V(LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image));
	}	

	ID3D11ShaderResourceView* srv;

	V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv));
	
	Texture* texture = new Texture(srv, image);
	texture->file = file;

	totalTexture.insert({ file,  texture});		

	return totalTexture[file];
}

void Texture::Delete()
{
	for (auto texture : totalTexture)
	{
		delete texture.second;
	}
}

void Texture::Set(UINT slot)
{
	DC->PSSetShaderResources(slot, 1, &srv);	
}

vector<Float4> Texture::ReadPixels()
{
	vector<Float4> result;

	uint8_t* colors = image.GetPixels();
	UINT size = image.GetPixelsSize();

	for (int i = 0; i < size / 4; i++)
	{
		Float4 color;

		float scale = 1.0f / 255.0f;		
		color.x = colors[i*4 + 0] * scale;
		color.y = colors[i*4 + 1] * scale;
		color.z = colors[i*4 + 2] * scale;
		color.w = colors[i*4 + 3] * scale;

		result.push_back(color);
	}

	return result;
}
