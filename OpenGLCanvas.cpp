#include "OpenGLCanvas.h"


#include <QColor>
#include <QMouseEvent>
#include <QSize>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using namespace std;




OpenGLCanvas::OpenGLCanvas(QWidget* pQW_Parent)
   : QGLWidget(pQW_Parent), _pucBuffer(NULL)
{
    ambient.blue = ambient.green = ambient.red = 0.3;
    maxDepth = 4;
    phongConst = 10;
    refraction = false;
    angle = pi/8;
    from.x = 160;
    from.y = 160;
    from.z = -386.27;

}

OpenGLCanvas::~OpenGLCanvas()
{
   delete[] _pucBuffer;
}

QSize OpenGLCanvas::minimumSizeHint() const
{
   return QSize(100, 100);
}

void OpenGLCanvas::SetPixelPositionX(int iX)
{
   _SetPixelPosition(QPoint(iX, _QPt_CurrentPosition.y()));
}

void OpenGLCanvas::SetPixelPositionY(int iY)
{
   _SetPixelPosition(QPoint(_QPt_CurrentPosition.x(), iY));
}

void OpenGLCanvas::refreshDraw()
{
    update();
}

void OpenGLCanvas::blackout()
{
       for(int i=0; i<bufferSize; ++i) _pucBuffer[i]=0;
}


void OpenGLCanvas::initializeGL()
{
   glClearColor(0.f, 0.f, 0.f, 0.f);
   glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void OpenGLCanvas::resizeGL(int iWidth, int iHeight)
{
#ifndef GL_VERSION_1_4
   // Projection settings for GL_Version<1.4
   glViewport(0, 0, iWidth, iHeight);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0., iWidth, 0., iHeight, -1., 1.);
   glMatrixMode(GL_MODELVIEW);
#endif // GL_VERSION_1_4

   // Allocate an new buffer
   delete[] _pucBuffer;

   halfWidth = iWidth / 2;
   halfHeight = iHeight / 2;

   const int kiBufferSize=width()*height()*4;
   bufferSize = kiBufferSize;
   _pucBuffer=new unsigned char[kiBufferSize];
   for(int i=0; i<kiBufferSize; ++i) _pucBuffer[i]=0;

   emit SizeChanged(QSize(iWidth, iHeight));
}

void OpenGLCanvas::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

#ifndef GL_VERSION_1_4
   glLoadIdentity();
   glRasterPos2i(0, 0);
#else // GL_VERSION_1_4
   glWindowPos2i(0, 0);
#endif // GL_VERSION_1_4


   glDrawPixels(width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, _pucBuffer);
   glFlush(); // Need This??
}

void OpenGLCanvas::mousePressEvent(QMouseEvent* pQME_Event)
{
   emit MousePositionChanged(pQME_Event->pos());
}

void OpenGLCanvas::mouseMoveEvent(QMouseEvent* pQME_Event)
{
   emit MousePositionChanged(pQME_Event->pos());
}

void OpenGLCanvas::_MakePixel(const QPoint& QPt_Position, const QColor& QC_Color)
{
   const int kiX=QPt_Position.x();
   const int kiY=QPt_Position.y();
   const int kiWidth=width();
   const int kiHeight=height();

   if(kiX<0 || kiY<0 || kiX>=kiWidth || kiY>=kiHeight) return;

   _pucBuffer[kiX*4+kiY*kiWidth*4]=static_cast<unsigned char>(QC_Color.value());
   _pucBuffer[kiX*4+1+kiY*kiWidth*4]=static_cast<unsigned char>(QC_Color.value());
   _pucBuffer[kiX*4+2+kiY*kiWidth*4]=static_cast<unsigned char>(QC_Color.value());
   _pucBuffer[kiX*4+3+kiY*kiWidth*4]=static_cast<unsigned char>(QC_Color.value());
}

void OpenGLCanvas::_MakePixel(const QPoint& QPt_Position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
   const int kiX=QPt_Position.x();
   const int kiY=QPt_Position.y();
   const int kiWidth=width();
   const int kiHeight=height();

   if(kiX<0 || kiY<0 || kiX>=kiWidth || kiY>=kiHeight) return;

   _pucBuffer[kiX*4+kiY*kiWidth*4]=static_cast<unsigned char>(red);
   _pucBuffer[kiX*4+1+kiY*kiWidth*4]=static_cast<unsigned char>(green);
   _pucBuffer[kiX*4+2+kiY*kiWidth*4]=static_cast<unsigned char>(blue);
   _pucBuffer[kiX*4+3+kiY*kiWidth*4]=static_cast<unsigned char>(alpha);
}

