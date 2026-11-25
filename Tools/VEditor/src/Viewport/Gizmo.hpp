#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "GizmoVertex.hpp"

class Gizmo
{
private:
    inline static std::vector<GizmoVertex> vertices;

public:
    static void DrawLine(const glm::vec3 &a, const glm::vec3 &b, const glm::vec4 &color)
    {
        vertices.push_back({a, color});
        vertices.push_back({b, color});
    }

    static void DrawGrid(const glm::vec3 &camPos, int halfSize = 25, float step = 1.0f)
    {
        glm::vec4 minorColor(0.3f, 0.3f, 0.35f, 0.45f);
        glm::vec4 majorColor(0.45f, 0.45f, 0.52f, 0.75f);
        glm::vec4 xAxisColor(0.85f, 0.25f, 0.25f, 0.9f);
        glm::vec4 zAxisColor(0.25f, 0.55f, 0.95f, 0.9f);

        int centerX = static_cast<int>(std::floor(camPos.x / step));
        int centerZ = static_cast<int>(std::floor(camPos.z / step));

        float minX = (centerX - halfSize) * step;
        float maxX = (centerX + halfSize) * step;
        float minZ = (centerZ - halfSize) * step;
        float maxZ = (centerZ + halfSize) * step;

        for (int i = -halfSize; i <= halfSize; ++i)
        {
            int gx = centerX + i;
            float x = gx * step;
            glm::vec4 colX = (gx == 0) ? zAxisColor : ((gx % 10 == 0) ? majorColor : minorColor);
            DrawLine({x, 0.0f, minZ}, {x, 0.0f, maxZ}, colX);

            int gz = centerZ + i;
            float z = gz * step;
            glm::vec4 colZ = (gz == 0) ? xAxisColor : ((gz % 10 == 0) ? majorColor : minorColor);
            DrawLine({minX, 0.0f, z}, {maxX, 0.0f, z}, colZ);
        }
    }

    static void DrawAxisGizmo(const glm::vec3 &pos, float scale = 1.0f)
    {
        DrawLine(pos, pos + glm::vec3(scale, 0.0f, 0.0f), {1.0f, 0.2f, 0.2f, 1.0f}); // X
        DrawLine(pos, pos + glm::vec3(0.0f, scale, 0.0f), {0.2f, 1.0f, 0.2f, 1.0f}); // Y
        DrawLine(pos, pos + glm::vec3(0.0f, 0.0f, scale), {0.2f, 0.5f, 1.0f, 1.0f}); // Z
    }

    static const std::vector<GizmoVertex> &GetVertices() { return vertices; }
    static void Clear() { vertices.clear(); }
};
