#include<iostream>
#include <allegro5/allegro_font.h>
#include "menu.h"
class MovableObject{
public:
    float x,y;
    float vx,vy;
    bool hidden;
    ALLEGRO_BITMAP* img;
};



