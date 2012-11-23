HEADERS = include/BulbCalculator.h \
    include/nacafoil.h \
    include/SetBulParameter.h \
    include/BulbDataOptions.h \
    include/PrintBulb.h \
    include/ExportDraw.h \
    include/Utils.h

SOURCES = src/BulbCalculator.cpp \
    src/main.cpp \
    src/View_3D.cpp \
    src/nacafoil.cpp \
    src/SetBulParameter.cpp \
    src/BulbDataOptions.cpp \
    src/PrintBulb.cpp \
    src/ViewArea.cpp \
    src/ExportDraw.cpp \
    src/Utils.cpp

FORMS = Ui/MainWindow.ui \
    Ui/SetBulbParameterDialog.ui \
    Ui/BulbDataOptions.ui \
    Ui/BulbPrintOptions.ui

#CONFIG += debug release

CONFIG += debug

QT *= opengl \
    xml \
    network
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
