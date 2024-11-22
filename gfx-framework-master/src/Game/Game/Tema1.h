#pragma once

#include "components/simple_scene.h"
#include <glm/glm.hpp>
#include <vector>
#include "lab_m1/Tema1/Tank.h"
//#include "lab_m1/Tema1/Proiectil.h"

namespace m1 {
    class Tema1 : public gfxc::SimpleScene {
    public:
        Tema1();
        ~Tema1();

        void Init() override;
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods);
        void OnKeyPress(int key, int mods);

    private:
        void GenerateHeightMap();
        void CreateTerrainMesh();
        void RenderTerrain();
        glm::mat3 RenderTank(int nr, Tank* tank, float trans1, float trans2, float angle, float barrel_angle, float deltaTime);
        float GetTerrainHeight(float x);
        float GetTerrainSlope(float x);
        void SlideTerrain(std::vector<glm::vec2>& heightMap, float threshold, float epsilon, float deltaTime);
        //bool CheckCollision(const Tank& tank, const Projectile& projectile);

        void RenderTrajectory(Tank* tank, glm::mat3 matrix);

        int windowWidth, windowHeight;

        std::vector<glm::vec2> heightMap;
        Mesh* terrainMesh;
        Mesh* tankMesh;
        Tank* tank_roz;
        Tank* tank_mov;
        Mesh* proiectil;
        Mesh* trajectoryMesh;

        float tank_roz_life;
        float tank_mov_life;
        glm::vec2 tank_roz_position;
        glm::vec2 tank_mov_position;
        float tank_roz_angle;
        float tank_mov_angle;

        glm::mat3 modelMatrix_roz;
        glm::mat3 modelMatrix_mov;
    };
}
