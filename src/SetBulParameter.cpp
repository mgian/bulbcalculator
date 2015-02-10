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

#include "../include/SetBulParameter.h"
#include "QtWidgets"
#include <QDialogButtonBox>

SetBulbParam::SetBulbParam(QWidget *parent) : QDialog(parent), ui(new Ui::D_BulbParam) {

    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Reset),SIGNAL(clicked()),this, SLOT(ResetValues()));

}

SetBulbParam::~SetBulbParam() {

    delete ui;

}

void SetBulbParam::ResetValues(void) {

    SetDefaultValue();

}

float SetBulbParam::GetTargetWeight(void) {

    return(ui->DSP_TW->value());

}

float SetBulbParam::GetMaterialDensity(void) {

    return(ui->DSP_MD->value());

}

double SetBulbParam::GetHLR(void) {
    double t;
    t = ui->SP_HLR->value()/100.0;
    return(t);

}

double SetBulbParam::GetWHR(void) {

    return(ui->SP_WHR->value()/100.0);

}

void SetBulbParam::SetCurrentValue(float tw, float md, double hrl, double whr) {

    ui->DSP_TW->setValue(tw);
    ui->DSP_MD->setValue(md);
    ui->SP_HLR->setValue(hrl);
    ui->SP_WHR->setValue(whr);

}

void SetBulbParam::SetDefaultValue(void) {

    QSettings settings("GRYS","BulbCalculator");
    if (settings.childGroups().contains("BulbDefault",Qt::CaseInsensitive)){
        settings.beginGroup("BulbDefault");
        ui->DSP_TW->setValue(settings.value("TargetWeight").toDouble());
        ui->DSP_MD->setValue(settings.value("MatDensity").toDouble());
        ui->SP_HLR->setValue(settings.value("HLRatio").toInt());
        ui->SP_WHR->setValue(settings.value("WHRatio").toInt());
        settings.endGroup();
    } else {
        ui->DSP_TW->setValue(2.4);
        ui->DSP_MD->setValue(11.34);
        ui->SP_HLR->setValue(15);
        ui->SP_WHR->setValue(100);
    }

}
