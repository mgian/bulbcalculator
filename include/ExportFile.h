#ifndef EXPORTFILE_H
#define EXPORTFILE_H
#include <QFile>

#include "../include/BulbCalculator.h"

class ExportFile: public QObject {

    Q_OBJECT

    public:
        ExportFile();
        void ExportAsciiSTL(QString FileName);
        void SetBc(BulbCalculator *bcp);


    private:
        BulbCalculator *bc;
        void Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo, int divisor);
};

#endif // EXPORTFILE_H
