#ifndef EXPORTFILE_H
#define EXPORTFILE_H
#include <QFile>

#include "../include/BulbCalculator.h"

struct BinSTLHeader {
    char header[80];
    quint32 triangle_number;
};

struct STLTriangle {
    float normal[3];
    float vertex_1[3];
    float vertex_2[3];
    float vertex_3[3];
    quint16 attbytecount;
};

class ExportFile3D: public QObject {

    Q_OBJECT

    public:
        ExportFile3D(BulbCalculator *bcp);
        void ExportSTL(QString FileName);
        void SetResolution(int res);
        void SetFileType(int filetype);
        void SetHalf(int half_sim, int halfside);

        void ClearBinHeader(BinSTLHeader *h);

    private:
        BulbCalculator *bc;
        void WriteSTLBinHeader(QFile *fp);
        void WriteSTLAsciiHeader(QFile *fp);
        void WriteSTLBinTriangle(QFile *fp, STLTriangle *v);
        void WriteSTLAsciiTriangle(QFile *fp, STLTriangle *v);
        void WriteSTLAsciiFooter(QFile *fp);
        void Triangles(float x, float xradius, float yradiusi_u, float yradiusi_l, float xradiuse, float yradiuse_u,  float yradiuse_l, float step, QFile *fo);
        int resolution;
        int file_type;
        int half;
        int half_side;
        int tr_num;

};

#endif // EXPORTFILE_H
