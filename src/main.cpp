#include <QApplication>
#include <QBoxLayout>

#include "include/BulbCalculator.h"


int main(int argc, char *argv[]) {
    
    QApplication app(argc, argv);

    //QMainWindow* mainWindow = new QMainWindow;

    // Instantiate and layout the viewer

    BulbCalculator mw;
    mw.show();

    return app.exec();
}
