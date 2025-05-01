#include "Mesh.h"
#include <iostream>

Mesh::Mesh() {
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices) {
    IndexedModel model;

    for (unsigned int i = 0; i < numVertices; i++) {
        model.positions.push_back(*vertices[i].GetPos());
        model.texCoords.push_back(*vertices[i].GetTexCoord());
        model.normals.push_back(*vertices[i].GetNormal());
    }

    for (unsigned int i = 0; i < numIndices; i++) {
        model.indices.push_back(indices[i]);
    }

    initModel(model);
}

void Mesh::initModel(const IndexedModel& model) {
    drawCount = model.indices.size();

    if (model.positions.empty() || model.texCoords.empty() || model.normals.empty()) {
        std::cerr << "ERROR: Model data is missing!" << std::endl;
        return;
    }

    // Create Buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Create Interleaved Data
    std::vector<float> interleavedData;
    for (size_t i = 0; i < model.positions.size()-3; i+=3) {

        glm::vec3 edge1 = model.positions[i + 1] - model.positions[i];
        glm::vec3 edge2 = model.positions[i + 2] - model.positions[i];
        glm::vec2 deltaUV1 = model.texCoords[i + 1] - model.texCoords[i];
        glm::vec2 deltaUV2 = model.texCoords[i + 2] - model.texCoords[i];

        float f = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV2.x * deltaUV1.y));

        glm::vec3 tangent1 = glm::vec3(
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        );

        glm::vec3 bitangent1 = glm::vec3(
            f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
            f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
            f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
        );

        for (int j = 0; j < 3; j++)
        {
            interleavedData.push_back(model.positions[i + j].x);
            interleavedData.push_back(model.positions[i + j].y);
            interleavedData.push_back(model.positions[i + j].z);

            interleavedData.push_back(model.normals[i + j].x);
            interleavedData.push_back(model.normals[i + j].y);
            interleavedData.push_back(model.normals[i + j].z);

            interleavedData.push_back(model.texCoords[i + j].x);
            interleavedData.push_back(model.texCoords[i + j].y);

            interleavedData.push_back(tangent1.x);
            interleavedData.push_back(tangent1.y);
            interleavedData.push_back(tangent1.z);

            interleavedData.push_back(bitangent1.x);
            interleavedData.push_back(bitangent1.y);
            interleavedData.push_back(bitangent1.z);
        }
    }

    glBindVertexArray(vao);

    // Interleaved Data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float), interleavedData.data(), GL_STATIC_DRAW);

    // Index Data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), model.indices.data(), GL_STATIC_DRAW);

    // DVertex Attributes
    size_t stride = sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec3);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    glEnableVertexAttribArray(1); // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(2); // Texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));

    glEnableVertexAttribArray(3); // Tangents
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2)));

    glEnableVertexAttribArray(4); // Bitangents
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)));

    glBindVertexArray(0);
}

void Mesh::loadModel(const std::string& filename) {
    IndexedModel model = OBJModel(filename).ToIndexedModel();
    initModel(model);
} 

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}