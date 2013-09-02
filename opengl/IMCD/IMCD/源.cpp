#define GLUT_DISABLE_ATEXIT_HACK
#include<gl/glew.h>
#include<gl/glut.h>
#include<time.h>
#include<cstdio>
#pragma comment(lib,"glew32.lib")
static GLuint ag=0.0; //torusת���Ƕ�
int WIDTH = 800, HEIGHT = 600;//���ڴ�С
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


GLuint depthQuery;
bool CollisionDetection(){
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	//��ͶӰ������Ϊ��ͶӰ�������ɴ�������Ⱦ����
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.5,1.7,-0.7,0.8);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//������ȡ�ģ�塢�ü����ԣ����ü���������ΪA��BͶӰ�ཻ������
	//һ��������Ҫ�ð�Χ���ཻ�����������������˼򻯣�A��B�ཻ�������AͶӰ����
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//��һ����Ⱦ
	glClearDepth(0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	//A
	glDepthFunc(GL_GREATER);
	glStencilFunc(GL_ALWAYS,1,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	glColor3f(0.0,1.0,0.0);
	glutSolidTeapot(1.0);
	//B
	glDepthFunc(GL_LEQUAL);
	glStencilFunc(GL_EQUAL,1,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glLoadIdentity();
	glRotated(ag,0,1,0);
	glColor3f(1.0,0.0,0.0);
	glutSolidTorus(0.5,2.0,10,20);

	//�ڶ�����Ⱦ
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	//B
	glDepthFunc(GL_GREATER);
	glStencilFunc(GL_EQUAL,2,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glLoadIdentity();
	glRotated(ag,0,1,0);
	glColor3f(0.0,0.0,1.0);
	glutSolidTorus(0.5,2.0,10,20);
	//A����ʹ���ڵ���ѯ
	glGenQueries(1,&depthQuery);
	glBeginQuery(GL_SAMPLES_PASSED,depthQuery);
	glDepthFunc(GL_LEQUAL);
	glStencilFunc(GL_EQUAL,2,~0);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glColor3f(1.0,1.0,0.0);
	glutSolidTeapot(1.0);
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
	//�ָ�����
	glDisable(GL_STENCIL_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
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
	gluPerspective(60,(GLfloat)w/(GLfloat)h,1.0,100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void display(){
	printf("FPS = %f\t\t",CalFrequency());
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-10.0);
	glColor3f(0.0,1.0,0.0);
	glutSolidTeapot(1.0);
	
	glLoadIdentity();
	glTranslatef(0.0,0.0,-10.0);
	glRotated(ag,0,1,0);
	glColor3f(0.0,0.0,1.0);
	glutSolidTorus(0.5,2.0,10,20);
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