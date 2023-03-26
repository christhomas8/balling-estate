#include "gameclass.h"
#include "logger.h"



int main(int argc, char * argv[])
{
    std::cout << "Running.." << std::endl;
    //Logging file object
    Logger test;
    test.logtime('o');

    //Main mame object
    int height = 600;
    int width = 800;
    Balling app(height, width);
    app.run_game(width, height);

    //Log end time
    test.logtime('c');

   return 1;
}
