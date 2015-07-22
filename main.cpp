#include <stdio.h>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glaux.h>
#include <mmsystem.h>
#include <math.h>
#include "png.h"
#include "pngconf.h"
#include "zconf.h"
#include "zlib.h"
#pragma comment(lib, "winmm.lib")

#define MAX_PARTICLES 50 //snowflake
#define NUM_OF_TEXTURE 5

unsigned int texture[NUM_OF_TEXTURE];
GLubyte* pngtexture;

float fsize = 5.0f, bsize = 10.0f, psize = 10.0f, ssize = 20.0f; //size of snowflake, ball, present, star

typedef struct Vertex{
	float x, y, z;
	Vertex(){
		x = 0;
		y = 0;
		z = 0;
	}
}VERTEX;

void displayTree();

////////////////////////////////////////////////////////////////////////
// lighting setting
////////////////////////////////////////////////////////////////////////
float zeroPosition[] = {0, 0, 0, 1};

float ambient_light0[] = { 0.1f, 0.1f, 0.1f, 1.0f};
float diffuse_light0[] = { 1.0f, 1.0f, 0.0f, 1.0f};
float light_position0[] = { 100, 100, 50.0f, 1.0f};

float ambient_light1[] = { 0.1f, 0.1f, 0.1f, 1.0f};
float diffuse_light1[] = { 0.7f, 0.7f, 0.7f, 1.0f};      // 분산광
float light_position1[] = { 100, 50, 0.0f, 1.0f};          // 광원위치

float ambient_light2[] = { 0.1f, 0.1f, 0.1f, 1.0f};         // 주변광
float diffuse_light2[] = { 1.0f, 1.0f, 0.0f, 1.0f};      // 분산광
float light_position2[] = { -100, 50, 0.0f, 1.0f};          // 광원위치

float ambient_light3[] = { 0.1f, 0.1f, 0.1f, 1.0f};         // 주변광
float diffuse_light3[] = { 1.0f, 1.0f, 0.0f, 1.0f};      // 분산광
float light_position3[] = { 0, 500, 0.0f, 1.0f};          // 광원위치

float ambient_light4[] = { 0.1f, 0.1f, 0.1f, 1.0f};         // 주변광
float diffuse_light4[] = { 1.0f, 1.0f, 0.0f, 1.0f};      // 분산광
float light_position4[] = { -100, 100, 50.0f, 1.0f};          // 광원위치

float ambient_light5[] = { 0.1f, 0.1f, 0.1f, 1.0f};         // 주변광
float diffuse_light5[] = { 1.0f, 1.0f, 0.0f, 1.0f};      // 분산광
float light_position5[] = { 0, 100, 100.0f, 1.0f};          // 광원위치

float ambient_light6[] = { 0.1f, 0.1f, 0.1f, 1.0f};         // 주변광
float diffuse_light6[] = { 1.0f, 1.0f, 0.0f, 1.0f};      // 분산광
float light_position6[] = { -60, 250, 100.0f, 1.0f};          // 광원위치

float ambient_light7[] = { 0.1f, 0.1f, 0.1f, 1.0f};         // 주변광
float diffuse_light7[] = { 1.0f, 1.0f, 0.0f, 1.0f};      // 분산광
float light_position7[] = { 150, 80, 100.0f, 1.0f};          // 광원위치

//tree leaf
float leaf_ambient[]  = { 0.05f, 0.3f, 0.1f, 1.0f};		//material의 주변광
float leaf_diffuse[]  = { 0.2f, 0.5f, 0.2f, 1.0f};		//material의 확산광
float leaf_specular[] = { 0.1f, 0.3f, 0.1f, 0.5f};		//material의 반사광

//tree branch
float branch_ambient[]  = { 0.2f, 0.08f, 0.05f, 1.0f};		//material의 주변광
float branch_diffuse[]  = { 0.5f, 0.3f, 0.2f, 1.0f};		//material의 확산광
float branch_specular[] = { 0.5f, 0.2f, 0.5f, 0.5f};		//material의 반사광

