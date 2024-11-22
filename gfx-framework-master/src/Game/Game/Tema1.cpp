#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/Tema1_object2D.h"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include "utils/glm_utils.h"
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/Tank.h"


using namespace std;
using namespace m1;

Tema1::Tema1() {
}

Tema1::~Tema1() {
}



void Tema1::Init() {
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    //camera->SetPosition(glm::vec3(100, 50, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    windowWidth = resolution.x;
    windowHeight = resolution.y;

    GenerateHeightMap();
    CreateTerrainMesh();
    
    tank_roz = new Tank();
    //tank_roz->SetHeightMap(heightMap);
    glm::vec3 color1(0.8f, 0.4f, 0.5f);  // Culoare pentru baza tancului
    glm::vec3 color2(0.9f, 0.5f, 0.6f);  // Culoare pentru celelalte piese ale tancului
    tank_roz->CreateTankMesh(color1, color2);

    tank_mov = new Tank();
    glm::vec3 color1_m(0.5f, 0.0f, 0.5f);  // Culoare pentru baza tancului
    glm::vec3 color2_m(0.8f, 0.5f, 0.8f);  // Culoare pentru celelalte piese ale tancului
    tank_mov->CreateTankMesh(color1_m, color2_m);

    tank_roz->life = 1.0f;
    tank_mov->life = 1.0f;

    tank_roz->position = glm::vec2(500.0f, GetTerrainHeight(500.0f));  // Poziția tancului roz
    tank_mov->position = glm::vec2(600.0f, GetTerrainHeight(600.0f));  // Poziția tancului mov

    tank_roz_angle = GetTerrainSlope(tank_roz->position.x);  // Unghiul pentru tancul roz
    tank_mov_angle = GetTerrainSlope(tank_mov->position.x);  // Unghiul pentru tancul mov

    glm::vec3 center(0.0f, 55.0f, 0.0f);
    float radius = 5.0f;
    glm::vec3 color(1.0f, 0.0f, 0.0f);

    tank_roz->activeProjectiles = {};
    tank_mov->activeProjectiles = {};
}

void Tema1::RenderTrajectory(Tank* tank, glm::mat3 matrix) {

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < tank->trajectoryPoints.size(); ++i) {
        glm::vec2 currentPoint = tank->trajectoryPoints[i];
        //matrix *= transform2D::Translate(currentPoint.x, currentPoint.y);

        if (currentPoint.y <= GetTerrainHeight(currentPoint.x) || currentPoint.y < 0) {
            break;
        }

        vertices.push_back(VertexFormat(glm::vec3(currentPoint, 0), glm::vec3(1.0f, 1.0f, 1.0f))); // Culoare albă pentru traiectorie
        if (i > 0) {
            indices.push_back(i - 1);
            indices.push_back(i);
        }
    }

    trajectoryMesh = new Mesh("trajectory");
    trajectoryMesh->InitFromData(vertices, indices);
    trajectoryMesh->SetDrawMode(GL_LINE_STRIP);

    RenderMesh2D(trajectoryMesh, shaders["VertexColor"], matrix);
}


void Tema1::GenerateHeightMap() {
    const int numPoints = 200;
    const float xStep = windowWidth  / float(numPoints);

    float A1 = 50.0f, B1 = 30.0f;
    float omega1 = 0.01f, omega2 = 0.02f;

    heightMap.clear();
    for (int i = 0; i <= numPoints; ++i) {
        float x = i * xStep;

        float y = A1 * std::sin(omega1 * x) + B1 * std::sin(omega2 * x);

        heightMap.push_back(glm::vec2(x, y + windowHeight / 2));
    }
}

