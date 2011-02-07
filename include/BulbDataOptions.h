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

#ifndef BULBOPTIONS_H
#define BULBOPTIONS_H

#include "../ui_BulbDataOptions.h"

#include "../include/BulbCalculator.h"

class BulbDataOptions : public QDialog, private Ui::DBulbDataOptions
{
    Q_OBJECT

    public:
        BulbDataOptions();
        void SetBulbValue(double tl, double tw, double vl, double ws, int ns, int sc, int un, float st);
        int GetUnit();
        int GetNumSec();
        int GetSectDist();
        int GetSlice();
        float GetSliceThickness();


    private slots:
        void SetEven();
        void SetCosine();
        void SetMM();
        void SetInchF();
        void SetInchD();
        void SetDrawSlice();



    private:
        float w_ws;
        float w_vl;
        float w_md;
        int w_ns;
        int cosine;
        int unit;
        int sect;
        int slice;
        float slice_thickness;

};

#endif // BULBOPTIONS_H