//tree trunk
float mat_ambient[]  = { 0.7f, 0.6f, 0.5f, 1.0f};		//material의 주변광
float mat_diffuse[]  = { 0.6f, 0.4f, 0.2f, 1.0f};		//material의 확산광
float mat_specular[] = { 0.5f, 0.2f, 0.5f, 0.5f};		//material의 반사광

void drawPointLight(GLfloat light_color[]){
	glPushMatrix();	
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);  
		glEnable(GL_BLEND); 
		
		glMaterialfv(GL_FRONT,GL_EMISSION,light_color);
		GLUquadricObj *lightSphere = gluNewQuadric();	//sphere
		
		glColor4f(1,1,1,1);
		gluSphere(lightSphere, 3,20,20);
		
		GLfloat emissionOut[] = {0.0,0.0,0.0,0.0};
		glMaterialfv(GL_FRONT,GL_EMISSION,emissionOut);

		glDisable(GL_BLEND); 
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
	glPopMatrix();
}
void setLighting(){	
	glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );       
	glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );

	glPushMatrix();
	glTranslatef(light_position0[0],light_position0[1],light_position0[3]);
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light0 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT0, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	// LIGHT1 설정
	glTranslatef(light_position1[0],light_position1[1],light_position1[3]);
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient_light1 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse_light1 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT1, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position2[0],light_position2[1],light_position2[3]);
	glLightfv( GL_LIGHT2, GL_AMBIENT, ambient_light2 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuse_light2 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT2, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position3[0],light_position3[1],light_position3[3]);
	glLightfv( GL_LIGHT3, GL_AMBIENT, ambient_light3 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT3, GL_DIFFUSE, diffuse_light3 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT3, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position4[0],light_position4[1],light_position4[3]);
	glLightfv( GL_LIGHT4, GL_AMBIENT, ambient_light3 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT4, GL_DIFFUSE, diffuse_light3 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT4, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position5[0],light_position5[1],light_position5[3]);
	glLightfv( GL_LIGHT5, GL_AMBIENT, ambient_light3 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT5, GL_DIFFUSE, diffuse_light3 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT5, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position6[0],light_position6[1],light_position6[3]);
	glLightfv( GL_LIGHT6, GL_AMBIENT, ambient_light3 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT6, GL_DIFFUSE, diffuse_light3 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT6, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position7[0],light_position7[1],light_position7[3]);
	glLightfv( GL_LIGHT7, GL_AMBIENT, ambient_light3 );       // 주변광 성분 설정
	glLightfv( GL_LIGHT7, GL_DIFFUSE, diffuse_light3 );       // 분산광 성분 설정
	glLightfv( GL_LIGHT7, GL_POSITION, zeroPosition );     // 광원 위치 설정
	glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0);
	glPopMatrix();

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);

	glLoadIdentity();
	glPushMatrix();
	glTranslatef(light_position0[0],light_position0[1],light_position0[2]);
	drawPointLight(diffuse_light0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position1[0],light_position1[1],light_position1[2]);
	drawPointLight(diffuse_light1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position2[0],light_position2[1],light_position2[2]);
	drawPointLight(diffuse_light2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position3[0],light_position3[1],light_position3[2]);
	drawPointLight(diffuse_light3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position4[0],light_position4[1],light_position4[2]);
	drawPointLight(diffuse_light4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position5[0],light_position5[1],light_position5[2]);
	drawPointLight(diffuse_light5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(light_position6[0],light_position6[1],light_position6[2]);
	drawPointLight(diffuse_light6);
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////
// keyboard control
////////////////////////////////////////////////////////////////////////
int light[8] = {0, };
void keyPressed(unsigned char key, int x, int y) {
	switch(key){
	case '0':
		if(light[0] == 0) glEnable(GL_LIGHT0), light[0] = 1;
		else glDisable(GL_LIGHT0), light[0] = 0;
		break;
	case '1':
		if(light[1] == 0) glEnable(GL_LIGHT1), light[1] = 1;
		else glDisable(GL_LIGHT1), light[1] = 0;
		break;
	case '2':
		if(light[2] == 0) glEnable(GL_LIGHT2), light[2] = 1;
		else glDisable(GL_LIGHT2), light[2] = 0;
		break;
	case '3':
		if(light[3] == 0) glEnable(GL_LIGHT3), light[3] = 1;
		else glDisable(GL_LIGHT3), light[3] = 0;
		break;
	case '4':
		if(light[4] == 0) glEnable(GL_LIGHT4), light[4] = 1;
		else glDisable(GL_LIGHT4), light[4] = 0;
		break;
	case '5':
		if(light[5] == 0) glEnable(GL_LIGHT5), light[5] = 1;
		else glDisable(GL_LIGHT5), light[5] = 0;
		break;
	case '6':
		if(light[6] == 0) glEnable(GL_LIGHT6), light[6] = 1;
		else glDisable(GL_LIGHT6), light[6] = 0;
		break;
	case '7':
		if(light[7] == 0) glEnable(GL_LIGHT7), light[7] = 1;
		else glDisable(GL_LIGHT7), light[7] = 0;
		break;
	}
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////
// defining and generating snow
////////////////////////////////////////////////////////////////////////
typedef struct{
	VERTEX position, direction;
}PARTICLES;
PARTICLES particle[MAX_PARTICLES];
GLuint	loop;
float slowdown = 5.0f, xspeed = 0.0f, yspeed = 0.0f;
void generateSnow(){
	for(loop=0;loop<MAX_PARTICLES;loop++){
		particle[loop].position.x=float((rand()%1000)-500.0f);
		particle[loop].position.y=float((rand()%100)+250.0f);//250~350
		particle[loop].position.z=float((rand()%100)-150.0f);//-150~-50
		particle[loop].direction.x=float((rand()%200)-100.0f)/100.0f;
		particle[loop].direction.y= - float(rand()%100)/1.0f;
		particle[loop].direction.z=float((rand()%100)-50)/10.0f;//-5~5
	}
}
int drawSnow(GLvoid){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat scolor[4] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_EMISSION, scolor);
	for (loop=0;loop<MAX_PARTICLES;loop++){
		float x=particle[loop].position.x;
		float y=particle[loop].position.y;
		float z=particle[loop].position.z;
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix();
			glBegin(GL_QUADS);
					glNormal3f(0.0f, 0.0f, 1.0f); 
					glTexCoord2f(0.0f, 0.0f); glVertex3f(x - fsize, y + fsize, z);   
					glTexCoord2f(0.0f, 1.0f); glVertex3f(x - fsize, y - fsize, z);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(x + fsize, y - fsize, z);   
					glTexCoord2f(1.0f, 0.0f); glVertex3f(x + fsize, y + fsize, z); 
			glEnd();
		glPopMatrix();

		particle[loop].position.x+=particle[loop].direction.x/slowdown;
		particle[loop].position.y+=particle[loop].direction.y/slowdown;
		particle[loop].position.z+=particle[loop].direction.z/slowdown;

		if (particle[loop].position.y<-80.0f){
			particle[loop].position.x=float((rand()%1000)-500.0f);
			particle[loop].position.y=float((rand()%100)+250.0f);//250~350
			particle[loop].position.z=float((rand()%100)-150.0f);//-150~-50
		}
			glutPostRedisplay();
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// generating ornaments
////////////////////////////////////////////////////////////////////////
void generateBall(VERTEX v){
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	GLUquadric *sphere;
	sphere = gluNewQuadric();
	glPushMatrix();
		glTranslatef(v.x, v.y, v.z);
		gluSphere(sphere, bsize, 100, 100);	
	glPopMatrix();
}
void generatePresent(VERTEX v, int color){
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	if(color == 0) glBindTexture(GL_TEXTURE_2D, texture[0]);
	else glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
			glTranslatef(v.x, v.y, v.z);	
			glNormal3f(0.0f, 0.0f, 1.0f);//front
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x - psize, v.y + psize, v.z + psize);   
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x - psize, v.y - psize, v.z + psize);   
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x + psize, v.y - psize, v.z + psize);   
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x + psize, v.y + psize, v.z + psize);    
			glNormal3f(0.0f, 0.0f, -1.0f);//back
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x - psize, v.y - psize, v.z - psize); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x - psize, v.y + psize, v.z - psize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x + psize, v.y + psize, v.z - psize); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x + psize, v.y - psize, v.z - psize); 
			glNormal3f(0.0f, 1.0f, 0.0f);//top
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x - psize, v.y + psize, v.z - psize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x - psize, v.y + psize, v.z + psize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x + psize, v.y + psize, v.z + psize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x + psize, v.y + psize, v.z - psize);
			glNormal3f(0.0f, -1.0f, 0.0f);//bottom 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x - psize, v.y - psize, v.z - psize); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x + psize, v.y - psize, v.z - psize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x + psize, v.y - psize, v.z + psize); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x - psize, v.y - psize, v.z + psize);
			glNormal3f(1.0f, 0.0f, 0.0f);//right 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x + psize, v.y - psize, v.z - psize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x + psize, v.y + psize, v.z - psize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x + psize, v.y + psize, v.z + psize);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x + psize, v.y - psize, v.z + psize);
			glNormal3f(-1.0f, 0.0f, 0.0f);//left 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x - psize, v.y - psize, v.z - psize);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x - psize, v.y - psize, v.z + psize);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x - psize, v.y + psize, v.z + psize);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x - psize, v.y + psize, v.z - psize);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void generateStar(VERTEX v){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glPushMatrix();
		glBegin(GL_QUADS);
		glTranslatef(v.x, v.y, v.z);	
		glColor4f(1.0, 1.0, 1.0, 1.0);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(v.x - ssize, v.y + ssize, v.z + ssize);   
			glTexCoord2f(0.0f, 1.0f); glVertex3f(v.x - ssize, v.y - ssize, v.z + ssize);   
			glTexCoord2f(1.0f, 1.0f); glVertex3f(v.x + ssize, v.y - ssize, v.z + ssize);   
			glTexCoord2f(1.0f, 0.0f); glVertex3f(v.x + ssize, v.y + ssize, v.z + ssize);  
		glEnd();
	glPopMatrix();	
	glDisable(GL_TEXTURE_2D);
}
void generateBackground(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glBegin(GL_QUADS);
		float w = 700, h = 500, background = -300.0f;
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-w, h, background);   
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-w, -h, background);   
		glTexCoord2f(1.0f, 1.0f); glVertex3f(w, -h, background);   
		glTexCoord2f(1.0f, 0.0f); glVertex3f(w, h, background); 
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

