#include "movement.h"


int main(int argc, char * argv[])
{

    // Creating the object by passing Height and Width value.
    int height = 600;
    int width = 800;

    Balling fw(height, width);

    fw.move_ball(width, height);

   return 1;
}