void OpenGLCanvas::_SetPixelPosition(const QPoint& QPt_NewPosition)
{
   if(QPt_NewPosition==_QPt_CurrentPosition) return;

   _MakePixel(_QPt_CurrentPosition, Qt::black);
   _QPt_CurrentPosition=QPt_NewPosition;
   _MakePixel(_QPt_CurrentPosition, Qt::white);
   update(); // updateGL()?
}



inline int round(const float a) {return int (a + 0.5);}

//Check for special line cases, then draws line appropriately
void OpenGLCanvas::lineBres(int x0, int y0, int xEnd, int yEnd)
{
    if(x0 == xEnd)
    {
        if (y0 > yEnd)
            swap (y0, yEnd);

        for (int i = y0; i <= yEnd; i++)
            _MakePixel(QPoint(x0, i), Qt::white);
    }
    else if(y0 == yEnd)
    {
        if (x0 > xEnd)
            swap(x0, xEnd);

        for (int i = x0; i <= xEnd; i++)
            _MakePixel(QPoint(i, y0), Qt::white);
    }
    else
        lineBresDraw(x0, y0, xEnd, yEnd);
}


/* Bresenham for any slope*/
void OpenGLCanvas::lineBresDraw(int x0, int y0, int xEnd, int yEnd)
{
    /*Find if m > 1 or < 1. Swap if needed.*/
    bool steep = fabs(yEnd - y0) > fabs(xEnd - x0);
    if (steep)
    {
        swap(x0, y0);
        swap(xEnd, yEnd);
    }
    if (x0 > xEnd)
    {
        swap(x0, xEnd);
        swap(y0, yEnd);
    }

    /*Setup Bresenham values */
    int dx = fabs(xEnd - x0), dy = fabs(yEnd - y0);
    int p = 2 * dy - dx;
    int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
    int yDirection;
    int x = x0, y = y0;

    /*Is it positive slope or negative slope?*/
    if(y0 < yEnd)
        yDirection = 1;
    else
        yDirection = -1;


    if (steep)
        _MakePixel(QPoint(y, x), Qt::white);
    else
        _MakePixel(QPoint(x, y), Qt::white);

    /*Draw points on the line till endpoint*/
    while (x < xEnd)
    {
        x++;
        if (p < 0)
            p += twoDy;
        else
        {
            y += yDirection;
            p += twoDyMinusDx;
        }

        if (steep)
            _MakePixel(QPoint(y, x), Qt::white);
        else
            _MakePixel(QPoint(x, y), Qt::white);
    }
}

void OpenGLCanvas::drawScene(Scene myScene)
{
    int temp;
    color currColor;
    float value = -2*from.z * tan(angle);
    float leftx = from.x - (value/2);
    float lefty = from.y - (value/2);
    //float value = right - left;
    float X, Y;

    for (int y = 0; y < myScene.sizey; y++)
    {
        temp = ((float)y / (float)myScene.sizey) * 100;
        emit progress(temp);

        Y = lefty + value * (float)y / (float)(myScene.sizey - 1);

        for (int x = 0; x < myScene.sizex; x++)
        {
            X = leftx + value*(float)x / (float)(myScene.sizex - 1);

//            if (x == 160 && y == 160)
//                int debug = 0;
            point temper = {X, Y, 0};
            Vector dir = temper - from;

            float temp2 = dir * dir;
            temp2 = 1.0f / sqrtf(temp2);
            dir = temp2 * dir;

          // Vector temp = findRay(x, y, pi/8);
            ray viewRay = { temper, dir, false};
           currColor = rayTrace(myScene, viewRay, 0);
           _MakePixel(QPoint(x,y), min((int)(currColor.red * 255), 255), min((int)(currColor.green*255), 255), min((int)(currColor.blue*255), 255), 255);
       }
    }
    emit progress(100);
}

