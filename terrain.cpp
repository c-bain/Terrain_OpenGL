//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

void init();//initializes functions and a few variables 
void circleAlg();//performs the circle algorithm
void drawWire();//draws the wireframe without the polygon 
void resetPoints();//makes the grid flat 
void circleAlg();//performs the circle algorithm 
void faultAlg();//performs the fault algorithm 
void midAlg();//performs the midpoint algorithm 
void menuOptions();//opens the menu 
void normalVecFace();//computes the normals of each polygon face for use in Flat Shading 
void normalVecVertex();//computes the normals of each polygon vertex for use in Gourard shading
float characterHeight(int x, int z);//finds the height of the character given x and y 
void calcColour(float height);

float heightMap[300][300];//heightmap of the terrain 
float normalFaces[300][300][3];//array of face normals
float normalVertices[300][300][3];//aray of vertex normals
int circleRadius;//radius of each circle
float terrColour[3];//colour of the terrain 
float faultColour=0.0f;//adds extra colours for the fault terrain 
int sizeX=0;// length of the grid 
int sizeY=0;//height of the grid 
int numTerrPoints;//total number of terrain points
int randX;//random x val for circle
int randZ;//random y val for circle
int swidth=1000;//window size
int sheight=800;//window size 
int wToggle=0;//wireframe toggle
bool flatShade=false;//default shading is flat. Initally false so when light is turned on, it becomes true
bool triStrips=false;//terrain type. false represents quad strips and true represents triangle strips
bool faultMode=false;//fault mode status
bool lightOn=false;//light status
bool enhancedColour=false;//improved map colouring mode. Initially false 
float minHeight;//minimum height of the terrain 
float maxHeight;//maximum height of the terrain 
int charX=0;//x coordinate of the character 
float charY=0.0f;//y coordinate of the character 
int charZ=0;//z coordinate of the character 
float charDisp=1.0;//character displacement.
float camPos[] = {0, 0, 0};	//default camera position 
float camUp[] = {0,1,0};//up vector of the camera
float camTarget[] = {0,0,0};	//where the camera is looking at
float camSpeed = 2.0f;//camera speed 
float charSpeed = 0.2f;//character speed
float lightSpeed=2.0f;//light speed
float pos[] = {0,1,0};//character position
float rot[] = {0, 0, 0};//rotation of the character
float headRot[] = {0, 0, 0};//head rotation of the character 
float lightColour0[] = {1.0, 0.647, 0,1};//simulates global illumination //float lightColour0[] = {1, 0, 0,1};//simulates global illumination 
float lightColour1[] = {1.0, 0.647, 0,1};//simulates global illumination //float lightColour0[] = {1, 0, 0,1};//simulates global illumination 
float m_dif[] = {1, 1, 1, 1};//way light falls off of an object
float m_spec[] = {1, 1, 1, 1};//shininess of an object(tells how hard an object may be)
float shiny = 20;//shininess variable 
float lightPos0[4] = {57, 94, 85, 1};//define light0 pos
float lightPos1[4] = {57, 94, 0, 1};//define light1 pos at opposite of light0

void special(int key, int x, int y){//handes keypresses for the up,down, left, right arrows 
	//printf("\n%f",camPos[0]);printf("\n%f",camPos[1]);printf("\n%f\n",camPos[2]);
	switch (key){
	case GLUT_KEY_UP:camPos[0] += camSpeed;//increase x axis 
		break;
	case GLUT_KEY_DOWN:camPos[0] -= camSpeed;//decrease x axis 
		break;
	case GLUT_KEY_LEFT:camPos[1] -= camSpeed;//decrease y axis 
		break;
	case GLUT_KEY_RIGHT:camPos[1] += camSpeed;//increase y axis 
		break;	
	}
}


void drawLight0(){// draws the first light 
	glPushMatrix();
	glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
	glColor3f(1, 1, 0);//yellow
	glutSolidSphere(5, 10, 10);
	glPopMatrix();
}

void drawLight1(){//draws the second light 
	glPushMatrix();
	glTranslatef(lightPos1[0], lightPos1[1], lightPos1[2]);
	glColor3f(1, 1, 0);//yellow
	glutSolidSphere(5, 10, 10);
	glPopMatrix();
}

void redrawChar(){//redraws the snowman
	charY=characterHeight(pos[0],pos[2]);//get character height from X and Y coords
	pos[0]=charX;pos[1]=charY+0.5;pos[2]=charZ;//pos[1]=;//set character position
}

