#ifndef SETBULPARAMETER_H
#define SETBULPARAMETER_H

#include "../ui_SetBulbParameterDialog.h"


class SetBulbParam : public QDialog, private Ui::D_BulbParam
{
    Q_OBJECT

    public:
        SetBulbParam();
        void SetDefaultValue(void);
        float GetTargetWeight(void);
        float GetMaterialDensity(void);
        float GetHLR(void);
        float GetWHR(void);
        void SetCurrentValue(float tw, float md, float hrl, float whr);

    private slots:
        void ResetValues();


};
#endif // SETBULPARAMETER_H
