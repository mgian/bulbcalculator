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

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QtOpenGL>

#include <math.h>

#include "../include/BulbCalculator.h"
#include "../include/NacaFoil.h"


#define PI = 3.14159265358979323846
const float DEG2RAD = 3.14159/180;

void Vista3D::SetShow3DAxis(bool status) {

    this->show_axis = status;
    Vista3D::draw();

}

void Vista3D::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;
    Vista3D::draw();
}


void Vista3D::SetShowGrid(bool status) {

    this->show_grid = status;
    Vista3D::draw();

}

void Vista3D::SetProfile() {

    Vista3D::draw();

}

Vista3D::Vista3D(QWidget *parent) : QGLViewer(parent) {

    this->show_axis = true;
    this->show_grid = true;
    this->res_divisor = RES_LOW;
    this->l_res_divisor = L_RES_LOW;
    this->xRot = 0;
    this->yRot = 0;
    this->zRot = 0;
    this->zMin = 0;
    this->zMax = 0;
    this->bc = NULL;
}

Vista3D::~Vista3D() {

    makeCurrent();
    glDeleteLists(object, 1);

 }

QSize Vista3D::minimumSizeHint() const {

    return QSize(50, 50);

}

QSize Vista3D::sizeHint() const {

    return QSize(400, 400);

}


void Vista3D::SetXRotation(int angle) {

    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
    }

}


void Vista3D::SetYRotation(int angle) {

    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
      emit yRotationChanged(angle);
     }

}


void Vista3D::SetZRotation(int angle) {

    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
    }

}


void Vista3D::init() {

  // Restore previous viewer state.
  //restoreStateFromFile();

}

void Vista3D::DrawGrid() {

    glColor3f(1.0,1.0,1.0);
    this->drawGrid(1.2, 12);

}

void Vista3D::DrawAxis() {

    this->drawAxis(1.3);

}


void Vista3D::DrawWireframe() {

    long w, mult;
    double step;
    QSize t;
    double x;
    int in = 0;
    float p;

    float pos[4] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);


    this->naca_profile.HLRatio = this->bc->naca_profile.HLRatio;
    this->naca_profile.WHRatio = this->bc->naca_profile.WHRatio;
    this->naca_profile.foil_name.assign(this->bc->naca_profile.foil_name.c_str());
    this->naca_profile.calc();

    w = 2.0;
    glColor3f(1.0,1.0,1.0);

    mult = this->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);

    zMin = 0;
    zMax = w;


    x = 0.0;

    profile_data& pd(this->bc->naca_profile[(unsigned)((double)0.02*mult)]);
    Vista3D::DrawEllipse((0.02-2.0/2), (pd.width*2), pd.height_u, pd.height_l);

    for(int i=0; i<mult; i++) {
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        x = x + step;
        if (i%this->l_res_divisor == 0) {
            Vista3D::DrawEllipse((x-2.0/2), (pd.width*2), pd.height_u, pd.height_l);
        }
    }
    glColor3f(1.0,1.0,0.2);
    x = 0.0;



    for(int i=0; i<mult; i++) {
        if (i == 0) {
            p = 0.02;
            in = 1;
        } else {
            p = i;
            in = i + 1;
        }
        profile_data& pdi(this->bc->naca_profile[(unsigned)((double)p*mult)]);
        profile_data& pde(this->bc->naca_profile[(unsigned)((double)(in)*mult)]);
        x = x + step;
        Vista3D::DrawLine((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step);

    }

    glEnd();

}

