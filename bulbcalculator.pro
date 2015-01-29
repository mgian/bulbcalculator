HEADERS = include/BulbCalculator.h \
    include/SetBulParameter.h \
    include/BulbDataOptions.h \
    include/PrintBulb.h \
    include/ExportDraw.h \
    include/Utils.h \
    include/Preferences.h \
    include/ExportFile.h \
    include/KeelOptions.h \
    include/NacaFoil.h

SOURCES = src/BulbCalculator.cpp \
    src/View_3D.cpp \
    src/SetBulParameter.cpp \
    src/BulbDataOptions.cpp \
    src/PrintBulb.cpp \
    src/ViewArea.cpp \
    src/ExportDraw.cpp \
    src/Utils.cpp \
    src/Prefereces.cpp \
    src/ExportFile.cpp \
    src/KeelOptions.cpp \
    src/NacaFoil.cpp \
    src/Main.cpp

FORMS = Ui/MainWindow.ui \
    Ui/SetBulbParameterDialog.ui \
    Ui/BulbDataOptions.ui \
    Ui/BulbPrintOptions.ui \
    Ui/Preferences.ui \
    Ui/KeelOptions.ui

CONFIG += debug release

#CONFIG += debug

QT *= opengl \
    xml \
    network widgets printsupport
TRANSLATIONS = i18n/BulbCalculator_it.ts
TARGET = bin/BulbCalculator
win32:LIBS *= -lQGLViewer2
unix:LIBS *= -lQGLViewer -lGLU

# !win32 {
# INCLUDEPATH *= /usr/include/QGLViewer/
# LIBS *= -L/usr/lib -lQGLViewer
# }
# win32 {
# INCLUDEPATH *= C:/Programmi/libQGLViewer
# LIBS *= -LC:/WINDOWS/system32 -lQGLViewer2
# }

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
