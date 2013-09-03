#define GLUT_DISABLE_ATEXIT_HACK
#include<gl/glew.h>
#include<gl/glut.h>
#include<time.h>
#include<cstdio>
#pragma comment(lib,"glew32.lib")
static GLuint ag=0.0; //torus转动角度
int WIDTH = 800, HEIGHT = 600;//窗口大小
bool rendermode = true;
double CalFrequency(){
	static int count;
	static double save;
	static clock_t last,current;
	double timegap;

	++count;
	if(count<=50) return save;
	count = 0;
	last = current;
	current = clock();
	timegap=(current-last)/(double)CLK_TCK;
	save=50.0/timegap;
	return save;
}
void reshape(int w,int h);
inline void drawTorus(){
	glRotated(ag,0,1,0);
	glColor3f(0.0,0.0,1.0);
	glutSolidTorus(50.0,180.0,60,100);
}
inline void drawTeapot(){
	glColor3f(0.0,1.0,0.0);
	glutSolidTeapot(100.0);
}
GLuint depthQuery;
bool CollisionDetection(){
	if(rendermode)
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	//将投影矩阵设为正投影，并且由此限制渲染区域
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-160,180,-80,90,-10,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//启用深度、模板、裁剪测试，将裁剪区域设置为A、B投影相交的区域
	//一般来将需要用包围盒相交来做，但我这里做了简化，A、B相交区域就是A投影区域
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//第一次渲染
	glClearDepth(0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	//A
	glDepthFunc(GL_GREATER);
	glStencilFunc(GL_ALWAYS,1,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	drawTeapot();
	//B
	glDepthFunc(GL_LEQUAL);
	glStencilFunc(GL_EQUAL,1,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glPushMatrix();
	drawTorus();
	glPopMatrix();
	//第二次渲染
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	//B
	glDepthFunc(GL_GREATER);
	glStencilFunc(GL_EQUAL,2,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glPushMatrix();
	drawTorus();
	glPopMatrix();
	//A，并使用遮挡查询
	glGenQueries(1,&depthQuery);
	glBeginQuery(GL_SAMPLES_PASSED,depthQuery);
	glDepthFunc(GL_LEQUAL);
	glStencilFunc(GL_EQUAL,2,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	drawTeapot();
	glEndQuery(GL_SAMPLES_PASSED);
	int count = 1000;
	GLint queryReady = 0;
	while(!queryReady && --count){
		glGetQueryObjectiv(depthQuery,GL_QUERY_RESULT_AVAILABLE,&queryReady);
	}
	int samples = 0;
	if(queryReady){ 
		glGetQueryObjectiv(depthQuery,GL_QUERY_RESULT,&samples);
	}
	//恢复场景
	glDisable(GL_STENCIL_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glClear(GL_DEPTH_BUFFER_BIT);
	reshape(WIDTH,HEIGHT);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	return samples > 0;
}
void init(){
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
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

void display(){
	printf("FPS = %f\t",CalFrequency());
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);	
	if(rendermode){
		glPushMatrix();
		drawTeapot();
		glPopMatrix();

		glPushMatrix();
		drawTorus();
		glPopMatrix();
	}
	static clock_t last,current;
	last = clock();
	bool cd = CollisionDetection();
	current = clock();
	double timegap = (current-last)/(double)CLK_TCK;
	printf("%s\t%f\n",cd?"\tcollision detected":"non-collision",timegap*1000);
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
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_STENCIL);
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