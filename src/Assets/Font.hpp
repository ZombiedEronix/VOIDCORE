#pragma once
#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "Entity/Object.hpp"
#include "UI/Structures/Size.hpp"

struct Glyph
{
    float x0, y0, x1, y1;
    float xoff, yoff, xoff2, yoff2;
    float xadvance;
};

class Font : public Object
{
public:
    Font(const std::vector<uint8_t> &data, float fontSize);
    Font(const char *ttfPath, float pixelHeight);
    Font(const Font &) = delete;
    Font &operator=(const Font &) = delete;

    bool IsValid() const { return !glyphs.empty(); }

    float PixelHeight() const { return pixelHeight; }
    float Ascent() const { return ascent; }
    float Descent() const { return descent; }
    float LineHeight() const { return lineHeight; }
    float SpaceAdvance() const { return spaceAdvance; }

    const Glyph *GetGlyph(uint32_t codepoint) const;
    Size MeasureText(std::string_view utf8) const;

    int AtlasWidth() const { return atlasSize; }
    int AtlasHeight() const { return atlasSize; }
    const uint8_t *AtlasPixels() const { return atlasRGBA.data(); }
    float WhiteU() const { return whiteU; }
    float WhiteV() const { return whiteV; }

    static void SetDefault(Font *font) { defaultFont = font; }
    static Font *GetDefault() { return defaultFont; }

    static uint32_t DecodeUTF8(const char *&p, const char *end);

private:
    void InitFromMemory(const uint8_t *ttfData, size_t dataSize, const char *debugName);
    void BuildEmptyAtlas();

    float pixelHeight = 0;
    float ascent = 0, descent = 0, lineHeight = 0, spaceAdvance = 0;

    int atlasSize = 0;
    std::vector<uint8_t> atlasRGBA;
    float whiteU = 0, whiteV = 0;

    std::unordered_map<uint32_t, Glyph> glyphs;

    static inline Font *defaultFont = nullptr;
};