void keyboard(unsigned char key, int xIn, int yIn) //handles key presses for menu options 
{
	int mod = glutGetModifiers();
	switch (key)
	{
		case 27:	//27 is the esc key
			exit(0);
			break;

		case 'W'://draws only the wireframe 
		case 'w':
			printf("\n\n\n%s\n","**********Toggling Wireframe**********");
			wToggle++;
			if(wToggle>2)wToggle=0;//reset
			menuOptions();//dispay menu options 
			break;

		case 'R'://creates a new terrain 
		case 'r':
			printf("\n\n\n%s\n","**********Generating new terrain**********");
			resetPoints();
			if(!faultMode)circleAlg();
			else faultAlg();
			menuOptions();
			break;

		case 'S'://does flat shading 
		case 's':
		flatShade=!flatShade;
			if(flatShade){//Flat Shading //face normals 
				printf("\n\n\n%s\n","**********Flat Shading (Default)**********");
				glShadeModel(GL_FLAT);
			}
			else if (!flatShade){//Gourard Shading //vertex normals 
				glShadeModel(GL_SMOOTH);
				printf("\n\n\n%s\n","**********Gourard Shading**********");
			}
			menuOptions();
			break;

		case 'T'://does triangle strips 
		case 't':
		triStrips=true;//triangle strips
		printf("\n\n\n%s\n","**********Terrain generated with Triangle Strips **********");
		menuOptions();
		break;

		case 'Y'://does quad strips 
		case 'y':
		triStrips=false;//quad strips
		printf("\n\n\n%s\n","**********Terrain generated with Quad Strips**********");
		menuOptions();
		break;


		case 'F'://terrain generated using fault algorithm 
		case 'f':
		faultMode=!faultMode;
		if(faultMode){
			printf("\n\n\n%s\n","**********Terrain generated with Fault Algorithm**********");
			resetPoints();
			faultColour=fabs(faultColour);//5.0f;//adds extra colour for the fault Terrain. Without this, low(deep) height values are too black 
			faultAlg();
		}
		else {
			printf("\n\n\n%s\n","**********Terrain generated with Circle Algorithm**********");
			resetPoints();
			circleAlg();
		}
		menuOptions();
		break;

		case 'h'://character moves left 
		case 'H':
			if(pos[0] > -sizeX+2.5)//prevents character falling off terrain 
				pos[0]-=charSpeed;
				charY=characterHeight(pos[0],pos[2]);//get character height from X and Y coords
				pos[1]=charY+=charDisp;
			rot[1] = -90;
			break;

		case 'u'://character moves up
		case 'U':
			if(pos[2] > -sizeX+2.5)
				pos[2]-=charSpeed;
				charY=characterHeight(pos[0],pos[2]);//get character height from X and Y coords
				pos[1]=charY+=charDisp;
			rot[1] = 180;
			break;

		case 'k'://character moves right
		case 'K':
			if(pos[0] < sizeX-2.5)
				pos[0]+=charSpeed;
				charY=characterHeight(pos[0],pos[2]);//get character height from X and Y coords
				pos[1]=charY+=charDisp;
			rot[1] = 90;
			break;

		case 'j'://character moves down
		case 'J':
			if(pos[2] < sizeX-2.5)
				pos[2]+=charSpeed;
				charY=characterHeight(pos[0],pos[2]);//get character height from X and Y coords
				pos[1]=charY+=charDisp;
			rot[1] = 0;
			break;

		case 'l'://light is toggled on or off
		case 'L':
			printf("%s\n","Toggle light" );
			lightOn=!lightOn;
			if(lightOn){//if user turned on the light 
				printf("%s\n","It's nighttime" );
				glEnable(GL_LIGHTING); //enable lighting
				glEnable(GL_LIGHT0);//turn on light bulb 0
				glEnable(GL_LIGHT1);//turn on light bulb 0

			}
			else{//if user turns off the light 
				printf("%s\n","It's daytime" );
				glDisable(GL_LIGHTING); 
				glDisable(GL_LIGHT0);//turn off light bulb 0
				glDisable(GL_LIGHT1);//turn off light bulb 1
			}
			break;

		case 'p'://improved map colouring
		case 'P':
			enhancedColour=!enhancedColour;
			if(enhancedColour)printf("\n\n\n%s\n","**********Enhanced map colouring turned on**********");
			else printf("\n\n\n%s\n","**********Enhanced map colouring turned off**********");
			break;

		case 'c'://light0 moves left
		case 'C':
			lightPos0[0]-=lightSpeed;
			break;

		case 'g'://light0 moves up
		case 'G':
			lightPos0[2]-=lightSpeed;
			break;

		case 'b'://light0 moves right
		case 'B':
			lightPos0[0]+=lightSpeed;
			break;
			
		case 'v'://light0 moves down
		case 'V':
			lightPos0[2]+=lightSpeed;
			break;

		case 'z'://light1 moves left 
		case 'Z':
			lightPos1[0]-=lightSpeed;
			break;

		case 'i'://light1 moves up
		case 'I':
			lightPos1[2]-=lightSpeed;
			break;

		case 'x'://light1 moves right
		case 'X':
			lightPos1[0]+=lightSpeed;
			break;

		case 'm'://light1 moves down
		case 'M':
			lightPos1[2]+=lightSpeed;
			break;
		
	}
}

