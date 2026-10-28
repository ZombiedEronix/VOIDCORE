#pragma once
#include "Panel.hpp"

class Grid : public Panel
{
public:
    struct ColumnDef
    {
        float size;
        bool isStar;
    };
    struct RowDef
    {
        float size;
        bool isStar;
    };

    std::vector<ColumnDef> columns;
    std::vector<RowDef> rows;

    void AddColumn(float size, bool star = false)
    {
        columns.push_back({size, star});
    }

    void AddRow(float size, bool star = false)
    {
        rows.push_back({size, star});
    }

    void Add(std::shared_ptr<UIControl> child, int row, int col)
    {
        if (row < 0 || col < 0)
            return;
        children.push_back(child);
        positions.push_back({row, col});
    }

private:
    std::vector<std::pair<int, int>> positions;

    Size MeasureOverride(Size available) override
    {
        if (columns.empty())
            const_cast<Grid*>(this)->AddColumn(1.0f, true);
        if (rows.empty())
            const_cast<Grid*>(this)->AddRow(1.0f, true);

        float availW = std::max(0.0f, available.Width - Padding.Horizontal());
        float availH = std::max(0.0f, available.Height - Padding.Vertical());

        std::vector<float> colWidths(columns.size());
        std::vector<float> rowHeights(rows.size());

        float fixedW = 0, fixedH = 0;
        int starColsCount = 0, starRowsCount = 0;

        for (size_t i = 0; i < columns.size(); ++i)
        {
            if (columns[i].isStar)
                ++starColsCount;
            else
            {
                colWidths[i] = columns[i].size;
                fixedW += columns[i].size;
            }
        }

        for (size_t i = 0; i < rows.size(); ++i)
        {
            if (rows[i].isStar)
                ++starRowsCount;
            else
            {
                rowHeights[i] = rows[i].size;
                fixedH += rows[i].size;
            }
        }

        float starW = starColsCount > 0 ? (availW - fixedW) / starColsCount : 0;
        float starH = starRowsCount > 0 ? (availH - fixedH) / starRowsCount : 0;

        for (size_t i = 0; i < columns.size(); ++i)
            if (columns[i].isStar)
                colWidths[i] = columns[i].size * starW;

        for (size_t i = 0; i < rows.size(); ++i)
            if (rows[i].isStar)
                rowHeights[i] = rows[i].size * starH;

        for (size_t i = 0; i < children.size(); ++i)
        {
            int r = positions[i].first, c = positions[i].second;
            if (r >= (int)rows.size() || c >= (int)columns.size())
                continue;
            children[i]->Measure(colWidths[c], rowHeights[r]);
        }

        return {availW + Padding.Horizontal(), availH + Padding.Vertical()};
    }

    void ArrangeOverride(const Rect &content) override
    {
        if (columns.empty() || rows.empty())
            return;

        float innerX = content.x + Padding.Left;
        float innerY = content.y + Padding.Top;
        float innerW = std::max(0.0f, content.w - Padding.Horizontal());
        float innerH = std::max(0.0f, content.h - Padding.Vertical());

        std::vector<float> colWidths(columns.size());
        float fixedW = 0;
        int starColsCount = 0;
        for (size_t i = 0; i < columns.size(); ++i)
        {
            if (columns[i].isStar)
                ++starColsCount;
            else
                fixedW += columns[i].size;
        }
        float starW = starColsCount > 0 ? (innerW - fixedW) / starColsCount : 0;

        for (size_t i = 0; i < columns.size(); ++i)
        {
            if (columns[i].isStar)
                colWidths[i] = columns[i].size * starW;
            else
                colWidths[i] = columns[i].size;
        }

        std::vector<float> rowHeights(rows.size());
        float fixedH = 0;
        int starRowsCount = 0;
        for (size_t i = 0; i < rows.size(); ++i)
        {
            if (rows[i].isStar)
                ++starRowsCount;
            else
                fixedH += rows[i].size;
        }
        float starH = starRowsCount > 0 ? (innerH - fixedH) / starRowsCount : 0;

        for (size_t i = 0; i < rows.size(); ++i)
        {
            if (rows[i].isStar)
                rowHeights[i] = rows[i].size * starH;
            else
                rowHeights[i] = rows[i].size;
        }

        std::vector<float> colX(columns.size() + 1);
        colX[0] = innerX;
        for (size_t i = 0; i < columns.size(); ++i)
            colX[i + 1] = colX[i] + colWidths[i];

        std::vector<float> rowY(rows.size() + 1);
        rowY[0] = innerY;
        for (size_t i = 0; i < rows.size(); ++i)
            rowY[i + 1] = rowY[i] + rowHeights[i];

        for (size_t i = 0; i < children.size(); ++i)
        {
            int r = positions[i].first, c = positions[i].second;
            if (r >= (int)rows.size() || c >= (int)columns.size())
                continue;

            Rect cell{colX[c], rowY[r], colWidths[c], rowHeights[r]};
            children[i]->Arrange(cell);
        }
    }
};
