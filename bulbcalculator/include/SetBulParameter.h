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

#ifndef SETBULPARAMETER_H
#define SETBULPARAMETER_H

#include "../ui_SetBulbParameterDialog.h"
#include <QDialog>

class SetBulbParam : public QDialog
{
    Q_OBJECT

    public:
        explicit SetBulbParam(QWidget *parent = nullptr);
        void SetDefaultValue(void);
        double GetTargetWeight(void);
        float GetMaterialDensity(void);
        double GetHLR(void);
        double GetWHR(void);
        void SetCurrentValue(float tw, float md, double hrl, double whr);
        ~SetBulbParam();

    private slots:
        void ResetValues();

    private:
        Ui::D_BulbParam *ui;


};
#endif // SETBULPARAMETER_H
