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
