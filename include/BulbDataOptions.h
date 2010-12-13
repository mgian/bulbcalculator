#ifndef BULBOPTIONS_H
#define BULBOPTIONS_H

#include "../ui_BulbDataOptions.h"

#include "../include/BulbCalculator.h"

class BulbDataOptions : public QDialog, private Ui::DBulbDataOptions
{
    Q_OBJECT

    public:
        BulbDataOptions();
        void SetBulbValue(double tl, double tw, double vl, double ws, int ns, int sc, int un, float st);
        int GetUnit();
        int GetNumSec();
        int GetSectDist();
        int GetSlice();
        float GetSliceThickness();


    private slots:
        void SetEven();
        void SetCosine();
        void SetMM();
        void SetInchF();
        void SetInchD();
        void SetDrawSlice();



    private:
        float w_ws;
        float w_vl;
        float w_md;
        int w_ns;
        int cosine;
        int unit;
        int sect;
        int slice;
        float slice_thickness;

};

#endif // BULBOPTIONS_H
