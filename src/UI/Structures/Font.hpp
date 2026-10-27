#pragma once
#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Size.hpp"

// Один глиф в атласе. Все координаты в пикселях.
struct Glyph
{
    // Прямоугольник глифа в атласе (начало координат — верхний левый угол).
    float x0, y0, x1, y1;
    // Квад глифа относительно «пера» на базовой линии (y вниз: yoff обычно < 0).
    float xoff, yoff, xoff2, yoff2;
    float xadvance;
};

// Растровый шрифт: один TTF на один размер в пикселях.
//
//  * Чисто CPU-класс — ни OpenGL, ни RHI здесь нет. На GPU атлас заливает UIPass.
//  * Атлас RGBA8: RGB = белый, A = покрытие глифа. Поэтому существующий
//    Rectangle.frag (texture * color) работает без изменений.
//  * В левом верхнем углу атласа лежит белый блок. Им рисуются обычные
//    прямоугольники => весь UI (фоны + текст) идёт одним текстурным батчем.
//  * Разный размер текста = разный объект Font (так делают почти все).
class Font
{
public:
    Font(const char *ttfPath, float pixelHeight);
    Font(const Font &) = delete;
    Font &operator=(const Font &) = delete;

    bool IsValid() const { return !glyphs.empty(); }

    float PixelHeight() const { return pixelHeight; }
    float Ascent() const { return ascent; }         // > 0, от верха строки до базовой линии
    float Descent() const { return descent; }       // < 0
    float LineHeight() const { return lineHeight; } // шаг между строками
    float SpaceAdvance() const { return spaceAdvance; }

    // nullptr только если в шрифте нет даже '?'. Для отсутствующих символов отдаёт '?'.
    const Glyph *GetGlyph(uint32_t codepoint) const;

    // Размер текста (UTF-8). '\n' — перенос строки.
    Size MeasureText(std::string_view utf8) const;

    // --- атлас (для загрузки на GPU) ---
    int AtlasWidth() const { return atlasSize; }
    int AtlasHeight() const { return atlasSize; }
    const uint8_t *AtlasPixels() const { return atlasRGBA.data(); } // RGBA8, строка 0 = верх
    float WhiteU() const { return whiteU; }
    float WhiteV() const { return whiteV; }

    // Шрифт по умолчанию для элементов, у которых font == nullptr.
    static void SetDefault(Font *font) { defaultFont = font; }
    static Font *GetDefault() { return defaultFont; }

    // Декодер UTF-8 (сдвигает p). Некорректные байты -> U+FFFD.
    static uint32_t DecodeUTF8(const char *&p, const char *end);

private:
    void BuildEmptyAtlas();

    float pixelHeight = 0;
    float ascent = 0, descent = 0, lineHeight = 0, spaceAdvance = 0;

    int atlasSize = 0;
    std::vector<uint8_t> atlasRGBA;
    float whiteU = 0, whiteV = 0;

    std::unordered_map<uint32_t, Glyph> glyphs;

    static inline Font *defaultFont = nullptr;
};
