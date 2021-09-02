#include "main.hpp"

using namespace std;

int main(int argc, char **argv){
    srand((uint32_t)time(NULL));
        romfsInit();
	osSetSpeedupEnable(true);

	gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);
	//gfxSet3D(true);
        printf("1");
	pglInit();
        printf("2");
	init();
        reshape(400, 240);
        printf("3");
        while (aptMainLoop())
        {
             printf("4");
             pglSelectScreen(GFX_TOP, GFX_LEFT);
             printf("5");
             display();
             printf("6");
             
             printf("7");
             pglSwapBuffers();
        }
        pglExit();
	return 0;
}
