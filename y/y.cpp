
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <gl\GL.h>

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

#define WINDOWS_ERROR_CHECK    { \
   char szError[1024];                      \
   long errorCode = GetLastError();         \
   if ( errorCode ) {                       \
      sprintf(szError,"Error in Windows call method '%s' (%d)\n\nOpenGL returned error %d (%x)\n\n\nDebug ?",__FILE__,__LINE__,errorCode,errorCode); \
      errorCode = MessageBox(NULL,szError,"Windows usage exception",MB_YESNOCANCEL); \
      if ( IDCANCEL == errorCode ) _exit(0);\
      if ( IDYES == errorCode ) {           \
      _asm {   int 3   }                    \
      }                                     \
   }                                        \
   }

   struct DataPoint {
      double x,y,z;
   };

   PIXELFORMATDESCRIPTOR pfd = {
         sizeof(PIXELFORMATDESCRIPTOR),
         1,
         PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,// | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER, 
         PFD_TYPE_RGBA,   
         24,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,PFD_MAIN_PLANE,0,0,0,0};

   LRESULT EXPENTRY handler(HWND,UINT,WPARAM,LPARAM);

   void init();
   void draw();


   extern "C" int main(int argc,char *argv[] ) {

   WNDCLASS gClass;
 
   memset(&gClass,0,sizeof(WNDCLASS));
 
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
   gClass.lpfnWndProc = handler;
   gClass.cbClsExtra = 32;
   gClass.cbWndExtra = 32;
   gClass.hInstance = NULL;
   gClass.hIcon = NULL;
   gClass.hCursor = NULL;
   gClass.hbrBackground = 0;
   gClass.lpszMenuName = NULL;
   gClass.lpszClassName = "WTF";
  
   RegisterClass(&gClass);

   HWND hwnd = CreateWindowEx(0L,"WTF","",WS_VISIBLE,256,256,1024,768,HWND_DESKTOP,NULL,NULL,NULL);

   HDC deviceContext = GetDC(hwnd);
  
   int pixelFormat = ChoosePixelFormat(deviceContext,&pfd);

   if ( ! pixelFormat ) {
      pfd.dwFlags = pfd.dwFlags & ~ PFD_GENERIC_ACCELERATED;
      pixelFormat = ChoosePixelFormat(deviceContext,&pfd);
   }

   SetPixelFormat(deviceContext,pixelFormat,&pfd);

   HGLRC renderingContext = wglCreateContext(deviceContext);

   wglMakeCurrent(deviceContext,renderingContext);

   glClearColor(1.0, 1.0, 1.0, 1.0);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   glDrawBuffer(GL_FRONT_AND_BACK);

   RECT rc;
   GetWindowRect(hwnd,&rc);
   long cx = rc.right - rc.left;
   long cy = rc.bottom - rc.top;

   glViewport(0, 0, (GLsizei) cx, (GLsizei) cy);

   glMatrixMode (GL_PROJECTION);

   glLoadIdentity();

   if (cx <= cy)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)cy/(GLfloat)cx,1.5*(GLfloat)cy/(GLfloat)cx, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)cx/(GLfloat)cy,1.5*(GLfloat)cx/(GLfloat)cy, -1.5, 1.5, -10.0, 10.0);

   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();

   init();

   MSG msg;
   while ( GetMessage(&msg,NULL,0L,0L) ) {
      if ( PeekMessage(&msg,NULL,WM_QUIT,WM_QUIT,PM_REMOVE) ) 
         break;
      if ( WM_KEYFIRST <= msg.message && msg.message <= WM_KEYLAST ) 
         TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   _getch();

   return 0;
   }


   LRESULT EXPENTRY handler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   switch ( msg ) {

   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      draw();
      EndPaint(hwnd,&ps);
      }
      return (LRESULT)0L;

   default:
      break;
   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }


   void draw() {

   static double twoPi = 0.0;
   static double pi,dTheta,dThetaBase,dPhi,piOver2;

   if ( 0.0 == twoPi ) {
      piOver2 = 2.0 * atan(1.0);
      pi = 2.0 * piOver2;
      twoPi = 2.0 * pi;
      dTheta = twoPi / 60.0;
      dPhi = pi / 60.0;
   }

   double xRadius = 1.0;
   double yScaleFactor = 1.0;
   double zScaleFactor = 1.0;

   glBegin(GL_TRIANGLE_STRIP);
OPENGL_ERROR_CHECK

   double phi,theta;

   DataPoint v[5];
   double avgNormal[5][4];
   DataPoint homePoint,firstPoint,secondPoint;

   homePoint.x = 0.0;
   homePoint.y = 0.0;
   homePoint.z = 0.0;

   for ( theta = 0.0; theta <= twoPi; theta += dTheta ) {

      double nextTheta = theta + dTheta;

      double cosTheta = cos(theta);
      double cosNextTheta = cos(theta + dTheta);
      double sinTheta = sin(theta);
      double sinNextTheta = sin(theta + dTheta);

      for ( phi = -piOver2; phi < piOver2; phi += dPhi ) {

         double nextPhi = phi + dPhi;

         firstPoint = homePoint;
         firstPoint.x += xRadius * cosTheta * cos(phi);
         firstPoint.y += xRadius * sinTheta * cos(phi) / yScaleFactor;
         firstPoint.z += xRadius * sin(phi) / zScaleFactor;

         secondPoint = homePoint;
         secondPoint.x += xRadius * cosNextTheta * cos(phi);
         secondPoint.y += xRadius * sinNextTheta * cos(phi) / yScaleFactor;
         secondPoint.z += xRadius * sin(phi) / zScaleFactor;

         v[0] = firstPoint;
         v[1] = secondPoint;

         avgNormal[0][0] = firstPoint.x;
         avgNormal[0][1] = firstPoint.y;
         avgNormal[0][2] = firstPoint.z;

         glNormal3dv(avgNormal[0]);
         for ( int vk = 0; vk < 2; vk++ ) {
            glVertex3d(v[vk].x,v[vk].y,v[vk].z);
         }

      }

   }

   glEnd();

   glFlush();

   return;
   }

void init() 
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