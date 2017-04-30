//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_MODEL_H
#define FLOOID2_MODEL_H

#include "Shader.h"
#include "Mesh.h"
#include <assimp/scene.h>

class Model {
private:
    /*  Model Data  */
    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    string directory;
    /*  Functions   */
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

public:
    /*  Functions   */
    Model() {}
    void render(Shader &shader);
    void load(string path);
};

GLint TextureFromFile(const char* path, string directory);

#endif //FLOOID2_MODEL_H
