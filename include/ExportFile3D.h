#ifndef EXPORTFILE_H
#define EXPORTFILE_H
#include <QFile>

#include "../include/BulbCalculator.h"

class ExportFile3D: public QObject {

    Q_OBJECT

    public:
        ExportFile3D();
        void ExportAsciiSTL(QString FileName);
        void ExportBinarySTL(QString FileName);
        void SetBc(BulbCalculator *bcp);
        void SetResolution(int res);
        void SetFileType(int filetype);
        void SetHalf(int half_sim, int halfside);


    private:
        BulbCalculator *bc;
        void Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo);
        int resolution;
        int file_type;
        int half;
        int half_side;

};

#endif // EXPORTFILE_H
