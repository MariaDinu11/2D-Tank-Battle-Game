#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "lab_m1/Tema1/Tema1_object2D.h"
#include "lab_m1/Tema1/Proiectil.h"

class Tank {
public:
    Tank();
    ~Tank();

    void CreateTankMesh(glm::vec3 color1, glm::vec3 color2);

    void RotateBarrel(float angle);

    Mesh* base1;
    Mesh* base2;
    Mesh* turret;
    Mesh* barrel; 
    Mesh* LifeBar;
    Mesh* Life;

    void GenerateTrajectory(float pos_init_x, float pos_init_y, float barrel_angle, float angle, float magnitude, float deltaTime, std::vector<glm::vec2>& heightMap);
    float GetTerrainHeight(float x, std::vector<glm::vec2>& heightMap);

    void LaunchProjectile(float magnitude, float deltaTime, std::vector<glm::vec2> trajectory, glm::vec2 tank_position);
    void UpdateProjectiles(Tank* otherTank, float deltaTime, std::vector<glm::vec2>& heightMap);
    void DeformTerrain(float impactX, float impactY, float radius, std::vector<glm::vec2>& heightMap);
    bool CheckCollision(Tank* tank, const Projectile& projectile);

    float barrelAngle = 0.0f;

    float life;
    float radius = 55.0f;
    glm::vec2 position;

    std::vector<glm::vec2> trajectoryPoints;
    Mesh* trajectoryMesh;
    std::vector<Projectile*> activeProjectiles;

private:
    

};
