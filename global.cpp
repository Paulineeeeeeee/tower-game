#include "global.h"

/*int window_width = 800;
int window_height = 600;
int field_width = 600;
int field_height = 600;
int NumOfTower = 5;*/

int TowerRadius[] = {80, 80, 70, 110, 150};
char TowerClass[][20] = {"Arcane", "Archer", "Canon", "Poison", "Storm"};
int TowerWidth[] = {72, 66, 72, 59, 45};
int TowerHeight[] = {121, 108, 69, 98, 112};

bool EASY = false;
bool HARD = false;

MovableObject O1;
MovableObject O2;
MovableObject B1[5];
MovableObject B2[5];

double now1=0;
double wall1 = 0;
double now2 = 0;
double wall2 = 0;
int HealthPoint1 = 100;
int HealthPoint2 = 100;
int Coin1 = 0;
int Coin2 = 0;
int count1 = 0;
int count2 = 0;
int han1 = 0;
int han2 = 0;
int chen1 = 0;
int chen2 =0;
int k1 = 0;
int k2 = 0;
