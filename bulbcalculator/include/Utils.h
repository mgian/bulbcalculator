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

/*
#ifndef UTILS_H
#define UTILS_H

class Utils
{
public:
    Utils();
};

#endif // UTILS_H
*/
#include <QString>
#include <QRegExp>

QString DisplayInchesDecimal(double val);
QString DisplayInchesFractional(double inch);
QString DisplayValue(double val, int which);
QString DisplayMM(double val);
int CheckFileOverwrite(QString FileName);
