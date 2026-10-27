#include "Font.hpp"
#include "Core/Debug.h"

#include <algorithm>
#include <cmath>
#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

namespace
{
    // Какие символы кладём в атлас. Добавишь свои диапазоны — появятся в шрифте.
    struct RangeDef
    {
        int first;
        int count;
    };
    constexpr RangeDef kRanges[] = {
        {0x0020, 0x5F},  // Basic Latin
        {0x00A0, 0x60},  // Latin-1 Supplement
        {0x0400, 0x100}, // Кириллица
        {0x2010, 0x20},  // Пунктуация: – — ‘ ’ “ ” … •
        {0x20AC, 1},     // €
        {0x2116, 1},     // №
    };

    // Верхние строки атласа зарезервированы под белый блок (для прямоугольников).
    constexpr int kReservedRows = 4;
    constexpr int kWhiteBlock = 4;
    constexpr int kPadding = 2;
    constexpr int kOversample = 1;

    std::vector<unsigned char> ReadFile(const char *path)
    {
        std::ifstream in(path, std::ios::binary | std::ios::ate);
        if (!in)
            return {};
        const std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);
        std::vector<unsigned char> data(static_cast<size_t>(size));
        in.read(reinterpret_cast<char *>(data.data()), size);
        return data;
    }
}

uint32_t Font::DecodeUTF8(const char *&p, const char *end)
{
    const unsigned char c = static_cast<unsigned char>(*p++);
    if (c < 0x80)
        return c;

    int extra = 0;
    uint32_t cp = 0;
    if ((c & 0xE0) == 0xC0) { extra = 1; cp = c & 0x1F; }
    else if ((c & 0xF0) == 0xE0) { extra = 2; cp = c & 0x0F; }
    else if ((c & 0xF8) == 0xF0) { extra = 3; cp = c & 0x07; }
    else return 0xFFFD;

    for (int i = 0; i < extra; ++i)
    {
        if (p >= end || (static_cast<unsigned char>(*p) & 0xC0) != 0x80)
            return 0xFFFD;
        cp = (cp << 6) | (static_cast<unsigned char>(*p++) & 0x3F);
    }
    return cp;
}

void Font::BuildEmptyAtlas()
{
    // Шрифт не загрузился: атлас из одного белого блока. Прямоугольники всё равно рисуются.
    atlasSize = 64;
    atlasRGBA.assign(static_cast<size_t>(atlasSize) * atlasSize * 4, 0);
    for (int y = 0; y < kWhiteBlock; ++y)
        for (int x = 0; x < kWhiteBlock; ++x)
            for (int c = 0; c < 4; ++c)
                atlasRGBA[(static_cast<size_t>(y) * atlasSize + x) * 4 + c] = 255;
    whiteU = (kWhiteBlock * 0.5f) / atlasSize;
    whiteV = (kWhiteBlock * 0.5f) / atlasSize;
}

