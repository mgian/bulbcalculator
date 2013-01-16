/*
Copyright (C) 2011-2013  Gianluca Montecchi <gian@grys.it>

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

#include "QtGui"

#include "../include/Preferences.h"
#include "../include/BulbCalculator.h"

BcPreference::BcPreference() {

    setupUi(this);
    this->setWindowIcon(QIcon(QString("share/images/settings.png")));
    connect(buttonBox->button(QDialogButtonBox::Apply),SIGNAL(clicked()),this, SLOT(ApplyChange()));
    connect(TWI_Settings, SIGNAL(currentChanged(int)), this, SLOT(UpdateListSel()));
    this->lw_category->setCurrentRow(0);

}

void BcPreference::UpdateListSel() {

    this->lw_category->setCurrentRow(this->TWI_Settings->currentIndex());

}

void BcPreference::ApplyChange() {

    int curIdx;

    curIdx = this->TWI_Settings->currentIndex();

    qDebug() << curIdx;

    switch(curIdx) {
        case 0:
            BcPreference::UpdInterfacePref();
            break;
        case 1:
            BcPreference::UpdBulbPref();
            break;
        case 2:
            BcPreference::UpdPrinterPref();
            break;
        case 3:
            BcPreference::UpdRepoPref();
            break;

    }
}

void BcPreference::UpdInterfacePref() {

}

void BcPreference::UpdBulbPref() {

}

void BcPreference::UpdPrinterPref(){

}

void BcPreference::UpdRepoPref() {

}
