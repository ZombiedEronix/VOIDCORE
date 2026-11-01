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

#include "Graphics/Resources/Material.h"


struct UIBatch
{
    const Material *material;
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

    void AddRect(const Material* mat, const Rect &r, const Color &c = Color::White(), const Thickness cornerSize = Thickness::Zero())
    {
        if (!mat || r.w <= 0 || r.h <= 0 || !c.IsVisible())
            return;

        const Font *f = Font::GetDefault();
        float u0 = f ? f->WhiteU() : 0.0f;
        float v0 = f ? f->WhiteV() : 0.0f;
        AddQuad(mat, r.x, r.y, r.Right(), r.Bottom(), u0, v0, u0, v0, c, cornerSize);
    }

    void AddText(const Material *mat, const Font *font, std::string_view text, float x, float y, const Color &c)
    {
        if (!font || !font->IsValid() || !c.IsVisible())
            return;

        if (!mat)
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
                AddQuad(mat, qx, qy, qx + (g->xoff2 - g->xoff), qy + (g->yoff2 - g->yoff),
                        g->x0 * invW, g->y0 * invH, g->x1 * invW, g->y1 * invH, c);
            }
            penX += g->xadvance;
        }
    }

    void AddQuad(const Material *mat, float x0, float y0, float x1, float y1,
                 float u0, float v0, float u1, float v1, const Color &c,
                 const Thickness cornerRadius = Thickness::Zero())
    {
        if (!mat || !c.IsVisible())
            return;

        float width = std::abs(x1 - x0);
        float height = std::abs(y1 - y0);
        if (width <= 0.0f || height <= 0.0f)
            return;

        if (batches.empty() || batches.back().material != mat)
            batches.push_back({mat, static_cast<uint32_t>(indices.size()), 0});

        const uint32_t base = static_cast<uint32_t>(vertices.size());

        Rect elementBounds{x0, y0, width, height};

        vertices.push_back({ {x0, y0}, {u0, v0}, {elementBounds.x, elementBounds.y, elementBounds.w, elementBounds.h}, {c.r, c.g, c.b, c.a}, {cornerRadius.Bottom, cornerRadius.Left, cornerRadius.Right, cornerRadius.Top} });
        vertices.push_back({ {x1, y0}, {u1, v0}, {elementBounds.x, elementBounds.y, elementBounds.w, elementBounds.h}, {c.r, c.g, c.b, c.a}, {cornerRadius.Bottom, cornerRadius.Left, cornerRadius.Right, cornerRadius.Top} });
        vertices.push_back({ {x1, y1}, {u1, v1}, {elementBounds.x, elementBounds.y, elementBounds.w, elementBounds.h}, {c.r, c.g, c.b, c.a}, {cornerRadius.Bottom, cornerRadius.Left, cornerRadius.Right, cornerRadius.Top} });
        vertices.push_back({ {x0, y1}, {u0, v1}, {elementBounds.x, elementBounds.y, elementBounds.w, elementBounds.h}, {c.r, c.g, c.b, c.a}, {cornerRadius.Bottom, cornerRadius.Left, cornerRadius.Right, cornerRadius.Top} });

        indices.insert(indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
        batches.back().indexCount += 6;
    }
};
