#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"

class Menu : public Object
{
public:

    Menu();
    virtual ~Menu();

    void Reset();
    void Draw();
    // Detect if cursor hovers over any of tower on menu
    // If so, return its type
    // Otherwise, return -1
    //void KeyIn(int);
    int MouseIn(int mouse_x, int mouse_y);
    // static function that detect if one point is on a line
    // This function is just used to simplify "MouseIn"
    static bool isInRange(int, int, int);

    // Check if current coin is not less than needed coin
    bool Enough_Coin(int);

    bool Enough(int,int);
    void wall(int);
    void alcohol(int);
    void blade(int);

    void Change_Coin(int change) {
        Coin1 += 10;
        Coin2 += 10;
     }
    bool Subtract_HP(int escapeNum = 1);
    void Gain_Score(int);

    int getTowerCoin(int type) { return need_coin[type]; }
    int getScore() { return Score; }
    int getCoin() { return Coin1; }
    int getKilled() { return killedMonster; }

    int winner();

private:
    std::vector<ALLEGRO_BITMAP*> menu_tower;
    ALLEGRO_BITMAP *love = NULL;
    std::vector<ALLEGRO_BITMAP*> ch;
    ALLEGRO_FONT *menuFont;

    int killedMonster = 0;
    int Score = 0;

    float vx,vy;
    int selectedTower = -1;
    int need_coin[Num_TowerType] = {-5, -10, -20, -40, -70};

};

#endif // MENU_H_INCLUDED