color OpenGLCanvas::rayTrace(Scene myScene, ray viewRay, int depth)
{
    float t = 2000.0f;
    int currentSphere= -1;
    color currColor = {0.0, 0.0, 0.0};
    char hitObj;
    point newStart;
    Vector n;
    texture currentText;

    for (unsigned int i = 0; i < myScene.sphereContainer.size(); i++)
    {
        if (hitSphere(viewRay, myScene.sphereContainer[i], t))
        {
      //      if (depth == 0)
        //        int debug;
            currentSphere = i;
            hitObj = 's';

        }
    }

    for (unsigned int i = 0; i < myScene.planeContainer.size(); i++)
    {
        if (hitPlane(viewRay, myScene.planeContainer[i], t))
        {
            currentSphere = i;
            hitObj = 'p';
        }
    }

    if (currentSphere == -1)
    {
        return currColor;
    }

    if (hitObj == 's')
    {
    newStart = viewRay.start + t * viewRay.dir;
    n = newStart - myScene.sphereContainer[currentSphere].pos;
    float temp = n * n;
    temp = 1.0f / sqrtf(temp);
    n = temp * n;

    currentText = myScene.textureContainer[myScene.sphereContainer[currentSphere].textureID];
/*
    for (unsigned int j = 0; j < myScene.lightContainer.size(); ++j)
    {
         light current = myScene.lightContainer[j];
         Vector dist = current.pos - newStart;
         if (n * dist <= 0.0f)
             continue;
         float t = sqrtf(dist * dist);
         if ( t <= 0.0f )
             continue;
         ray lightRay;
         lightRay.start = newStart;
         lightRay.dir = (1/t) * dist;
         // computation of the shadows
         bool inShadow = false;
         for (unsigned int i = 0; i < myScene.sphereContainer.size(); ++i)
         {
             if (hitSphere(lightRay, myScene.sphereContainer[i], t)) {
                 inShadow = true;
                 break;
             }
         }
         for (unsigned int i = 0; i < myScene.planeContainer.size(); i++)
         {
             if (hitPlane(lightRay, myScene.planeContainer[i], t)){
                 inShadow = true;
                 break;
             }
         }
         if (!inShadow)
         {
             float diffuse = (lightRay.dir * n);
             float reflet = 2.0f * (lightRay.dir * n);
             Vector phongDir = lightRay.dir - reflet * n;
             float specular = max(phongDir * viewRay.dir, 0.0f);
             specular = currentText.spec * powf(specular, phongConst);
             currColor.red += specular * current.red + diffuse * current.red * currentText.red;
             currColor.green += specular * current.green + diffuse * current.green * currentText.green;
             currColor.blue += specular * current.blue + diffuse * current.blue * currentText.blue;

         }
     }

    currColor.red += ambient.red * currentText.red;
    currColor.green += ambient.green * currentText.green;
    currColor.blue += ambient.blue * currentText.blue;

    if (depth < maxDepth)
    {
        float fViewProjection = viewRay.dir * vNormal;
        fCosThetaI = fabsf(fViewProjection);
        fSinThetaI = sqrtf(1 - fCosThetaI * fCosThetaI);
        fSinThetaT = (fDensity1 / fDensity2) * fSinThetaI;
        if (!(fSinThetaT * fSinThetaT > 0.9999f))
        {

        }


        float reflet = 2.0f * (viewRay.dir * n);
        viewRay.start = newStart;
        viewRay.dir = viewRay.dir - reflet * n;
        currColor += currentText.reflection * rayTrace(myScene, viewRay, depth+1);
  //      currColor += currentText.refraction * rayTrace(myScene, )
    }*/
}
    else if (hitObj == 'p')
    {
        newStart = viewRay.start + t * viewRay.dir;
        n = myScene.planeContainer[currentSphere].dir;

        currentText = myScene.textureContainer[myScene.planeContainer[currentSphere].textureID];
    }
        for (unsigned int j = 0; j < myScene.lightContainer.size(); ++j)
        {
             light current = myScene.lightContainer[j];
             Vector dist = current.pos - newStart;
             if (n * dist <= 0.0f)
                 continue;
             float t = sqrtf(dist * dist);
             if ( t <= 0.0f )
                 continue;
             ray lightRay;
             lightRay.start = newStart;
             lightRay.dir = (1/t) * dist;
             // computation of the shadows
             bool inShadow = false;
             for (unsigned int i = 0; i < myScene.sphereContainer.size(); ++i)
             {
                 if (hitSphere(lightRay, myScene.sphereContainer[i], t)) {
                     inShadow = true;
                     break;
                 }

             }
             for (unsigned int i = 0; i < myScene.planeContainer.size(); i++)
             {
                 if (hitPlane(lightRay, myScene.planeContainer[i], t)){
                     inShadow = true;
                     break;
                 }
             }
             if (!inShadow)
             {
                 float diffuse = (lightRay.dir * n);
                 float reflet = 2.0f * (lightRay.dir * n);
                 Vector phongDir = lightRay.dir - reflet * n;
                 float specular = max(phongDir * viewRay.dir, 0.0f);
                 specular = currentText.spec * powf(specular, phongConst);
                 currColor.red += specular * current.red + diffuse * current.red * currentText.red;
                 currColor.green += specular * current.green + diffuse * current.green * currentText.green;
                 currColor.blue += specular * current.blue + diffuse * current.blue * currentText.blue;

             }
         }

        currColor.red += ambient.red * currentText.red;
        currColor.green += ambient.green * currentText.green;
        currColor.blue += ambient.blue * currentText.blue;

        if (depth < maxDepth)
        {
        /*    ray tr;
            Vector incident = -1 * viewRay.dir;
            float N;
            if (viewRay.in)
            {
                N = currentText.n;
                n = -1 * n;
            }
            else
                N = 1 / currentText.n;

            float c1 = (-1 * incident)*n;
            float c2;

            c2 = 1 - N*N*(1 - c1*c1);
            if(c2 > 0.0 && refraction)
            {
                    c2 = sqrt(c2);
                    tr.dir = N*incident + (N*c1 - c2)*n;
                    float temp2 = tr.dir*tr.dir;
                    temp2 = 1.0f / sqrtf(temp2);
                    tr.dir = temp2 * tr.dir;
                    tr.start = newStart;
                    if (viewRay.in)
                        tr.in = false;
                    else
                        tr.in = true;

                    color debugColor = currentText.refraction * rayTrace(myScene, tr, depth+1);
                    if (debugColor.red > 0 || debugColor.green > 0 || debugColor.blue > 0)
                        currColor += debugColor;
            }
*/

            ray tr;
            float N;
            if (viewRay.in)
            {
                N = currentText.n;
                n = -1 * n;
            }
            else
                N = 1/ currentText.n;

            float cosI = n*viewRay.dir;
            float sinT2 = N * N * (1.0 - cosI * cosI);
            if (!(sinT2 > 1.0) && refraction)
            {
                tr.dir = N * viewRay.dir - (N + sqrt(1 - sinT2)) * n;
                tr.start = newStart;
                if (viewRay.in)
                    tr.in = false;
                else
                    tr.in = true;

                currColor += currentText.refraction * rayTrace(myScene, tr, depth+1);
            }

            float reflet = 2.0f * (viewRay.dir * n);
            viewRay.start = newStart;
            viewRay.dir = viewRay.dir - reflet * n;
            currColor += currentText.reflection * rayTrace(myScene, viewRay, depth+1);
        }




    return currColor;
}

