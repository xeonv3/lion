#define GLUT_DISABLE_ATEXIT_HACK
#include<gl/glew.h>
#include<gl/glut.h>
#include<time.h>
#include<cstdio>
#include<cstring>
#pragma comment(lib,"glew32.lib")
#define ImageWidth 256
#define ImageHeight 256
#define MXWIDTH 512 
#define MXHEIGHT 512
int WIDTH = 800, HEIGHT = 600;//窗口大小
static GLuint ag=0.0; //torus转动角度
GLsizei bufferSize = ImageWidth * ImageHeight * 3; //3 components for RGB
GLuint pixelBuffer;

double CalFrequency(){
	static int count;
	static double save;
	static clock_t last,current;
	double timegap;

	++count;
	if(count<=10) return save;
	count = 0;
	last = current;
	current = clock();
	timegap=(current-last)/(double)CLK_TCK;
	save=10.0/timegap;
	return save;
}

inline void drawTorus(GLfloat r, GLfloat g, GLfloat b){
	glPushMatrix();
	glRotated(ag,0,1,0);
	glColor3f(r,g,b);
	glutSolidTorus(50.0,180.0,60,100);
	glPopMatrix();
}
inline void drawTeapot(GLfloat r, GLfloat g, GLfloat b){
	glColor3f(r,g,b);
	glutSolidTeapot(100.0);
}

void init(){
	glEnable(GL_DEPTH_TEST);
	glGenBuffers(1,&pixelBuffer);
	glBindBuffer(GL_PIXEL_PACK_BUFFER,pixelBuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER,bufferSize,NULL,GL_STATIC_READ);
}

void reshape(int w,int h){
	WIDTH = w; HEIGHT = h;
	glViewport(0,0,(GLsizei)WIDTH,(GLsizei)HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)w/(GLfloat)h,1.0,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,600.0,0.0,0.0,-10.0,0,1,0);
}

//coding of Binary Indexed Trees
inline int lowbit(int x){return x&(-x);}
void update(int c[MXHEIGHT][MXWIDTH],int x,int y,int d){//d = -1 or 1
	for(int i = y; i <= ImageHeight; i+=lowbit(i)){
		for(int j = x; j <= ImageWidth; j+=lowbit(j)){
			c[i][j]+=d;
		}
	}
}
int sum(int c[MXHEIGHT][MXWIDTH],int x,int y){
	int res = 0;
	for(int i = y; i > 0; i-=lowbit(i)){
		for(int j = x; j > 0; j-= lowbit(j)){
			res += c[i][j];
		}
	}
	return res;
}
inline int sum(int c[MXHEIGHT][MXWIDTH],int x1,int y1,int x2,int y2){
	return sum(c,x2,y2)+sum(c,x1-1,y1-1)-sum(c,x1-1,y2)-sum(c,x2,y1-1);
}
bool initCollisionDetection(){
	bool flag=false;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-160,180,-80,90,-100.0,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0,0,ImageWidth,ImageHeight);
	
	//Render A
	glClearColor(0.0,0.0,0.0,1.0);
	glClearDepth(0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDepthFunc(GL_GREATER);
	drawTeapot(0.0,0.0,0.0);
	//Render B
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	drawTorus(1.0,1.0,1.0);
	//Render A
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	drawTeapot(0.0,0.0,0.0);
	//Render B
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_AND);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_FALSE);
	glDepthFunc(GL_GEQUAL);
	drawTorus(0.0,1.0,0.0);
	glReadPixels(0,0,ImageWidth,ImageHeight,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER,GL_READ_ONLY);
	for(int i = 0; i < ImageHeight; ++i){
		for(int j = 0; j < ImageWidth; ++j){
			if(*ptr ==  0 && *(ptr+1) == 255 && *(ptr+2) == 0){
				flag=true;break;			
			}
			ptr+=3;
		}
	}
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

	glDisable(GL_COLOR_LOGIC_OP);
	glViewport(0,0,WIDTH,HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	return flag;
}
//bool TestCollision(int x,int y,int w,int h){
//	if(w < 1 || h < 1) return true;
//	if(sum(Ea,x,y,x+w-1,y+h-1)==0|| sum(Eb,x,y,x+w-1,y+h-1)==0) return false;
//	if(h > w){
//		int mh = h>>1;
//		return TestCollision(x,y,w,mh)||TestCollision(x,y+mh,w,h-mh);
//	}else{
//		int mw = w>>1;
//		return TestCollision(x,y,mw,h)||TestCollision(x+mw,y,w-mw,h);
//	}
//}

void display(){
	printf("FPS = %f\t",CalFrequency());

	static clock_t last,current;
	last = clock();
	bool cd =initCollisionDetection();
	//bool cd = TestCollision(1,1,ImageWidth,ImageHeight);
	current = clock();
	double timegap = (current-last)/(double)CLK_TCK;
	printf("%s\t%f\n",cd?"\tcollision detected":"non-collision",timegap*1000);

	glClearColor(1.0,1.0,1.0,0.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glPushMatrix();
	drawTeapot(0.0,1.0,0.0);
	glPopMatrix();

	glPushMatrix();
	drawTorus(0.0,0.0,1.0);
	glPopMatrix();
	glutSwapBuffers();
}
void idle(){
	++ag;
	if(ag > 360) ag=0;
	glutPostRedisplay();
}
void keyboard(unsigned char c,int x,int y){
	switch(c){
	case 'r':
		ag = (ag+2)%360;
		glutPostRedisplay();
		break;
	case 'R':
		ag = (ag-2)%360;
		glutPostRedisplay();
		break;
	}
}
int main(int argc,char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	GLenum err = glewInit();
	if(err != GLEW_OK){
		fprintf(stderr,"glew error!!\n");
		getchar();
		return 0;
	}
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}



