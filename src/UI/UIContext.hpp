#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <vector>

#include "Structures/Color.hpp"
#include "Structures/Font.hpp"
#include "Structures/Rect.hpp"
#include "Structures/UIVertex.hpp"
#include "Structures/Thickness.hpp"

struct UIBatch
{
    const Font *font;
    uint32_t indexOffset;
    uint32_t indexCount;
};

class UIContext
{
public:
    std::vector<UIVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<UIBatch> batches;

    void Clear()
    {
        vertices.clear();
        indices.clear();
        batches.clear();
    }

    void AddRect(const Rect &r, const Color &c = Color::White(), const Thickness cornerSize = Thickness::Zero())
    {
        const Font *f = Font::GetDefault();
        if (!f || r.w <= 0 || r.h <= 0)
            return;
        AddQuad(f, r.x, r.y, r.Right(), r.Bottom(),
                f->WhiteU(), f->WhiteV(), f->WhiteU(), f->WhiteV(), c, cornerSize);
    }

    void AddText(const Font *font, std::string_view text, float x, float y, const Color &c)
    {
        if (!font || !font->IsValid() || !c.IsVisible())
            return;

        const float startX = std::round(x);
        float penX = startX;
        float baseY = std::round(y + font->Ascent());
        const float invW = 1.0f / font->AtlasWidth();
        const float invH = 1.0f / font->AtlasHeight();

        const char *p = text.data();
        const char *end = p + text.size();
        while (p < end)
        {
            const uint32_t cp = Font::DecodeUTF8(p, end);
            if (cp == '\n')
            {
                penX = startX;
                baseY += font->LineHeight();
                continue;
            }
            if (cp == '\r')
                continue;
            if (cp == '\t')
            {
                penX += font->SpaceAdvance() * 4.0f;
                continue;
            }

            const Glyph *g = font->GetGlyph(cp);
            if (!g)
                continue;

            if (g->x1 > g->x0 && g->y1 > g->y0)
            {
                const float qx = std::round(penX + g->xoff);
                const float qy = std::round(baseY + g->yoff);
                AddQuad(font, qx, qy, qx + (g->xoff2 - g->xoff), qy + (g->yoff2 - g->yoff),
                        g->x0 * invW, g->y0 * invH, g->x1 * invW, g->y1 * invH, c);
            }
            penX += g->xadvance;
        }
    }

    void AddQuad(const Font *font, float x0, float y0, float x1, float y1,
                 float u0, float v0, float u1, float v1, const Color &c,
                 const Thickness cornerRadius = Thickness::Zero())
    {
        if (!font || !this || !c.IsVisible())
            return;

        float width = std::abs(x1 - x0);
        float height = std::abs(y1 - y0);
        if (width <= 0.0f || height <= 0.0f)
            return;

        if (batches.empty() || batches.back().font != font)
            batches.push_back({font, static_cast<uint32_t>(indices.size()), 0});

        const uint32_t base = static_cast<uint32_t>(vertices.size());

        Rect elementBounds{x0, y0, width, height};

        vertices.emplace_back(x0, y0, u0, v0, c, width, height, cornerRadius, elementBounds);
        vertices.emplace_back(x1, y0, u1, v0, c, width, height, cornerRadius, elementBounds);
        vertices.emplace_back(x1, y1, u1, v1, c, width, height, cornerRadius, elementBounds);
        vertices.emplace_back(x0, y1, u0, v1, c, width, height, cornerRadius, elementBounds);

        indices.insert(indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
        batches.back().indexCount += 6;
    }
};
