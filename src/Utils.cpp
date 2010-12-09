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