bool OpenGLCanvas::hitSphere(const ray &r, const sphere &s, float &t)
{
   Vector dist = s.pos - r.start;
   float B = r.dir * dist;
   float D = B*B - dist * dist + s.size * s.size;
   if (D < 0.0f)
       return false;
   float t0 = B - sqrtf(D);
   float t1 = B + sqrtf(D);
   bool retvalue = false;
   if ((t0 > 0.1f) && (t0 < t))
   {
       t = t0;
       retvalue = true;
   }
   if ((t1 > 0.1f) && (t1 < t))
   {
       t = t1;
       retvalue = true;
   }
   return retvalue;
}

color OpenGLCanvas::directPhong(Vector l, Vector n, Vector v, light I, texture t)
{
    color c;

    float reflet = 2.0f * (n*l);
    Vector r = reflet * n - l;

    c.red = I.red * (t.red * (l*n) + t.spec * pow((r*v),phongConst));
    c.green = I.green * (t.green * (l*n) + t.spec * pow((r*v),phongConst));
    c.blue = I.blue * (t.blue * (l*n) + t.spec * pow((r*v),phongConst));

    return c;
}

Vector OpenGLCanvas::findRay(int x, int y, float angle)
{
    float tempAngleX, tempAngleY;
    float tempX, tempY;

    tempX = (float)(x - (width() / 2));
    tempY = (float)(y - (height() / 2));
    tempAngleX = tempX/width() * angle;
    tempAngleY = tempY/height() * angle;
    tempX = tan(tempAngleX);
    tempY = tan(tempAngleY);

    Vector temp = {tempX, tempY, 1};
    float t = temp * temp;

    return ((1/t)*temp);

}

bool OpenGLCanvas::hitPlane(const ray &r, const plane &p, float &t)
{
    bool retvalue = false;
    float Vd, V0, t0;

    Vd = p.dir * r.dir;
    if (Vd >= 0 )
        return false;

    V0 = -((p.dir.x*r.start.x + p.dir.y*r.start.y + p.dir.z*r.start.z) + p.D);
    t0 = V0/Vd;
    if (t0 < 0)
        return false;

    if (t0 < t)
    {
        t = t0;
        retvalue = true;
    }
    return retvalue;
}
