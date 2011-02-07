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

#ifndef EXPORTDRAW_H
#define EXPORTDRAW_H

#include "../include/BulbCalculator.h"

class ExportDraw: public QObject {

    Q_OBJECT

    public:
        ExportDraw();
        void ExportTextFile(QString FileName);
        void SetBc(BulbCalculator *bcp);


    private:
        void ExportTextFileMM(QString FileOut);
        void ExportTextFileInchD(QString FileOut);
        void ExportTextFileInchF(QString FileOut);

        BulbCalculator *bc;
};

#endif // EXPORTDRAW_H
