#ifndef KEELOPTIONS
#define KEELOPTIONS

#include "../ui_KeelOptions.h"


class KeelOptions : public QDialog, private Ui::QKeelOptions
{
    Q_OBJECT

    public:
        KeelOptions();
        float GetKeelSlotPos(void);
        float GetKeelSlotLenght(void);
        float GetKeelSlotWidth(void);
        float GetScrewHolePos(void);
        float GetScrewHoleDiam(void);
        void SetCurrentValues(float SlotP, float SlotLen, float SlotWid, float ScrewPos, float ScrewDiam);
        bool GetKeelSlotState(void);
        bool GetScrewHoleState(void);

};

#endif // KEELOPTIONS

