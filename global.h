#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <allegro5/allegro_font.h>

#define font_size 12
#define grid_width 40
#define grid_height 40

#define window_width 1150
#define window_height 644
#define field_width 600
#define field_height 600

#define Num_TowerType 5
#define Num_MonsterType 4
#define NumOfGrid (field_width / grid_width) * (field_height / grid_height)

extern int TowerRadius[];
extern char TowerClass[][20];
extern int TowerWidth[];
extern int TowerHeight[];

typedef struct{
        int player;
        float player_x,player_y;
        float player_w,player_h;
        float player_vx,player_vy;
        bool hidden;
        ALLEGRO_BITMAP *cha;
}MovableObject;

extern double now1;
extern double wall1;
extern double now2;
extern double wall2;
extern int HealthPoint1 ;
extern int HealthPoint2 ;
extern int Coin1;
extern int Coin2;
extern int count1;
extern int count2;
extern int han1;
extern int han2 ;
extern int chen1;
extern int chen2;
extern int k1;
extern int k2;

extern MovableObject O1;
extern MovableObject O2;
extern MovableObject B1[5];
extern MovableObject B2[5];

extern bool EASY;
extern bool HARD;

enum {ARCANE = 0, ARCHER, CANON, POISON, STORM};
enum {WOLF = 0, WOLFKNIGHT, DEMONNIJIA, CAVEMAN};


#endif // GLOBAL_H_INCLUDED
