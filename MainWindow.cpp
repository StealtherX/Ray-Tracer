#include "MainWindow.h"


#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGLFormat>
#include <QMenuBar>  // <QMenu>, <QAction>
#include <QMessageBox>
#include <QMessageBox>
#include <QString>
#include <fstream>
#include <stdlib.h>
#include <cmath>

#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget* pQW_Parent)
   : QMainWindow(pQW_Parent)
{
   // Check OpenGL support
   _CheckOpenGL();

   // Create GUIs
   myGui = new Ui::mainWindowGui();
   myGui->setupUi(this);
   fileIsOpen = false;


}

void MainWindow::_CheckOpenGL()
{
   // Check OpenGL support
   if(!QGLFormat::hasOpenGL())
   {
      qCritical("Error: This program requires OpenGL!");

      exit(ERR_OPENGL_NOT_SUPPORTED);
   }
}


void MainWindow::closeEvent(QCloseEvent* pQCE)
{
   QMessageBox::StandardButton QMB_SB_Choice=QMessageBox::warning(this, "ECS-175 Project 1", "Do you really want to quit this application?", QMessageBox::Yes|QMessageBox::No);

   if(QMessageBox::Yes==QMB_SB_Choice)
   {
      pQCE->accept();

      _Exit();
   }
   // else if(QMessageBox::No==QMB_SB_Choice)
   {
      pQCE->ignore();

      return;
   }
}

void MainWindow::_Exit()
{
   qApp->quit();
}

void MainWindow::_FileOpen()
{
   // TODO: More than one file?
   const QString& kQStr_FileName=QFileDialog::getOpenFileName(this, "Select a data file", ".", "Datasets (*.3d)");
   sphere temp;
   texture tempText;
   light tempLight;
   plane tempPlane;
   ifstream myfile;
   //char tempCh = 'A';

   if(!kQStr_FileName.isEmpty())
   {
      //emit openTheFile(kQStr_FileName);

   }
   else // i.e. cQStr_FileName.isEmpty()
   {
      qWarning("Warning: No file selected!");
   } // if(!kQStr_FileName.isEmpty())




   name = kQStr_FileName.toStdString();

   myfile.open(name.c_str());

   if (myfile.fail())
   {
       QMessageBox::warning(this, tr("Warning!"), tr("Cannot read file."));
       return;
   }

   mainScene = new Scene(myGui->graphics->width(), myGui->graphics->height());

   myfile >> numObj;

   myGui->comboBox_3->clear();

   for (int i = 0; i < numObj; i++)
   {
        myfile >> temp.pos.x >> temp.pos.y >> temp.pos.z;
        myfile >> temp.size;
        myfile >> temp.textureID;

        mainScene->insertSphere(temp);
        myGui->comboBox_3->insertItem(i,QString("%1").arg(i+1), i);
   }

   myfile >> numPlanes;

   for (int i = 0; i < numPlanes; i++)
   {
       myfile >> tempPlane.dir.x >> tempPlane.dir.y >> tempPlane.dir.z >> tempPlane.D;
       myfile >> tempPlane.textureID;

       mainScene->insertPlane(tempPlane);
   }

   myfile >> numTextures;
   myGui->spinBox->setMaximum(numTextures - 1);
    myGui->comboBox->clear();

   for (int i = 0; i < numTextures; i++)
   {
       myfile >> tempText.red >> tempText.green >> tempText.blue;
       myfile >> tempText.reflection >> tempText.refraction >> tempText.spec >> tempText.n;

       mainScene->insertTexture(tempText);
       myGui->comboBox->insertItem(i, QString("%1").arg(i), i);
   }

   myfile >> numLights;
    myGui->comboBox_2->clear();

   for (int i = 0; i < numLights; i++)
   {
       myfile >> tempLight.pos.x >> tempLight.pos.y >> tempLight.pos.z;
       myfile >> tempLight.red >> tempLight.green >> tempLight.blue;

       mainScene->insertLight(tempLight);
       myGui->comboBox_2->insertItem(i, QString("%1").arg(i+1), i);
   }


   fileIsOpen = true;


   myfile.close();
   //redraw();
}

void MainWindow::writeFile()
{
    ofstream myFile;

    myFile.open(name.c_str());

    if (myFile.fail())
    {
        QMessageBox::warning(this, tr("Warning!"), tr("Cannot write file."));
        return;
    }

    myFile << numObj << endl;
    for (int i = 0; i < numObj; i++)
    {

    }


    myFile.close();
}

/*void MainWindow::CanvasMousePositionChanged(QPoint QP_NewPosition)
{

}
*/
void MainWindow::CanvasSizeChanged(QSize QS_NewSize)
{
   //const int kiNewWidth=QS_NewSize.width();
   //const int kiNewHeight=QS_NewSize.height();
    if (fileIsOpen)
    {
    mainScene->sizex = QS_NewSize.width();
    mainScene->sizey = QS_NewSize.height();
    }
    //redraw();
}

