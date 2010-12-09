#ifndef PRINTBULB_H
#define PRINTBULB_H

#include <QtGui>
#include <QPrintDialog>
#include <QPrinter>

#include "../include/BulbCalculator.h"

#define P_SECTIONS_TOP      1
#define P_SECTIONS_SIDE     2
#define P_LINESPLAN         3
#define P_DATA              4

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
        void DrawBulbProfile(QPainter *painter);
        void DrawBulbSection(QPainter *painter, int MainView);
        void DrawBulbLinesPlan(QPainter *painter);
        void DrawBulbTop(QPainter *painter);
        void CheckBulbDimension(QPainter *painter);
        long DrawBulbView(QPainter *p, int w, int h);
        int PrintType;

    private slots:

};

#endif // PRINTBULB_H
