/*
Copyright (C) 2011-2014  Gianluca Montecchi <gian@grys.it>

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

#include <QString>
#include <QDialog>
#include <QFileDialog>
#include <QtWidgets>
#include <QMessageBox>

#include "../include/ExportFile3D.h"
#include "../include/Utils.h"

const float DEG2RAD = 3.14159/180;


ExportFile3D::ExportFile3D() {

}

void ExportFile3D::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;

}

void ExportFile3D::SetResolution(int res) {

    this->resolution = res;

}

void ExportFile3D::SetFileType(int filetype) {

    this->file_type = filetype;

}

void ExportFile3D::SetHalf(int half_sim, int halfside) {

    this->half = half_sim;
    this->half_side = halfside;

}


void ExportFile3D::ExportAsciiSTL(QString FileName) {

    long w, mult;
    double step;
    double x;


    QFile fOut(FileName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fOutStream(&fOut);

    this->bc->UpdateStatusMessage(QString(tr("Exporting to ASCII STL file")));
    w = this->bc->length*10;

    mult = this->bc->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);
    fOutStream << "solid " << this->bc->naca_profile.foil_name.c_str() << "\n";
    fOutStream.flush();

    x = 0.0;
    this->bc->UpdateProgressRange(0, mult);

    profile_data &pdi(this->bc->naca_profile[(unsigned)((double)0*mult)]);
    profile_data &pde(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    ExportFile3D::Triangles(x, (pdi.width*w), (pdi.height_u/2)*w, (pdi.height_l/2)*w, (pde.width*w), (pde.height_u/2)*w, (pde.height_l/2)*w, step, &fOut);

    for(int i=1; i<mult; i++) {
        profile_data &pdi(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        profile_data &pde(this->bc->naca_profile[(unsigned)((double)(i+1)*mult)]);
        x = x + step;       
        ExportFile3D::Triangles(x, (pdi.width*w), pdi.height_u/2*w, pdi.height_l/2*w, (pde.width*w), pde.height_u/2*w, pde.height_l/2*w, step, &fOut);
        this->bc->UpdateProgressValue(i);

    }
    profile_data &pdif(this->bc->naca_profile[(unsigned)((double)100*mult)]);
    profile_data &pdef(this->bc->naca_profile[(unsigned)((double)0*mult)]);
    x = x + step;
    ExportFile3D::Triangles(x, (pdif.width*w), pdif.height_u/2*w, pdif.height_l/2*w, (pdef.width*w), pdef.height_u/2*w, pdef.height_l/2*w,0, &fOut);
    this->bc->UpdateProgressValue(mult);
    fOutStream << "endsolid " << this->bc->naca_profile.foil_name.c_str() << "\n";
    fOutStream.flush();

    fOut.close();
    this->bc->UpdateStatusMessage(QString(tr("Done")));

}



void ExportFile3D::ExportBinarySTL(QString FileName) {

    long w, mult;
    double step;
    double x;


    QFile fOut(FileName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fOutStream(&fOut);

    this->bc->UpdateStatusMessage(QString(tr("Exporting to ASCII STL file")));
    w = this->bc->length*10;

    mult = this->bc->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);
    fOutStream << "solid " << this->bc->naca_profile.foil_name.c_str() << "\n";

    x = 0.0;
    this->bc->UpdateProgressRange(0, mult);

    profile_data &pdi(this->bc->naca_profile[(unsigned)((double)0*mult)]);
    profile_data &pde(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    ExportFile3D::Triangles(x, (pdi.width*w), (pdi.height_u/2)*w, (pdi.height_l/2)*w, (pde.width*w), (pde.height_u/2)*w, (pde.height_l/2)*w, step, &fOut);

    for(int i=1; i<mult; i++) {
        profile_data &pdi(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        profile_data &pde(this->bc->naca_profile[(unsigned)((double)(i+1)*mult)]);
        x = x + step;
        ExportFile3D::Triangles(x, (pdi.width*w), pdi.height_u/2*w, pdi.height_l/2*w, (pde.width*w), pde.height_u/2*w, pde.height_l/2*w, step, &fOut);
        this->bc->UpdateProgressValue(i);

    }
    profile_data &pdif(this->bc->naca_profile[(unsigned)((double)100*mult)]);
    profile_data &pdef(this->bc->naca_profile[(unsigned)((double)0*mult)]);
    x = x + step;
    ExportFile3D::Triangles(x, (pdif.width*w), pdif.height_u/2*w, pdif.height_l/2*w, (pdef.width*w), pdef.height_u/2*w, pdef.height_l/2*w,0, &fOut);
    this->bc->UpdateProgressValue(mult);
    fOut.close();
    this->bc->UpdateStatusMessage(QString(tr("Done")));

}


void ExportFile3D::Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo) {

    float diam_i, diam_e, ecc_i, ecc_e;
    int gr;
    int start_angle, end_angle, m = 1;

    diam_i = yradiusi_u - yradiusi_l;
    diam_e = yradiuse_u - yradiuse_l;
    ecc_i = yradiusi_u - (diam_i/2.0);
    ecc_e = yradiuse_u - (diam_e/2.0);
    gr = 360/this->resolution;
    gr = 360/gr;

    switch(this->half) {
        case OBJECT_FULL:
            start_angle = 0;
            end_angle = 360;
            break;
        case OBJECT_HALF_XY:
            if (this->half_side == OBJECT_HALF_TOP) {
                start_angle = 90;
                end_angle = 270;
                m = -1;
            } else if (this->half_side == OBJECT_HALF_BOTTOM) {
                start_angle = 90;
                end_angle = 270;
            }
            break;
        case OBJECT_HALF_XZ:
            if (this->half_side == OBJECT_HALF_LEFT) {
                start_angle = 0;
                end_angle = 180;
            } else if (this->half_side == OBJECH_HALF_RIGHT) {
                start_angle = 180;
                end_angle = 360;
            }
            break;
    }


    QTextStream fOut(fo);



    for (int i=start_angle; i < end_angle;  i=i + this->resolution) {
        float degInRad = i*DEG2RAD;
        float degInRad1 = (i+gr)*DEG2RAD;
        fOut << "facet normal 0 0 0" << "\n";
        fOut << "  outer loop" << "\n";
        fOut << "    vertex " << x << " " <<  m*(cos(degInRad)*xradius) << " " << m*((sin(degInRad)*diam_i)+ecc_i) << "\n";
        fOut << "    vertex " << x+step << " " << m*(cos(degInRad1)*xradiuse) << " " <<  m*((sin(degInRad1)*diam_e)+ecc_e) << "\n";
        fOut << "    vertex " << x+step << " " << m*(cos(degInRad)*xradiuse) << " " << m*((sin(degInRad)*diam_e)+ecc_e) << "\n";
        fOut << "  endloop" << "\n";
        fOut << "endfacet" << "\n";
        fOut << "facet normal 0 0 0" << "\n";
        fOut << "  outer loop" << "\n";
        fOut << "    vertex " << x << " " <<  m*(cos(degInRad)*xradius) << " " << m*((sin(degInRad)*diam_i)+ecc_i) << "\n";
        fOut << "    vertex " << x+step << " " << m*(cos(degInRad1)*xradiuse) << " " << m*((sin(degInRad1)*diam_e)+ecc_e) << "\n";
        fOut << "    vertex " << x << " " << m*(cos(degInRad1)*xradius) << " " << m*((sin(degInRad1)*diam_i)+ecc_i) << "\n";
        fOut << "  endloop" << "\n";
        fOut << "endfacet" << "\n";
    }


}

