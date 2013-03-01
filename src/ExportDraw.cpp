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

#include <QString>
#include <QtGui>
#include <QDialog>
#include <QFileDialog>

#include "../include/ExportDraw.h"
#include "../include/Utils.h"

ExportDraw::ExportDraw() {

}

void ExportDraw::SetBc(BulbCalculator *bcp) {

    this->bc = bcp;

}

void ExportDraw::ExportTextFile(QString fileName) {



    QString ext = ".txt";

    if (!fileName.endsWith(ext)) {
        fileName.append(ext);
    }

    switch (this->bc->BcPrefs->Gui_Unit) {
        case UNIT_MM:
            this->ExportTextFileMM(fileName);
            break;
        case UNIT_INCH:
            this->ExportTextFileInchD(fileName);
            break;
        case UNIT_INCH_F:
            this->ExportTextFileInchF(fileName);
            break;
    }
}

void ExportDraw::ExportTextFileMM(QString FileOut) {

    double tl;

    tl = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);



    QFile fo(FileOut);
    if (!fo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fos(&fo);

    // write the data
    fos << QString("%1").arg(tr("Foil Type"), -20, ' ') << ": " << this->bc->naca_profile.foil_name.c_str() << "\n";
    fos << QString("%1").arg(tr("Height/Length Ratio"),-20,' ') << ": " << QString::number(this->bc->naca_profile.HLRatio*100.0) << "%" << "\n";
    fos << QString("%1").arg(tr("Width/Height Ratio"), -20, ' ') << ": "  << QString::number(this->bc->naca_profile.WHRatio*100.0) << "%" << "\n";

    fos << QString("%1").arg(tr("Lenght (cm)"), -25, ' ') << ": " << this->bc->length << "\n";
    fos << QString("%1").arg(tr("Center (cm)"), -25, ' ') << ": " << tl * this->bc->naca_profile.gcentre << "\n";
    fos << QString("%1").arg(tr("Wetted Surface (cm^2)"), -25, ' ') << ": " << this->bc->bulb_wet_surface << "\n";
    fos << QString("%1").arg(tr("Volume (cm^3)"), -25, ' ') << ": " << this->bc->bulb_volume << "\n";
    fos << "\n" << "\n";
    fos << QString("%1").arg(tr("Position"), 15, ' ');
    fos << QString("%1").arg(tr("X"), 15, ' ');
    fos << QString("%1").arg(tr("Height Max"), 15, ' ');
    fos << QString("%1").arg(tr("Height Min"), 15, ' ');
    fos << QString("%1").arg(tr("Width/2"), 15, ' ');
    fos << "\n";
    fos << QString("%1").arg("", 15, ' ');
    fos << QString("%1").arg(tr("(cm)"), 15, ' ');
    fos << QString("%1").arg(tr("(cm)"), 15, ' ');
    fos << QString("%1").arg(tr("(cm)"), 15, ' ');
    fos << QString("%1").arg(tr("(cm)"), 15, ' ');
    fos << "\n" << "\n";

    for (int i=0; i<this->bc->num_sect+2; i++) {
        fos << this->bc->GetSectionData(i) << "\n";
    }
    fos << "\n" << "\n";
    fo.close();
}


void ExportDraw::ExportTextFileInchD(QString FileOut) {

    double tl;

    tl = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);

    QFile fo(FileOut);
    if (!fo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fos(&fo);


    fos << QString("%1").arg(tr("Lenght (in.)"), -25, ' ') << ": " << DisplayValue(this->bc->length/2.54, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << QString("%1").arg(tr("Center (in.)"), -25, ' ') << ": " << DisplayValue(tl * this->bc->naca_profile.gcentre/2.54, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << QString("%1").arg(tr("Wetted Surface (sq.in.)"), -25, ' ') << ": " << DisplayValue(this->bc->bulb_wet_surface*0.155, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << QString("%1").arg(tr("Volume (cu.in.)"), -25, ' ') << ": " << DisplayValue(this->bc->bulb_volume*0.061, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << "\n" << "\n";
    fos << QString("%1").arg(tr("Position"), 15, ' ');
    fos << QString("%1").arg(tr("X (in.)"), 15, ' ');
    fos << QString("%1").arg(tr("Height Max (in.)"), 15, ' ');
    fos << QString("%1").arg(tr("Height Min (in.)"), 15, ' ');
    fos << QString("%1").arg(tr("Width/2 (in.)"), 15, ' ');
    fos <<  "\n";
    fos << QString("%1").arg("", 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << "\n" << "\n";
    for (int i=0; i<this->bc->num_sect+2; i++) {
        fos << this->bc->GetSectionData(i) << "\n";
    }

    fo.close();

}

void ExportDraw::ExportTextFileInchF(QString FileOut) {

    double tl;

    tl = pow((this->bc->target_weight*1000.0)/(this->bc->naca_profile.volume*this->bc->material_density), 1.0/3.0);

    QFile fo(FileOut);
    if (!fo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(NULL, tr("Error"), tr("Could not open file"), QMessageBox::Ok);
        return;
    }

    QTextStream fos(&fo);


    fos << QString("%1").arg(tr("Lenght (in.)"), -25, ' ') << ": " << DisplayValue(this->bc->length/2.54, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << QString("%1").arg(tr("Center (in.)"), -25, ' ') << ": " << DisplayValue(tl * this->bc->naca_profile.gcentre/2.54, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << QString("%1").arg(tr("Wetted Surface (sq.in.)"), -25, ' ') << ": " << DisplayValue(this->bc->bulb_wet_surface*0.155, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << QString("%1").arg(tr("Volume (cu.in.)"), -25, ' ') << ": " << DisplayValue(this->bc->bulb_volume*0.061, this->bc->BcPrefs->Gui_Unit) << "\n";
    fos << "\n" << "\n";
    fos << QString("%1").arg(tr("Position"), 15, ' ');
    fos << QString("%1").arg(tr("X"), 15, ' ');
    fos << QString("%1").arg(tr("Height Max"), 15, ' ');
    fos << QString("%1").arg(tr("Height Min"), 15, ' ');
    fos << QString("%1").arg(tr("Width/2"), 15, ' ');
    fos <<  "\n";
    fos << QString("%1").arg("", 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << QString("%1").arg(tr("(in.)"), 15, ' ');
    fos << "\n" << "\n";
    for (int i=0; i<this->bc->num_sect+2; i++) {
        fos << this->bc->GetSectionData(i) << "\n";
    }

    fo.close();

}
