#include "../include/SetBulParameter.h"
#include "QtGui"

SetBulbParam::SetBulbParam() {

    setupUi(this);
    connect(buttonBox->button(QDialogButtonBox::Reset),SIGNAL(clicked()),this,
            SLOT(ResetValues()));

}

void SetBulbParam::ResetValues(void) {

    SetDefaultValue();

}

float SetBulbParam::GetTargetWeight(void) {

    return(this->DSP_TW->value());

}

float SetBulbParam::GetMaterialDensity(void) {

    return(DSP_MD->value());

}

float SetBulbParam::GetHLR(void) {

    return(SP_HLR->value()/100.0);

}

float SetBulbParam::GetWHR(void) {

    return(this->SP_WHR->value()/100.0);

}

void SetBulbParam::SetCurrentValue(float tw, float md, float hrl, float whr) {

    this->DSP_TW->setValue(tw);
    this->DSP_MD->setValue(md);
    this->SP_HLR->setValue(hrl*100.0);
    this->SP_WHR->setValue(whr*100.0);

}

void SetBulbParam::SetDefaultValue(void) {

    this->DSP_TW->setValue(2.4);
    this->DSP_MD->setValue(11.34);
    this->SP_HLR->setValue(15);
    this->SP_WHR->setValue(100);

}
