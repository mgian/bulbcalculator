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
#include <QComboBox>

#define VERSION     "2.2.0"

#define UNIT_MM      0
#define UNIT_INCH    1
#define UNIT_INCH_F  2

#define DIST_EVEN   0
#define DIST_COS    1

#define SIDE_VIEW   0
#define TOP_VIEW    1
#define PLAN_VIEW   2

#define NO      0
#define YES     1

#define MDI     0
#define TABBED  1

#define RES_LOW         30
#define RES_MED         15
#define RES_HIGH         8
#define RES_HIGHEST      1


#define MAXRECENTFILE   5

enum WF_RES {
    WF_LOW,
    WF_MEDIUM,
    WF_HIGH,
    WF_HIGHEST
};

enum VIEWMODE_3D {
    WIREFRAME,
    TRIANGLE,
    SURFACE
};

class Vista3D;
class ViewArea;
class BulbCalcPref;
class BulbCalcStatus;

class BulbCalculator : public QMainWindow
{
    Q_OBJECT
    
    public:
        BulbCalculator(QMainWindow *form=0);
        void UpdateCalculations();
        void UpdateResults();
        void UpdateCalcs();
        void SetImp();
        void SetMetric();
        QString GetSectionData(int SectNum);
        QString GetSectionValue(int SectNum, int val);
        QActionGroup *Resolution3D;
        BulbCalcPref *BcPrefs;
        BulbCalcStatus * BcStatus;

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
        //int units;       // 0 = Metric, 1 = imperial
        int slice;
        float slice_thickness;
        vector<double> sect_pos;
        double length;

// Printer options
        int page_orientation;
        int page_size;

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
        void PrintBulbLinesPlanFromTop();
        void PrintBulbLinesPlanFromSide();
        void PageSetup();
        void PrintBulbData();
        void SetBulbDataOptions();
        void Show2dWindow();
        void Show3dWindow();
        void ShowDataWindow();
        void ImportFoilData();
        void SetFoilProfile(QString ProfName);
        void ShowAbout();
        void DownloadUIUC();
        void ExportTextFile();
        void ShowPrefWindow();
        void Change3DResolution(int CurRes);
        void OpenRecentFile();
        void Set3dViewMode(int mode3d);

        void SetCurrentFile(const QString &ProjectName);

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
        QMdiSubWindow *View2DWin;
        ViewArea *GV_2DView;
        QWidget *widget3d;
        QWidget *wdCalc;
        QWidget *wdData;
        Vista3D *view3d;
        QGraphicsScene *scSide;
        QGraphicsScene *scTop;
        QGraphicsScene *sc2d;
        QToolBar *tb;
        QComboBox *res3d;
        QComboBox *profs;
        QComboBox *view3dMode;
        Ui::MainWindow ui;
        int Modified;
        QActionGroup *BulbMenu;
        int cosine;

        void SetDefaultValue();
        void Create3dWin();
        void Create2dWin();
        void CreateDataWin();
        void ClearBulb();
        void FillBulbDataValue();
        void WritePrefereces();
        void WritePagePreferences();

        // Preferences functions
        void ReadPreferences();
        void ReadGuiPreferences();
        void ReadBulbPreferences();
        void ReadRepoPreferences();
        void ReadPrinterPreferences();
        void UpdateRecentFileActions();
        QString StrippedName(const QString &ProjectFile);
        QAction *RecentProjects[MAXRECENTFILE];
        QAction *separatorAct;

        void LoadFile();

    protected:
        virtual void resizeEvent( QResizeEvent *event );
        virtual void closeEvent(QCloseEvent *event);
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
        void SetXRotation(int angle);
        void SetYRotation(int angle);
        void SetZRotation(int angle);
        void Set3DResolution(int res);
        void Set3dViewMode(int mode);
        

    signals:
        void xRotationChanged(int angle);
        void yRotationChanged(int angle);
        void zRotationChanged(int angle);

    protected:
        virtual void draw();
        virtual void init();

    private:
        void normalizeAngle(int *angle);
        void DrawEllipse(float x, float xdiam, float yradius, float yradius_l);
        void DrawLine(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step);
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
        int mode_3d_view;
};

#endif

#ifndef VIEW_AREA
#define VIEW_AREA

class ViewArea : public QGraphicsView {
    Q_OBJECT
    public:
        ViewArea(QGraphicsScene *scene,  BulbCalculator *bcp, int type, QWidget *parent = 0);
        void UpdateView();
        profile naca_profile;
        profile_data data_profile;

    protected:
        void resizeEvent ( QResizeEvent * event );

    private:
        BulbCalculator *bc;
        void DrawAxisTopSide(long wr, long hr, long origin);
        void DrawAxisFront(long wr, long hr, long origin, long wl);
        void DrawBulbTop(long hl, long Origin_Top);
        void DrawBulbSide(long hl, long Origin_Side);
        void DrawBulbFront(long Origin_X, long Origin_Top);
        void DrawText(long w, long h);
        int view;

};
#endif

#ifndef BULB_CALC_PREF
#define BULB_CALC_PREF

class BulbCalcPref {

    public:
        /* Gui Preferences */
        int Gui_BcViewMode;
        int Gui_TabPos;
        int Gui_Unit;

        /* Bulb Preferences */
        float Bulb_Tw;
        float Bulb_Md;
        int Bulb_Ds;
        double Bulb_St;
        int Bulb_Hrl;
        int Bulb_Whr;

        /* Repo Preferences */
        QString LocalRepo;
        QStringList RemoteRepo;

        /* Page Size */
        int PaperSize;
        int PageOrientation;

};

class BulbCalcStatus {

    public:
        int St_Modified;
        int St_CanSave;
        QString CurFile;

};


class BulcDataDef {
    int num_sect;
    int sect_dist;  // 0 = Even, 1 = Cosine
    int slice;
    float slice_thickness;
    vector<double> sect_pos;
    double length;

};

#endif
