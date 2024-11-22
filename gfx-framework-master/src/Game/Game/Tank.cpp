#include "lab_m1/Tema1/Tank.h"
#include "Tema1_object2D.h"
#include <iostream>\
//#include "lab_m1/lab3/transform2D.h"

using namespace std;

Tank::Tank() {
    base1 = nullptr;
    base2 = nullptr;
    turret = nullptr;
    barrel = nullptr;
    LifeBar = nullptr;
    Life = nullptr;
}

Tank::~Tank() {
}

void Tank::CreateTankMesh(glm::vec3 color1, glm::vec3 color2) {
    base1 = Tema1_object2D::CreateTrapez(
        "base1",
        glm::vec3(-40, 10, 0),
        glm::vec3(40, 10, 0), 
        glm::vec3(30, 0, 0),  
        glm::vec3(-30, 0, 0),
        color1
    );

    base2 = Tema1_object2D::CreateTrapez(
        "base2",
        glm::vec3(-50, 30, 0),
        glm::vec3(50, 30, 0), 
        glm::vec3(55, 10, 0), 
        glm::vec3(-55, 10, 0),
        color2
    );

    turret = Tema1_object2D::CreateArc("turret", glm::vec3(0, 30, 0), 20.0f, glm::radians(0.0f), glm::radians(180.0f), color2);

    glm::vec3 corner1 = glm::vec3(-2.5f, 10, 0.0f);
    glm::vec3 corner2 = glm::vec3(2.5f, 10, 0.0f);
    glm::vec3 corner3 = glm::vec3(2.5f, 60, 0.0f);
    glm::vec3 corner4 = glm::vec3(-2.5f, 60, 0.0f);

    glm::vec3 color(0.0f, 0.0f, 0.0f);
    barrel = Tema1_object2D::CreateRectangle("barrel", corner1, corner2, corner3, corner4, color);

    glm::vec3 barColor = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 frameColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float barWidth = 40.0f;
    float barHeight = 5.0f;

    Life = Tema1_object2D::CreateRectangle(
        "Life",
        glm::vec3(-barWidth, 120.0f, 0.0f),
        glm::vec3(barWidth, 120.0f, 0.0f),
        glm::vec3(barWidth, 135.0f, 0.0f),
        glm::vec3(-barWidth, 135.0f, 0.0f),
        barColor
    );

    LifeBar = Tema1_object2D::CreateFrame(
        "LifeBar",
        glm::vec3(-barWidth, 120.0f, 0.0f),
        glm::vec3(barWidth, 120.0f, 0.0f),
        glm::vec3(barWidth, 135.0f, 0.0f),
        glm::vec3(-barWidth, 135.0f, 0.0f),
        frameColor
    );
}

void Tank::RotateBarrel(float angle) {
    barrelAngle += angle;

    if (barrelAngle > glm::pi<float>() / 2) { 
        barrelAngle = glm::pi<float>() / 2;
    }
    if (barrelAngle < -glm::pi<float>() / 2) {
        barrelAngle = -glm::pi<float>() / 2;
    }
}

float Tank::GetTerrainHeight(float x, std::vector<glm::vec2>& heightMap) {

    for (size_t i = 0; i < heightMap.size() - 1; ++i) {
        glm::vec2 p1 = heightMap[i];
        glm::vec2 p2 = heightMap[i + 1];

        if (x >= p1.x && x <= p2.x) {
            float t = (x - p1.x) / (p2.x - p1.x);
            return glm::mix(p1.y, p2.y, t);
        }
    }

    return heightMap.back().y;
}

void Tank::DeformTerrain(float impactX, float impactY, float radius, std::vector<glm::vec2>& heightMap) {

    for (auto& point : heightMap) {

        float distance = glm::sqrt((point.x - impactX) * (point.x - impactX) +
            (point.y - impactY) * (point.y - impactY));

        if (distance <= radius) {
            float deformationFactor = (1.0f - (distance / radius));

            point.y -= deformationFactor * radius;
           
            point.y = glm::max(point.y, 0.0f);
        }
    }
}

void Tank::GenerateTrajectory(float pos_init_x, float pos_init_y, float barrel_angle, float angle, float magnitude, float deltaTime, std::vector<glm::vec2>& heightMap) {

    trajectoryPoints.clear();

    float g = 30.00f;

    glm::vec2 velocity(magnitude * cos(barrel_angle + angle + glm::pi<float>() / 2), magnitude * sin(barrel_angle + angle + glm::pi<float>() / 2));

    float t = 0.0f;
    float x = pos_init_x;
    float y = pos_init_y;
    
    for (int i = 0; i <= 300; ++i) {
        trajectoryPoints.push_back(glm::vec2(x, y));

        x = x + velocity.x * t;
        y = y + velocity.y * t + 0.5f * g * t * t;
        float terreinHeight = GetTerrainHeight(x, heightMap);

        velocity.y -= g * t;

        t = t + deltaTime * 0.05f;
    }
}

void Tank::LaunchProjectile(float magnitude, float deltaTime, std::vector<glm::vec2> trajectory, glm::vec2 tank_position) {
    if (!trajectoryPoints.empty()) {

        glm::vec2 initialPosition = trajectoryPoints.front();

        Projectile* projectile = new Projectile();
        projectile->InitializeProjectile(glm::vec2(0, 30), trajectory, glm::vec3(1.0f, 0.0f, 0.0f));
        activeProjectiles.push_back(projectile);
    }
}

bool Tank::CheckCollision(Tank* tank, const Projectile& projectile) {
    float dx = tank->position.x - projectile.position.x;
    float dy = tank->position.y - projectile.position.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= (radius + projectile.radius)) {

        float damage = 0.1f;
        tank->life -= damage;
        if (tank->life == 0) {
            tank->life = 0;
        }
        return true;
    }
    return false;
}

void Tank::UpdateProjectiles(Tank* otherTank, float deltaTime, std::vector<glm::vec2>& heightMap) {
    const float collisionThreshold = 0.5f;
    const float explosionRadius = 50.0f;

    for (auto it = activeProjectiles.begin(); it != activeProjectiles.end();) {
        Projectile* projectile = *it;

        if (projectile->trajectoryIndex < projectile->trajectory.size()) {
            projectile->position = projectile->trajectory[projectile->trajectoryIndex];
            if (CheckCollision(otherTank, *projectile)) {
                cout << "lovit"<<endl;
                it = activeProjectiles.erase(it);
                continue;
            }

            float terrainHeight = GetTerrainHeight(projectile->position.x, heightMap);
            if (projectile->position.y -10 <= terrainHeight) {

                DeformTerrain(projectile->position.x, terrainHeight, explosionRadius, heightMap);

                it = activeProjectiles.erase(it);
                continue;
            }

            projectile->mesh->positions.push_back(glm::vec3(projectile->position, 0.0f));
            projectile->trajectoryIndex++;
        }
        else {
            it = activeProjectiles.erase(it);
            continue;
        }

        ++it;
    }
    
}
