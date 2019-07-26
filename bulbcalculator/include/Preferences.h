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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "../ui_Preferences.h"

#include "../include/BulbCalculator.h"


class BcPreference: public QDialog, private Ui::D_Preferences
{
    Q_OBJECT

    public:
        BcPreference();

    private slots:
        void ApplyChange();
        void UpdateListSel(int idx);
        void EnableApply();
        void SelectLocalRepo();
        void SaveChanges();

    private:
        void UpdInterfacePref();
        void UpdBulbPref();
        void UpdRepoPref();
        void ReadCurrentPref();

        int ModPref;
        int CurIndex;

};


#endif // PREFERENCES_H
