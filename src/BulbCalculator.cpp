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

#include <QtGui>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QtXml>
#include <QFileDialog>
#include <QDesktopServices>

#include "../include/BulbCalculator.h"
#include "../include/nacafoil.h"
#include "../include/SetBulParameter.h"
#include "../include/ExportDraw.h"
#include "../include/Utils.h"
#include "../include/BulbDataOptions.h"
#include "../include/PrintBulb.h"


BulbCalculator::BulbCalculator(QMainWindow *form) : QMainWindow(form){


    ui.setupUi(this);

    this->Modified = 0;
    this->num_sect = 6;
    this->sect_dist = DIST_EVEN;
    this->units = UNIT_MM;

    this->BulbMenu = new QActionGroup(this);
    this->Resolution3D = new QActionGroup(this);
    this->setWindowTitle(QString("Bulb Calculator - Version ").append(VERSION));
    BulbCalculator::SetDefaultValue();
    BulbCalculator::CreateCalcWin();
    BulbCalculator::CreateDataWin();
    BulbCalculator::DrawView();
    BulbCalculator::CreateTopWin();
    BulbCalculator::CreateSideWin();
    BulbCalculator::Create3dWin();
    view3d->SetBc(this);

    //ui.mdiArea->tileSubWindows();

    connect( ui.action_Quit, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( ui.action_SetParameters, SIGNAL (triggered() ), this, SLOT(SetBulbParameter()));
    connect( ui.action00xx, SIGNAL (triggered() ), this, SLOT(Set00xx()));
    connect( ui.action63_0xx, SIGNAL (triggered()), this, SLOT(Set63_0xx()));
    connect( ui.action63A0xx, SIGNAL (triggered()), this, SLOT(Set63A0xx()));
    connect( ui.action64_0xx, SIGNAL(triggered()), this, SLOT(Set640xx()));
    connect( ui.action64A0xx, SIGNAL (triggered()), this, SLOT(Set64A0xx()));
    connect( ui.action65_0xx, SIGNAL (triggered()), this, SLOT(Set650xx()));
    connect( ui.action65A0xx, SIGNAL (triggered()), this, SLOT(Set65A0xx()));
    connect( ui.action66_0xx, SIGNAL (triggered()), this, SLOT(Set660xx()));
    connect( ui.action67_0xx, SIGNAL (triggered()), this, SLOT(Set670xx()));
    connect( ui.action_Save, SIGNAL (triggered()), this, SLOT(Save()));
    connect( ui.action_Saveas, SIGNAL (triggered()), this, SLOT(SaveAs()));
    connect( ui.action_New, SIGNAL (triggered()), this,SLOT(NewBulb()));
    connect( ui.actionShow_Axis, SIGNAL (toggled(bool)), this, SLOT(Show3DAxis()));
    connect( ui.actionShow_Grid, SIGNAL (toggled(bool)), this, SLOT(ShowGrid()));
    connect( ui.action_Open, SIGNAL(triggered()), this, SLOT(Open()));
    connect( ui.actionShow_Top_view, SIGNAL(triggered()), this, SLOT(ShowTopWindow()));
    connect( ui.actionShow_Side_view, SIGNAL(triggered()), this, SLOT(ShowSideWindow()));
    connect( ui.actionShow_3D_view, SIGNAL(triggered()), this, SLOT(Show3dWindow()));
    connect( ui.actionShow_Calculation, SIGNAL(triggered()), this, SLOT(ShowCalcWindow()));
    connect( ui.actionShow_Bulb_Data, SIGNAL(triggered()), this, SLOT(ShowDataWindow()));
    connect( ui.actionCascade, SIGNAL(triggered()), ui.mdiArea, SLOT(cascadeSubWindows()));
    connect( ui.actionTile, SIGNAL(triggered()), ui.mdiArea, SLOT(tileSubWindows()));
    connect( ui.actionData_Options, SIGNAL(triggered()), this, SLOT(SetBulbDataOptions()));
    connect( ui.actionImport_Foil_Data, SIGNAL(triggered()), this, SLOT(ImportFoilData()));
    connect( ui.actionAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
    connect( ui.actionUIUC_foils, SIGNAL(triggered()), this, SLOT(DownloadUIUC()));
    connect( ui.actionText_File, SIGNAL(triggered()), this, SLOT(ExportTextFile()));
    connect( ui.actionPrintSectionFromTop, SIGNAL(triggered()), this, SLOT(PrintBulbSectionFromTop()));
    connect( ui.actionPrintSectionFromSide, SIGNAL(triggered()), this, SLOT(PrintBulbSectionFromSide()));
    connect( ui.actionPrintLinesPlan, SIGNAL(triggered()), this, SLOT(PrintBulbLinesPlan()));
    connect( ui.actionPrintData, SIGNAL(triggered()), this, SLOT(PrintBulbData()));

    connect( ui.actionLow, SIGNAL(triggered()), this, SLOT(Set3DResolutionLow()));
    connect( ui.actionMedium, SIGNAL(triggered()), this, SLOT(Set3DResolutionMedium()));
    connect( ui.actionHigh, SIGNAL(triggered()), this, SLOT(Set3DResolutionHigh()));
    connect( ui.actionHighest, SIGNAL(triggered()), this, SLOT(Set3DResolutionHighest()));

    this->BulbMenu->addAction(ui.action00xx);
    this->BulbMenu->addAction(ui.action63A0xx);
    this->BulbMenu->addAction(ui.action63_0xx);
    this->BulbMenu->addAction(ui.action64A0xx);
    this->BulbMenu->addAction(ui.action64_0xx);
    this->BulbMenu->addAction(ui.action65A0xx);
    this->BulbMenu->addAction(ui.action65_0xx);
    this->BulbMenu->addAction(ui.action66_0xx);
    this->BulbMenu->addAction(ui.action67_0xx);
    this->Resolution3D->addAction(ui.actionLow);
    this->Resolution3D->addAction(ui.actionMedium);
    this->Resolution3D->addAction(ui.actionHigh);
    this->Resolution3D->addAction(ui.actionHighest);

    ui.actionLow->setChecked(true);
    ui.action00xx->setChecked(true);

}


void BulbCalculator::ExportTextFile() {

    ExportDraw *ExpDraw = new ExportDraw();

    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Export Text Data"),
             "./",
            tr("Text Files (*.txt)"));

    ExpDraw->SetBc(this);

    if (fileName.isEmpty()) {
        QMessageBox::warning(NULL, tr("BulbCalculator"),
                                tr("No filename given, aborting operation"),
                                QMessageBox::Ok );
        return;
    }
    ExpDraw->ExportTextFile(fileName);

}

QString BulbCalculator::GetSectionData(int SectNum) {

    QString res;
    int c = 0;
    QString value;

    for (c=0; c<this->TW_SubBulbDataSec->columnCount(); c++) {
        QTableWidgetItem *it = this->TW_SubBulbDataSec->item(SectNum,c);
        res.append(QString("%1").arg(it->text(), 15, ' '));

    }
    return res;
}

QString BulbCalculator::GetSectionValue(int SectNum, int val) {

    QTableWidgetItem *it = this->TW_SubBulbDataSec->item(SectNum,val);
    return it->text();

}

void BulbCalculator::ShowAbout() {

    QString msg;
    msg.append("<center>");
    msg.append("<h2>BulbCalculator<p>");
    msg.append("Version: ");
    msg.append(VERSION);
    msg.append("</h2>");
    msg.append("Copyright 2000, 2001 by Marko Majic (Some Rights Reserved)<br>");
    msg.append("Copyright 2010, 2011 by Gianluca Montecchi (Some Rights Reserved)<p>");
    msg.append("Contact: gian@grys.it");
    msg.append("</center>");
    QMessageBox::about(this, "About BulbCalculator", msg);

}

void BulbCalculator::CreateTopWin() {


    int w,h;
    scTop = new QGraphicsScene();
    this->GV_TopView = new ViewArea(scTop, this, TOP_VIEW);

    w = this->GV_TopView->width();
    h = this->GV_TopView->height();
    scTop->setSceneRect(0,0,w-10,h-10);
    this->GV_TopView->setScene(scTop);

    // Bulb top view window
    this->ViewTopWin = new QMdiSubWindow;
    ViewTopWin->setWidget(this->GV_TopView);
    ViewTopWin->setWindowTitle(tr("Top View"));
    ui.mdiArea->addSubWindow(this->ViewTopWin);

}

void BulbCalculator::CreateSideWin() {

    int w,h;
    scSide = new QGraphicsScene();
    this->GV_SideView = new ViewArea(scSide, this, SIDE_VIEW);

    w = this->GV_SideView->width();
    h = this->GV_SideView->height();
    scSide->setSceneRect(0,0,w-10,h-10);
    this->GV_SideView->setScene(scSide);

    // Bulb side view window
    this->ViewSideWin = new QMdiSubWindow;
    ViewSideWin->setWidget(this->GV_SideView);
    ViewSideWin->setWindowTitle(tr("Side View"));

    ui.mdiArea->addSubWindow(this->ViewSideWin);


}

void BulbCalculator::Create3dWin() {

    this->widget3d = new QWidget();

    view3d = new Vista3D(this->widget3d);
    view3d->resize(this->widget3d->width(), this->widget3d->height());

    // Bulb 3d view window
    this->View3DWin = new QMdiSubWindow;

    View3DWin->setWidget(this->view3d);
    View3DWin->setWindowTitle(tr("3d View"));
    ui.mdiArea->addSubWindow(this->View3DWin);

}

void BulbCalculator::CreateCalcWin() {

    // Bulb Calculation window


    this->BulbCalculation = new QMdiSubWindow;
    this->TW_Bulb = new QTableWidget(10,8);

    QStringList hrows;
    hrows << "Lenght / Height" << "Material density" << "Lenght" << "Center" << "Center / Lenght" << "Projected Weight";
    hrows << "Volume" << "Wetted Surface" << "Max Diameter" << "Frontal Area";
    this->TW_Bulb->setVerticalHeaderLabels(hrows);
    this->TW_Bulb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->TW_Bulb->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->TW_Bulb->horizontalHeader()->setVisible(false);
    BulbCalculation->setWindowTitle(tr("Bulb Calculation"));
    QVBoxLayout *l1 = new QVBoxLayout;
    this->wdCalc = new QWidget;
    this->BulbName = new QLabel();
    l1->addWidget(this->BulbName);
    this->BulbName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    l1->addWidget(this->TW_Bulb);
    this->wdCalc->setLayout(l1);
    BulbCalculation->setWidget(this->wdCalc);
    ui.mdiArea->addSubWindow(this->BulbCalculation);

}

void BulbCalculator::CreateDataWin() {


    // Bulb data window
    this->BulbData = new QMdiSubWindow;
    this->TW_SubBulbDataGen = new QTableWidget(5,2);
    this->TW_SubBulbDataGen->horizontalHeader()->setVisible(false);
    this->TW_SubBulbDataSec = new QTableWidget(0,5);
    this->gb1 = new QGroupBox(tr("General data"));
    this->gb2 = new QGroupBox(tr("Sections data"));
    this->TW_SubBulbDataGen->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->TW_SubBulbDataSec->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QVBoxLayout *layout = new QVBoxLayout;
    QVBoxLayout *l1 = new QVBoxLayout;
    QVBoxLayout *l2 = new QVBoxLayout;

    this->wdData = new QWidget;
    this->gb1->setLayout(l1);
    this->gb2->setLayout(l2);
    l1->addWidget(this->TW_SubBulbDataGen);
    l2->addWidget(this->TW_SubBulbDataSec);

    layout->addWidget(gb1);
    layout->addWidget(gb2);
    this->wdData->setLayout(layout);
    BulbData->setWidget(wdData);
    BulbData->setWindowTitle(tr("Bulb Data"));
    ui.mdiArea->addSubWindow(this->BulbData);

}

void BulbCalculator::UpdateCalcs() {

    double tl;

    int count = 0;
    QTableWidgetItem *it;

    QStringList h;
    h.append(QString(tr("Position")));
    h.append(QString(tr("X")));
    h.append(QString(tr("Height Max")));
    h.append(QString(tr("Height Min")));
    h.append(QString(tr("Width")));

    QStringList v;
    v.append(QString(tr("Lenght")));
    v.append(QString(tr("Projected weight")));
    v.append(QString(tr("Center")));
    v.append(QString(tr("Volume")));
    v.append(QString(tr("Wetted surface")));


    this->TW_SubBulbDataGen->setVerticalHeaderLabels(v);
    this->TW_SubBulbDataSec->setHorizontalHeaderLabels(h);
    this->TW_SubBulbDataSec->setRowCount(0);

    sect_pos.clear();
    for(int i=1; i<this->num_sect+1; i++) {
        double xc = (double)i/(double)(this->num_sect+1);
        if(this->sect_dist == DIST_EVEN){
            sect_pos.push_back(xc);
        } else {
            sect_pos.push_back(1.0-cos(M_PI*xc/2.0));
        }
    }

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm");
    } else {
        it->setText("in.");
    }
    this->TW_SubBulbDataGen->setItem(0,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("gr.");
    } else {
        it->setText("oz.");
    }
    this->TW_SubBulbDataGen->setItem(1,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm");
    } else {
        it->setText("in.");
    }
    this->TW_SubBulbDataGen->setItem(2,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm^3");
    } else {
        it->setText("cu.in.");
    }
    this->TW_SubBulbDataGen->setItem(3,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm^2");
    } else {
        it->setText("sq.in.");
    }
    this->TW_SubBulbDataGen->setItem(4,0,it);

    // Lenght
    QTableWidgetItem *itG = new QTableWidgetItem;
    switch(this->units) {
        case UNIT_MM:
            itG->setText(QString::number(this->length,'.',2));
            break;
        case UNIT_INCH_F:
        case UNIT_INCH:
            itG->setText(DisplayValue(this->length/2.54,this->units));
            break;
    }
    this->TW_SubBulbDataGen->setItem(0,1,itG);

    // target weight
    QTableWidgetItem *itG1 = new QTableWidgetItem;
    switch(this->units) {
        case UNIT_MM:
            itG1->setText(QString::number(this->target_weight*1000,'.',2));
            break;
        case UNIT_INCH_F:
        case UNIT_INCH:
            itG1->setText(DisplayValue(this->target_weight*1000*0.035, this->units));
            break;
    }
    this->TW_SubBulbDataGen->setItem(1,1,itG1);

    // Center
    QTableWidgetItem *itG2 = new QTableWidgetItem;
    tl = pow((target_weight*1000.0)/(naca_profile.volume*material_density), 1.0/3.0);
    double gc = tl * this->naca_profile.gcentre;
    switch(this->units) {
        case UNIT_MM:
            itG2->setText(QString::number(gc,'.',2));
            break;
        case UNIT_INCH_F:
        case UNIT_INCH:
            itG2->setText(DisplayValue(gc/2.54,this->units));
            break;
    }
    this->TW_SubBulbDataGen->setItem(2,1,itG2);

    // Volume
    QTableWidgetItem *itG3 = new QTableWidgetItem;
    double vol = this->bulb_volume;
    switch(this->units) {
        case UNIT_MM:
            itG3->setText(QString::number(vol,'.',2));
            break;
        case UNIT_INCH_F:
        case UNIT_INCH:
            itG3->setText(DisplayValue(vol*0.061, this->units));
            break;
    }
    this->TW_SubBulbDataGen->setItem(3,1,itG3);

    // Wetted surface
    QTableWidgetItem *itG4 = new QTableWidgetItem;
    double ws_e = naca_profile.wetted_surface* (double)pow(tl, 2);
    switch(this->units) {
        case UNIT_MM:
            itG4->setText(QString::number(ws_e,'.',2));
            break;
        case UNIT_INCH_F:
        case UNIT_INCH:
            itG4->setText(DisplayValue(ws_e*0.155, this->units));
            break;
    }
    this->TW_SubBulbDataGen->setItem(4,1,itG4);

    this->TW_SubBulbDataGen->resizeColumnsToContents();

    this->gb1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    vector<double> data_pos(sect_pos);
    data_pos.insert(data_pos.begin(), 0.0);
    data_pos.push_back(1.0);
    for(vector<double>::iterator sp_it = data_pos.begin(); sp_it != data_pos.end(); sp_it++) {
        this->TW_SubBulbDataSec->insertRow(count);
        double x = *sp_it;
        profile_data& pd(this->naca_profile[(unsigned)floor(profile::num_step*x+0.5)]);
        double x_pos		= x * this->length;
        double height_max	= pd.height_u * this->length;
        double height_min	= pd.height_l * this->length;
        double width		= pd.width * this->length;

        QTableWidgetItem *itPos = new QTableWidgetItem(QString::number(count+1));
        this->TW_SubBulbDataSec->setItem(count,0,itPos);

        QTableWidgetItem *xPos = new QTableWidgetItem();
        QTableWidgetItem *hMax = new QTableWidgetItem();
        QTableWidgetItem *hMin = new QTableWidgetItem();
        QTableWidgetItem *Width = new QTableWidgetItem();

        switch (this->units) {
            case UNIT_MM:
                xPos->setText(QString::number(x_pos,'.',2));
                hMax->setText(QString::number(height_max,'.',2));
                hMin->setText(QString::number(height_min,'.',2));
                Width->setText(QString::number(width,'.',2));
                break;
            case UNIT_INCH:
            case UNIT_INCH_F:
                xPos->setText(DisplayValue(x_pos/2.54, this->units));
                hMax->setText(DisplayValue(height_max/2.54, this->units));
                hMin->setText(DisplayValue(height_min/2.54, this->units));
                Width->setText(DisplayValue(width/2.54, this->units));
                break;
        }
        this->TW_SubBulbDataSec->setItem(count,1,xPos);
        this->TW_SubBulbDataSec->setItem(count,2,hMax);
        this->TW_SubBulbDataSec->setItem(count,3,hMin);
        this->TW_SubBulbDataSec->setItem(count,4,Width);
        count++;
    }
    this->TW_SubBulbDataSec->resizeRowsToContents();
    this->TW_SubBulbDataSec->resizeColumnsToContents();
    this->TW_SubBulbDataSec->setAlternatingRowColors(true);

    this->TW_SubBulbDataGen->resizeRowsToContents();
    this->TW_SubBulbDataGen->resizeColumnsToContents();
    this->TW_SubBulbDataGen->setAlternatingRowColors(true);

}

