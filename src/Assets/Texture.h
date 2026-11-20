#pragma once
#include "Device/RHI.hpp"

class Texture
{
public:
	int widthTexture = 0, heightTexture = 0;

	explicit Texture(const char *filename, bool srgb = false);
	explicit Texture(const uint8_t *rawPixels, int width, int height, const char *debugName, bool srgb = false);
	
	Texture(const uint8_t *data, size_t size, const char *debugName, bool srgb = false);

	VoidCore::RHI::ITexture *GetRHI() const { return texture.get(); }
	VoidCore::RHI::ISampler *GetSampler() const;
	void SetSampler(VoidCore::RHI::Ref<VoidCore::RHI::ISampler> s) { sampler = std::move(s); }

	static void ReleaseSharedResources();

private:
	VoidCore::RHI::Ref<VoidCore::RHI::ITexture> texture;
	VoidCore::RHI::Ref<VoidCore::RHI::ISampler> sampler; // nullptr → common by default
};
