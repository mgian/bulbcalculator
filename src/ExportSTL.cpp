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


#include "../include/BulbCalculator.h"
#include "../include/ExportSTL.h"

#include "ui_ExportSTL.h"


ExportStl::ExportStl(QWidget *parent) : QDialog(parent), ui(new Ui::ExportStl) {

    ui->setupUi(this);
    QStringList res;
    res << tr("Low") << tr("Medium") << tr("High") << tr("Highest");
    ui->CB_STLResolution->addItems(res);
    ui->CB_STLResolution->setCurrentIndex(3);

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

}

int ExportStl::GetHalf(void) {

}