void BulbCalculator::ShowTopWindow() {

    this->ViewTopWin->show();
    this->GV_TopView->show();

}

void BulbCalculator::ShowSideWindow() {

    this->ViewSideWin->show();
    this->GV_SideView->show();

}


void BulbCalculator::Show3dWindow() {
    
    this->View3DWin->show();
    this->view3d->show();

}

void BulbCalculator::ShowCalcWindow() {

    this->BulbCalculation->show();
    this->TW_Bulb->show();

    this->wdCalc->show();
    
}

void BulbCalculator::ShowDataWindow(){
    
    this->BulbData->show();
    this->gb1->show();
    this->gb2->show();
    this->TW_SubBulbDataSec->show();
    this->TW_SubBulbDataGen->show();
    this->wdData->show();
}


void BulbCalculator::PrintBulbSectionFromTop() {

      PrintDraw(this,P_SECTIONS_TOP);

}

void BulbCalculator::PrintBulbSectionFromSide() {

    PrintDraw(this,P_SECTIONS_SIDE);

}

void BulbCalculator::PrintBulbLinesPlan() {

    PrintDraw(this, P_LINESPLAN);


}

void BulbCalculator::PrintBulbData() {

    PrintDraw(this, P_DATA);

}

void BulbCalculator::SetBulbDataOptions() {

    int ret, x, y;
    double tl;

    BulbDataOptions *DlgBulbDataOpt = new BulbDataOptions;
    x = this->ui.centralwidget->width();
    y = this->ui.centralwidget->height();
    tl = pow((target_weight*1000.0)/(naca_profile.volume*material_density), 1.0/3.0);

    DlgBulbDataOpt->SetBulbValue(tl, this->target_weight, this->bulb_volume,
                                this->bulb_wet_surface, this->num_sect, this->sect_dist, this->units,
                                this->slice_thickness);


    ret = DlgBulbDataOpt->exec();
    if(ret == QDialog::Accepted) {
        this->sect_dist = DlgBulbDataOpt->GetSectDist();
        this->units = DlgBulbDataOpt->GetUnit() ;
        this->num_sect = DlgBulbDataOpt->GetNumSec();
        this->slice = DlgBulbDataOpt->GetSlice();
        this->slice_thickness = DlgBulbDataOpt->GetSliceThickness();
    }

    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Show3DAxis() {

    bool st;
    if(ui.actionShow_Axis->isChecked() == true) {
        st = true;
    } else {
        st = false;
    }
    view3d->SetShow3DAxis(st);

}

void BulbCalculator::ShowGrid() {

    bool st;
    if(ui.actionShow_Grid->isChecked() == true) {
        st = true;
    } else {
        st = false;
    }
    view3d->SetShowGrid(st);

}

void BulbCalculator::DrawView() {

    BulbCalculator::UpdateResults();
    BulbCalculator::UpdateCalcs();


}

void BulbCalculator::NewBulb() {

    if (this->Modified == 1) {
        QMessageBox msgBox;
        msgBox.setText("The design has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                BulbCalculator::Save();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
                break;
            default:
                break;
        }
    }
    BulbCalculator::SetDefaultValue();
    BulbCalculator::SetBulbParameter();
    BulbCalculator::Set00xx();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();
    BulbCalculator::DrawView();

    this->Modified = 1;
}

void BulbCalculator::Save() {

    if (this->FileName == "") {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                        "",
                        tr("Bulb (*.blb)"));
        if (fileName != "") {
            QString ext = ".blb";
            if (!fileName.endsWith(ext)) {
                fileName.append(ext);
            }

            this->FileName = fileName;
        }
    }

    QDomDocument doc("BulbCalculatorV1");
    QFile file(this->FileName);
    QDomElement root = doc.createElement("BulbV1");
    doc.appendChild(root);

    QDomElement param = doc.createElement("Parameters");
    root.appendChild(param);

    QDomElement name = doc.createElement("foil");
    param.appendChild(name);
    QDomText vname = doc.createTextNode(this->naca_profile.foil_name.c_str());
    name.appendChild(vname);

    QDomElement whr = doc.createElement("whr");
    param.appendChild(whr);
    QDomText vwhr = doc.createTextNode(QString("%1").arg(this->naca_profile.WHRatio*100));
    whr.appendChild(vwhr);

    QDomElement hlr = doc.createElement("hlr");
    param.appendChild(hlr);
    QDomText vhlr = doc.createTextNode(QString("%1").arg(this->naca_profile.HLRatio*100));
    hlr.appendChild(vhlr);

    QDomElement tw = doc.createElement("target_weight");
    param.appendChild(tw);
    QDomText vtw = doc.createTextNode(QString("%1").arg(this->target_weight*1000));
    tw.appendChild(vtw);

    QDomElement md = doc.createElement("material_density");
    param.appendChild(md);
    QDomText vmd = doc.createTextNode(QString("%1").arg(this->material_density*100));
    md.appendChild(vmd);

    QDomElement metadata = doc.createElement("Properties");
    root.appendChild(metadata);

    QString xml = doc.toString();

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("BulbCalculator"),
                                tr("Cannot open target file"),
                                QMessageBox::Ok );
        return;
    }

    file.write(xml.toStdString().c_str());
    file.close();
    this->Modified = 0;
}