void Vista3D::DrawTriangle() {

    long w, mult;
    double step;
    QSize t;
    double x;
    QColor Col;

    float pos[4] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    this->naca_profile.HLRatio = this->bc->naca_profile.HLRatio;
    this->naca_profile.WHRatio = this->bc->naca_profile.WHRatio;
    this->naca_profile.foil_name.assign(this->bc->naca_profile.foil_name.c_str());
    this->naca_profile.calc();

    w = 2.0;
    glColor3f(1.0,1.0,1.0);

    mult = this->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);

    zMin = 0;
    zMax = w;


    x = 0.0;

    profile_data& pdi(this->bc->naca_profile[(unsigned)((double)0.02*mult)]);
    profile_data& pde(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    x = x + step;
    Vista3D::DrawTrianglesFilled((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step, QColor(Qt::darkGray));
    Vista3D::DrawTrianglesBorder((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step, QColor(Qt::black));

    for(int i=1; i<mult; i++) {
        if (i%2 == 0) {
            Col = Qt::darkGray;
        } else {
            Col = Qt::lightGray;
        }
        profile_data& pdi(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        profile_data& pde(this->bc->naca_profile[(unsigned)((double)(i+1)*mult)]);
        x = x + step;
        Vista3D::DrawTrianglesFilled((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step,  Col);
        Vista3D::DrawTrianglesBorder((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step,  QColor(Qt::black));

    }


}

void Vista3D::DrawSurface() {

    long w, mult;
    double step;
    QSize t;
    double x;
    QColor Col;

    float pos[4] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    this->naca_profile.HLRatio = this->bc->naca_profile.HLRatio;
    this->naca_profile.WHRatio = this->bc->naca_profile.WHRatio;
    this->naca_profile.foil_name.assign(this->bc->naca_profile.foil_name.c_str());
    this->naca_profile.calc();

    w = 2.0;
    glColor3f(1.0,1.0,1.0);

    mult = this->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);

    zMin = 0;
    zMax = w;


    x = 0.0;

    profile_data& pdi(this->bc->naca_profile[(unsigned)((double)0.02*mult)]);
    profile_data& pde(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    x = x + step;
    Vista3D::DrawTrianglesFilled((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step, QColor(Qt::lightGray));

    for(int i=1; i<mult; i++) {
        profile_data& pdi(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        profile_data& pde(this->bc->naca_profile[(unsigned)((double)(i+1)*mult)]);
        x = x + step;
        Vista3D::DrawTrianglesFilled((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step,  QColor(Qt::lightGray));
    }


}

void Vista3D::draw() {

    if (show_axis == true) {
        this->DrawAxis();
    }

    if (show_grid == true) {
        this->DrawGrid();
    }

    switch (this->mode_3d_view) {
        case WIREFRAME:
            Vista3D::DrawWireframe();
            break;
        case TRIANGLE:
            Vista3D::DrawTriangle();
            break;
        case SURFACE:
            Vista3D::DrawSurface();
            break;
    }

}

void Vista3D::DrawTrianglesFilled(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QColor color) {

    float diam_i, diam_e, ecc_i, ecc_e;
    int gr;
    diam_i = yradiusi_u - yradiusi_l;
    diam_e = yradiuse_u - yradiuse_l;
    ecc_i = yradiusi_u - (diam_i/2.0);
    ecc_e = yradiuse_u - (diam_e/2.0);
    gr = 360/this->res_divisor;
    gr = 360/gr;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBegin(GL_TRIANGLES);

    glColor3ub(color.red(), color.green(), color.blue());
    for (int i=0; i < 360;  i=i + this->res_divisor) {
        float degInRad = i*DEG2RAD;
        float degInRad1 = (i+gr)*DEG2RAD;
        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
        glVertex3f(x+step, cos(degInRad1)*xradiuse,(sin(degInRad1)*diam_e)+ecc_e);
        glVertex3f(x+step, cos(degInRad)*xradiuse,(sin(degInRad)*diam_e)+ecc_e);

        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
        glVertex3f(x+step, cos(degInRad1)*xradiuse,(sin(degInRad1)*diam_e)+ecc_e);
        glVertex3f(x, cos(degInRad1)*xradius,(sin(degInRad1)*diam_i)+ecc_i);
    }
    glEnd();


}


void Vista3D::DrawTrianglesBorder(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QColor color) {

    float diam_i, diam_e, ecc_i, ecc_e;
    int gr;
    diam_i = yradiusi_u - yradiusi_l;
    diam_e = yradiuse_u - yradiuse_l;
    ecc_i = yradiusi_u - (diam_i/2.0);
    ecc_e = yradiuse_u - (diam_e/2.0);

    gr = 360/this->res_divisor;
    gr = 360/gr;
    glBegin(GL_LINE_LOOP);
    glColor3ub(color.red(), color.green(), color.blue());
    glLineWidth(0.5);
    for (int i=0; i < 360;  i=i + this->res_divisor) {
        float degInRad = i*DEG2RAD;
        float degInRad1 = (i+gr)*DEG2RAD;
        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
        glVertex3f(x+step, cos(degInRad1)*xradiuse,(sin(degInRad1)*diam_e)+ecc_e);
        glVertex3f(x+step, cos(degInRad)*xradiuse,(sin(degInRad)*diam_e)+ecc_e);
        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);

        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
        glVertex3f(x+step, cos(degInRad1)*xradiuse,(sin(degInRad1)*diam_e)+ecc_e);
        glVertex3f(x, cos(degInRad1)*xradius,(sin(degInRad1)*diam_i)+ecc_i);
        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
    }
    glEnd();


}


void Vista3D::DrawLine(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step) {

    float diam_i, diam_e, ecc_i, ecc_e;

    diam_i = yradiusi_u - yradiusi_l;
    diam_e = yradiuse_u - yradiuse_l;
    ecc_i = yradiusi_u - (diam_i/2.0);
    ecc_e = yradiuse_u - (diam_e/2.0);
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    for (int i=0; i<360; i=i + this->res_divisor) {
        float degInRad = i*DEG2RAD;
        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
        glVertex3f(x+step, cos(degInRad)*xradiuse,(sin(degInRad)*diam_e)+ecc_e);
    }
    glEnd();
}


void Vista3D::Set3dViewMode(int mode) {

    this->mode_3d_view = mode;
    Vista3D::draw();

}

void Vista3D::Set3DResolution(int res) {

    switch(res) {
        case WF_LOW:
            this->res_divisor = RES_LOW;
            this->l_res_divisor = L_RES_LOW;
            break;
        case WF_MEDIUM:
            this->res_divisor = RES_MED;
            this->l_res_divisor = L_RES_MED;
            break;
        case WF_HIGH:
            this->res_divisor = RES_HIGH;
            this->l_res_divisor = L_RES_HIGH;
            break;
        case WF_HIGHEST:
            this->res_divisor = RES_HIGHEST;
            this->l_res_divisor = L_RES_HIGHEST;
            break;
    }
    Vista3D::draw();
}

void Vista3D::DrawEllipse(float x, float xdiam, float yradius, float yradius_l ) {

    float ecc;
    float diam;
    int i;
    diam = yradius - yradius_l;
    ecc = yradius - (diam/2.0);
    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    for (i=0; i<360; i = i + this->res_divisor) {
        float degInRad = i*DEG2RAD;
        float degInRad_1 = (i+this->res_divisor)*DEG2RAD;
        glVertex3f(x, cos(degInRad)*xdiam,(sin(degInRad)*diam)+ecc);
        glVertex3f(x, cos(degInRad_1)*xdiam,(sin(degInRad_1)*diam)+ecc);
    }
    glEnd();
}



void Vista3D::normalizeAngle(int *angle) {
     while (*angle < 0)
         *angle += 360 * 16;
     while (*angle > 360 * 16)
         *angle -= 360 * 16;
}
