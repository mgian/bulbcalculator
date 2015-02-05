#ifndef EXPORTFILE_H
#define EXPORTFILE_H
#include <QFile>

#include "../include/BulbCalculator.h"

class ExportFile3D: public QObject {

    Q_OBJECT

    public:
        ExportFile3D();
        void ExportAsciiSTL(QString FileName, int half);
        void ExportBinarySTL(QString FileName, int half);
        void SetBc(BulbCalculator *bcp);


    private:
        BulbCalculator *bc;
        void Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo, int divisor, bool ascii);
};

#endif // EXPORTFILE_H
