#include "Proiectil.h"

Projectile::Projectile()
    : position(glm::vec2(0.0f, 0.0f)), 
    trajectoryIndex(0), mesh(nullptr) {} 

void Projectile::CreateProjectileMesh(const std::string& name, glm::vec3 color) {
    mesh = Tema1_object2D::CreateCircle(name, position, 5.0f, color);
}

void Projectile::InitializeProjectile(glm::vec2 startPosition, const std::vector<glm::vec2>& trajectoryPoints, glm::vec3 color) {
    position = startPosition;
    trajectory = trajectoryPoints;
    trajectoryIndex = 0;
    CreateProjectileMesh("projectileMesh", color);
}

void Projectile::Update(float deltaTime) {

    if (trajectoryIndex < trajectory.size() - 1) {
        glm::vec2 currentPoint = trajectory[trajectoryIndex];
        glm::vec2 nextPoint = trajectory[trajectoryIndex + 1];

        glm::vec2 direction = nextPoint - currentPoint;
        float distance = glm::length(direction);

        if (distance > 0.0f) {
            direction = glm::normalize(direction);

            position += direction * deltaTime * 15.5f;

            if (glm::length(position - nextPoint) < 0.1f) {
                trajectoryIndex++;
            }
        }
    }
}