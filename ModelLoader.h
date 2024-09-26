#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


// Structure to represent a mesh
struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);
    void setupMesh();
    void render();
};

class ModelLoader {
public:
    // Constructor
    ModelLoader();

    // Load a model from file
    bool loadModel(const std::string& path);

    // Render the loaded model
    void renderModel();

private:
    // Process a node recursively
    void processNode(aiNode* node, const aiScene* scene);

    // Process a mesh and return a Mesh object
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Mesh> meshes;
};

#endif // MODELLOADER_H
