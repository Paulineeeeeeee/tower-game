#include "Attack.h"

const int Velocity[] = {8, 8, 6, 10, 20};
const int Harm[] = {5, 7, 15, 3, 5};

Attack::Attack(Circle *created_circle, Circle *target_circle, int harm_point, int velocity, ALLEGRO_BITMAP* attack_img)
{
}

Attack::~Attack()
{
    //delete circle;
    al_destroy_sample_instance(arrow);
    al_destroy_sample(sample);
}

void
Attack::Draw()
{

}

