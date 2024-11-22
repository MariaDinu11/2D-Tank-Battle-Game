#include "Tema1_object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Tema1_object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 corner1,
    glm::vec3 corner2,
    glm::vec3 corner3,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color)
    };

    Mesh* triangle = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2 };
    
     triangle->SetDrawMode(GL_TRIANGLE_STRIP);
    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* Tema1_object2D::CreateTrapez(
    const std::string& name,
    glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color),
        VertexFormat(corner4, color)
    };

    Mesh* trapezoid = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

    trapezoid->SetDrawMode(GL_TRIANGLES);

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}

Mesh* Tema1_object2D::CreateArc(
    const std::string& name,
    glm::vec3 center, float radius, float angleStart, float angleEnd, glm::vec3 color)
{
    const int numSegments = 30;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));
    unsigned int centerIndex = 0;

    for (int i = 0; i <= numSegments; ++i) {
        float angle = angleStart + (angleEnd - angleStart) * float(i) / float(numSegments);
        glm::vec3 point = center + glm::vec3(radius * cos(angle), radius * sin(angle), 0.0f);
        vertices.push_back(VertexFormat(point, color));
        if (i > 0) {
            indices.push_back(centerIndex);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }

    Mesh* arc = new Mesh(name);
    arc->SetDrawMode(GL_TRIANGLES);
    arc->InitFromData(vertices, indices);
    return arc;
}

Mesh* Tema1_object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color),
        VertexFormat(corner4, color)
    };

    Mesh* rectangle = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* Tema1_object2D::CreateFrame(
    const std::string& name,
    glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4, glm::vec3 color)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color),
        VertexFormat(corner4, color)
    };

    Mesh* rectangle = new Mesh(name);

    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    rectangle->SetDrawMode(GL_LINE_LOOP);

    rectangle->InitFromData(vertices, indices);

    return rectangle;
}

Mesh* Tema1_object2D::CreateCircle(
    const std::string& name,
    glm::vec2 center, float radius, glm::vec3 color)
{
    const int numSegments = 30;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3(center, 0.0f), color));  // Convertim glm::vec2 în glm::vec3
    unsigned int centerIndex = 0;

    for (int i = 0; i < numSegments; ++i) {
        float angle = 2 * glm::pi<float>() * float(i) / float(numSegments);
        glm::vec2 point = center + glm::vec2(radius * cos(angle), radius * sin(angle)); // Calculăm 2D
        vertices.push_back(VertexFormat(glm::vec3(point, 0.0f), color));

        if (i > 0) {
            indices.push_back(centerIndex);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }

    indices.push_back(centerIndex);
    indices.push_back(numSegments);
    indices.push_back(1);

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLES);
    circle->InitFromData(vertices, indices);

    return circle;
}

