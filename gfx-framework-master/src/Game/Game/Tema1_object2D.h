#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace Tema1_object2D
{

    Mesh* CreateTriangle(const std::string& name,
        glm::vec3 vertex1,
        glm::vec3 vertex2,
        glm::vec3 vertex3,
        glm::vec3 color);

    Mesh* CreateTrapez(const std::string& name,
        glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4, glm::vec3 color);

    Mesh* CreateArc(const std::string& name, glm::vec3 center, float radius,
        float angleStart, float angleEnd, glm::vec3 color);

    Mesh* CreateRectangle(const std::string& name,
        glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4, glm::vec3 color);

    Mesh* CreateFrame(
        const std::string& name,
        glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4, glm::vec3 color);

    Mesh* CreateCircle(
        const std::string& name,
        glm::vec2 center, float radius, glm::vec3 color);
}
