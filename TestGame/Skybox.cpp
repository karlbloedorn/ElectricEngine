#include "Skybox.h"

using namespace std;

Skybox::Skybox()
{
	 glGenTextures(6, textures);

	 Textures::SetupTexture(textures[0], "assets/textures/skybox/cloud/negx.jpg", false);
	 Textures::SetupTexture(textures[1], "assets/textures/skybox/cloud/posy.jpg", false);
	 Textures::SetupTexture(textures[2], "assets/textures/skybox/cloud/negz.jpg", false);
	 Textures::SetupTexture(textures[3], "assets/textures/skybox/cloud/posx.jpg", false);
	 Textures::SetupTexture(textures[4], "assets/textures/skybox/cloud/negy.jpg", false);
	 Textures::SetupTexture(textures[5], "assets/textures/skybox/cloud/posz.jpg", false);
}


Skybox::~Skybox()
{

}

void Skybox::Render()
{
	for (int i = 0; i < 6; i++){
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glBegin(GL_QUADS);

		switch (i){
		case 0:
			//negx 0
		  glTexCoord2f(0, 0);
			glVertex3f(-800, 800, -800);
			glTexCoord2f(1, 0);
			glVertex3f(-800, 800, 800);
			glTexCoord2f(1, 1);
			glVertex3f(-800, -800, 800);
			glTexCoord2f(0, 1);
			glVertex3f(-800, -800, -800);
			break;
		case 1:
			//negy 1
			glTexCoord2f(1, 0);
			glVertex3f(800, 800, -800);
			glTexCoord2f(1, 1);
			glVertex3f(800, 800, 800);
			glTexCoord2f(0, 1);
			glVertex3f(-800, 800, 800);
			glTexCoord2f(0, 0);
			glVertex3f(-800, 800, -800);

			break;
		case 2:
			//negz 2
			glTexCoord2f(0, 1);
			glVertex3f(800, -800, -800);
			glTexCoord2f(0, 0);
			glVertex3f(800, 800, -800);
			glTexCoord2f(1, 0);
			glVertex3f(-800, 800, -800);
			glTexCoord2f(1, 1);
			glVertex3f(-800, -800, -800); 
			break;
		case 3:
			//posx 3
			glTexCoord2f(1, 1);
			glVertex3f(800, -800, -800);
			glTexCoord2f(0, 1);
			glVertex3f(800, -800, 800);
			glTexCoord2f(0, 0);
			glVertex3f(800, 800, 800);
			glTexCoord2f(1, 0);
			glVertex3f(800, 800, -800);
			break;
		case 4:
			//posy 4
			glTexCoord2f(0, 1);
			glVertex3f(-800, -800, -800);
			glTexCoord2f(0, 0);
			glVertex3f(-800, -800, 800);
			glTexCoord2f(1, 0);
			glVertex3f(800, -800, 800);
			glTexCoord2f(1, 1);
			glVertex3f(800, -800, -800);
			break;
		case 5:
			//posz 5
			
			 glTexCoord2f(1, 0);
			glVertex3f(800, 800, 800);
			  glTexCoord2f(1, 1);
			glVertex3f(800, -800, 800);
			  glTexCoord2f(0, 1);
			glVertex3f(-800, -800, 800);
			glTexCoord2f(0, 0);
			glVertex3f(-800, 800, 800);
			break;
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}