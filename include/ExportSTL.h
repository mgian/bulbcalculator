/*
Copyright (C) 2011-2015  Gianluca Montecchi <gian@grys.it>

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


#ifndef EXPORTSTL_H
#define EXPORTSTL_H

#include <QDialog>

namespace Ui {
    class ExportStl;
}

class ExportStl : public QDialog {

    Q_OBJECT

public:
    explicit ExportStl(QWidget *parent = 0);
    int GetFormat(void);
    int GetHalf(void);
    int GetResolution(void);
    ~ExportStl();

private:
    Ui::ExportStl *ui;
};

#endif // EXPORTSTL_H
