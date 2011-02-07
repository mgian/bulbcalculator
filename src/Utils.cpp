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

#include "../include/Utils.h"
#include "stdlib.h"
#include "math.h"

/*
Utils::Utils() {

}
*/
#define UNIT_MM      0
#define UNIT_INCH    1
#define UNIT_INCH_F  2

QString DisplayValue(double val, int which) {

    switch(which) {
        case UNIT_MM:
            return DisplayMM(val);
            break;
        case UNIT_INCH:
            return DisplayInchesDecimal(val);
            break;
        case UNIT_INCH_F:
            return DisplayInchesFractional(val);
            break;
    }
    return QString::null;
}

QString DisplayMM(double val) {

    QString result = "";

    result.append(QString::number(val));

    return result;

}

QString DisplayInchesDecimal(double val) {

    QString result = "";

    result.append(QString::number(val));

    return result;
}

QString DisplayInchesFractional(double inch) {

    bool neg = (inch<0.0);
    inch = fabs(inch);
    int whole = floor(inch);
    int denominator = 64;
    int numerator = floor((inch-floor(inch))*(double)denominator+0.5);
    if(numerator>0)
            while((numerator%2)==0) {
                    denominator /= 2;
                    numerator /= 2;
            }
    QString result = "";
    if (whole==0 && numerator==0) {
        result.append(QString::number(0,10));
    } else {
        if(neg) {
            result.append("-") ;
        }
        if(whole) {
            result.append(QString::number(whole,10));
        }
        if(numerator) {
            if(whole) {
                result.append(" ");
            }
            result.append(QString::number(numerator,10));
            result.append("/");
            result.append(QString::number(denominator,10));
        }
    }

    return result;
}