void setNumTerrPoints(){// calculated from the x and z size of the terrain
	numTerrPoints=((sizeX+sizeY)/3)*4;//sweet spot 
}

void init(){
	if(sizeX==50){//camera, window size changes depending on terrain sizes 
		camPos[0]=91;camPos[1]=92;camPos[2]=85;
	}
	else if(sizeX>50 && sizeX<=60){//camera, window size changes depending on terrain sizes 
		camPos[0]=85;camPos[1]=140;camPos[2]=85;
	}
	else if(sizeX>60 && sizeX<=100){//camera, window size changes depending on terrain sizes 
		glutReshapeWindow(1200,900); 
		camTarget[0]+=60;camTarget[1]+=50;camTarget[2]+=60;
		camPos[0]=133;camPos[1]=180;camPos[2]=85;
	}
	else if(sizeX>100 && sizeX<=200){//camera, window size changes depending on terrain sizes 
		glutReshapeWindow(1200,900);//w,h//increase window size to account for bigger terrain 
		camTarget[0]+=60;camTarget[1]+=50;camTarget[2]+=60;
		camPos[0]=-17;camPos[1]=90;camPos[2]=85;
	}
	else if(sizeX>200 && sizeX<=300){//camera, window size changes depending on terrain sizes 
		glutReshapeWindow(1200,900);//increase window size to account for bigger terrain 
		camTarget[0]+=60;camTarget[1]+=50;camTarget[2]+=60;
		camPos[0]=253;camPos[1]=86;camPos[2]=85;
		camSpeed+=3;
	}
	setNumTerrPoints();//sets the number of terrain points(used in loop)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,1,1200);//value 1200 prevents pop in 
}