void BulbCalculator::SaveAs() {

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                        "",
                        tr("Bulb (*.blb)"));
    if (fileName != "") {
        this->FileName = fileName;
        BulbCalculator::Save();
    }

}

void BulbCalculator::DownloadUIUC() {

    QDesktopServices::openUrl(QUrl("http://www.ae.illinois.edu/m-selig/ads/coord_database.html"));
}

void BulbCalculator::resizeEvent( QResizeEvent *e ) {



}


void BulbCalculator::UpdateResults() {

    double tl;
    int i,r, hs, he, cur_h;
    double hl, vol, mx, my;

    QTableWidgetItem *it;

    naca_profile.calc();

    data_profile.calc(this->naca_profile.WHRatio, naca_profile.volume);


    tl = pow((target_weight*1000.0)/(naca_profile.volume*material_density), 1.0/3.0);
    this->length = tl;
    this->bulb_volume = naca_profile.volume*(double)pow(tl,3);
    this->bulb_wet_surface = naca_profile.wetted_surface*(double)pow(tl,2);
    tl = floor(tl*1000.0+.5)/1000.0;
    r = 0;
    hl = this->naca_profile.HLRatio * 100;
    hs = (int)hl - 3;
    he = (int)hl + 3;
    it = new QTableWidgetItem;
    it->setText("");

    this->TW_Bulb->setHorizontalHeaderItem(0,it);

    QString tmpBulbName = "";
    QString tmpVal = "";

    tmpBulbName.append(this->naca_profile.foil_name.c_str());
    tmpVal.sprintf("%02d",(int)hl);

    tmpBulbName.replace("XX",tmpVal);
    this->BulbName->setText(QString("Bulb Name: ") + tmpBulbName);

    it = new QTableWidgetItem;
    it->setText("[%]");
    this->TW_Bulb->setItem(0,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("gr/cm^3");
    } else {
        it->setText("lb/cu.ft.");
    }
    this->TW_Bulb->setItem(1,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm");
    } else {
        it->setText("in.");
    }
    this->TW_Bulb->setItem(2,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm");
    } else {
        it->setText("in.");
    }
    this->TW_Bulb->setItem(3,0,it);

    it = new QTableWidgetItem;
    it->setText("[%]");
    this->TW_Bulb->setItem(4,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("gr.");
    } else {
        it->setText("oz.");
    }
    this->TW_Bulb->setItem(5,0,it);

    it = new QTableWidgetItem;
    if (this->units == UNIT_MM) {
        it->setText("cm^3");
    } else {
        it->setText("cu.in.");
    }
    this->TW_Bulb->setItem(6,0,it);

    it = new QTableWidgetItem;
     if (this->units == UNIT_MM) {
        it->setText("cm^2");
    } else {
        it->setText("sq.in.");
    }
    this->TW_Bulb->setItem(7,0,it);

    it = new QTableWidgetItem;
     if (this->units == UNIT_MM) {
        it->setText("cm");
    } else {
        it->setText("in.");
    }
    this->TW_Bulb->setItem(8,0,it);

    it = new QTableWidgetItem;
 if (this->units == UNIT_MM) {
        it->setText("cm^2");
    } else {
        it->setText("sq.in.");
    }
    this->TW_Bulb->setItem(9,0,it);


    for(i=1; i<=7; i++) {
        // Height
        cur_h = hs+i-1;
        it = new QTableWidgetItem;
        it->setText(QString::number(cur_h).append(" %"));
        this->TW_Bulb->setItem(0,i,it);

        // Material Density
        it = new QTableWidgetItem;
        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(this->material_density,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(this->material_density*62.42, this->units));
                break;
        }
        this->TW_Bulb->setItem(1,i,it);

        // Lenght
        it = new QTableWidgetItem;
        this->naca_profile.HLRatio = cur_h / 100.0;
        this->naca_profile.calc();
        tl = pow((target_weight*1000.0)/(naca_profile.volume*material_density), 1.0/3.0);

        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(tl,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(tl/2.54, this->units));
                break;
        }
        this->TW_Bulb->setItem(2,i,it);

        // Center
        it = new QTableWidgetItem;
        double gc = tl * this->naca_profile.gcentre;
        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(gc,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(gc/2.54, this->units));
                break;
        }
        this->TW_Bulb->setItem(3,i,it);

        // Center/Lenght
        it = new QTableWidgetItem;
        it->setText(QString::number(this->naca_profile.gcentre*100,'.',2).append(("%")));
        this->TW_Bulb->setItem(4,i,it);

        vol = naca_profile.volume * (double)pow(tl, 3);

        // Projected Weight
        it = new QTableWidgetItem;
        double vol_e = vol * (double)this->material_density;

        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(vol_e,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(vol_e*0.035, this->units));
                break;
        }

        this->TW_Bulb->setItem(5,i,it);

        // Volume
        it = new QTableWidgetItem;
        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(vol,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(vol*0.061, this->units));
                break;
        }
        this->TW_Bulb->setItem(6,i,it);


        // Wetted surface
        it = new QTableWidgetItem;
        double ws_e = naca_profile.wetted_surface* (double)pow(tl, 2);
        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(ws_e,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(ws_e*0.155, this->units));
                break;
        }
        this->TW_Bulb->setItem(7,i,it);

        // Max Diameter
        it = new QTableWidgetItem;
        // X axis
        mx = this->naca_profile.max_width * tl;
        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(mx*2,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue((mx*2)/2.54,this->units));
                break;
        }
        this->TW_Bulb->setItem(8,i,it);

        // Frontal area
        it = new QTableWidgetItem;
        // Y axis
        double mu, ml;
        mu = this->naca_profile.max_height_u * tl;
        ml = this->naca_profile.max_height_l * tl;

        if (ml < 0) {
            ml *= -1;
        }
        my = mu+ml;
        double fal = (ml*mx*3.14)/2.0;
        double fau = (mu*mx*3.14)/2.0;
        double fa = fal+fau;

        switch(this->units) {
            case UNIT_MM:
                it->setText(QString::number(fa,'.',2));
                break;
            case UNIT_INCH_F:
            case UNIT_INCH:
                it->setText(DisplayValue(fa*0.155, this->units));
                break;
        }
        this->TW_Bulb->setItem(9,i,it);

    }

    this->naca_profile.HLRatio = hl / 100.0;
    this->naca_profile.calc();


    this->TW_Bulb->resizeColumnsToContents();
    this->TW_Bulb->resizeRowsToContents();


}

