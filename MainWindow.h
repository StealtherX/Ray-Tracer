#ifndef __MainWindow_h__
#define __MainWindow_h__


#include <QMainWindow>
#include <vector>
#include <string.h>
using namespace std;

#include "struct.h"
#include "scene.h"


class QAction;


#define ERR_OPENGL_NOT_SUPPORTED 1

namespace Ui{
    class mainWindowGui;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
      MainWindow(QWidget* pQW_Parent=NULL);


   signals:
    void refresh();


   protected:
      void closeEvent(QCloseEvent* pQCE);
      void updateList();


   private slots:
      void _FileOpen();
      void _Exit();
      void writeFile();
      //void CanvasMousePositionChanged(QPoint QP_NewPosition);
      void CanvasSizeChanged(QSize QS_NewSize);
      //void selectObj(int x);
      void redraw();
      void changeTexture(int i);
      void applyTexture();
      void changeLight(int i);
      void applyLight();
      void changeSphere(int i);
      void applySphere();
      void updateAmbient();
      void updateDepth(int i);
      void updatePhong(int i);
      void updateRef(bool i);
      void updateCamera();



   private:
      void _CheckOpenGL();

      Ui::mainWindowGui *myGui;
      int numObj;
      int numTextures;
      int numLights;
      int numPlanes;
      Scene *mainScene;
      string name;
      int currObj;
      bool fileIsOpen;


      // Actions
      QAction* _pQA_File_Open;
      QAction* _pQA_File_Exit;
};

#endif // __MainWindow_h__
