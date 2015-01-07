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

#include "../include/ExportFile.h"
#include "../include/Utils.h"

const float DEG2RAD = 3.14159/180;


ExportFile::ExportFile() {

}

void ExportFile::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;

}

void ExportFile::ExportAsciiSTL(QString FileName) {

    long w, mult;
    double step;
    double x;


    QFile fOut(FileName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fOutStream(&fOut);

    w = this->bc->length*10;

    mult = this->bc->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);
    fOutStream << "solid " << this->bc->naca_profile.foil_name.c_str() << "\n";

    x = 0.0;
    this->bc->UpdateProgressRange(0, mult);

    profile_data &pdi(this->bc->naca_profile[(unsigned)((double)0*mult)]);
    profile_data &pde(this->bc->naca_profile[(unsigned)((double)1*mult)]);
    ExportFile::Triangles(x, (pdi.width*w), (pdi.height_u/2)*w, (pdi.height_l/2)*w, (pde.width*w), (pde.height_u/2)*w, (pde.height_l/2)*w, step, &fOut, RES_MED);

    for(int i=1; i<mult; i++) {
        profile_data &pdi(this->bc->naca_profile[(unsigned)((double)i*mult)]);
        profile_data &pde(this->bc->naca_profile[(unsigned)((double)(i+1)*mult)]);
        x = x + step;       
        ExportFile::Triangles(x, (pdi.width*w), pdi.height_u/2*w, pdi.height_l/2*w, (pde.width*w), pde.height_u/2*w, pde.height_l/2*w, step, &fOut, RES_MED);
        this->bc->UpdateProgressValue(i);

    }
    profile_data &pdif(this->bc->naca_profile[(unsigned)((double)100*mult)]);
    profile_data &pdef(this->bc->naca_profile[(unsigned)((double)0*mult)]);
    x = x + step;
    ExportFile::Triangles(x, (pdif.width*w), pdif.height_u/2*w, pdif.height_l/2*w, (pdef.width*w), pdef.height_u/2*w, pdef.height_l/2*w,0, &fOut, RES_MED);
    this->bc->UpdateProgressValue(mult);
    fOut.close();

}

void ExportFile::Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo, int divisor) {

    float diam_i, diam_e, ecc_i, ecc_e;
    int gr;
    diam_i = yradiusi_u - yradiusi_l;
    diam_e = yradiuse_u - yradiuse_l;
    ecc_i = yradiusi_u - (diam_i/2.0);
    ecc_e = yradiuse_u - (diam_e/2.0);
    gr = 360/divisor;
    gr = 360/gr;

    QTextStream fOut(fo);

    for (int i=0; i < 360;  i=i + divisor) {
        float degInRad = i*DEG2RAD;
        float degInRad1 = (i+gr)*DEG2RAD;
        fOut << "facet normal 0 0 0" << "\n";
        fOut << "  outer loop" << "\n";
        fOut << "    vertex " << x << " " <<  cos(degInRad)*xradius << " " << (sin(degInRad)*diam_i)+ecc_i << "\n";
        fOut << "    vertex " << x+step << " " << cos(degInRad1)*xradiuse << " " <<  (sin(degInRad1)*diam_e)+ecc_e << "\n";
        fOut << "    vertex " << x+step << " " << cos(degInRad)*xradiuse << " " << (sin(degInRad)*diam_e)+ecc_e << "\n";
        fOut << "  endloop" << "\n";
        fOut << "  outer loop" << "\n";
        fOut << "    vertex " << x << " " <<  cos(degInRad)*xradius << " " << (sin(degInRad)*diam_i)+ecc_i << "\n";
        fOut << "    vertex " << x+step << " " << cos(degInRad1)*xradiuse << " " << (sin(degInRad1)*diam_e)+ecc_e << "\n";
        fOut << "    vertex " << x << " " << cos(degInRad1)*xradius << " " << (sin(degInRad1)*diam_i)+ecc_i << "\n";
        fOut << "  endloop" << "\n";
    }


}
