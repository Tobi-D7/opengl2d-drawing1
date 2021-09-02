#include "main.hpp"

using namespace std;

int main(int argc, char **argv){
    srand((uint32_t)time(NULL));

	pglInit();
	init();
        while (aptMainLoop())
        {
             pglSelectScreen(GFX_TOP, GFX_LEFT);
             display();
             reshape();
             pglSwapBuffers();
        }
        pglExit();
	return 0;
}