/*void MainWindow::selectObj(int x)
{
    obj[currObj].active = false;
    currObj = x;
    updateList();
    updateKnots();
    updateOrder();
    obj[currbj].active = true;
    redraw();
}
*/
void MainWindow::updateList()
{
    QString temp;
    //myGui->listWidget->clear();

    /*for (int i = 0; i < (int)obj[currObj].controlPoints.size(); i++)
        myGui->listWidget->addItem(QString("%1: X: %2 Y: %3").arg(i).arg(obj[currObj].controlPoints[i].x)
                                   .arg(obj[currObj].controlPoints[i].y));

*/}


void MainWindow::redraw()
{

    myGui->graphics->blackout();

    if (fileIsOpen)
    {
        myGui->graphics->drawScene(*mainScene);
    }
    emit refresh();
}

void MainWindow::changeTexture(int i)
{
    texture temp = mainScene->textureContainer[i];

    myGui->doubleSpinBox->setValue(temp.red);
    myGui->doubleSpinBox_2->setValue(temp.green);
    myGui->doubleSpinBox_3->setValue(temp.blue);
    myGui->doubleSpinBox_4->setValue(temp.reflection);
    myGui->doubleSpinBox_5->setValue(temp.refraction);
    myGui->doubleSpinBox_6->setValue(temp.spec);
    myGui->doubleSpinBox_7->setValue(temp.n);
}

void MainWindow::applyTexture()
{
    int i = myGui->comboBox->currentIndex();

    mainScene->textureContainer[i].red = myGui->doubleSpinBox->value();
    mainScene->textureContainer[i].green = myGui->doubleSpinBox_2->value();
    mainScene->textureContainer[i].blue = myGui->doubleSpinBox_3->value();
    mainScene->textureContainer[i].reflection = myGui->doubleSpinBox_4->value();
    mainScene->textureContainer[i].refraction = myGui->doubleSpinBox_5->value();
    mainScene->textureContainer[i].spec = myGui->doubleSpinBox_6->value();
    mainScene->textureContainer[i].n = myGui->doubleSpinBox_7->value();

}

void MainWindow::changeLight(int i)
{
    light temp = mainScene->lightContainer[i];

    myGui->doubleSpinBox_8->setValue(temp.pos.x);
    myGui->doubleSpinBox_9->setValue(temp.pos.y);
    myGui->doubleSpinBox_10->setValue(temp.pos.z);
    myGui->doubleSpinBox_11->setValue(temp.red);
    myGui->doubleSpinBox_12->setValue(temp.green);
    myGui->doubleSpinBox_13->setValue(temp.blue);

}

void MainWindow::applyLight()
{
    int i = myGui->comboBox_2->currentIndex();

    mainScene->lightContainer[i].pos.x = myGui->doubleSpinBox_8->value();
    mainScene->lightContainer[i].pos.y = myGui->doubleSpinBox_9->value();
    mainScene->lightContainer[i].pos.z = myGui->doubleSpinBox_10->value();
    mainScene->lightContainer[i].red = myGui->doubleSpinBox_11->value();
    mainScene->lightContainer[i].green = myGui->doubleSpinBox_12->value();
    mainScene->lightContainer[i].blue = myGui->doubleSpinBox_13->value();
}

void MainWindow::changeSphere(int i)
{
    sphere temp = mainScene->sphereContainer[i];

    myGui->doubleSpinBox_14->setValue(temp.pos.x);
    myGui->doubleSpinBox_15->setValue(temp.pos.y);
    myGui->doubleSpinBox_16->setValue(temp.pos.z);
    myGui->doubleSpinBox_17->setValue(temp.size);
    myGui->spinBox->setValue(temp.textureID);
}

void MainWindow::applySphere()
{
    int i = myGui->comboBox_3->currentIndex();

    mainScene->sphereContainer[i].pos.x = myGui->doubleSpinBox_14->value();
    mainScene->sphereContainer[i].pos.y = myGui->doubleSpinBox_15->value();
    mainScene->sphereContainer[i].pos.z = myGui->doubleSpinBox_16->value();
    mainScene->sphereContainer[i].size = myGui->doubleSpinBox_17->value();
    mainScene->sphereContainer[i].textureID = myGui->spinBox->value();
}

void MainWindow::updateAmbient()
{
    myGui->graphics->ambient.red = myGui->doubleSpinBox_18->value();
    myGui->graphics->ambient.green = myGui->doubleSpinBox_19->value();
    myGui->graphics->ambient.blue = myGui->doubleSpinBox_20->value();
}

void MainWindow::updateDepth(int i)
{
    myGui->graphics->maxDepth = i;
}

void MainWindow::updatePhong(int i)
{
    myGui->graphics->phongConst = i;
}

void MainWindow::updateRef(bool i)
{
    myGui->graphics->refraction = i;
}

void MainWindow::updateCamera()
{
    myGui->graphics->from.x = myGui->doubleSpinBox_21->value();
    myGui->graphics->from.y = myGui->doubleSpinBox_22->value();
    myGui->graphics->from.z = myGui->doubleSpinBox_23->value();
    myGui->graphics->angle = myGui->doubleSpinBox_24->value();
}
