#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "Sphere.hh"

using namespace std;

Sphere::Sphere()
{
    init(48);
}

Sphere::Sphere(int prec) // prec - precision / number of slices
{
    init(prec);
}

float Sphere::toRadians(float degrees)
{
    const float PI = 3.14159f;
    return (degrees * 2.0f * PI) / 360.0f;
}

void Sphere::init(int prec)
{
    _numVertices = (prec + 1) * (prec + 1);
    _numIndices = prec * prec * 6;

    for(int i = 0; i < _numVertices; i++)
    {
        _vertices.push_back(glm::vec3());
    }

    for(int i = 0; i < _numVertices; i++)
    {
        _texCoords.push_back(glm::vec2());
    }

    for(int i = 0; i < _numVertices; i++)
    {
        _normals.push_back(glm::vec3());
    }

    for(int i = 0; i < _numIndices; i++)
    {
        _indices.push_back(0);
    }

    // calculate triangle vertices
    for(int i = 0; i < prec; i++)
    {
        for(int j = 0; j < prec; j++)
        {
            float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float) abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));

            auto arrIndex = i * (prec + 1) + j;
            _vertices[arrIndex] = glm::vec3(x, y, z);
            _texCoords[arrIndex] = glm::vec2((float) j / prec, (float) i / prec);
            _normals[arrIndex] = glm::vec3(x, y, z);
        }
    }

    // calculate triangle indices
    for(int i = 0; i < prec; i++)
    {
        for(int j = 0; j < prec; j++)
        {
            auto baseIndex = 6 * (i * prec + j);

            _indices[baseIndex + 0] = i * (prec + 1) + j;
            _indices[baseIndex + 1] = i * (prec + 1) + j + 1;
            _indices[baseIndex + 2] = (i + 1) * (prec + 1) + j;
            _indices[baseIndex + 3] = i * (prec + 1) + j + 1;
            _indices[baseIndex + 4] = (i + 1) * (prec + 1) + j + 1;
            _indices[baseIndex + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

// accessors

int Sphere::getNumVertices()
{
    return _numVertices;
}

int Sphere::getNumIndices()
{
    return _numIndices;
}

std::vector<int> Sphere::getIndices()
{
    return _indices;
}

std::vector<glm::vec3> Sphere::getVertices()
{
    return _vertices;
}

std::vector<glm::vec2> Sphere::getTextureCoords()
{
    return _texCoords;
}

std::vector<glm::vec3> Sphere::getNormals()
{
    return _normals;
}


