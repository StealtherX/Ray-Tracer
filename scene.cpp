#include "scene.h"

Scene::Scene(int x, int y)
{
    sizex = x;
    sizey = y;
}

void Scene::insertTexture(texture a)
{
    textureContainer.push_back(a);
}

void Scene::insertSphere(sphere a)
{
    sphereContainer.push_back(a);
}

void Scene::insertLight(light a)
{
    lightContainer.push_back(a);
}

void Scene::insertPlane(plane a)
{
    planeContainer.push_back(a);
}