void normalVecVertex(){//Reference: lighthouse 3D Website;creates normalized vectors per vertex for use in gourard(smooth) shading
	float vec1[3];float vec2[3];float vec3[3];float vec4[3];
	float crossP12[3];float crossP23[3];float crossP34[3];float crossP41[3];
	float lenVec12=0.0f;float lenVec23=0.0f;float lenVec34=0.0f;float lenVec41=0.0f;
	float totalX;float totalY;float totalZ;float sum;
	for(int i=1;i<sizeX-1;i++){
		for(int j=1;j<sizeY-1;j++){
			vec1[0] = 0;//create a vec1 vector to be cross producted with vec2
			vec1[1] = heightMap[i][j-1]-heightMap[i][j];//moves across by 1 on the x axis,get y value there
			vec1[2] = -1;

			vec2[0] = 1;//create a vec2 vector to be cross producted with vec1
			vec2[1] = heightMap[i+1][j]-heightMap[i][j];
			vec2[2] = 0;


			vec3[0] = 0;//create a vec2 vector to be cross producted with vec1
			vec3[1] = heightMap[i][j+1]-heightMap[i][j];
			vec3[2] = 1;

			vec4[0] = -1;//create a vec2 vector to be cross producted with vec1
			vec4[1] = heightMap[i-1][j]-heightMap[i][j];
			vec4[2] = 0;

			crossP12[0]=vec1[1]*vec2[2] - vec1[2]*vec2[1];//cross product calculation of v1 and v2
			crossP12[1]=vec1[2]*vec2[0] - vec1[0]*vec2[2];
			crossP12[2]=vec1[0]*vec2[1] - vec1[1]*vec2[0];
			lenVec12=sqrtf(pow(crossP12[0],2) + pow(crossP12[1],2) + pow(crossP12[2],2));//normalize vector(make unit length)

			crossP23[0]=vec2[1]*vec3[2] - vec2[2]*vec3[1];//cross product calculation of v2 and v3
			crossP23[1]=vec2[2]*vec3[0] - vec2[0]*vec3[2];
			crossP23[2]=vec2[0]*vec3[1] - vec2[1]*vec3[0];
			lenVec23=sqrtf(pow(crossP23[0],2) + pow(crossP23[1],2) + pow(crossP23[2],2));//normalize vector(make unit length)


			crossP34[0]=vec3[1]*vec4[2] - vec3[2]*vec4[1];//cross product calculation of v3 and v4
			crossP34[1]=vec3[2]*vec4[0] - vec3[0]*vec4[2];
			crossP34[2]=vec3[0]*vec4[1] - vec3[1]*vec4[0];
			lenVec34=sqrtf(pow(crossP34[0],2) + pow(crossP34[1],2) + pow(crossP34[2],2));//normalize vector(make unit length)


			crossP41[0]=vec4[1]*vec1[2] - vec4[2]*vec1[1];//cross product calculation of v4 and v1
			crossP41[1]=vec4[2]*vec1[0] - vec4[0]*vec1[2];
			crossP41[2]=vec4[0]*vec1[1] - vec4[1]*vec1[0];
			lenVec41=sqrtf(pow(crossP41[0],2) + pow(crossP41[1],2) + pow(crossP41[2],2));//normalize vector(make unit length)

			crossP12[0]/=lenVec12;crossP12[1]/=lenVec12;crossP12[2]/=lenVec12;
			crossP23[0]/=lenVec23;crossP23[1]/=lenVec23;crossP23[2]/=lenVec23;
			crossP34[0]/=lenVec34;crossP34[1]/=lenVec34;crossP34[2]/=lenVec34;
			crossP41[0]/=lenVec41;crossP41[1]/=lenVec41;crossP41[2]/=lenVec41;

			totalX=crossP12[0]+crossP23[0]+crossP34[0]+crossP41[0];//sum(v's)
			totalY=crossP12[1]+crossP23[1]+crossP34[1]+crossP41[1];
			totalZ=crossP12[2]+crossP23[2]+crossP34[2]+crossP41[2];

			sum=sqrtf(pow(totalX,2) + pow(totalY,2) + pow(totalZ,2));//normalize vector(make unit length)
			normalVertices[i][j][0]=totalX/sum;
			normalVertices[i][j][1]=totalY/sum;
			normalVertices[i][j][2]=totalZ/sum;
		}
	}
}

void normalVecFace(){//Creates normalized vectors per face for use in flat shading  
	float vec1[3];
	float vec2[3];
	float vec3[3];
	float crossP[3];//cross product 
	float lenVec=0.0f;
	for(int i=1;i<sizeX-1;i++){
		for(int j=1;j<sizeY-1;j++){
			vec1[0] = i;//create a vec1 vector to be cross producted with vec2
			vec1[1] = heightMap[i][j];//moves across by 1 on the x axis,get y value there
			vec1[2] = j;

			vec2[0] = i+1;//create a vec2 vector to be cross producted with vec1
			vec2[1] = heightMap[i+1][j];
			vec2[2] = j;

			vec3[0] = i;//create a vec2 vector to be cross producted with vec1
			vec3[1] = heightMap[i][j+1];
			vec3[2] = j+1;

			crossP[0]=vec1[1]*vec2[2] - vec1[2]*vec2[1];//cross product calculation of v1 and v2
			crossP[1]=vec1[2]*vec2[0] - vec1[0]*vec2[2];
			crossP[2]=vec1[0]*vec2[1] - vec1[1]*vec2[0];
			lenVec=sqrtf(pow(crossP[0],2) + pow(crossP[1],2) + pow(crossP[2],2));//normalize vector(make unit length)
			normalFaces[i+1][j][0]=crossP[0]/lenVec;//normalized vector nv calculated 
			normalFaces[i+1][j][1]=crossP[1]/lenVec;
			normalFaces[i+1][j][2]=crossP[2]/lenVec;

			crossP[0]=vec2[1]*vec3[2] - vec2[2]*vec3[1];//cross product calculation of v2 and v3
			crossP[1]=vec2[2]*vec3[0] - vec2[0]*vec3[2];
			crossP[2]=vec2[0]*vec3[1] - vec2[1]*vec3[0];
			lenVec=sqrtf(pow(crossP[0],2) + pow(crossP[1],2) + pow(crossP[2],2));//normalize vector(make unit length)
			normalFaces[i][j+1][0]=crossP[0]/lenVec;//normalized vector nv calculated 
			normalFaces[i][j+1][1]=crossP[1]/lenVec;
			normalFaces[i][j+1][2]=crossP[2]/lenVec;

		}
	}
}