void BulbCalculator::SetBulbParameter() {

    int ret;

    SetBulbParam *DlgParam = new SetBulbParam;
    
    DlgParam->SetCurrentValue(this->target_weight, this->material_density,
                              naca_profile.HLRatio, naca_profile.WHRatio);
    ret = DlgParam->exec();
    if (ret == QDialog::Accepted) {
        naca_profile.HLRatio = DlgParam->GetHLR();
        naca_profile.WHRatio = DlgParam->GetWHR();
        this->target_weight = DlgParam->GetTargetWeight();
        this->material_density = DlgParam->GetMaterialDensity();
        delete DlgParam;
        BulbCalculator::DrawView();
        this->GV_SideView->UpdateView();
        this->GV_TopView->UpdateView();
        this->Modified = 1;
    }

    view3d->SetProfile();

}

void BulbCalculator::ClearBulb() {

    naca_profile.xu.clear();
    naca_profile.yu.clear();
    naca_profile.xl.clear();
    naca_profile.yl.clear();

}


void BulbCalculator::Set00xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("00XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Set63_0xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("63-0XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Set63A0xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("63A0XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Set640xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("640XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Set64A0xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("64A0XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();
}


void BulbCalculator::Set650xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("650XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();
}

void BulbCalculator::Set65A0xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("65A0XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Set660xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("660XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();

}

void BulbCalculator::Set670xx() {

    BulbCalculator::ClearBulb();
    naca_profile.foil_name.assign("670XX");
    view3d->SetProfile();
    BulbCalculator::DrawView();
    this->GV_SideView->UpdateView();
    this->GV_TopView->UpdateView();
}


void BulbCalculator::Set3DResolutionLow() {

   ui.actionLow->setChecked(true);
    view3d->Set3DResolution(0);

}

void BulbCalculator::Set3DResolutionMedium() {

    ui.actionMedium->setChecked(true);
    view3d->Set3DResolution(1);

}

void BulbCalculator::Set3DResolutionHigh() {

    ui.actionHigh->setChecked(true);
    view3d->Set3DResolution(2);

}

void BulbCalculator::Set3DResolutionHighest() {

    ui.actionHighest->setChecked(true);
    view3d->Set3DResolution(3);


}

void BulbCalculator::Open() {

    QDomElement root, params;
    QDomNode value;
    int err = 0;
    if (this->Modified == 1) {
        QMessageBox msgBox;
        msgBox.setText("The design has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                BulbCalculator::Save();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
                break;
            default:
                break;
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                    "",
                    tr("Bulb (*.blb)"));
    if (fileName != "") {
        QString ext = ".blb";
        if (!fileName.endsWith(ext)) {
            fileName.append(ext);
        }

        this->FileName = fileName;
    } else {
        return;
    }

    QDomDocument doc("BulbCalculatorV1");
    QFile file(this->FileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("BulbCalculator"),
                                tr("Cannot open file"),
                                QMessageBox::Ok );
        return;
    }

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    root = doc.firstChildElement("BulbV1");
    params = root.firstChildElement("Parameters");
    value = params.namedItem("foil");
    if (value.isNull()) {
        err = 1;
    }
    this->naca_profile.foil_name.assign(value.toElement().text().toAscii());
    value = params.namedItem("whr");
    if (value.isNull()) {
        err = 1;
    }
    this->naca_profile.WHRatio = value.toElement().text().toFloat() / 100.0;
    value = params.namedItem("hlr");
    if (value.isNull()) {
        err = 1;
    }
    this->naca_profile.HLRatio = value.toElement().text().toFloat() / 100.0;
    value = params.namedItem("target_weight");
    if (value.isNull()) {
        err = 1;
    }
    this->target_weight = value.toElement().text().toFloat() / 1000.0;
    value = params.namedItem("material_density");
    if (value.isNull()) {
        err = 1;
    }
    this->material_density = value.toElement().text().toFloat() / 100.0;

    if (err == 1) {
        QMessageBox::critical(this, tr("BulbCalculator"),
                                tr("Not a valid file"),
                                QMessageBox::Ok );
    } else {
        BulbCalculator::DrawView();
        this->GV_SideView->UpdateView();
        this->GV_TopView->UpdateView();
    }

}


void BulbCalculator::SetDefaultValue() {

    this->target_weight = 2.4;
    this->material_density = 11.34;
    naca_profile.HLRatio = 0.15;
    naca_profile.WHRatio = 1;
    this->slice = 0;
    this->slice_thickness = 0.2;

}


void BulbCalculator::SetMetric() {

    this->units = UNIT_MM;

}

void BulbCalculator::ImportFoilData() {

    profile tmp_foil = naca_profile;

    QStringList fileNameS;
    QString fileName;
    QFileDialog *fd = new QFileDialog(this);

    fd->setFileMode(QFileDialog::ExistingFile);
    fd->setOption(QFileDialog::DontUseNativeDialog);
    fd->setFilter("Data Foil (*.dat)");

    if (fd->exec() == QDialog::Accepted ) {
        fileNameS = fd->selectedFiles();
    }

    if (fileNameS.isEmpty() == true) {
        return;
    }
    fileName = fileNameS.first();

    if (fileName != "") {
        QString ext = ".dat";
        if (!fileName.endsWith(ext.toLower()) && !fileName.endsWith(ext.toUpper()) ) {
            fileName.append(ext);
        }
    }

    if(tmp_foil.import_foil(fileName.toStdString().c_str(), 0)) {
        naca_profile = tmp_foil;
        view3d->SetBc(this);
        view3d->SetProfile();
        BulbCalculator::DrawView();
        this->GV_SideView->UpdateView();
        this->GV_TopView->UpdateView();
        BulbCalculator::SetUnchecked();
        return;
    }

    if(tmp_foil.import_foil(fileName.toStdString().c_str(), 1)) {
        naca_profile = tmp_foil;
        view3d->SetBc(this);
        view3d->SetProfile();
        BulbCalculator::DrawView();
        this->GV_SideView->UpdateView();
        this->GV_TopView->UpdateView();
        BulbCalculator::SetUnchecked();
        return;
    }

}

void BulbCalculator::SetUnchecked() {

    ui.action00xx->setChecked(false);
    ui.action63A0xx->setChecked(false);
    ui.action63_0xx->setChecked(false);
    ui.action64A0xx->setChecked(false);
    ui.action64_0xx->setChecked(false);
    ui.action65A0xx->setChecked(false);
    ui.action65_0xx->setChecked(false);
    ui.action66_0xx->setChecked(false);
    ui.action67_0xx->setChecked(false);

}

void BulbCalculator::SetImp() {

    this->units = UNIT_INCH;

}
