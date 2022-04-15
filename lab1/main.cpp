#include "main.hpp"

using namespace std;

int main(int argc, char **argv){
    srand((uint32_t)time(NULL));
        romfsInit();
	osSetSpeedupEnable(true);

	//gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);
        gfxInitDefault();
	//gfxSet3D(true);
        printf("1");
	pglInit();
        pglSelectScreen(GFX_TOP, GFX_LEFT);
        glClearColor(0.4f, 0.1f, 0.9f, 1.0f);
	glViewport(0,0, 400, 240);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthFunc(GL_LESS);

	//glEnable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);

	//glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(80.0f, 400.f/240.0f, 0.01f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

        printf("2");
	//init();
        
        
        printf("3");
        while (aptMainLoop())
        {
             printf("4");
             gspWaitForVBlank();

		hidScanInput();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

             printf("5");
             //display();
             //reshape(400, 240);
             printf("6");
             glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POLYGON);
    		glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
    		glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
    		glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
  		glEnd();

  		// Flush drawing command buffer to make drawing happen as soon as possible.
 	 	glFlush();
             
             	pglSwapBuffers();

		if (hidKeysDown() & KEY_START)
			break;
	}

	pglExit();
	gfxExit();
	
	return 0;
}
