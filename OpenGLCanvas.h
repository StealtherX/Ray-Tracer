#ifndef __OpenGLCanvas_h__
#define __OpenGLCanvas_h__


// NOTE: GL_GLEXT_PROTOTYPES has to be defined before include QGLWidget on Linux boxes
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#endif // __linux__
#include <QGLWidget>
#include <QPoint>
#include <vector>
using namespace std;

#include "struct.h"
#include "scene.h"


class QColor;

const double pi = 3.14159;


class OpenGLCanvas : public QGLWidget
{
   Q_OBJECT

   public:
      OpenGLCanvas(QWidget* pQW_Parent=NULL);
      ~OpenGLCanvas();

      QSize minimumSizeHint() const;
      void lineBres(int x0, int y0, int xEnd, int yEnd);
      void lineBresDraw(int x0, int y0, int xEnd, int yEnd);
      void blackout();
      void drawScene(Scene myScene);

      light ambient;
      int phongConst;
      int maxDepth;
      bool refraction;
      float angle;
      point from;

   public slots:
      void SetPixelPositionX(int iX);
      void SetPixelPositionY(int iY);
      void refreshDraw();

   signals:
      void SizeChanged(QSize QS_NewSize);
      void MousePositionChanged(QPoint QP_NewPosition);
      void progress(int a);


   protected:
      void initializeGL();
      void paintGL();
      void resizeGL(int iWidth, int iHeight);
      void mousePressEvent(QMouseEvent* pQME_Event);
      void mouseMoveEvent(QMouseEvent* pQME_Event);
      bool hitSphere(const ray &r, const sphere &s, float &t);
      color rayTrace(Scene myScene, ray viewRay, int depth);
      color directPhong(Vector l, Vector n, Vector v, light I, texture t);
      Vector findRay(int x, int y, float angle);
      bool hitPlane(const ray &r, const plane &p, float &t);

   private:
      void _MakePixel(const QPoint& QPt_Position, const QColor& QC_Color);
      void _MakePixel(const QPoint& QPt_Position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
      void _SetPixelPosition(const QPoint& QPt_NewPosition);


      unsigned char* _pucBuffer;
      int bufferSize;
      QPoint _QPt_CurrentPosition;
      int halfWidth;
      int halfHeight;


};

#endif // __OpenGLCanvas_h__
