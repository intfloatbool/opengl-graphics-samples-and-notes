#include <cmath>
#include <vector>
#include <glm/glm.hpp>

class Sphere
{

private:
    int _numVertices;
    int _numIndices;
    std::vector<int> _indices;
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec2> _texCoords;
    std::vector<glm::vec3> _normals;

    void init(int);
    float toRadians(float degrees);

public:
    Sphere();
    Sphere(int prec);
    int getNumVertices();
    int getNumIndices();

    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTextureCoords();
    std::vector<glm::vec3> getNormals();

};