#include "Skybox.h"

using namespace std;

Skybox::Skybox()
{
	 glGenTextures(6, textures);
	 Textures::SetupTexture(textures[0], "assets/Teide/negx.jpg", false);
	 Textures::SetupTexture(textures[1], "assets/Teide/posy.jpg", false);
	 Textures::SetupTexture(textures[2], "assets/Teide/negz.jpg", false);
	 Textures::SetupTexture(textures[3], "assets/Teide/posx.jpg", false);
	 Textures::SetupTexture(textures[4], "assets/Teide/negy.jpg", false);
	 Textures::SetupTexture(textures[5], "assets/Teide/posz.jpg", false);
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