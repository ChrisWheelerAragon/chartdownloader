QT += core gui xml network quick widgets qml
HEADERS = src/airport.h src/auscontenthandler.h src/ausservice.h src/chartinfo.h src/downloadmanager.h src/faaservice.h src/filedownloader.h src/iconprovider.h src/mycontenthandler.h src/serviceprovider.h src/utils.h
SOURCES = src/main.cpp src/airport.cpp src/auscontenthandler.cpp src/ausservice.cpp src/chartinfo.cpp src/downloadmanager.cpp src/faaservice.cpp src/filedownloader.cpp src/iconprovider.cpp src/mycontenthandler.cpp src/serviceprovider.cpp src/utils.cpp
RESOURCES += qml.qrc
CONFIG += qt warn_on release
INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/lib/ -lpoppler-qt5