void Tema1::CreateTerrainMesh() {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.5f);

    for (size_t i = 0; i < heightMap.size() - 1; ++i) {
        glm::vec2 p1 = heightMap[i];
        glm::vec2 p2 = heightMap[i + 1];
        glm::vec2 p3 = glm::vec2(p1.x, 0);
        glm::vec2 p4 = glm::vec2(p2.x, 0);

        vertices.push_back(VertexFormat(glm::vec3(p1, 0), color));
        vertices.push_back(VertexFormat(glm::vec3(p3, 0), color));
        vertices.push_back(VertexFormat(glm::vec3(p2, 0), color));
        vertices.push_back(VertexFormat(glm::vec3(p4, 0), color));

        int baseIndex = i * 4;
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);
    }

    terrainMesh = new Mesh("terrain");
    terrainMesh->InitFromData(vertices, indices);
    terrainMesh->SetDrawMode(GL_TRIANGLE_STRIP);
}

float Tema1::GetTerrainHeight(float x) {
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

void Tema1::FrameStart() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::RenderTerrain() {
   RenderMesh2D(terrainMesh, shaders["VertexColor"], glm::mat3(1));

}

float Tema1::GetTerrainSlope(float x) {

    float y1 = GetTerrainHeight(x);
    float y2 = GetTerrainHeight(x + 1.0f);

    float slope = (y2 - y1);

    float angle = std::atan(slope);

    return angle;
}

void Tema1::SlideTerrain(std::vector<glm::vec2>& heightMap, float threshold, float epsilon, float deltaTime) {

    for (size_t i = 0; i < heightMap.size() - 1; ++i) {
        float heightDiff = heightMap[i].y - heightMap[i + 1].y;

        if (std::abs(heightDiff) > threshold) {
            float transfer = epsilon * deltaTime;

            if (heightDiff > 0) {
                heightMap[i].y -= transfer;
                heightMap[i + 1].y += transfer;
            }
            else {
                heightMap[i].y += transfer;
                heightMap[i + 1].y -= transfer;
            }

            heightMap[i].y = glm::max(heightMap[i].y, 0.0f);
            heightMap[i + 1].y = glm::max(heightMap[i + 1].y, 0.0f);
        }
    }
}

glm::mat3 Tema1::RenderTank(int nr, Tank* tank, float trans1, float trans2, float angle, float barrel_angle, float deltaTime) {
    glm::mat3 modelMatrix = glm::mat3(1);

    modelMatrix *= transform2D::Translate(trans1, trans2);
    modelMatrix *= transform2D::Rotate(angle);
    //modelMatrix *= transform2D::Translate(-trans1, -trans2);

    RenderMesh2D(tank->base1, shaders["VertexColor"], modelMatrix);

    RenderMesh2D(tank->base2, shaders["VertexColor"], modelMatrix);

    RenderMesh2D(tank->turret, shaders["VertexColor"], modelMatrix);

    glm::mat3 turretMatrix = modelMatrix;
    turretMatrix *= transform2D::Translate(0, 30);
    turretMatrix *= transform2D::Rotate(barrel_angle);

    glm::vec2 start;
    start.x = turretMatrix[2][0];
    start.y = turretMatrix[2][1];

    RenderMesh2D(tank->barrel, shaders["VertexColor"], turretMatrix);

    float x = 0;

    tank->GenerateTrajectory(start.x, start.y, barrel_angle, angle, 125.0f, deltaTime, heightMap);

    glm::mat3 trajectoryMatrix = glm::mat3(1);
    RenderTrajectory(tank, trajectoryMatrix);

    glLineWidth(3.0f);
    tank->LifeBar->SetDrawMode(GL_LINE_LOOP);
    RenderMesh2D(tank->LifeBar, shaders["VertexColor"], modelMatrix);
    glLineWidth(1.0f);

    float healthPercentage = tank->life;
        glm::mat3 lifeMatrix = modelMatrix;
        lifeMatrix *= transform2D::Translate(-40 * (1.0f - healthPercentage), 0);
        lifeMatrix *= transform2D::Scale(healthPercentage, 1.0f);

        RenderMesh2D(tank->Life, shaders["VertexColor"], lifeMatrix);

    for (const auto& projectile : tank->activeProjectiles) {
        glm::mat3 projectileModelMatrix = turretMatrix;

        if (projectile->trajectoryIndex < projectile->trajectory.size()) {

            glm::vec2 currentPosition = projectile->trajectory[projectile->trajectoryIndex];
            if(nr == 1)
                tank->UpdateProjectiles(tank_mov, deltaTime, heightMap);
            else
                tank->UpdateProjectiles(tank_roz, deltaTime, heightMap);

            projectile->Update(deltaTime);
           //CreateTerrainMesh();
           projectileModelMatrix = transform2D::Translate(currentPosition.x, currentPosition.y);
        }


        if (projectile->mesh) {
            RenderMesh2D(projectile->mesh, shaders["VertexColor"], projectileModelMatrix);
        }
    }

    return turretMatrix;
}

void Tema1::Update(float deltaTime) {
    float threshold = 7.0f;
    float epsilon = 40.0f;
    SlideTerrain(heightMap, threshold, epsilon, deltaTime);
    CreateTerrainMesh();
    RenderTerrain();

    if(tank_roz->life > 0)
        modelMatrix_roz = RenderTank(1, tank_roz, tank_roz->position.x, tank_roz->position.y, tank_roz_angle, tank_roz->barrelAngle, deltaTime);
    
    if(tank_mov->life > 0)
        modelMatrix_mov = RenderTank(2, tank_mov, tank_mov->position.x, tank_mov->position.y, tank_mov_angle, tank_mov->barrelAngle, deltaTime);
  
}


void Tema1::FrameEnd() {
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_A)) {
        tank_roz->position.x -= 200.f * deltaTime;
        if (tank_roz->position.x <= 0)
            tank_roz->position.x = 0;
        else if(tank_roz->position.x >= window->GetResolution().x)
            tank_roz->position.x = window->GetResolution().x;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tank_roz->position.x += 200.f * deltaTime;
        if (tank_roz->position.x <= 0)
            tank_roz->position.x = 0;
        else if (tank_roz->position.x >= window->GetResolution().x)
            tank_roz->position.x = window->GetResolution().x;
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tank_mov->position.x -= 200.f * deltaTime;
        if (tank_mov->position.x <= 0)
            tank_mov->position.x = 0;
        else if (tank_mov->position.x >= window->GetResolution().x)
            tank_mov->position.x = window->GetResolution().x;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tank_mov->position.x += 200.f * deltaTime;
        if (tank_mov->position.x <= 0)
            tank_mov->position.x = 0;
        else if (tank_mov->position.x >= window->GetResolution().x)
            tank_mov->position.x = window->GetResolution().x;
    }

    tank_roz->position.y = GetTerrainHeight(tank_roz->position.x);
    tank_mov->position.y = GetTerrainHeight(tank_mov->position.x);

    tank_roz_angle = GetTerrainSlope(tank_roz->position.x);
    tank_mov_angle = GetTerrainSlope(tank_mov->position.x);

    if (window->KeyHold(GLFW_KEY_W)) {
        tank_roz->RotateBarrel(1.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        tank_roz->RotateBarrel(-1.0f * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_UP)) {
        tank_mov->RotateBarrel(1.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        tank_mov->RotateBarrel(-1.0f * deltaTime);
    } 

}

void Tema1::OnKeyPress(int key, int mods)
{
    // TODO(student): Switch between GL_FRONT and GL_BACK culling.
    // Save the state in `cullFace` variable and apply it in the
    // `Update()` method, NOT here!

    if (key == GLFW_KEY_SPACE) {
        std::cout << "Lansare proiectil roz!" << std::endl;
        /*glm::vec2 start;
        start.x = modelMatrix_roz[2][0];
        start.y = modelMatrix_roz[2][1];*/
        tank_roz->LaunchProjectile(150.0f, 0.05f, tank_roz->trajectoryPoints, tank_roz->position);
    }
    if (key == GLFW_KEY_ENTER) {
        tank_mov->LaunchProjectile(150.0f, 0.05f, tank_mov->trajectoryPoints, tank_mov->position);
        std::cout << "Lansare proiectil mov!" << std::endl;
    }
}

