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

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "../include/BulbCalculator.h"
#include "../include/nacafoil.h"


#define PI = 3.14159265358979323846
const float DEG2RAD = 3.14159/180;

void Vista3D::SetShow3DAxis(bool status) {

    this->show_axis = status;
    object = makeObject();
    Vista3D::draw();

}

void Vista3D::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;
    Vista3D::draw();
}


void Vista3D::SetShowGrid(bool status) {

    this->show_grid = status;
    object = makeObject();
    Vista3D::draw();

}

void Vista3D::SetProfile() {

    object = makeObject();

}

Vista3D::Vista3D(QWidget *parent) : QGLViewer(parent) {

    this->show_axis = true;
    this->show_grid = true;
    this->res_divisor = 32;

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


void Vista3D::setXRotation(int angle) {

    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
    }

}


void Vista3D::setYRotation(int angle) {

    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
      emit yRotationChanged(angle);
     }

}


void Vista3D::setZRotation(int angle) {

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

    this->drawAxis(1.2);

}

void Vista3D::draw() {

    long w, mult;
    double step;
    QSize t;
    double x;

    float pos[4] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    if (show_axis == true) {
        this->DrawAxis();
    }

    if (show_grid == true) {
        this->DrawGrid();
    }

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
    glBegin(GL_POINTS);
    glColor3f(1.0,1.0,1.0);
    x = 0.0;

    profile_data& pd2(this->bc->naca_profile[(unsigned)((double)0.02*mult)]);

    Vista3D::drawEllipse((0.02-2.0/2), (pd2.width*2), pd2.height_u, pd2.height_l);

    for(int i=0; i<mult; i++) {
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        x = x + step;
        Vista3D::drawEllipse((x-2.0/2), (pd.width*2), pd.height_u, pd.height_l);
    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0,1.0,0.2);
    x = 0.0;
    int in = 0;
    profile_data& pdl1(this->bc->naca_profile[(unsigned)((double)0.02*mult)]);
    profile_data& pdl3(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    Vista3D::drawLine((0.02-2.0/2), (pdl1.width*2), pdl1.height_u, pdl1.height_l, (pdl3.width*2), pdl3.height_u, pdl3.height_l,  0.02);
    float p;
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
        Vista3D::drawLine((x-2.0/2), (pdi.width*2), pdi.height_u, pdi.height_l, (pde.width*2), pde.height_u, pde.height_l, step);

    }
    glEnd();


}


void Vista3D::drawLine(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step) {

    float diam_i, diam_e, ecc_i, ecc_e;

    diam_i = yradiusi_u - yradiusi_l;
    diam_e = yradiuse_u - yradiuse_l;
    ecc_i = yradiusi_u - (diam_i/2.0);
    ecc_e = yradiuse_u - (diam_e/2.0);

    for (int i=0; i<360; i=i + this->res_divisor) {
        float degInRad = i*DEG2RAD;
        glVertex3f(x, cos(degInRad)*xradius,(sin(degInRad)*diam_i)+ecc_i);
        glVertex3f(x+step, cos(degInRad)*xradiuse,(sin(degInRad)*diam_e)+ecc_e);
    }

}

void Vista3D::Set3DResolution(int res) {

    switch(res) {
        case 0:
            this->res_divisor = 32;
            break;
        case 1:
            this->res_divisor = 16;
            break;
        case 2:
            this->res_divisor = 8;
            break;
        case 3:
            this->res_divisor = 1;
            break;
    }
    Vista3D::draw();
}

void Vista3D::drawEllipse(float x, float xdiam, float yradius, float yradius_l ) {

    float ecc;
    float diam;
    int i;

    diam = yradius - yradius_l;
    ecc = yradius - (diam/2.0);

    for (i=0; i<360; i = i + this->res_divisor) {
        float degInRad = i*DEG2RAD;
        glVertex3f(x, cos(degInRad)*xdiam,(sin(degInRad)*diam)+ecc);
    }
}


GLuint Vista3D::makeObject() {

    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    //const float DEG2RAD = 3.14159/180;
    long w, mult;
    double tl;
    QSize t;

    if (show_axis == true) {
        this->DrawAxis();
    }

    if (show_grid == true) {
        this->DrawGrid();
    }



    tl = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);
    tl = floor(tl*1000.0+.5)/1000.0;
    t = this->size();

    w = long(tl);
    zMin = 0;
    zMax = w;

    mult = this->bc->naca_profile.num_step/(double)w;

    glBegin(GL_POINTS);
    glColor3f(1.0,1.0,1.0);
    for(int i=0; i<=w; i++) {
        profile_data& pd(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        Vista3D::drawEllipse((i-w/2)/10.0, (pd.width*2)/10.0, (pd.height_u)/10.0,(pd.height_l)/10.0);
    }
    glEnd();
    glEndList();
    return list;

 }


void Vista3D::normalizeAngle(int *angle) {
     while (*angle < 0)
         *angle += 360 * 16;
     while (*angle > 360 * 16)
         *angle -= 360 * 16;
}
