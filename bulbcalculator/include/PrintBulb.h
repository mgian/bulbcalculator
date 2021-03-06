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

#ifndef PRINTBULB_H
#define PRINTBULB_H

#include "../include/BulbCalculator.h"

#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include <QPrintDialog>
#include <QPrinter>


#define P_SECTIONS_TOP      1
#define P_SECTIONS_SIDE     2
#define P_LINESPLAN_TOP     3
#define P_LINESPLAN_SIDE    4
#define P_DATA              5

class PrintDraw: public QTextDocument {
    Q_OBJECT
    public:
        PrintDraw(BulbCalculator *bc, int pt);


    public slots:
        void DrawBulbData(QPrinter *painter);
        void PrintPreview(QPrinter *pr);
        void SetBc(BulbCalculator *bc);

    private:
        BulbCalculator *bc;
        void DrawBulbProfile(QPainter *painter, int ori);
        void DrawBulbSection(QPainter *painter, int MainView, int ori);
        void DrawBulbLinesPlanTop(QPainter *painter, int ori);
        void DrawBulbLinesPlanSide(QPainter *painter, int ori);
        void DrawBulbTop(QPainter *painter, int ori);
        void CheckBulbDimension(QPainter *painter, int ori);
        long DrawBulbView(QPainter *p, int w);
        int PrintType;

    private slots:

};

#endif // PRINTBULB_H
