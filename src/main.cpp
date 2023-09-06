#include "app.h"


void MainLoop();


//App app(1440, 1080);
App app(720, 540);


int main()
{
    app.Create();

    emscripten_set_main_loop(MainLoop, 0, 0);

    return 0;
}



void MainLoop()
{
    app.Update();
}
