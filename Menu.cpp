#include "Menu.h"

#define MAX_BULLET 5

const int ThumbWidth = 50;
const int ThumbHeight = 50;
const int gapX = 40, gapY = 30;
const int offsetX = 30 + field_width, offsetY = 150;

const int Initial_Health1 = 500;
const int Initial_Coin1 = 0;
const int Initial_Health2 = 500;
const int Initial_Coin2 = 0;



bool
Menu::isInRange(int point, int startPos, int length)
{
    if(point >= startPos && point <= startPos + length)
        return true;

    return false;
}

bool
Menu::Enough_Coin(int type)
{

    if(type < 0 || type >= Num_TowerType)
        return false;

    return (Coin1 + need_coin[type] >= 0);
}
bool Menu::Enough(int a,int b){

    if(a==1){
        if(Coin1>=b){
            Coin1 -= b;
            return true;
        }else return false;
    }if(a==2){
        if(Coin2>=b){
            Coin2 -= b;
            return true;
        }else return false;
    }

}

void Menu::wall(int a){
    if(a == 1){
        wall1 = al_get_time();
    }if(a == 2){
        wall2 = al_get_time();
    }
}
void Menu::alcohol(int a){
    if(a == 1){
        HealthPoint1 += 20;
    }if(a == 2){
        HealthPoint2 += 20;
    }
}

void Menu::blade(int a){
    if(a == 1){
        count1 = 10;
        return;
    }
    if(a == 2){
        count2 = 10;
        return;
    }
}

Menu::Menu()
{
    HealthPoint1 = 500;
    Coin1 = 0;
    HealthPoint2 = 500;
    Coin2 = 0;
    now1=0;
    wall1 = 0;
    now2 = 0;
    wall2 = 0;

    //ch[1] = al_load_bitmap("chen.png");


    love = al_load_bitmap("./Tower/love.png");

    menuFont = al_load_ttf_font("pirulen.ttf", 15, 0); // load font

}

Menu::~Menu()
{
    al_destroy_bitmap(love);
    al_destroy_font(menuFont);

    menu_tower.clear();
}

void
Menu::Reset()
{
    HealthPoint1 = Initial_Health1;
    Coin1 = Initial_Coin1;
    HealthPoint2 = Initial_Health2;
    Coin2 = Initial_Coin2;
    count1 = 0;
    count2 = 0;
    chen1 = 0;
    chen2 = 0;
    han1 = 0;
    han2 = 0;
    k1 = 0;
    k2 = 0;
    HARD = false;
    EASY = false;
}


void
Menu::Draw() //draw mwnu
{
    char buffer[50];

    sprintf(buffer, "%d", HealthPoint1);
    al_draw_bitmap(love, 20, 25, 0);
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 40, 20, 0, buffer);

    sprintf(buffer, "%d", HealthPoint2);
    al_draw_bitmap(love, 1020, 25, 0);
    al_draw_text(menuFont, al_map_rgb(255, 255, 255),1040 , 20, 0, buffer);

    sprintf(buffer, "Coin: %d", Coin1);
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 20, 20 + gapY, 0, buffer);

    sprintf(buffer, "Coin: %d", Coin2);
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 1020, 20 + gapY, 0, buffer);


    if(wall1||wall2){
    if(wall1)
    al_draw_filled_rectangle(570,0,575,487,al_map_rgb(0,100,100));
    if(wall2){
    al_draw_filled_rectangle(575,0,580,487,al_map_rgb(100,100,0));
    }}else{
    al_draw_line(575,0,575,487,al_map_rgb(0,0,0),7);
    }
    if(k1){
        al_draw_filled_circle(O1.player_x, O1.player_y,35, al_map_rgba(196, 79, 79, 200));
    }if(k2){
        al_draw_filled_circle(O2.player_x, O2.player_y,35, al_map_rgba(196, 79, 79, 200));
    }if(chen1){
        sprintf(buffer, "%d", chen1);
        al_draw_text(menuFont, al_map_rgb(255, 255, 0), O1.player_x-20, O1.player_y-45, 0, buffer);
    }if(chen2){
        sprintf(buffer, "%d", chen2);
        al_draw_text(menuFont, al_map_rgb(255, 255, 0), O2.player_x-20, O2.player_y-45, 0, buffer);
    }

}

int
Menu::MouseIn(int mouse_x, int mouse_y)
{
    bool enoughCoin;

    selectedTower = -1;

    for(int i=0; i < Num_TowerType; i++)
    {
        int pos_x = offsetX + (ThumbWidth + gapX) * (i % 2);
        int pos_y = offsetY + (ThumbHeight + gapY) * (i / 2);

        if(isInRange(mouse_x, pos_x, ThumbWidth) && isInRange(mouse_y, pos_y, ThumbHeight))
        {
            if(Enough_Coin(i))
            {
                selectedTower = i;
                break;
            }
        }
    }
    return selectedTower;
}

bool
Menu::Subtract_HP(int escapeNum)
{
    HealthPoint1 -= escapeNum;

    return (HealthPoint1 == 0);
}

void
Menu::Gain_Score(int scoreWorth)
{
    return ;
}

int Menu::winner(){
    if(HealthPoint1 <= 0) return 2;
    if(HealthPoint2 <= 0) return 1;
    else return 0;
 }
