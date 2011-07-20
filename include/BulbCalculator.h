/*
Copyright (C) 2011  Gianluca Montecchi <gian@grys.it>

This file is part of BulbCalculator.

BulbCalculator is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 2 of the License, or (at your
option) any later version.

BulbCalculator is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with BulbCalculator.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BULBCAL_H
#define BULBCAL_H

#include "../ui_MainWindow.h"
#include "../include/nacafoil.h"
#include "QTableWidget"

#include <QGLWidget>
#include <QGraphicsView>
#include <QGLViewer/qglviewer.h>
#include <QGroupBox>
#include <QLabel>

#define VERSION     "1.9.4"

#define UNIT_MM      0
#define UNIT_INCH    1
#define UNIT_INCH_F  2

#define DIST_EVEN   0
#define DIST_COS    1

#define SIDE_VIEW   0
#define TOP_VIEW    1

class Vista3D;
class ViewArea;

class BulbCalculator : public QMainWindow
{
    Q_OBJECT
    
    public:
        BulbCalculator(QMainWindow *form=0);
        void DrawView();
        void UpdateResults();
        void UpdateCalcs();
        void SetImp();
        void SetMetric();
        QString GetSectionData(int SectNum);
        QString GetSectionValue(int SectNum, int val);
        QActionGroup *Resolution3D;

// Bulb Values
        profile naca_profile;
        profile_data data_profile;
        float target_weight;
        float material_density;
        float bulb_volume;
        float bulb_wet_surface;

// Bulb Options
        int num_sect;
        int sect_dist;  // 0 = Even, 1 = Cosine
        int units;       // 0 = Metric, 1 = imperial
        int slice;
        float slice_thickness;
        vector<double> sect_pos;
        double length;


    public slots:
        void Show3DAxis();
        void ShowGrid();
        void Open();
        void SetBulbParameter();
        void Set00xx();
        void Set63_0xx();
        void Set63A0xx();
        void Set640xx();
        void Set64A0xx();
        void Set650xx();
        void Set65A0xx();
        void Set660xx();
        void Set670xx();
        void Save();
        void SaveAs();
        void NewBulb();
        void PrintBulbSectionFromTop();
        void PrintBulbSectionFromSide();
        void PrintBulbLinesPlan();
        void PrintBulbData();
        void SetBulbDataOptions();
        void ShowTopWindow();
        void ShowSideWindow();
        void Show3dWindow();
        void ShowCalcWindow();
        void ShowDataWindow();
        void ImportFoilData();
        void ShowAbout();
        void DownloadUIUC();
        void ExportTextFile();
        void Set3DResolutionLow();
        void Set3DResolutionMedium();
        void Set3DResolutionHigh();
        void Set3DResolutionHighest();


    private:
        QMdiSubWindow *BulbCalculation;
        QTableWidget *TW_Bulb;
        QLabel *BulbName;
        QMdiSubWindow *BulbData;
        QTableWidget *TW_SubBulbDataGen;
        QTableWidget *TW_SubBulbDataSec;
        QGroupBox *gb1;
        QGroupBox *gb2;
        QMdiSubWindow *View3DWin;
        QMdiSubWindow *ViewTopWin;
        QMdiSubWindow *ViewSideWin;
        ViewArea *GV_TopView;
        ViewArea *GV_SideView;
        QWidget *widget3d;
        QWidget *wdCalc;
        QWidget *wdData;
        Vista3D *view3d;
        QGraphicsScene *scSide;
        QGraphicsScene *scTop;
        Ui::MainWindow ui;
        QString FileName;
        int Modified;
        QActionGroup *BulbMenu;
        int cosine;

        void SetDefaultValue();
        void CreateTopWin();
        void CreateSideWin();
        void Create3dWin();
        void CreateCalcWin();
        void CreateDataWin();
        void SetUnchecked();
        void ClearBulb();
        void FillBulbDataValue();



    protected:
        virtual void resizeEvent( QResizeEvent *event );

};

#endif

#ifndef GLWIDGET_H
#define GLWIDGET_H


class Vista3D : public QGLViewer
 {
     Q_OBJECT

    public:
        #if QT_VERSION < 0x040000
            Vista3D(QWidget *parent, const char *name);
        #else
            Vista3D(QWidget *parent);
        #endif
        ~Vista3D();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        void SetProfile();

        void SetShow3DAxis(bool status);
        void SetShowGrid(bool status);
        void SetBc(BulbCalculator *bcp);


    public slots:
        void setXRotation(int angle);
        void setYRotation(int angle);
        void setZRotation(int angle);
        void Set3DResolution(int res);
        

    signals:
        void xRotationChanged(int angle);
        void yRotationChanged(int angle);
        void zRotationChanged(int angle);

    protected:
        virtual void draw();
        virtual void init();

    private:
        GLuint makeObject();
        void normalizeAngle(int *angle);
        void drawEllipse(float x, float xdiam, float yradius, float yradius_l);
        void drawLine(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step);
        void DrawAxis();
        void DrawGrid();
        GLuint object;
        int xRot;
        int yRot;
        int zRot;
        int zMin;
        int zMax;
        QPoint lastPos;
        QColor qtGreen;
        QColor qtPurple;
        bool show_axis;
        bool show_grid;
        int res_divisor;
        BulbCalculator *bc;
        profile naca_profile;
        profile_data data_profile;
        float target_weight;
        float material_density;
        float bulb_volume;
        float bulb_wet_surface;
};

#endif

#ifndef VIEW_AREA
#define VIEW_AREA

class ViewArea : public QGraphicsView {
    Q_OBJECT
    public:
        ViewArea(QGraphicsScene *scene,  BulbCalculator *bcp, int type, QWidget *parent = 0);
        void DrawBulb();
        void UpdateView();
        profile naca_profile;
        profile_data data_profile;

    protected:
        void resizeEvent ( QResizeEvent * event );

    private:
        BulbCalculator *bc;
        void DrawAxis(void);
        int view;
        void DrawBulbTop();
        void DrawBulbSide();

};
#endif
