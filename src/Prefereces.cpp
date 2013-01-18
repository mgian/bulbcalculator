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
    connect(this->buttonBox->button(QDialogButtonBox::Apply),SIGNAL(clicked()),this, SLOT(ApplyChange()));
    connect(this->TWI_Settings, SIGNAL(currentChanged(int)), this, SLOT(UpdateListSel(int)));
    connect(this->RB_MdiWin, SIGNAL(clicked()), this, SLOT(EnableApply()));
    connect(this->RB_TabWin, SIGNAL(clicked()), this, SLOT(EnableApply()));
    connect(this->CB_TabPosition, SIGNAL(currentIndexChanged(int)), this, SLOT(EnableApply()));
    this->lw_category->setCurrentRow(0);
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
    this->ReadCurrentPref();
    this->ModPref = NO;
    this->CurIndex = 0;

}

void BcPreference::EnableApply() {

    this->ModPref = YES;
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);

}

void BcPreference::ReadCurrentPref() {

    int val;

    QSettings settings("GRYS","BulbCalculator");

    settings.beginGroup("Gui");
    val = settings.value("ViewMode").toInt();
    switch (val) {
        case 0:
            this->RB_MdiWin->setChecked(true);
            break;
        case 1:
            this->RB_TabWin->setChecked(true);
            break;
    }
    val = settings.value("TabPos").toInt();
    this->CB_TabPosition->setCurrentIndex(val);

    val = settings.value("Unit").toInt();
    switch(val) {
        case UNIT_MM:
            this->RB_DefMetric->setChecked(true);
            break;
        case UNIT_INCH:
            this->RB_DefImp_D->setChecked(true);
            break;
        case UNIT_INCH_F:
            this->RB_DefImp_F->setChecked(true);
            break;
    }

    settings.endGroup();

}


void BcPreference::UpdateListSel(int idx) {

    if (this->ModPref == YES) {
        QMessageBox msgBox;
        msgBox.setText(tr("Some preferences where modified"));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setWindowIcon(QIcon(QString("share/images/aboutbulb.png")));
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            switch (idx) {
                case 0:
                    this->UpdInterfacePref();
                    break;
                case 1:
                    this->UpdBulbPref();
                    break;
                case 2:
                    this->UpdPrinterPref();
                    break;
                case 3:
                    this->UpdRepoPref();
                    break;
            }
        }
    }

    this->CurIndex = idx;
    this->lw_category->setCurrentRow(this->TWI_Settings->currentIndex());
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);

}


void BcPreference::ApplyChange() {

    int curIdx;

    curIdx = this->TWI_Settings->currentIndex();


    switch(curIdx) {
        case 0:
            this->UpdInterfacePref();
            break;
        case 1:
            this->UpdBulbPref();
            break;
        case 2:
            this->UpdPrinterPref();
            break;
        case 3:
            this->UpdRepoPref();
            break;
    }
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void BcPreference::UpdInterfacePref() {

    QSettings settings("GRYS","BulbCalculator");

    settings.beginGroup("Gui");
    if (this->RB_MdiWin->isChecked() == true) {
        settings.setValue("ViewMode", 0);
    } else if (this->RB_TabWin->isChecked() == true) {
        settings.setValue("ViewMode", 1);
    }
    settings.setValue("TabPos",CB_TabPosition->currentIndex());
    if (this->RB_DefMetric->isChecked() == true) {
        settings.setValue("Unit", UNIT_MM);
    } else if (this->RB_DefImp_D->isChecked() == true) {
        settings.setValue("Unit", UNIT_INCH);
    } else if (this->RB_DefImp_F->isChecked() == true) {
        settings.setValue("Unit", UNIT_INCH_F);
    }

    settings.endGroup();


}

void BcPreference::UpdBulbPref() {

}

void BcPreference::UpdPrinterPref(){

}

void BcPreference::UpdRepoPref() {

}
