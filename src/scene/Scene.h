//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_SCENE_H
#define FLOOID_SCENE_H

#include <assimp/scene.h>
#include "Mesh.h"

using namespace std;

GLint texture_from_file(const char* path, string directory);

class Scene {
private:
    vector<Mesh> meshes;
    string dir;

    void load_scene(string path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> load_material_texture(aiMaterial* mat, aiTextureType type,
                                         string typeName);
    vector<Texture> textures_loaded;

public:
    Scene(GLchar *path) {
        load_scene(path);
    }

    void draw(Shader &shader);

};


#endif //FLOOID_SCENE_H
