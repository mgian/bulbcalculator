#include "../include/KeelOptions.h"
#include "QtWidgets"
#include <QDialogButtonBox>


KeelOptions::KeelOptions() {

    setupUi(this);
    //connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));

}

float KeelOptions::GetKeelSlotPos(void) {

    return this->SP_SlotPos->value();

}

float KeelOptions::GetKeelSlotLenght(void) {

    return this->SP_SlotLenght->value();

}

float KeelOptions::GetKeelSlotWidth(void) {

    return this->SP_SlotWidth->value();

}

float KeelOptions::GetScrewHolePos(void) {

    return this->SP_ScrewHolePos->value();

}

float KeelOptions::GetScrewHoleDiam(void) {

    return this->SP_ScrewHoleDiam->value();

}

bool KeelOptions::GetKeelSlotState(void) {

    return this->GB_KeelSlot->isChecked();

}

bool KeelOptions::GetScrewHoleState(void) {

    return this->GB_ScrewHole->isChecked();

}

void KeelOptions::SetCurrentValues(float SlotP, float SlotLen, float SlotWid, float ScrewPos, float ScrewDiam) {

    this->SP_SlotPos->setValue(SlotP);
    this->SP_SlotLenght->setValue(SlotLen);
    this->SP_SlotWidth->setValue(SlotWid);
    this->SP_ScrewHolePos->setValue(ScrewPos);
    this->SP_ScrewHoleDiam->setValue(ScrewDiam);

}
