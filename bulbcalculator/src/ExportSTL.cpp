/*
Copyright (C) 2011-2015  Gianluca Montecchi <gian@grys.it>

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

#include <QDebug>

#include "../include/BulbCalculator.h"
#include "../include/ExportSTL.h"

#include "ui_ExportSTL.h"


ExportStl::ExportStl(QWidget *parent) : QDialog(parent), ui(new Ui::ExportStl) {

    ui->setupUi(this);
    QStringList res;
    res << tr("Low") << tr("Medium") << tr("High") << tr("Highest");
    ui->CB_STLResolution->addItems(res);
    ui->CB_STLResolution->setCurrentIndex(3);
    connect( ui->RB_XYPlane, SIGNAL( toggled(bool) ), this, SLOT( EnableTopBottom()) );
    connect( ui->RB_XZPlane, SIGNAL(toggled(bool) ), this, SLOT( EnableLeftRight()) );
    ui->RB_XYPlane->setChecked(true);
    ui->RB_TopSim->setChecked(true);

}

void ExportStl::EnableTopBottom() {

    ui->RB_LeftSim->setEnabled(false);
    ui->RB_RightSim->setEnabled(false);
    ui->RB_TopSim->setEnabled(true);
    ui->RB_BottomSim->setEnabled(true);
    ui->RB_TopSim->setChecked(true);

}

void ExportStl::EnableLeftRight() {

    ui->RB_LeftSim->setEnabled(true);
    ui->RB_RightSim->setEnabled(true);
    ui->RB_TopSim->setEnabled(false);
    ui->RB_BottomSim->setEnabled(false);
    ui->RB_RightSim->setChecked(true);

}

ExportStl::~ExportStl() {

    delete ui;

}

int ExportStl::GetFormat(void) {

    if (ui->RB_AsciiFormat->isChecked() == true) {
        return STL_ASCII;
    } else if(ui->RB_BinaryFormat->isChecked() == true) {
        return STL_BINARY;
    }
    return STL_NONE;

}

int ExportStl::GetResolution(void) {


    switch(ui->CB_STLResolution->currentIndex()) {
        case 0:
            return RES_LOW;
            break;
        case 1:
            return RES_MED;
            break;
        case 2:
            return RES_HIGH;
            break;
        case 3:
            return RES_HIGHEST-1;
            break;
    }
    return -1;

}

int ExportStl::GetSimmetry(void) {

    if(ui->RB_TopSim->isChecked() == true) {
        return OBJECT_HALF_TOP;
    } else if (ui->RB_BottomSim->isChecked() == true) {
        return OBJECT_HALF_BOTTOM;
    } else if(ui->RB_LeftSim->isChecked() == true) {
        return OBJECT_HALF_LEFT;
    } else if(ui->RB_RightSim->isChecked() == true) {
        return OBJECH_HALF_RIGHT;
    }
    return -1;

}

int ExportStl::GetHalf(void) {

    int obj;

    obj = OBJECT_FULL;
    if (ui->GB_HalfObject->isChecked() == true) {
        if (ui->RB_XYPlane->isChecked() == true) {
            obj = OBJECT_HALF_XY;
        }
        if (ui->RB_XZPlane->isChecked() == true) {
            obj = OBJECT_HALF_XZ;
        }
    }

    return obj;
}
