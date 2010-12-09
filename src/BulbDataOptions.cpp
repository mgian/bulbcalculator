#include "QtGui"

#include "../include/BulbDataOptions.h"
#include "../include/BulbCalculator.h"

BulbDataOptions::BulbDataOptions() {

    setupUi(this);
    connect(RB_Even, SIGNAL(clicked()),this, SLOT(SetEven()));
    connect(RB_Cosine, SIGNAL(clicked()),this, SLOT(SetCosine()));
    connect(RB_Metric, SIGNAL(clicked()), this, SLOT(SetMM()));
    connect(RB_Imp_F, SIGNAL(clicked()), this, SLOT(SetInchF()));
    connect(RB_Imp_D, SIGNAL(clicked()), this, SLOT(SetInchD()));


}

void BulbDataOptions::SetDrawSlice() {
/*
    if (this->CB_DrawSlice->isChecked() == true) {
        this->slice = 1;
    } else {
        this->slice = 0;
    }
*/
}

void BulbDataOptions::SetBulbValue(double tl, double tw, double vl, double ws, int ns, int sc, int un, int ds, float st) {


    this->LE_Len->setText(QString::number(tl,'.',4));
    this->LE_ProWei->setText(QString::number(tw,'.',4));
    this->LE_Vol->setText(QString::number(vl,'.',4));
    this->LE_WetSurf->setText(QString::number(ws,'.',4));
    this->DSP_SliceThickness->setValue(st);

    this->SP_NumOfSec->setValue(ns);

    if (un != UNIT_MM) {
        this->LE_Len->setText(QString::number(this->LE_Len->text().toFloat() / 2.54,'.',4));
        this->LE_ProWei->setText(QString::number(this->LE_ProWei->text().toFloat() * 0.035,'.',4));
        this->LE_Vol->setText(QString::number(this->LE_Vol->text().toFloat() * 0.061,'.',4));
        this->LE_WetSurf->setText(QString::number(this->LE_WetSurf->text().toFloat() * 0.155,'.',4));
    }

    switch(sc) {
        case DIST_EVEN:
            RB_Even->setChecked(true);
            break;
        case DIST_COS:
            RB_Cosine->setChecked(true);
            break;
    }

    switch(un) {
        case UNIT_MM:
            RB_Metric->setChecked(true);
            break;
        case UNIT_INCH_F:
            RB_Imp_F->setChecked(true);
            break;
        case UNIT_INCH:
            RB_Imp_D->setChecked(true);
            break;
    }
}

int BulbDataOptions::GetSlice() {
    return this->slice;
}

float BulbDataOptions::GetSliceThickness() {
    return this->DSP_SliceThickness->value();
}

void BulbDataOptions::SetMM() {

    this->unit = UNIT_MM;

    this->label->setText(QString("Lenght (cm):"));
    this->label_2->setText(QString("Projected Weight (Kg):"));
    this->label_3->setText(QString("Volume (cm³):"));
    this->label_4->setText(QString("Wetted Surface (cm²):"));
    this->label_6->setText(QString("Slice thickness (cm):"));

    this->DSP_SliceThickness->setValue(this->DSP_SliceThickness->value() * 2.54);
    this->LE_Len->setText(QString::number(this->LE_Len->text().toFloat() * 2.54,'.',4));
    this->LE_ProWei->setText(QString::number(this->LE_ProWei->text().toFloat() / 0.035,'.',4));
    this->LE_Vol->setText(QString::number(this->LE_Vol->text().toFloat() / 0.061,'.',4));
    this->LE_WetSurf->setText(QString::number(this->LE_WetSurf->text().toFloat() / 0.155,'.',4));

}

void BulbDataOptions::SetInchF() {


    if (this->unit == UNIT_MM) {
        this->label->setText(QString("Lenght (in):"));
        this->label_2->setText(QString("Projected Weight (oz):"));
        this->label_3->setText(QString("Volume (cu.in.):"));
        this->label_4->setText(QString("Wetted Surface (sq.in.):"));
        this->label_6->setText(QString("Slice thickness (in.):"));

        this->DSP_SliceThickness->setValue(this->DSP_SliceThickness->value() / 2.54);
        this->LE_Len->setText(QString::number(this->LE_Len->text().toFloat() / 2.54,'.',4));
        this->LE_ProWei->setText(QString::number(this->LE_ProWei->text().toFloat() * 0.035,'.',4));
        this->LE_Vol->setText(QString::number(this->LE_Vol->text().toFloat() * 0.061,'.',4));
        this->LE_WetSurf->setText(QString::number(this->LE_WetSurf->text().toFloat() * 0.155,'.',4));
    }
    this->unit = UNIT_INCH_F;
}


void BulbDataOptions::SetInchD() {

    if (this->unit == UNIT_MM) {
        this->label->setText(QString("Lenght (in):"));
        this->label_2->setText(QString("Projected Weight (oz):"));
        this->label_3->setText(QString("Volume (cu.in.):"));
        this->label_4->setText(QString("Wetted Surface (sq.in.):"));
        this->label_6->setText(QString("Slice thickness (in.):"));

        this->DSP_SliceThickness->setValue(this->DSP_SliceThickness->value() / 2.54);
        this->LE_Len->setText(QString::number(this->LE_Len->text().toFloat() / 2.54,'.',4));
        this->LE_ProWei->setText(QString::number(this->LE_ProWei->text().toFloat() * 0.035,'.',4));
        this->LE_Vol->setText(QString::number(this->LE_Vol->text().toFloat() * 0.061,'.',4));
        this->LE_WetSurf->setText(QString::number(this->LE_WetSurf->text().toFloat() * 0.155,'.',4));
    }
    this->unit = UNIT_INCH;

}

int BulbDataOptions::GetSectDist() {

    return (this->cosine);

}

void BulbDataOptions::SetEven() {

    this->cosine = DIST_EVEN;

}

void BulbDataOptions::SetCosine() {

    this->cosine = DIST_COS;

}

int BulbDataOptions::GetUnit() {

    return (this->unit);

}

int BulbDataOptions::GetNumSec() {

    return (this->SP_NumOfSec->value());

}

