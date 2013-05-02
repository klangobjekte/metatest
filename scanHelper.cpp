//#include <QtGui>
#include <iostream>
//#include <psfdefinitions.h>
#include "scanHelper.h"
using namespace std;
using namespace meta;
#ifdef __APPLE__
    #include <dlfcn.h>
#endif


ScanHelper::ScanHelper()
      //cursor(new WaveformCursorProxy(this)),
      //ccontroller(new CController()),
      //sndInfoClass(new SndInfoClass()),
      //bextInfoClass(new BextInfoClass())
{
    //openLibrary();
    //while(!mutex.tryLock())
    //{msleep(1000);
    //}
    //mutex.tryLock();

    //mutex.unlock();

}


ScanHelper::~ScanHelper()
{
}



#ifdef __APPLE__

/**
void* ScanHelper::openLibrary()
{
    void * metalib = dlopen("libmeta.dylib", RTLD_LAZY);

    if(metalib == NULL)
    {
       // report error ...
    }
    else
    {
       // use the result in a call to dlsym
	//dlsym()
    }
}
*/



//! Verbindung zur meta dll
//! ueber getMetaData
//! ohne new scanHelper kann kein new COutput erzeugt werden
bool ScanHelper::openPath(int isMToolsChecked, QString path)
{

    //qDebug() << "openPath" << path;
    if(!Ldataentry.isEmpty())
        Ldataentry.clear();

    if(path.length()>=MAX_META_PATHLEN)
    {
    //cout << "ERROR: PathName to Long scanHelper 70" << path << endl;
	return false;
    }

    //! Path must be UTF String!
    string strAscii = path.toStdString();
    char *srcpath = NULL;
    srcpath  = new  (nothrow)char[strlen(strAscii.c_str())+1];
    if (srcpath == NULL)
    {
        cout << "Error: main (48) memory could not be allocated" << endl;
	if(srcpath)
	    delete[] srcpath;
	return false;
    }
    else
    {
	strcpy(srcpath,strAscii.c_str());
    }

    if(!(pfile = fopen(srcpath, "rb")))
    {
	    printf("Sorry, can't open %s\n", srcpath);
	    if(srcpath)
		delete[] srcpath;
	    return false;
    }
    fclose(pfile);


    cOutput = new COutput();

    int argument = 0;

    switch(isMToolsChecked)
    {
        case 0:
        argument = DIGICOMMENT;
        break;

        case 2:
        argument = MTOOLSCOMMENT;
        break;
    }

    //cout << "argument: " << argument << endl;
    //qDebug() << "srcpath" << srcpath;
    mutex.lock();
    char *arraypointer;
    arraypointer=NULL;
    arraypointer = cOutput->getMetaDataV2(argument,srcpath);
    //mutex.unlock();//! stuerzt ab wenn array leer ist!
    if(arraypointer == NULL)
    {
        noScanDataFoundList.append(path);
        //qDebug() << "NO <ARRAY>" << path;
        if(cOutput)
            delete cOutput;
        if(srcpath)
            delete []srcpath;
        return false;
    }
    QByteArray data;
    data.append(arraypointer);
    int count = data.count("\n");
    for(int i = 0; i<count;i++)
    {
        QString string = data.left(data.indexOf("\n"));
        QString utfstring = string.toUtf8();
        //qDebug() << "UNICODE: " << utfstring;
        Ldataentry.append(utfstring);
        //Ldataentry.append(data.left(data.indexOf("\n")));
        data.remove(0,data.indexOf("\n")+1);
    }
#ifdef M_PRINT_DATAENTRYLIST
    printDataEntryList();
#endif
    if(cOutput)
	delete cOutput;
    if(srcpath)
	delete []srcpath;
    return true;
}

#endif

QList<QVariant> ScanHelper::getLdataEntry()
{
    return Ldataentry;
}

QStringList ScanHelper::getNoScanDataFoundList()
{
    return noScanDataFoundList;
}

void ScanHelper::printDataEntryList()
{
    foreach(QVariant entry, Ldataentry){
        qDebug() << entry.toString();
    }
    qDebug() << endl;

}




#ifdef WIN32
//! Verbindung zur meta dll
//! ueber getMetaData
//! ohne new scanHelper kann kein new COutput erzeugt werden
bool ScanHelper::openPath(int isMToolsChecked, QString path)
{
    if(!Ldataentry.isEmpty())
        Ldataentry.clear();
    if(path.length()>=MAX_META_PATHLEN)
    {
        qDebug() << "ERROR: PathName to Long scanHelper 62" << path << endl;
        return false;
    }

    //! Path must be UTF String!
    wstring wString = path.toStdWString();
    int len = wcslen(wString.c_str());
    wchar_t *wpath = new  wchar_t [len+1];
    wcscpy(wpath,wString.c_str());

    wchar_t *srcpath;
    srcpath = wpath;


    FILE *tmpfile;
    if(!(tmpfile = _wfopen(srcpath, L"r")))

    {
            //printf("Sorry, can't open %s\n", srcPath);
            qDebug() << "Sorry, cant open !!!!!!!!! ";
    }

    fclose(tmpfile);



    cOutput = new COutput();
    int argument;
    switch(isMToolsChecked)
    {
    case 0:
        argument = DIGICOMMENT;
        break;

    case 2:
        argument = MTOOLSCOMMENT;
        break;
    }
    wchar_t *arraypointer;
    arraypointer=NULL;
    arraypointer = cOutput->getMetaDataV2(argument,srcpath);
    //! stuerzt ab wenn array leer ist!
    if(arraypointer == NULL)
    {
        noScanDataFoundList.append(path);
        //qDebug() << "NO <ARRAY>" << path;
        if(cOutput)
            delete cOutput;
        if(srcpath)
            delete []srcpath;
        return false;
    }

    int arraylen = wcslen(arraypointer);
    QString indata;
    indata = QString::fromWCharArray(arraypointer,arraylen);

    //! Filenamen werden immer richtig dekodier,
    //! wenn allerdings text Codec auf UTF-8 steht
    //! dann nur wenn hier to Utf8 steht:
     QString data = indata.toUtf8();
    int count = data.count("\n");
    for(int i = 0; i<count;i++)
    {
        Ldataentry.append(data.left(data.indexOf("\n")));
        data.remove(0,data.indexOf("\n")+1);
    }
    if(cOutput)
        delete cOutput;
    if(srcpath)
        delete []srcpath;
    return true;
}
#endif //WIN 32








