HEADERS = include/BulbCalculator.h \
    include/SetBulParameter.h \
    include/BulbDataOptions.h \
    include/PrintBulb.h \
    include/Utils.h \
    include/Preferences.h \
    include/KeelOptions.h \
    include/NacaFoil.h \
    include/ExportSTL.h \
    include/ExportFile3D.h \
    include/ExportFileData.h

SOURCES = src/BulbCalculator.cpp \
    src/View_3D.cpp \
    src/SetBulParameter.cpp \
    src/BulbDataOptions.cpp \
    src/PrintBulb.cpp \
    src/ViewArea.cpp \
    src/Utils.cpp \
    src/Prefereces.cpp \
    src/KeelOptions.cpp \
    src/NacaFoil.cpp \
    src/Main.cpp \
    src/ExportSTL.cpp \
    src/ExportFile3D.cpp \
    src/ExportFileData.cpp

FORMS = Ui/MainWindow.ui \
    Ui/SetBulbParameterDialog.ui \
    Ui/BulbDataOptions.ui \
    Ui/BulbPrintOptions.ui \
    Ui/Preferences.ui \
    Ui/KeelOptions.ui \
    Ui/ExportSTL.ui

CONFIG += debug release

#CONFIG += debug

QT *= opengl \
    xml \
    network widgets printsupport
TRANSLATIONS = i18n/BulbCalculator_it.ts
TARGET = bin/BulbCalculator
win32:LIBS *= -lQGLViewer2
unix:LIBS *= -lQGLViewer-qt5 -lGLU

!win32 {
    INCLUDEPATH *= ../QGLViewer/
    LIBS *= -L../ -L../QGLViewer -lQGLViewer-qt5
}

win32 {
    INCLUDEPATH *= ..\\QGLViewer
    LIBS *= -L..\\QGLViewer\QGLViewer -lQGLViewer2  -lopengl32 -lglu32
}

# install

target.path = /usr/bin/

sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    *.pro

sources.path = .

INSTALLS = target

RESOURCES += BulbCalculator.qrc