////////////////////////////////////////////////////////////////////////
// loading image files
////////////////////////////////////////////////////////////////////////
AUX_RGBImageRec *LoadBMP(char *filename){
	FILE *f=NULL;
	if(!filename) return NULL;

	f=fopen(filename, "r");
	if(f){
		fclose(f);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}
int LoadGLTextures(){
	AUX_RGBImageRec *TextureImage[NUM_OF_TEXTURE];
	memset(TextureImage, 0, sizeof(void*)*NUM_OF_TEXTURE);

	if((TextureImage[0]=LoadBMP("box.bmp")) && (TextureImage[1]=LoadBMP("snowflake.bmp")) && (TextureImage[2]=LoadBMP("star.bmp")) && (TextureImage[3]=LoadBMP("background.bmp")) && (TextureImage[4]=LoadBMP("box2.bmp"))){
		printf("true");
		glGenTextures(NUM_OF_TEXTURE, &texture[0]);

		for(int i=0;i<NUM_OF_TEXTURE;i++){
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			//no mipmap
			//glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//apply mipmap
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
	}else return FALSE;

	for(int i=0;i<NUM_OF_TEXTURE;i++){
		if(TextureImage[i]){
			if(TextureImage[i]->data) free(TextureImage[i]->data);
			free(TextureImage[i]);
		}
	}
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);   
    //glEnable(GL_BLEND); 
    //glDisable(GL_DEPTH_TEST);
	return TRUE;
}
bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return false;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
 
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
 
    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
 
    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }
 
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);
 
    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < outHeight; i++) {
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
 
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return true;
}
int LoadPNGTextures(){
	return 0;
}