Font::Font(const char *ttfPath, float px) : pixelHeight(px)
{
    std::vector<unsigned char> ttf = ReadFile(ttfPath);
    if (ttf.empty())
    {
        Debug::LogError("Font: can't read file: ", ttfPath);
        BuildEmptyAtlas();
        return;
    }

    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, ttf.data(), stbtt_GetFontOffsetForIndex(ttf.data(), 0)))
    {
        Debug::LogError("Font: not a valid TTF: ", ttfPath);
        BuildEmptyAtlas();
        return;
    }

    const float scale = stbtt_ScaleForPixelHeight(&info, px);
    int asc = 0, desc = 0, gap = 0;
    stbtt_GetFontVMetrics(&info, &asc, &desc, &gap);
    ascent = asc * scale;
    descent = desc * scale;
    lineHeight = (asc - desc + gap) * scale;

    std::vector<uint8_t> a8;
    std::vector<std::vector<stbtt_packedchar>> packed(std::size(kRanges));
    bool ok = false;

    for (int size = 512; size <= 4096 && !ok; size *= 2)
    {
        atlasSize = size;
        a8.assign(static_cast<size_t>(size) * size, 0);

        stbtt_pack_context pc;
        if (!stbtt_PackBegin(&pc, a8.data(), size, size, size, kPadding, nullptr))
            break;

        stbtt_PackSetOversampling(&pc, kOversample, kOversample);
        stbtt_PackSetSkipMissingCodepoints(&pc, 1);

        std::vector<stbtt_pack_range> ranges(std::size(kRanges));
        for (size_t i = 0; i < ranges.size(); ++i)
        {
            packed[i].assign(static_cast<size_t>(kRanges[i].count), stbtt_packedchar{});
            ranges[i] = {};
            ranges[i].font_size = px;
            ranges[i].first_unicode_codepoint_in_range = kRanges[i].first;
            ranges[i].num_chars = kRanges[i].count;
            ranges[i].chardata_for_range = packed[i].data();
        }

        ok = stbtt_PackFontRanges(&pc, ttf.data(), 0, ranges.data(), static_cast<int>(ranges.size())) != 0;
        stbtt_PackEnd(&pc);
    }

    if (!ok)
    {
        Debug::LogError("Font: atlas too small for ", ttfPath, " at ", px, "px");
        BuildEmptyAtlas();
        return;
    }

    // --- глифы ---
    for (size_t r = 0; r < std::size(kRanges); ++r)
    {
        for (int i = 0; i < kRanges[r].count; ++i)
        {
            const uint32_t cp = static_cast<uint32_t>(kRanges[r].first + i);
            if (stbtt_FindGlyphIndex(&info, static_cast<int>(cp)) == 0)
                continue;

            const stbtt_packedchar &c = packed[r][static_cast<size_t>(i)];
            Glyph g;
            // ИСПРАВЛЕНИЕ 2: Убираем ручные сдвиги на kReservedRows, так как упаковывали честно с нуля
            g.x0 = c.x0;
            g.x1 = c.x1;
            g.y0 = c.y0;
            g.y1 = c.y1;
            g.xoff = c.xoff;
            g.yoff = c.yoff;
            g.xoff2 = c.xoff2;
            g.yoff2 = c.yoff2;
            g.xadvance = c.xadvance;
            glyphs.emplace(cp, g);
        }
    }

    if (const Glyph *sp = GetGlyph(' '))
        spaceAdvance = sp->xadvance;

    for (int y = 0; y < kWhiteBlock; ++y)
        for (int x = 0; x < kWhiteBlock; ++x)
            a8[static_cast<size_t>(y) * atlasSize + x] = 255;

    atlasRGBA.resize(a8.size() * 4);
    for (size_t i = 0; i < a8.size(); ++i)
    {
        atlasRGBA[i * 4 + 0] = 255;
        atlasRGBA[i * 4 + 1] = 255;
        atlasRGBA[i * 4 + 2] = 255;
        atlasRGBA[i * 4 + 3] = a8[i];
    }

    whiteU = (kWhiteBlock * 0.5f) / atlasSize;
    whiteV = (kWhiteBlock * 0.5f) / atlasSize;
}

const Glyph *Font::GetGlyph(uint32_t codepoint) const
{
    auto it = glyphs.find(codepoint);
    if (it != glyphs.end())
        return &it->second;
    it = glyphs.find('?');
    return it != glyphs.end() ? &it->second : nullptr;
}

Size Font::MeasureText(std::string_view text) const
{
    float lineW = 0.0f, maxW = 0.0f;
    int lines = 1;

    const char *p = text.data();
    const char *end = p + text.size();
    while (p < end)
    {
        const uint32_t cp = DecodeUTF8(p, end);
        if (cp == '\n')
        {
            maxW = std::max(maxW, lineW);
            lineW = 0.0f;
            ++lines;
        }
        else if (cp == '\r')
            continue;
        else if (cp == '\t')
            lineW += spaceAdvance * 4.0f;
        else if (const Glyph *g = GetGlyph(cp))
            lineW += g->xadvance;
    }
    maxW = std::max(maxW, lineW);
    return {std::ceil(maxW), lines * lineHeight};
}
