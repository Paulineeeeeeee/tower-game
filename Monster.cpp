#include "Monster.h"

const int axis_x[] = {-1, 1, 0, 0};
const int axis_y[] = {0, 0, -1, 1};
const char direction_name[][10] = {"LEFT", "RIGHT", "UP", "DOWN"};
const int MAX_BULLET = 5;
// set counter frequency of drawing moving animation
const int draw_frequency = 10;
const int vx = 10;

Monster::Monster(std::vector<int> path)
{/*
    this->path = path;
    this->step = 0;

    // default direction is right
    direction = RIGHT;

    circle = new Circle;
    circle->x = (path.front() % 15) * grid_width + grid_width/2;
    circle->y = (path.front() / 15) * grid_height + grid_height/2;
    circle->r = grid_width/2;

    direction_count[LEFT] = 1;
    direction_count[RIGHT] = 1;
    direction_count[UP] = 1;
    direction_count[DOWN] = 1;

    sprite_pos = 0;
    counter = 0;
    strncpy(class_name, "Wolf", 20);*/
}
Monster::Monster(){
    for(int i = 0;i<5;i++){
    B1[i].hidden = true;
    B1[i].player_x = 0;
    B1[i].player_y = -3;
    B1[i].cha = al_load_bitmap("normal1.png");

    B2[i].hidden = true;
    B2[i].player_x = 0;
    B2[i].player_y = -3;
    B2[i].cha = al_load_bitmap("normal.png");
    }
}

Monster::~Monster()
{
    for(unsigned int i=0; i<moveImg.size(); i++)
    {
        ALLEGRO_BITMAP *img = moveImg[i];

        moveImg.erase(moveImg.begin() + i);

        i--;
        al_destroy_bitmap(img);
    }
    moveImg.clear();

    delete circle;
}

void
Monster::Load_Move()
{
    int i = 0;
        for (i=0;i<MAX_BULLET;i++) {
                if (B1[i].hidden)
                    continue;
                B1[i].player_x += vx;
                if(wall2){
                    now2 = al_get_time();
                    if(now2 - wall2 <= 3){
                        if (B1[i].player_x > 575){
                            B1[i].hidden = true;
                            B1[i].player_x = -80;
                            B1[i].player_y = -80;
                        }
                    }else wall2 = 0;
                }else {
                    if (B1[i].player_x > 1000)
                        B1[i].hidden = true;
                }
            }for(i = 0;i<MAX_BULLET;i++){
                if (B2[i].hidden)
                    continue;
                B2[i].player_x -= vx;
                if(wall1){
                    now1 = al_get_time();
                    if(now1 - wall1 <= 3){
                        if (B2[i].player_x < 575){
                            B2[i].hidden = true;
                            B2[i].player_x = -80;
                            B2[i].player_y = -80;
                        }
                    }else wall1 = 0;
                }else if (B2[i].player_x < 150)
                    B2[i].hidden = true;
        }
        Monster::Attack();
}

void
Monster::Draw()
{
    for(int i = 0;i<5;i++){
        if (!B1[i].hidden)
        al_draw_bitmap(B1[i].cha, B1[i].player_x - 25, B1[i].player_y -40, 0);
        if (!B2[i].hidden)
        al_draw_bitmap(B2[i].cha, B2[i].player_x - 25, B2[i].player_y-40 , 0);
    }
}

void Monster::Attack(){
    for(int i=0;i<MAX_BULLET;i++){
                if(B1[i].player_x+30>O2.player_x - 25&&B1[i].player_x+30<O2.player_x +25&&B1[i].player_y>O2.player_y - 25-15 &&B1[i].player_y<O2.player_y + 50){
        						if(han1!=0){
                                    han1--;
                                    HealthPoint2 -= 30;
        						}else if(k2){
                                    k2--;
        						}else{
                                    HealthPoint2 -= 5;
        						}
        						B1[i].hidden = true;
        						B1[i].player_x = -80;
        						B1[i].player_y = -80;
					}
                if(B2[i].player_x+30>O1.player_x - 25&&B2[i].player_x+30<O1.player_x +25&&B2[i].player_y>O1.player_y - 25 - 15 &&B2[i].player_y<O1.player_y + 50){
        						if(han2){
                                    han2--;
                                    HealthPoint1 -= 30;
        						}else if(k1){
                                    k1--;
        						}else{
                                    HealthPoint1 -= 5;
        						}
        						B2[i].player_x = -80;
        						B2[i].player_y = -80;
        						B2[i].hidden = true;

					}
		}
}

bool
Monster::Move()
{
/*
    int target_grid_x, target_grid_y;

    counter = (counter + 1) % draw_frequency;

    if(counter == 0)
        sprite_pos = (sprite_pos + 1) % direction_count[direction];

    if(step + 1 < path.size())
    {
        // coordinate of next grid
        target_grid_x = (path[step] % 15) * grid_width + grid_width/2;
        target_grid_y = (path[step] / 15) * grid_height + grid_height/2;

        if(circle->x == target_grid_x && circle->y == target_grid_y)
        {
            int cur_grid = path[step];
            int next_grid = path[step+1];
            int prev_direction = direction;

            switch(direction)
            {
                case LEFT:
                case RIGHT:
                    if(next_grid == cur_grid - 15)
                        direction = UP;
                    else if(next_grid == cur_grid + 15)
                        direction = DOWN;

                    break;
                case UP:
                case DOWN:
                    if(next_grid == cur_grid - 1)
                        direction = LEFT;
                    else if(next_grid == cur_grid + 1)
                        direction = RIGHT;

                    break;

            }

            step++;

            if(prev_direction != direction)
                sprite_pos = 0;

            // reach final grid and set end point
            if(step == path.size() - 1){
                end_x = circle->x + axis_x[direction] * (2 * grid_width);
                end_y = circle->y + axis_y[direction] * (2 * grid_height);
            }
        }
    }

    // when getting to end point, return true
    if(circle->x == end_x && circle->y == end_y)
        return true;

    circle->x += speed * axis_x[direction];
    circle->y += speed * axis_y[direction];

    // if not reaching end point, return false*/
    return false;
}

bool
Monster::Subtract_HP(int harm_point)
{
    HealthPoint -= harm_point;

    return (HealthPoint <= 0);
}

