#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <vector>
#include <functional>

#include "Structures/Color.hpp"
#include "Structures/Font.hpp"
#include "Structures/Rect.hpp"
#include "Structures/UIVertex.hpp"
#include "Structures/Thickness.hpp"
#include "UIMaterial.hpp"

struct UIBatch
{
    const UIMaterial *material;
    uint32_t indexOffset;
    uint32_t indexCount;
};

class UIContext
{
public:
    std::vector<UIVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<UIBatch> batches;

    const UIMaterial *defaultMaterial = nullptr;
    const Font *defaultFont = nullptr;
    std::function<const UIMaterial*(const Font*)> getFontMaterial;

    void Clear()
    {
        vertices.clear();
        indices.clear();
        batches.clear();
    }

    void AddRect(const Rect &r, const Color &c = Color::White(), const Thickness cornerSize = Thickness::Zero())
    {
        if (!defaultMaterial || r.w <= 0 || r.h <= 0 || !c.IsVisible())
            return;
        
        const Font *f = Font::GetDefault();
        float u0 = f ? f->WhiteU() : 0.0f;
        float v0 = f ? f->WhiteV() : 0.0f;
        AddQuad(defaultMaterial, r.x, r.y, r.Right(), r.Bottom(), u0, v0, u0, v0, c, cornerSize);
    }

    void AddText(const Font *font, std::string_view text, float x, float y, const Color &c)
    {
        const Font *f = font ? font : (defaultFont ? defaultFont : Font::GetDefault());
        if (!f || !f->IsValid() || !c.IsVisible())
            return;

        const UIMaterial *mat = getFontMaterial ? getFontMaterial(f) : defaultMaterial;
        if (!mat)
            return;

        const float startX = std::round(x);
        float penX = startX;
        float baseY = std::round(y + f->Ascent());
        const float invW = 1.0f / f->AtlasWidth();
        const float invH = 1.0f / f->AtlasHeight();

        const char *p = text.data();
        const char *end = p + text.size();
        while (p < end)
        {
            const uint32_t cp = Font::DecodeUTF8(p, end);
            if (cp == '\n')
            {
                penX = startX;
                baseY += f->LineHeight();
                continue;
            }
            if (cp == '\r')
                continue;
            if (cp == '\t')
            {
                penX += f->SpaceAdvance() * 4.0f;
                continue;
            }

            const Glyph *g = f->GetGlyph(cp);
            if (!g)
                continue;

            if (g->x1 > g->x0 && g->y1 > g->y0)
            {
                const float qx = std::round(penX + g->xoff);
                const float qy = std::round(baseY + g->yoff);
                AddQuad(mat, qx, qy, qx + (g->xoff2 - g->xoff), qy + (g->yoff2 - g->yoff),
                        g->x0 * invW, g->y0 * invH, g->x1 * invW, g->y1 * invH, c);
            }
            penX += g->xadvance;
        }
    }

    void AddQuad(const UIMaterial *material, float x0, float y0, float x1, float y1,
                 float u0, float v0, float u1, float v1, const Color &c,
                 const Thickness cornerRadius = Thickness::Zero())
    {
        if (!material || !c.IsVisible())
            return;

        float width = std::abs(x1 - x0);
        float height = std::abs(y1 - y0);
        if (width <= 0.0f || height <= 0.0f)
            return;

        if (batches.empty() || batches.back().material != material)
            batches.push_back({material, static_cast<uint32_t>(indices.size()), 0});

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
