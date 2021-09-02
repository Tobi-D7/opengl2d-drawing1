#include "main.hpp"

using namespace std;

int main(int argc, char **argv){
    srand((uint32_t)time(NULL));
        romfsInit();
	osSetSpeedupEnable(true);

	gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);
	//gfxSet3D(true);

	pglInit();
	init();
        while (aptMainLoop())
        {
             pglSelectScreen(GFX_TOP, GFX_LEFT);
             display();
             reshape(400, 240);
             pglSwapBuffers();
        }
        pglExit();
	return 0;
}
