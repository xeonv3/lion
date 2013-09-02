//#define GLUT_DISABLE_ATEXIT_HACK
//#include<gl/glew.h>
//#include<gl/glut.h>
//#include<time.h>
//#include<cstdio>
//#pragma comment(lib,"glew32.lib")
//GLuint ag = 0;
//int WIDTH = 800, HEIGHT = 600;//´°¿Ú´óÐ¡
//void init(){
//	glClearColor(1.0,1.0,1.0,1.0);
//	glEnable(GL_DEPTH_TEST);
//	glColor3f(0.0,1.0,0.0);
//}
//void reshape(int w,int h){
//	WIDTH = w; HEIGHT = h;
//	glViewport(0,0,(GLsizei)WIDTH,(GLsizei)HEIGHT);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(-1.5,1.7,-0.7,0.8);
//	glMatrixMode(GL_MODELVIEW);
//}
//
//void display(){
//	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	glRotated(ag,0,1,0);
//	glutSolidTeapot(1.0);
//	glutSwapBuffers();
//}
//void idle(){
//	++ag;
//	if(ag > 360) ag=0;
//	glutPostRedisplay();
//}
//
//int main(int argc,char** argv){
//	glutInit(&argc,argv);
//	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
//	glutInitWindowSize(WIDTH,HEIGHT);
//	glutInitWindowPosition(100,100);
//	glutCreateWindow(argv[0]);
//	GLenum err = glewInit();
//	if(err != GLEW_OK){
//		fprintf(stderr,"glew error!!\n");
//		getchar();
//		return 0;
//	}
//	init();
//	glutDisplayFunc(display);
//	glutReshapeFunc(reshape);
//	glutIdleFunc(idle);
//	glutMainLoop();
//	return 0;
//}