////////////////////////////////////////////////////////////////////////
// generating and drawing tree
////////////////////////////////////////////////////////////////////////
#define max_tree_height 200
long leafIndex = 0, clyIndex = 0;

struct Metrix{
	float met[16];
};
Metrix *pl = new Metrix [1000000];
Metrix *cly = new Metrix [1000000];
Metrix  *leaf = new Metrix [1000000];

float randf(){ //0~1
	return (float)rand()/RAND_MAX;
}
float treeMatrix[4];
float fovy = 60, trunk_radius = 5, trunk_height = 80, trunk_slices = 20, trunk_stacks = 2, branch_radius = 4, tree_height = 200;
int tree_level = 3, branch_level = 2, branch_pos = 1, branch_left = (int)(360/branch_level), branch_down = 20, leaf_term = 20;
float branch_scale[3] = {0.6, 0.6, 0.6};

GLUquadricObj *cylinder;

void drawLand(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glColor4f(0.9, 0.8, 0.9, 1.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
	glPopMatrix();
}
void drawBranch(int current_level){
	GLUquadricObj *cylinder = gluNewQuadric();	//cylinder을 위한 object
	glPushMatrix();
			if(current_level >= tree_level-2){
				glGetFloatv(GL_MODELVIEW_MATRIX,leaf[leafIndex].met);
				leafIndex++;
				//glColor3fv(color_leap);	//나뭇가지 끝부분은 초록색으로 그림
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
				glPushMatrix();
				
				if(randf() > 0.9){
					GLfloat emission_color[] = {0,0,1,1};
					glGetFloatv(GL_MODELVIEW_MATRIX,cly[clyIndex].met);
				
				//	drawPointLight(emission_color);
				}
				else if(randf() > 0.8){
					GLfloat emission_color_linghting[] = {1,0,0,1};
				//	drawPointLight(emission_color_linghting);
				}
				glBegin(GL_LINES);

				glMaterialfv( GL_FRONT, GL_AMBIENT, leaf_ambient );       
				glMaterialfv( GL_FRONT, GL_DIFFUSE, leaf_diffuse );
				glMaterialfv( GL_FRONT, GL_SPECULAR, leaf_specular );
				//방사형으로 퍼져나가는 작은 잎들

				
				for(int leaf_height=0; leaf_height < trunk_height; leaf_height+=leaf_term){
					for(int leaf_angle=0; leaf_angle< 360; leaf_angle+=30){
					
						glVertex3f(0,leaf_height,0);
						glVertex3f(10*cos((float)leaf_angle)+randf(),leaf_height+4*randf(),10*sin((float)leaf_angle)+randf());
					}
				}
				glEnd();
				glPopMatrix();
				glScalef(0.5,1,0.5);
			}
			glPushMatrix();	
				glMaterialfv( GL_FRONT, GL_AMBIENT, branch_ambient );       
				glMaterialfv( GL_FRONT, GL_DIFFUSE, branch_diffuse );
				glMaterialfv( GL_FRONT, GL_SPECULAR, branch_specular );
				
				//Metrix tmpMet = {{0,1,0,0},{-1,0,0,0},{
				glRotatef(-90,1,0,0);
				glGetFloatv(GL_MODELVIEW_MATRIX,cly[clyIndex].met);
				
				clyIndex++;
				gluCylinder(cylinder, 5, 2, trunk_height, 5, 1);
			glPopMatrix();
	glPopMatrix();
}
void reTree(int current_level){
	float pos;
	int rotate;
	

	drawBranch(current_level);

	if(current_level <= tree_level){ 
		
		float currunt_branch_pos = branch_pos;
		for(int index_branch_pos = 0; index_branch_pos < currunt_branch_pos; index_branch_pos++) { // 가지가 몇 단계(위아래)를 가지는가
				for(int index_branch_level = 0; index_branch_level< branch_level; index_branch_level++) { // 가지가 몇개(1단계에)가 있는가, +-1 
					glPushMatrix();
						// 가지를 붙일 위치를 정함(중간부분 이상에서 시작)
						pos  = (float)(trunk_height * (index_branch_pos / currunt_branch_pos));
						//pos /= (float)(branch_pos/3 * randf() +0.05);
						pos  = (trunk_height - pos)*(0.5 + 0.3*randf());
						
						rotate = (int)((branch_left * index_branch_pos) / (branch_level + randf()));
						// 가지를 
						glTranslatef(0.0, pos, 0.0);
						// 가지를 붙일 때의 각도를 정함
						
						glRotatef(rotate + branch_left * index_branch_level + randf()*35, 0.0, 1.0, 0.0);
						glRotatef(branch_down + randf()*10, 0.0, 0.0, 1.0);
						
						// 가지의 크기를 줄임
						glScalef(branch_scale[0], branch_scale[1], branch_scale[2]);
						// 가지를 다시 그림(재귀로)
						reTree(current_level + 1);

					glPopMatrix();
				}
		}
	}
	else return;

}
void drawTree(){
	
	glPushMatrix();
		glRotated(-90, 1,0.0,0);
		
			 //가지에 대한 재질을 설정
			glMaterialfv( GL_FRONT, GL_AMBIENT, branch_ambient );       
			glMaterialfv( GL_FRONT, GL_DIFFUSE, branch_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, branch_specular );
			
		glutSolidCone(7, tree_height, trunk_slices, trunk_stacks);
	glPopMatrix();
	
	float branch_height= tree_height*0.2;
	float branch_angle=0 ;
	while(branch_height < tree_height){
			glPushMatrix();
			float per_height = branch_height/tree_height;
			//tree가 자라는과정을 위해 다시 scale
			glTranslatef(0,branch_height,0);
			
			
			
			//높아질 수록 가지를 작게함
			float scaleHigh = (1-per_height);
			glScalef(scaleHigh*0.8,scaleHigh,scaleHigh);
			branch_angle = branch_angle + 30 + (randf())*10;
			//큰 가지의 각도를 정함
			glRotated(branch_angle,0,1,0);
			glRotated(100-10*abs(randf())*per_height,0,0,1);
			reTree(0);
			glPopMatrix();
			//높아질수록 큰 가지가 천천히 올라가도록
			branch_height += randf()*(1.005-per_height)* tree_height*0.05;
	}

	glPopMatrix();

	glBegin(GL_POINTS);
		generateSnow();
	glEnd();
}

////////////////////////////////////////////////////////////////////////
// basic functions
////////////////////////////////////////////////////////////////////////
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pngtexture[0]);
		glPushMatrix();
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0, 0.0), glVertex3f(-200.0, -100.0, 0.0);
			glTexCoord2f(0.0, 1.0), glVertex3f(-200.0, 100.0, 0.0);
			glTexCoord2f(1.0, 1.0), glVertex3f(0.0, 100.0, 0.0);
			glTexCoord2f(1.0, 0.0), glVertex3f(0.0, -100.0, 0.0);
		glEnd();
		glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//drawSnow();
	generateBackground();	

	//drawing ornaments
	VERTEX present[3];
	VERTEX star;
	present[0].x = -80.0, present[0].y = -30.0, present[0].z = -50.0;
	present[1].x = 50.0, present[1].y = -40.0, present[1].z = 50.0;
	present[2].x = 70.0, present[2].y = -40.0, present[2].z = 0.0;
	star.x = 0.0, star.y = 180.0, star.z = -50.0;

	generatePresent(present[0], 1);
	generatePresent(present[1], 0);
	generatePresent(present[2], 0);
	generateStar(star);

	int angle = 0;
	for(int i=0; i< 200; i+=5){
			angle+=30;
			glTranslatef(50*cos((float)angle),(float)i,50*sin((float)angle));
			GLfloat emission_color[] = {0,0,1,1};
			drawPointLight(emission_color);
	}

	//displayTree();
	glutSwapBuffers();
}
void reshape(int w, int h){
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60,(GLfloat)w/(GLfloat)h,0.1,1000.0);
	gluLookAt(0,50,300,0,50,-1,0,1,0);
	glutDisplayFunc(display);
	glMatrixMode(GL_MODELVIEW);
}
int isInitial = 1;
void displayTree(){	
	if(isInitial == 1){		
		glLoadIdentity();	
		glTranslatef(0,-50,0);
		drawTree();
		drawLand();
		isInitial = 0;
	}
	else{
		GLUquadricObj *cylinder = gluNewQuadric();
		glLoadIdentity();
		glTranslatef(0,-50,0);
		glMaterialfv( GL_FRONT, GL_AMBIENT, branch_ambient );       
		glMaterialfv( GL_FRONT, GL_DIFFUSE, branch_diffuse );
		glMaterialfv( GL_FRONT, GL_SPECULAR, branch_specular );
		glPushMatrix();
			glRotated(-90, 1,0.0,0);
			glMaterialfv( GL_FRONT, GL_AMBIENT, branch_ambient );       
			glMaterialfv( GL_FRONT, GL_DIFFUSE, branch_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, branch_specular );
			glutSolidCone(7, tree_height, trunk_slices, trunk_stacks);
		glPopMatrix();
		for(int current_cly_index=0; current_cly_index< clyIndex; current_cly_index++){
				glLoadMatrixf(cly[current_cly_index].met);	
				gluCylinder(cylinder, 5, 2, trunk_height, 5, 1);
		}
		glLoadIdentity();
		glMaterialfv( GL_FRONT, GL_AMBIENT, leaf_ambient );       
		glMaterialfv( GL_FRONT, GL_DIFFUSE, leaf_diffuse );
		glMaterialfv( GL_FRONT, GL_SPECULAR, leaf_specular );
		
		for(int current_leaf_index=0; current_leaf_index < leafIndex; current_leaf_index++){
			glLoadMatrixf(leaf[current_leaf_index].met);			
			glBegin(GL_LINES);
				for(int leaf_height=0; leaf_height < trunk_height; leaf_height+=leaf_term){
					for(int leaf_angle=0; leaf_angle< 360; leaf_angle+=30){
						glVertex3f(0,leaf_height,0);
						glVertex3f(10*cos((float)leaf_angle),leaf_height,10*sin((float)leaf_angle));
					}
				}
			glEnd();
		}
	}
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Merry Christmas");

	if(!LoadGLTextures()) return false;
	int width, height;
	bool hasAlpha;
	if(!loadPngImage("bow2.png", width, height, hasAlpha, &pngtexture)) return false;
	//if(!LoadPNGTextures()) return false;
	glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pngtexture);


	PlaySound("bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	setLighting();
	generateSnow();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING); 

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutKeyboardFunc(keyPressed);

	glutMainLoop();

	return 0;
}
