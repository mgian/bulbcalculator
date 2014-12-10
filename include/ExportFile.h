#ifndef EXPORTFILE_H
#define EXPORTFILE_H

#include "../include/BulbCalculator.h"

class ExportFile: public QObject {

    Q_OBJECT

    public:
        ExportFile();
        void ExportAsciiSTL(QString FileName);
        void SetBc(BulbCalculator *bcp);


    private:
        BulbCalculator *bc;
};

#endif // EXPORTFILE_H
