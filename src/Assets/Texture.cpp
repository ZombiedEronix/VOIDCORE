#include "Assets/Texture.h"
#include <stb/stb_image.h>
#include "Core/Debug.h"

using namespace VoidCore::RHI;

static Ref<ISampler> s_defaultSampler;

static ISampler *DefaultSampler()
{
	if (!s_defaultSampler)
	{
		SamplerDesc sd; // linear + repeat
		sd.debugName = "DefaultLinearRepeat";
		s_defaultSampler = GetDevice().CreateSampler(sd);
	}
	return s_defaultSampler.get();
}

void Texture::ReleaseSharedResources()
{
	s_defaultSampler.reset();
}

ISampler *Texture::GetSampler() const
{
	return sampler ? sampler.get() : DefaultSampler();
}

Texture::Texture(const char *filename, bool srgb)
{
	// Флип — про порядок строк в данных, от графического API не зависит.
	stbi_set_flip_vertically_on_load(true);

	int channels = 0;
	// Всегда 4 канала: RGB8 нет в D3D11/Metal.
	unsigned char *pixels = stbi_load(filename, &widthTexture, &heightTexture, &channels, 4);

	unsigned char whitePixel[4] = {255, 255, 255, 255};
	const void *data = pixels;
	if (!pixels)
	{
		Debug::LogError("Texture not found, using 1x1 white: ", filename);
		widthTexture = 1;
		heightTexture = 1;
		data = whitePixel;
	}

	TextureDesc d;
	d.width = static_cast<uint32_t>(widthTexture);
	d.height = static_cast<uint32_t>(heightTexture);
	d.mipLevels = 0; // полная цепочка, генерирует бэкенд
	d.format = srgb ? Format::RGBA8_SRGB : Format::RGBA8_UNORM;
	d.usage = TextureUsage::Sampled;
	d.initialData = data;
	d.debugName = filename;
	texture = GetDevice().CreateTexture(d);

	if (pixels)
		stbi_image_free(pixels);
}
