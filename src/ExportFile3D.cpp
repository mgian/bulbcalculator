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


void ExportFile3D::ClearBinHeader(BinSTLHeader *h) {

    memset(h, 0x00, sizeof(*h));

}

void ExportFile3D::WriteSTLAsciiHeader(QFile *fp) {

    QTextStream fOutTxtStream(fp);
    fOutTxtStream << "solid " << this->bc->naca_profile.foil_name.c_str() << "\n";
    fOutTxtStream.flush();

}

void ExportFile3D::WriteSTLBinHeader(QFile *fp) {

    BinSTLHeader *bsh = new(BinSTLHeader);

    this->ClearBinHeader(bsh);
    memcpy(bsh->header,this->bc->naca_profile.foil_name.c_str(), strlen(this->bc->naca_profile.foil_name.c_str()));
    bsh->triangle_number = this->tr_num;
    QDataStream fOutBinStream(fp);
    fOutBinStream.setByteOrder(QDataStream::LittleEndian); // *** set little endian byte order
    fOutBinStream << bsh->header << bsh->triangle_number;

    delete bsh;
}

void ExportFile3D::WriteSTLBinTriangle(QFile *fp, STLTriangle *v) {

    QDataStream ds(fp);
    ds.setByteOrder(QDataStream::LittleEndian); // *** set little endian byte order
    ds << v->normal[0] << v->normal[1] << v->normal[2];
    ds << v->vertex_1[0] << v->vertex_1[1] << v->vertex_1[2];
    ds << v->vertex_2[0] << v->vertex_2[1] << v->vertex_2[2];
    ds << v->vertex_3[0] << v->vertex_3[1] << v->vertex_3[2];
    ds << v->attbytecount;

}

void ExportFile3D::WriteSTLAsciiTriangle(QFile *fp, STLTriangle *v) {

    QTextStream ts(fp);
    ts << "facet normal " << v->normal[0] << " " << v->normal[1] << " " << v->normal[2] << " " << "\n";
    ts << "  outer loop" << "\n";
    ts << "    vertex " << v->vertex_1[0] << " " << v->vertex_1[1] << " " << v->vertex_1[2] << "\n";
    ts << "    vertex " << v->vertex_2[0] << " " << v->vertex_2[1] << " " << v->vertex_2[2] << "\n";
    ts << "    vertex " << v->vertex_3[0] << " " << v->vertex_3[1] << " " << v->vertex_3[2] << "\n";
    ts << "  endloop" << "\n";
    ts << "endfacet" << "\n";

}

void ExportFile3D::WriteSTLAsciiFooter(QFile *fp) {

    QTextStream fOutTxtStream(fp);
    fOutTxtStream << "endsolid " << this->bc->naca_profile.foil_name.c_str() << "\n";
    fOutTxtStream.flush();

}

void ExportFile3D::ExportSTL(QString FileName) {

    long w, mult;
    double step;
    double x;

    this->tr_num = 0;

    QFile fOut(FileName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    this->bc->UpdateStatusMessage(QString(tr("Exporting to ASCII STL file")));
    w = this->bc->length*10;

    mult = this->bc->naca_profile.num_step/(double)100.0;
    step = w/(mult*1.0);
    switch (this->file_type) {
        case STL_ASCII:
            this->WriteSTLAsciiHeader(&fOut);
            break;
        case STL_BINARY:
            this->WriteSTLBinHeader(&fOut);
            break;
    }

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

    switch (this->file_type) {
        case STL_ASCII:
            this->WriteSTLAsciiFooter(&fOut);
            break;
        case STL_BINARY:
            break;
    }

    fOut.close();
    this->bc->UpdateStatusMessage(QString(tr("Done")));

}

void ExportFile3D::Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo) {

    float diam_i, diam_e, ecc_i, ecc_e;
    int gr;
    int start_angle, end_angle, m = 1;
    STLTriangle *Triangle = new(STLTriangle);

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

    for (int i=start_angle; i < end_angle;  i=i + this->resolution) {
        float degInRad = i*DEG2RAD;
        float degInRad1 = (i+gr)*DEG2RAD;
        memset(Triangle, 0x00, sizeof(STLTriangle));

        Triangle->normal[0] = 0.0;
        Triangle->normal[1] = 0.0;
        Triangle->normal[2] = 0.0;

        Triangle->vertex_1[0] = x;
        Triangle->vertex_1[1] = m*(cos(degInRad)*xradius);
        Triangle->vertex_1[2] = m*((sin(degInRad)*diam_i)+ecc_i);

        Triangle->vertex_2[0] = x+step;
        Triangle->vertex_2[1] = m*(cos(degInRad1)*xradiuse);
        Triangle->vertex_2[2] = m*((sin(degInRad1)*diam_e)+ecc_e);

        Triangle->vertex_3[0] = x+step;
        Triangle->vertex_3[1] = m*(cos(degInRad)*xradiuse);
        Triangle->vertex_3[2] = m*((sin(degInRad)*diam_e)+ecc_e);

        switch(this->file_type) {
            case STL_ASCII:
                WriteSTLAsciiTriangle(fo, Triangle);
                break;
            case STL_BINARY:
                WriteSTLBinTriangle(fo, Triangle);
                this->tr_num++;
                break;
        }

        memset(Triangle, 0x00, sizeof(STLTriangle));

        Triangle->normal[0] = 0.0;
        Triangle->normal[1] = 0.0;
        Triangle->normal[2] = 0.0;

        Triangle->vertex_1[0] = x;
        Triangle->vertex_1[1] = m*(cos(degInRad)*xradius);
        Triangle->vertex_1[2] = m*((sin(degInRad)*diam_i)+ecc_i);

        Triangle->vertex_2[0] = x+step;
        Triangle->vertex_2[1] = m*(cos(degInRad1)*xradiuse);
        Triangle->vertex_2[2] = m*((sin(degInRad1)*diam_e)+ecc_e);

        Triangle->vertex_3[0] = x;
        Triangle->vertex_3[1] = m*(cos(degInRad1)*xradius);
        Triangle->vertex_3[2] = m*((sin(degInRad1)*diam_i)+ecc_i);

        switch(this->file_type) {
            case STL_ASCII:
                WriteSTLAsciiTriangle(fo, Triangle);
                break;
            case STL_BINARY:
                WriteSTLBinTriangle(fo, Triangle);
                break;
        }
    }

}

