#pragma once
#include "Device/RHI.hpp"

class Texture
{
public:
	int widthTexture = 0, heightTexture = 0;

	// srgb = true для цветных (albedo) текстур; false — данные/как раньше.
	explicit Texture(const char *filename, bool srgb = false);

	VoidCore::RHI::ITexture *GetRHI() const { return texture.get(); }
	VoidCore::RHI::ISampler *GetSampler() const;
	void SetSampler(VoidCore::RHI::Ref<VoidCore::RHI::ISampler> s) { sampler = std::move(s); }

	// Общий сэмплер по умолчанию (linear + repeat). Освободить до уничтожения устройства.
	static void ReleaseSharedResources();

private:
	VoidCore::RHI::Ref<VoidCore::RHI::ITexture> texture;
	VoidCore::RHI::Ref<VoidCore::RHI::ISampler> sampler; // nullptr → общий по умолчанию
};
