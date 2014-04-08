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

#include <QtWidgets>
#include <QMessageBox>

#include "../include/Preferences.h"
#include "../include/BulbCalculator.h"

BcPreference::BcPreference() {

    setupUi(this);
    this->setWindowIcon(QIcon(QString("share/images/settings.png")));

    connect(this->buttonBox->button(QDialogButtonBox::Apply),SIGNAL(clicked()),this, SLOT(ApplyChange()));
    connect(this->buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked()),this, SLOT(SaveChanges()));

    connect(this->TWI_Settings, SIGNAL(currentChanged(int)), this, SLOT(UpdateListSel(int)));
    connect(this->RB_MdiWin, SIGNAL(clicked()), this, SLOT(EnableApply()));
    connect(this->RB_TabWin, SIGNAL(clicked()), this, SLOT(EnableApply()));

    connect(this->DSP_DefTW, SIGNAL(valueChanged(double)), this, SLOT(EnableApply()));
    connect(this->DSP_DefMD, SIGNAL(valueChanged(double)), this, SLOT(EnableApply()));
    connect(this->SP_DefHLR, SIGNAL(valueChanged(int)), this, SLOT(EnableApply()));
    connect(this->SP_DefWHR, SIGNAL(valueChanged(int)), this, SLOT(EnableApply()));
    connect(this->CB_DrawSlice, SIGNAL(toggled(bool)), this, SLOT(EnableApply()));
    connect(this->DSP_DefSliceThickness, SIGNAL(valueChanged(double)), this, SLOT(EnableApply()));
    connect(this->CB_DefRefPoints, SIGNAL(toggled(bool)), this, SLOT(EnableApply()));
    connect(this->SP_DefRefPointNum, SIGNAL(valueChanged(int)), this, SLOT(EnableApply()));
    connect(this->SP_DefNumOfSec, SIGNAL(valueChanged(int)), this, SLOT(EnableApply()));
    connect(this->RB_DefEven, SIGNAL(toggled(bool)), this, SLOT(EnableApply()));
    connect(this->RB_DefCosine, SIGNAL(toggled(bool)), this, SLOT(EnableApply()));
    connect(this->CB_TabPosition, SIGNAL(currentIndexChanged(int)), this, SLOT(EnableApply()));
    connect(this->B_LocRepoSelect, SIGNAL(clicked()), this, SLOT(SelectLocalRepo()));
    connect(this->LE_LocalRepo, SIGNAL(textChanged(QString)), SLOT(EnableApply()));


    this->lw_category->setCurrentRow(0);
    this->ReadCurrentPref();
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
    this->ModPref = NO;
    this->CurIndex = 0;

}


void BcPreference::SaveChanges() {

    if (this->ModPref == YES) {
        QMessageBox msgBox;
        msgBox.setText(tr("Some preferences where modified"));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setWindowIcon(QIcon(QString("share/images/aboutbulb.png")));
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            this->UpdInterfacePref();
            this->UpdBulbPref();
            this->UpdRepoPref();
        }
    }
    this->ModPref = NO;
    this->accept();

}

void BcPreference::EnableApply() {

    this->ModPref = YES;
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);

}

void BcPreference::SelectLocalRepo() {

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select loacal repository"),
                                                    this->LE_LocalRepo->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);


    if (dir.isNull() == false) {
        this->LE_LocalRepo->setText(dir);
        this->EnableApply();
    }

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

    settings.beginGroup("BulbDefault");
    this->DSP_DefTW->setValue(settings.value("TargetWeight").toDouble());
    this->DSP_DefMD->setValue(settings.value("MatDensity").toDouble());
    if (settings.value("DrawSlice").toInt() == YES) {
        this->CB_DrawSlice->setChecked(true);
    } else {
        this->CB_DrawSlice->setChecked(false);
    }

    this->DSP_DefSliceThickness->setValue(settings.value("SliceThickness").toDouble());
    this->SP_DefHLR->setValue(settings.value("HLRatio").toInt());
    this->SP_DefWHR->setValue(settings.value("WHRatio").toInt());
    settings.endGroup();

    settings.beginGroup("Repositories");
    this->LE_LocalRepo->setText(settings.value("LocalRepo").toString());
    settings.endGroup();

    int size = settings.beginReadArray("RemoteRepo");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        this->LW_RemoteRepo->addItem(settings.value("Repo").toString());
    }
    settings.endArray();


}


void BcPreference::UpdateListSel(int idx) {

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
            this->UpdRepoPref();
            break;
    }
    this->ModPref = NO;
    this->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

void BcPreference::UpdInterfacePref() {

    QSettings settings("GRYS","BulbCalculator");

    settings.beginGroup("Gui");
    if (this->RB_MdiWin->isChecked() == true) {
        settings.setValue("ViewMode", MDI);
    } else if (this->RB_TabWin->isChecked() == true) {
        settings.setValue("ViewMode", TABBED);
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

    QSettings settings("GRYS","BulbCalculator");

    settings.beginGroup("BulbDefault");

    settings.setValue("TargetWeight", this->DSP_DefTW->value());
    settings.setValue("MatDensity", this->DSP_DefMD->value());
    if (this->CB_DrawSlice->isChecked() == true) {
        settings.setValue("DrawSlice", YES);
    } else {
        settings.setValue("DrawSlice", NO);
    }
    settings.setValue("SliceThickness", this->DSP_DefSliceThickness->value());
    settings.setValue("HLRatio", this->SP_DefHLR->value());
    settings.setValue("WHRatio", this->SP_DefWHR->value());


    settings.endGroup();

}

void BcPreference::UpdRepoPref() {

    QSettings settings("GRYS","BulbCalculator");

    settings.beginGroup("Repositories");
    settings.setValue("LocalRepo", this->LE_LocalRepo->text());
    settings.endGroup();
    settings.beginWriteArray("RemoteRepo");
    for (int i = 0; i < this->LW_RemoteRepo->count(); ++i) {
         settings.setArrayIndex(i);
         settings.setValue("Repo", this->LW_RemoteRepo->item(i)->text());
     }
    settings.endArray();

}
