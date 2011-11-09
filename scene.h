#ifndef SCENE_H
#define SCENE_H

#include <vector>
using namespace std;
#include "struct.h"

class Scene
{
public:
    vector<texture> textureContainer;
    vector<sphere> sphereContainer;
    vector<light> lightContainer;
    vector<plane> planeContainer;
    int sizex, sizey;

    Scene(int x, int y);
    void insertTexture(texture a);
    void insertSphere(sphere a);
    void insertLight(light a);
    void insertPlane(plane a);

};

#endif // SCENE_H
