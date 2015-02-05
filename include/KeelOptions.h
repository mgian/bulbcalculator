#ifndef KEELOPTIONS
#define KEELOPTIONS

#include <QDialog>

namespace Ui {
    class QKeelOptions;
}

class KeelOptions : public QDialog {

    Q_OBJECT

    public:
        explicit KeelOptions(QWidget *parent = 0);
        float GetKeelSlotPos(void);
        float GetKeelSlotLenght(void);
        float GetKeelSlotWidth(void);
        float GetScrewHolePos(void);
        float GetScrewHoleDiam(void);
        void SetCurrentValues(float SlotP, float SlotLen, float SlotWid, float ScrewPos, float ScrewDiam);
        bool GetKeelSlotState(void);
        bool GetScrewHoleState(void);
        ~KeelOptions();

private:
    Ui::QKeelOptions *ui;
};

#endif // KEELOPTIONS

