#ifndef EXPORTFILE_H
#define EXPORTFILE_H
#include <QFile>

#include "../include/BulbCalculator.h"

struct BinSTLHeader {
    char header[80];
    quint32 triangle_number;
};

struct STLTriangle {
    double normal[3];
    double vertex_1[3];
    double vertex_2[3];
    double vertex_3[3];
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
        void WriteSTLAsciiTriangle(QFile *fp, STLTriangle *v);
        void WriteSTLAsciiFooter(QFile *fp);
        void Triangles(double x, double xradius, double yradiusi_u, double yradiusi_l, double xradiuse, double yradiuse_u,  double yradiuse_l, double step, QFile *fo);
        int resolution;
        int file_type;
        int half;
        int half_side;
        int tr_num;

};

#endif // EXPORTFILE_H
