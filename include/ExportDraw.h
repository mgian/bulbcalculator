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
