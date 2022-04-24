#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

class Circle{
public:
    Circle() {}

    Circle(int x, int y, int r)
    {
        this->x = x;
        this->y = y;
        this->r = r;
        this->vx = 2;
        this->vy = 2;
    }

    static bool isOverlap(Circle*, Circle*);

    int r;
    int x, y;
    int vx,vy;
};



#endif // RECT_H_INCLUDED
