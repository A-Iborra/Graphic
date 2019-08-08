
#include <windows.h>
#include <math.h>
#include <stdio.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

#define OPENGL_ERROR_CHECK    { \
   char szError[1024];                      \
   long errorCode = glGetError();           \
   if ( errorCode ) {                       \
      sprintf(szError,"Error in OpenGL call method '%s' (%d)\n\nOpenGL returned error %d (%x)\n\n\nDebug ?",__FILE__,__LINE__,errorCode,errorCode); \
      errorCode = MessageBoxA(NULL,szError,"OpenGL usage exception",MB_YESNOCANCEL); \
      if ( IDCANCEL == errorCode ) _exit(0);\
      if ( IDYES == errorCode ) {           \
      _asm {   int 3   }                    \
      }                                     \
   }                                        \
   }

   struct dataPoint {
   double x,y,z;
   };

void init(void) 
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 128.0 };
   GLfloat light_position[] = { 5.0, 5.0, 5.0, 0.0 };

   glClearColor (1.0, 1.0, 1.0, 1.0);

   glShadeModel(GL_SMOOTH);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float fvColor[] = {1.0,0.0,0.0};

   double lw = 1.0;

   long segmentID = 1;

   glDeleteLists(segmentID,1);

   glNewList(segmentID,GL_COMPILE);

   glColor3f(fvColor[0],fvColor[1],fvColor[2]);

   glLineWidth((float)lw);

   static double twoPi = 0.0;
   static double dTheta,dThetaBase,dPhi,piOver2;

   if ( 0.0 == twoPi ) {
      piOver2 = 2.0 * atan(1.0);
      twoPi = 4.0 * piOver2;
      dTheta = twoPi / 80.0;
      dPhi = piOver2 / 20.0;
   }

   double xRadius = 1.0;
   double yScaleFactor = 1.0;
   double zScaleFactor = 1.0;

   double avgNormal[3];

   dataPoint homePoint,firstPoint,secondPoint;

   double v[2][3];

   homePoint.x = 0.0;
   homePoint.y = 0.0;
   homePoint.z = 0.0;

   for ( double theta = 0.0; theta <= twoPi; theta += dTheta ) {

      double nextTheta = theta + dTheta;

      double cosTheta = cos(theta);
      double cosNextTheta = cos(theta + dTheta);
      double sinTheta = sin(theta);
      double sinNextTheta = sin(theta + dTheta);

      glBegin(GL_TRIANGLE_STRIP);

      for ( double phi = -piOver2; phi <= piOver2; phi += dPhi ) {

         double nextPhi = phi + dPhi;

         firstPoint = homePoint;
         firstPoint.x += xRadius * cosTheta * cos(phi);
         firstPoint.y += xRadius * sinTheta * cos(phi) / yScaleFactor;
         firstPoint.z += xRadius * sin(phi) / zScaleFactor;

         secondPoint = homePoint;
         secondPoint.x += xRadius * cosNextTheta * cos(phi);
         secondPoint.y += xRadius * sinNextTheta * cos(phi) / yScaleFactor;
         secondPoint.z += xRadius * sin(phi) / zScaleFactor;

         v[0][0] = firstPoint.x;
         v[0][1] = firstPoint.y;
         v[0][2] = firstPoint.z;

         v[1][0] = secondPoint.x;
         v[1][1] = secondPoint.y;
         v[1][2] = secondPoint.z;

         avgNormal[0] = firstPoint.x;
         avgNormal[1] = firstPoint.y;
         avgNormal[2] = firstPoint.z;

         glNormal3dv(avgNormal);

         for ( int vk = 0; vk < 2; vk++ ) {
            glVertex3dv(v[vk]);
         }

      }

      glEnd();

   }

      glEndList();
OPENGL_ERROR_CHECK

//glGetError();

      glCallList(segmentID);
OPENGL_ERROR_CHECK

      glFlush ();
OPENGL_ERROR_CHECK

}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

extern "C" int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
