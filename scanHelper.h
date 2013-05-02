#ifndef SCANHELPER_H
#define SCANHELPER_H
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QStringList>
#include <QVariant>
#include <QDebug>
//#include <psfdefinitions.h>
#include "coutput.h"
#include "test_Definitions.h"


class ScanHelper
{


public:
    ScanHelper();
    ~ScanHelper();
    //void* openLibrary();
    bool openPath(int isMToolsChecked, QString path);
    QList<QVariant> getMetaData();
    void cleanUp();
     QList<QVariant> getLdataEntry();
     QStringList getNoScanDataFoundList();

public slots:



private:

    void printDataEntryList();
    QString scanPath;
    //COutput *cOutput;
    QList<QVariant> Ldataentry;
    COutput *cOutput;
    QMutex mutex;
    int mToolsCheckedState;
    //CController *ccontroller;
    //SndInfoClass *sndInfoClass;
    //BextInfoClass * bextInfoClass;
    QStringList noScanDataFoundList;

};



#endif // SCANHELPER_H
