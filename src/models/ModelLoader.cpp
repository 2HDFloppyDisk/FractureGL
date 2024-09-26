#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "LogUtils.h"
#include "ModelLoader.h"

// Constructor
ModelLoader::ModelLoader() {}

// Load a model from a file using Assimp
bool ModelLoader::loadModel(const std::string& path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        debugMessages.push_back("Assimp Error: " + std::string(importer.GetErrorString()));
        return false;
    }

    // Process the root node recursively
    processNode(scene->mRootNode, scene);
    debugMessages.push_back("Model loaded successfully from: " + path);
    return true;
}

// Render the loaded model
void ModelLoader::renderModel() {
    for (auto& mesh : meshes) {
        mesh.render();
    }
}

// Process a node recursively
void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
    // Process each mesh located at this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // Recurse into each child node
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

// Process an individual mesh and convert to OpenGL-compatible format
Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    // Extract vertex positions
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 vertex;
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;
        vertices.push_back(vertex);
    }

    // Extract indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Create a new Mesh object
    Mesh resultMesh(vertices, indices);
    resultMesh.setupMesh();
    return resultMesh;
}

// Mesh constructor and setup
Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices)
    : vertices(vertices), indices(indices) {}

// Setup the mesh for rendering
void Mesh::setupMesh() {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Check if the correct VAO is bound
    int currentVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    debugMessages.push_back("Current VAO for modelloader: " + std::to_string(currentVAO));
}

// Render the mesh
void Mesh::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
