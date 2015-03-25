
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QDialogButtonBox>

#include "../include/BulbCalculator.h"
#include "../include/KeelOptions.h"

#include "ui_KeelOptions.h"

KeelOptions::KeelOptions(QWidget *parent) : QDialog(parent), ui(new Ui::QKeelOptions) {

    ui->setupUi(this);

}

KeelOptions::~KeelOptions() {

    delete ui;

}

float KeelOptions::GetKeelSlotPos(void) {

    return ui->SP_SlotPos->value();

}

float KeelOptions::GetKeelSlotLenght(void) {

    return ui->SP_SlotLenght->value();

}

float KeelOptions::GetKeelSlotWidth(void) {

    return ui->SP_SlotWidth->value();

}

float KeelOptions::GetScrewHolePos(void) {

    return ui->SP_ScrewHolePos->value();

}

float KeelOptions::GetScrewHoleDiam(void) {

    return ui->SP_ScrewHoleDiam->value();

}

bool KeelOptions::GetKeelSlotState(void) {

    return ui->GB_KeelSlot->isChecked();

}

bool KeelOptions::GetScrewHoleState(void) {

    return ui->GB_ScrewHole->isChecked();

}

void KeelOptions::SetCurrentValues(float SlotP, float SlotLen, float SlotWid, float ScrewPos, float ScrewDiam) {

    ui->SP_SlotPos->setValue(SlotP);
    ui->SP_SlotLenght->setValue(SlotLen);
    ui->SP_SlotWidth->setValue(SlotWid);
    ui->SP_ScrewHolePos->setValue(ScrewPos);
    ui->SP_ScrewHoleDiam->setValue(ScrewDiam);

}