void calcHeight(){//calculates the minimum and maximum height of the map
	for(int i=0;i<sizeX;i++){
		for(int j=0;j<sizeY;j++){
			if(heightMap[i][j]<minHeight) minHeight=heightMap[i][j];
			else if(heightMap[i][j]>maxHeight) maxHeight=heightMap[i][j];
		}
	}
}

void resetPoints(){//zeroes out the entire terrain(makes it flat) in preparation for it to be generated again 
	int i =0;
	while(i<numTerrPoints){
		for(int j=0;j<sizeX;j++){
			for(int k=0;k<sizeY;k++){
					heightMap[j][k]*=0.0;
			}
		}
		i++;
	}	
}

float characterHeight(int x, int z){//find the height respective of coords x and z for character placement
	for(int i=0;i<sizeX;i++){
		for(int j=0;j<sizeY;j++){
			if(heightMap[i][j]==heightMap[x][z]){
				return heightMap[i][j];
			}
		}
	}
}

void character(){//Reference: Avenue lecture code; Creates the snowman character which can move around the terrain through user input.
	glPushMatrix();//Reference: Avenue sample code 
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[1], 0, 1, 0);
	glColor3f(0.282, 0.820, 0.800);
	glutSolidSphere(1, 16, 16);
	glPushMatrix();
	glTranslatef(0, 0.35, 0.9);//0.561, 0.737, 0.561
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.05, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();//translate relative to body, and draw head
	glTranslatef(0, 1.25, 0);
	glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
	glColor3f(1,1,1);
	glutSolidSphere(0.5, 16, 16);

	glPushMatrix();//translate and draw right eye
	glTranslatef(0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();//translate and draw left eye
	glTranslatef(-0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix(); //translate and draw nose
	glTranslatef(0, 0, 0.5);
	glColor3f(1,0.4,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPopMatrix();//body
	glPopMatrix();//snowman
}


void circleAlg(){//circle algorithm;Reference: lighthouse3D website
	int i =0;
	while(i<numTerrPoints){
		randX=rand()%sizeX;
  		randZ=rand()%sizeY;
		float disp=(rand()%2);//variation of peak
		circleRadius=(rand()%50)+1;
		for(int j=0;j<sizeX;j++){//double loop needed because for each terrain point(tx,tz)
			for(int k=0;k<sizeY;k++){
				float dist = sqrtf(pow((j-randX),2) +  pow((k-randZ),2));//distance from circle centre 
				float pd = (dist * 2)/circleRadius;
				if (fabs(pd)<=4.5){
					heightMap[j][k]+=(disp/2.0)+(cos(pd*3.14)*disp/2.0);
				}
				charZ=k;//character end Z coord
			}
		charX=j;//character end X coord
		}
		i++;
	}
	calcHeight();//sets minimum and maximum height values
	charX=(rand()%charX);//choose a random character X and Y, inward from edge 
	charZ=(rand()%charZ);
	float charY=characterHeight(charX,charZ);//get character height(Y) from X and Z coords
	pos[0]=charX;pos[1]=charY+charDisp;pos[2]=charZ;//pos[1]=;//set character position
	normalVecFace();//calculates normals using faces 
	normalVecVertex();//calculates normals using vertices
}



void faultAlg(){//Fault Algorithm;//Reference: lighthouse3D website
	int i=0;
	int disp=1;//variation of peak
	for(int i=0;i<numTerrPoints;i++){ 
		float v =rand();
		float a = sin(v);
		float b = cos(v);
		float d = sqrt((sizeX*sizeY)+(sizeX*sizeY));//modification of fault algorithm. Replacing 1*1 with sizeX*sizeY gives a levelled terrain
		float c = ((static_cast<float>(rand())/static_cast<float>(RAND_MAX))*d)-(d/2);
		for(int j=0;j<sizeX;j++){
			for(int k=0;k<sizeY;k++){
				if((a*j) + (b*k) -c > 0){
					heightMap[j][k]+=disp;
				}
				else{
					heightMap[j][k]-=disp;
				}
				charZ=k;//character end Z coord
			}
			charX=j;//character end X coord
		}
		calcHeight();
		
	}
	charX=(rand()%charX)+1;//choose a random character X and Y pos, inward from edge 
	charZ=(rand()%charZ)+1;
	float charY=characterHeight(charX,charZ);//get character height(Y) from X and Z coords
	pos[0]=charX;pos[1]=charY+charDisp;pos[2]=charZ;//pos[1]=;//set character position
	normalVecFace();
	normalVecVertex();
}

void drawBoth(){//draws the terrain as both a polygon and as a wireframe 
	gluLookAt(camPos[0], camPos[1], camPos[2], 
		camTarget[0], camTarget[1], camTarget[2],
		camUp[0], camUp[1], camUp[2]);
	terrColour[0]=0.0f;terrColour[1]=0.5f;terrColour[2]=0.5f; //0.0f,0.5f,0.5f
	glColor3f(terrColour[0],terrColour[1],terrColour[2]);
	for(int i=0;i<sizeX;i++){
		for(int j=1;j<sizeY-1;j++){
			glBegin(GL_QUAD_STRIP);
				if(!triStrips){//Quad Strips 
					if(i+1<sizeX){
						if(enhancedColour){//if improved map colouring is turned on 
							calcColour(heightMap[i+1][j]);
						}
						else glColor3f(fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour));
						if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
						else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
						glVertex3f(i+1,heightMap[i+1][j],j);
					}
					if(enhancedColour){//if improved map colouring is turned on
							calcColour(heightMap[i][j]);
					}
					else glColor3f(fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i,heightMap[i][j],j);
				}
				else {//Triangle Strips
					if(enhancedColour)calcColour(heightMap[i][j]);
					else glColor3f(fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i,heightMap[i][j],j);
					
					if(enhancedColour)calcColour(heightMap[i][j+1]);
					else glColor3f(fabs(heightMap[i][j+1])/(maxHeight+faultColour),fabs(heightMap[i][j+1])/(maxHeight+faultColour),fabs(heightMap[i][j+1])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i,heightMap[i][j+1],j+1);
					
					if(enhancedColour)calcColour(heightMap[i+1][j]);
					else glColor3f(fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i+1,heightMap[i+1][j],j);

					if(enhancedColour)calcColour(heightMap[i+1][j+1]);
					else glColor3f(fabs(heightMap[i+1][j+1])/(maxHeight+faultColour),fabs(heightMap[i+1][j+1])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i+1,heightMap[i+1][j+1],j+1);
				}
		}
	}
	glEnd();
	terrColour[0]=0;terrColour[1]=0;terrColour[2]=0; //0.0f,0.5f,0.5f
	glColor3f(terrColour[0],terrColour[1],terrColour[2]);
	for(int i=1;i<sizeX-1;i++){
		for(int j=1;j<sizeY-1;j++){
			glBegin(GL_LINE_STRIP);
					if(!triStrips){//Quad Strips 
					if(i+1<sizeX){
						glVertex3f(i+1,heightMap[i+1][j],j);
					}
					glVertex3f(i,heightMap[i][j],j);
				}
				else {//Triangle Strips
					glVertex3f(i,heightMap[i][j],j);
					glVertex3f(i,heightMap[i][j+1],j+1);
					glVertex3f(i+1,heightMap[i+1][j],j);
					glVertex3f(i+1,heightMap[i+1][j+1],j+1);
				}
		}
			glEnd();
	}
}

void drawWire(){//Draws the wireframe only 
	gluLookAt(camPos[0], camPos[1], camPos[2], 
		camTarget[0], camTarget[1], camTarget[2],
		camUp[0], camUp[1], camUp[2]);
	glLineWidth(1.0);
	terrColour[0]=0;terrColour[1]=0;terrColour[2]=0; //0.0f,0.5f,0.5f
	glColor3f(terrColour[0],terrColour[1],terrColour[2]);
	for(int i=1;i<sizeX-1;i++){
		for(int j=1;j<sizeY-1;j++){
			glBegin(GL_LINE_STRIP);
					if(!triStrips){//Quad Strips 
					if(i+1<sizeX){
						glVertex3f(i+1,heightMap[i+1][j],j);
					}
					glVertex3f(i,heightMap[i][j],j);
				}
				else {//Triangle Strips
					glVertex3f(i,heightMap[i][j],j);
					glVertex3f(i,heightMap[i][j+1],j+1);
					glVertex3f(i+1,heightMap[i+1][j],j);
					glVertex3f(i+1,heightMap[i+1][j+1],j+1);
				}
		}
		glEnd();
	}	
}

void calcColour(float height){//Calculates the colours for improved map colouring 
	float temp=height/maxHeight;
	if(faultMode)temp+=faultColour+0.5;//add a little bit of height to compensate for the deep fault algorithm craters
	if (temp>=0.8) glColor3f(0.502,0.502,0.502);//gray
	else if (temp>=0.75) glColor3f(0.412,0.412,0.412);//dim gray
	else if (temp>=0.70) glColor3f(0.467,0.533,0.600);//lightSlateGray
	else if (temp>=0.65) glColor3f(0.545,0,0);//dark red
	else if (temp>=0.60) glColor3f(1,0,0);//red
	else if (temp>=0.55) glColor3f(1,0.271,0.000);//orange red
	else if (temp>=0.50) glColor3f(1,0.549,0);//dark orange
	else if (temp>=0.45) glColor3f(1,0.647,0);//orange
	else if (temp>=0.40) glColor3f(0.000,0.392,0.000);//dark green
	else if (temp>=0.35) glColor3f(0.000,0.502,0.000);//green
	else if (temp>=0.30) glColor3f(0.133,0.545,0.133);//forest green
	else if (temp>=0.25) glColor3f(0.180,0.545,0.341);//sea green
	else if (temp>=0.20) glColor3f(0.235,0.702,0.443);//medium sea green
	else if (temp>=0.15) glColor3f(0,1,0.498);//spring green
	else if (temp>=0.10) glColor3f(0.000,0.980,0.604);//medium spring green
	else if (temp>=0.05) glColor3f(0.565,0.933,0.565);//light green
	if(height==minHeight) glColor3f(0.596,0.984,0.596);//colour the lowest point pale green
}

void drawMap(){// Draws only the terrain polygon without the wireframe
	glFrontFace(GL_CCW);//culling 
	gluLookAt(camPos[0], camPos[1], camPos[2], 
		camTarget[0], camTarget[1], camTarget[2],
		camUp[0], camUp[1], camUp[2]);
	for(int i=1;i<sizeX-1;i++){
		for(int j=1;j<sizeY-1;j++){
			glBegin(GL_QUAD_STRIP);
				if(!triStrips){//Quad Strips 
					if(i+1<sizeX){
						if(enhancedColour){//if improved map colouring is turned on 
							calcColour(heightMap[i+1][j]);
						}
						else glColor3f(fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour));
						if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
						else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
						glVertex3f(i+1,heightMap[i+1][j],j);
					}
					if(enhancedColour){//if improved map colouring is turned on
							calcColour(heightMap[i][j]);
					}
					else glColor3f(fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i,heightMap[i][j],j);
				}
				else {//Triangle Strips
					if(enhancedColour)calcColour(heightMap[i][j]);
					else glColor3f(fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour),fabs(heightMap[i][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i,heightMap[i][j],j);
					
					if(enhancedColour)calcColour(heightMap[i][j+1]);
					else glColor3f(fabs(heightMap[i][j+1])/(maxHeight+faultColour),fabs(heightMap[i][j+1])/(maxHeight+faultColour),fabs(heightMap[i][j+1])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i,heightMap[i][j+1],j+1);
					
					if(enhancedColour)calcColour(heightMap[i+1][j]);
					else glColor3f(fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i+1,heightMap[i+1][j],j);

					if(enhancedColour)calcColour(heightMap[i+1][j+1]);
					else glColor3f(fabs(heightMap[i+1][j+1])/(maxHeight+faultColour),fabs(heightMap[i+1][j+1])/(maxHeight+faultColour),fabs(heightMap[i+1][j])/(maxHeight+faultColour));
					if(flatShade)glNormal3f(normalFaces[i][j][0],normalFaces[i][j][1],normalFaces[i][j][2]);//use face normals for flat shading 
					else glNormal3f(normalVertices[i][j][0],normalVertices[i][j][1],normalVertices[i][j][2]);//use vertex normals for gourard shading
					glVertex3f(i+1,heightMap[i+1][j+1],j+1);
				}
		}
		glEnd();
	}
}

void menuOptions(){
	printf("%s\n", "------------------Base Features----------------" );
	printf("%s\n", "Press left and right arrows to rotate the scene about the y axis" );
	printf("%s\n", "Press up and down arrows to rotate the scene about the x axis" );
	printf("%s\n", "Press 'w' to toggle wireframe mode; Press 'l' to toggle lighting");
	printf("%s\n", "Press 's' to toggle shading mode; Default: Flat Shading ");
	printf("%s\n", "Press 'r' to generate a new random terrain");
	printf("%s\n", "Press 't' to draw terrain using triangle strips;Press 'y' to draw terrain using quad strips");
	printf("%s\n","Press 'g to move light0 up");
	printf("%s\n","Press 'c' to move light0 left");
	printf("%s\n","Press 'b' to move light0 right");
	printf("%s\n","Press 'v' to move light0 down");
	printf("%s\n","Press 'i' to move light1 up" );
	printf("%s\n","Press 'z' to move light1 left" );
	printf("%s\n","Press 'x' to move light1 right" );
	printf("%s\n","Press 'm' to move light1 down" );
	printf("%s\n", "------------------Extra Features----------------" );
	printf("%s\n", "1) Press 'f' to toggle between fault algorithm and circle algorithm");
	printf("%s\n", "2) Press 'u' to move the snowman up");
	printf("%s\n", "   Press 'j' to move the snowman down");
	printf("%s\n", "   Press 'h' to move the snowman left");
	printf("%s\n", "   Press 'k' to move the snowman right");
	printf("%s\n", "3) Press 'p' to toggle improved map colouring" );
}

void display(void)
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour0);//upload light data to gpu
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);//upload light data to gpu
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColour1);//upload light data to gpu
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);//upload light data to gpu
	glCullFace(GL_BACK);//backface culling
	glEnable(GL_CULL_FACE);
	if (wToggle==0){//Polygon only 
		if(!lightOn)glClearColor( 0.196078,0.6,0.8,0);//skyblue
		else glClearColor(0,0,0,0);//nighttime
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //3rd: clear the depth channe;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		drawMap();//draws the terrain as a polygon 
		glutPostRedisplay();
		if(lightOn){//is user turns on light 
			glDisable(GL_LIGHTING);//allows lights to
			drawLight0();//draws the first light source 
			drawLight1();//draws the second light source 
			glEnable(GL_LIGHTING);//keep yellow colour
		}
		character();//draws the snowman character
		glutSwapBuffers();
	}
	else if(wToggle==1){//Wireframe only 
		if(!lightOn)glClearColor( 0.196078,0.6,0.8,0);//skyblue
		else glClearColor(0,0,0,0);//nighttime
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //3rd: clear the depth channe;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		drawWire();
		glutPostRedisplay();
		if(lightOn){//is user turns on light 
			glDisable(GL_LIGHTING);//allows lights to
			drawLight0();
			drawLight1();
			glEnable(GL_LIGHTING);//keep yellow colour
		}
		character();
		glutSwapBuffers();
	}
	else if(wToggle==2){//Polygon and Wireframe 
		if(!lightOn)glClearColor( 0.196078,0.6,0.8,0);//skyblue
		else glClearColor(0,0,0,0);//skyblue
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //3rd: clear the depth channe;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		drawBoth();
		glutPostRedisplay();
		if(lightOn){//is user turns on light 
			glDisable(GL_LIGHTING);//allows lights to
			drawLight0();
			drawLight1();
			glEnable(GL_LIGHTING);//keep yellow colour
		}
		character();
		glutSwapBuffers();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  //1st: enable frame buffer for depth
	glutInitWindowSize(swidth, sheight);
	printf("\n%s\n", "---------------------3D Terrain------------------" );
	while(sizeX<50 || sizeX>300){//user input checking 
		printf("\nEnter the grid size: ");
		scanf("%d",&sizeX);sizeY=sizeX;
	}
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Terrain");	//creates the window
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST); //2nd: enable the depth test and enable lights
	glShadeModel(GL_FLAT);//default 
	srand(time(NULL));//used to get random numbers 
	init();
	circleAlg();//calculates points using the circle algorithm 
	glutKeyboardFunc(keyboard);//register keyboard function callback 
	menuOptions();//displays the menu options 
	glutSpecialFunc(special);//registers the special keys callback 
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutMainLoop();				//starts the event loop
	return(0);					//return may not be necessary on all compilers
}