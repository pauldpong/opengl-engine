#include <iostream>
#include <random>
using namespace std;

#include "render/DisplayManager.h"
#include "render/Loader.h"
#include "render/MasterRenderer.h"
#include "render/base/StaticShader.h"
#include "render/base/Texture.h"
#include "render/base/TexturedModel.h"
#include "render/entities/Camera.h"

#include "render/base/ObjLoader.h"
#include "render/entities/Player.h"

int main() {
    DisplayManager display("Engine");
    DisplayCreationStatus displayCreationStatus = display.create();
    if (displayCreationStatus == INIT_ERROR) { return -1; }

    Loader loader;

    MasterRenderer renderer;

    RawModel model = ObjLoader::loadObjModel("../assets/fern.obj", loader);
    Material material = Material(0.1, 10);
    material.setTransparent(true);
    material.setFakeLighting(true);
    Material terrainMat = Material();
    Texture texture = Texture(loader.loadTexture("../assets/fern.png"), material);
    Texture grass = Texture(loader.loadTexture("../assets/grass.jpg"), terrainMat);

    RawModel model2 = ObjLoader::loadObjModel("../assets/grassModel.obj", loader);
    Texture grassTexture = Texture(loader.loadTexture("../assets/grassTexture.png"), material);

    TexturedModel texturedModel = TexturedModel(model, texture);
    TexturedModel grassModel = TexturedModel(model2, grassTexture);

    TerrainGrid grid0 = TerrainGrid(0, 0, loader, grass);
    TerrainGrid grid1 = TerrainGrid(-1, -1, loader, grass);
    TerrainGrid grid2 = TerrainGrid(-1, 0, loader, grass);
    TerrainGrid grid3 = TerrainGrid(0, -1, loader, grass);

    Camera camera;
    Light light(vec3(20000, 20000, 20000), vec3(1, 1, 1));

    std::vector<Entity> entities;

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0, 1);


    for (int i = 0; i < 500; i++) {
        Entity newEntity = Entity(texturedModel, vec3(distribution(generator) * 800 - 400, 0, distribution(generator) * -600), 0, 0, 0, 1);

        entities.push_back(newEntity);
    }

    for (int i = 0; i < 200; i++) {
        Entity newEntity = Entity(grassModel, vec3(distribution(generator) * 800 - 400, 0, distribution(generator) * -600), 0, 0, 0, 2);
        entities.push_back(newEntity);
    }

    Player player = Player(texturedModel, vec3(0, 0, 0), 0, 0, 0, 1);
    entities.push_back(player);

    while (!display.closeRequested()) {
//        player.move();

        for (auto entity : entities) {
            renderer.processEntity(entity);
        }
        renderer.processTerrainGrid(grid0);
        renderer.processTerrainGrid(grid1);
        renderer.processTerrainGrid(grid2);
        renderer.processTerrainGrid(grid3);

        renderer.render(light, camera);
        display.refresh();
        glfwPollEvents();
    }

    renderer.clean();
    loader.clean();
    display.close();
    return 0;
}