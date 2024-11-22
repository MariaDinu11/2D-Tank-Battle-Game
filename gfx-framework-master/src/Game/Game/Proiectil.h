#include <glm/glm.hpp>
#include <vector>
#include "lab_m1/Tema1/Tema1_object2D.h"

class Projectile {
public:
    Projectile(); 
    void InitializeProjectile(glm::vec2 startPosition, const std::vector<glm::vec2>& trajectoryPoints, glm::vec3 color);
    void Update(float deltaTime);
    void CreateProjectileMesh(const std::string& name, glm::vec3 color);

    Mesh* mesh;
    glm::vec2 position;  
    float radius = 5.0f;

    std::vector<glm::vec2> trajectory; // Traiectoria proiectilului
    size_t trajectoryIndex;
private:

};
