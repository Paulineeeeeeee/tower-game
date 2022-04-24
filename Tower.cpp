#include "Tower.h"

const int MAX_BULLET = 5;

Tower::Tower(){}

Tower::Tower(int a){
   if(a == 1){
    O1.player = 1;
    O1.hidden = false;
    O1.player_x = 170;
    O1.player_y = 300;
    O1.cha = al_load_bitmap("han.png");
   }

if(a == 2){
    O2.player = 1;
    O2.hidden = false;
    O2.player_x = 970;
    O2.player_y = 300;
    O2.cha = al_load_bitmap("han.png");
}
}

Tower::~Tower()
{
    delete circle;

    al_destroy_bitmap(img);
    al_destroy_bitmap(attack_img);

    for(auto&& child : this->attack_set) {
        delete child;
    }
    this->attack_set.clear();
}

void
Tower::Draw()
{
    if (!O1.hidden)
	al_draw_bitmap(O1.cha, O1.player_x - 25, O1.player_y - 25, 0);
    if (!O2.hidden)
	al_draw_bitmap(O2.cha, O2.player_x - 25, O2.player_y - 25, 0);
}

void
Tower::SelectedTower(int mouse_x, int mouse_y, int type)
{/*
    int draw_x = mouse_x - (TowerWidth[type]/2);
    int draw_y = mouse_y - (TowerHeight[type] - (TowerWidth[type]/2));
    char filename[50];
    ALLEGRO_BITMAP *bitmap;

    sprintf(filename, "./Tower/%s.png", TowerClass[type]);
    bitmap = al_load_bitmap(filename);

    al_draw_bitmap(bitmap, draw_x, draw_y, 0);
    al_draw_filled_circle(mouse_x, mouse_y, TowerRadius[type], al_map_rgba(196, 79, 79, 200));
    al_draw_filled_circle(mouse_x, mouse_y, 2, al_map_rgb(0, 0, 0));

    al_destroy_bitmap(bitmap);*/
}

bool
Tower::DetectAttack(Monster *monster)
{/*
    bool willAttack = false;
    Attack *attack;

    if(Circle::isOverlap(this->circle, monster->getCircle()))
    {
        if(attack_counter == 0)
        {
            attack = new Attack(
                this->circle,
                monster->getCircle(),
                this->attack_harm_point,
                this->attack_velocity,
                this->attack_img
            );

            this->attack_set.push_back(attack);
            willAttack = true;
        }

        attack_counter = (attack_counter + 1) % attack_frequency;
    }

    return willAttack;*/
}

bool
Tower::TriggerAttack(int player)
{
    if(player == 1){
    int i = 0;
        for (i = 0; i<5;i++) {
                if (B1[i].hidden)
                    break;
            }
            if (i < 5) {
                count1++;
                    if(count1 >= 20){
                        count1 = 0;
                        if(O1.player == 1){
                            han1 = 5;
                        }else if(O1.player == 2){
                            chen2 = 100;
                        }else if(O1.player == 3){
                             k1++;
                        }else if(O1.player == 4){
                            Coin2  = (Coin2/2)/2;
                        }
                    }
                else if(count1 || han1){
                    B1[i].hidden = false;
                    B1[i].player_x = O1.player_x - 25;//從那個x射出去
                    B1[i].player_y = O1.player_y + 25;//要從中心點減一半高度
                    return true;
                }
            }
    }
    else if(player == 2){
    int i = 0;
        for (i = 0; i<5;i++) {
                if (B2[i].hidden)
                    break;
            }
            if (i < 5) {
                    count2++;
                    if(count2 >= 20){
                        count2 = 0;
                        if(O2.player == 1){
                            han2 = 5;
                        }else if(O2.player == 2){
                            chen1 = 100;
                        }else if(O2.player == 3){
                            k2 ++;
                        }else if(O2.player == 4){
                            Coin1 = (Coin1/2)/ 2;
                        }
                    }
                if(count2 || han2){
                   // Attack::Attack(B2[i]);
                    B2[i].hidden = false;
                    B2[i].player_x = O2.player_x - 25;//從那個x射出去
                    B2[i].player_y = O2.player_y + 25;//要從中心點減一半高度
                    return true;
                }

            }
    }
return false;
}

void
Tower::update(int a,int b){
    vx = 0;
    vy = 0;
    if(b == 1){
        vy = -10;
    }else if(b == 2){
        vy = 10;
    }else if(b == 3){
        vx = 10;
    }else if(b == 4){
        vx = -10;
    }
    if(a==1){
        if(chen1){
            chen1--;
        }else {
            O1.player_x += vx;
            O1.player_y += vy;
            if(O1.player_x<150) O1.player_x = 150;
            if(O1.player_x>570) O1.player_x = 570;
            if(O1.player_y<0) O1.player_y = 0;
            if(O1.player_y>400) O1.player_y = 400;
        }
    }else if(a==2){
        if(chen2){
            chen2--;
        }else {
            O2.player_x += vx;
            O2.player_y += vy;
            if(O2.player_x<580) O2.player_x = 580;
            if(O2.player_x>1000) O2.player_x = 1000;
            if(O2.player_y<0) O2.player_y = 0;
            if(O2.player_y>400) O2.player_y = 400;
        }
    }
}
void
Tower::KeyIn(int player)
{
    if(player == 1){
        O1.cha = al_load_bitmap("han.png");
        O1.player = 1;
    }else if(player == 2){
        O1.cha = al_load_bitmap("chen.png");
        O1.player = 2;
    }else if(player == 3){
        O1.cha = al_load_bitmap("k.png");
        O1.player = 3;
    }else if(player == 4){
        O1.cha = al_load_bitmap("lee.png");
        O1.player = 4;
    }
    else if(player == 5){
        O2.cha = al_load_bitmap("han.png");
        O2.player = 1;
    }else if(player == 6){
        O2.cha = al_load_bitmap("chen.png");
        O2.player = 2;
    }else if(player == 7){
        O2.player = 3;
        O2.cha = al_load_bitmap("k.png");
    }else if(player == 8){
        O2.player = 4;
        O2.cha = al_load_bitmap("lee.png");
    }
}

