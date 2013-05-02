#include "directorythreadworker.h"
#include "iostream"

#ifndef NO_DATABASECONNECTION
#include <QSqlQuery>
#endif



#include <stdlib.h>
#include "qmutex.h"

using namespace std;

DirectoryThreadWorker::DirectoryThreadWorker(QString connectionName,QObject *parent)
    :QObject(parent)
{
#ifndef NO_DATABASECONNECTION

    QSqlDatabase db = QSqlDatabase::database(activeDatabase);
    if(db.driverName() == "QSQLITE"){
        _connectionName = "directorythreadDB";
        //_connectionName = "browser";
        QSqlError err = addGLobalConnection("QSQLITE",db.databaseName(),_connectionName);
        qDebug() << "Directory Thread createConnection: " << err;

    }
     else if (db.driverName() == "QMYSQL"){
        //_connectionName = "browser";
        _connectionName = connectionName;
    }
    sqLiteDriver = new SqLiteDriver(_connectionName);
    updatestate=SCAN_NOTFILTERED;
    updatestate = SCAN_NOUPDATE;
#endif
}

DirectoryThreadWorker::~DirectoryThreadWorker()
{
#ifndef NO_DATABASECONNECTION

    delete sqLiteDriver;
    closeConnection();
#endif
}

void DirectoryThreadWorker::cleardirectorySoundFileList()
{
    directorySoundFileList.clear();
}

void DirectoryThreadWorker::setStopped(bool stopOpenPath)
{
    stopped = stopOpenPath;
}

void DirectoryThreadWorker::clearAllFilesCount()
{
    allFilesCount=0;
}

void DirectoryThreadWorker::runOpenPath(QString path,const QVariantList &archiv)
{
    allFilesCount = 0;
    possibleFilesCount = 0;
    completeFileCount =0;
    orphantList.clear();



    //! get all filesInDB for an Archiv
    if(!archiv.isEmpty())
        setFilesInDB(archiv);

    //! make the directorySoundFileList:
    if(filterstate == SCAN_NOTFILTERED)
    {
        //cout << "open Path" << path.toStdString() << endl;
        openPath(path);
    }
    else if (filterstate == SCAN_FILTERED)
    {
        //qDebug() << "open Filtered Path";
        openFilteredPath(path);
    }

    //! decide which entry of the directorySoundFileList should be scanned:
    //! (the directorySoundFileList is modified)
    if(updatestate == SCAN_NOUPDATE){
        foreach(QString string, filesInDB){
                if(directorySoundFileList.contains(string)){
                    directorySoundFileList.removeOne(string);
                }
            }
    }
    else{
        cout << " filesInDB Size : " << filesInDB.size() << endl;
        cout << "directorySoundFileList Size : " << directorySoundFileList.size() << endl;
        foreach(QString string, filesInDB){
                int i = directorySoundFileList.indexOf(string);
                if(!directorySoundFileList.contains(string)){
                    orphantList.append(string);
                }
                else{
                    directorySoundFileList.removeOne(string);
                }
            }
            if(!orphantList.isEmpty()){
                deleteOrphantEntries(archiv);
            }
        cout << "directorySoundFileList update Size: " << directorySoundFileList.size() << endl;
        cout << "orphantList Size: " << orphantList.size();
        //qDebug() << "directorySoundFileList update: " << directorySoundFileList << endl;
        }
}

void DirectoryThreadWorker::deleteOrphantEntries(const QVariantList &archiv){

    int  Archiv_Id = archiv.at(1).toInt();

    foreach (QString path, orphantList) {
#ifndef NO_DATABASECONNECTION
        sqLiteDriver->deleteSoundfilesAllRowsForArchiv(currentGlobalConnection(_connectionName), path, Archiv_Id);
#endif
    }

}

void DirectoryThreadWorker::openPath(QString path)
{
    QDir dir(path);
    QString scanPath;
    QString fileutf8string;
    QStringList dirfilterstringlist;

    #ifdef WIN32
        QString absolutePath;
        dirfilterstringlist << "Programm Files (x86)"
                        << "Program Files" << "ProgramData" << "Windows" << "tools" << "Intel";
    #else
        dirfilterstringlist << "System" << "Library" << "Applications" << "system" << "Contents"<< "MacOS";
    #endif

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotDot | QDir::NoDotAndDotDot );

    foreach (QString file, dir.entryList())
    {
        allFilesCount++;
        emit emitAllFilesCount(allFilesCount);
        fileutf8string = file.toUtf8();

        #ifdef WIN32
            absolutePath = dir.filePath(fileutf8string);
            scanPath = dir.toNativeSeparators(absolutePath);
        #else
            scanPath = dir.absoluteFilePath(fileutf8string);
        #endif
            QFileInfo finfo(scanPath);
        if(finfo.exists())
        {
            directorySoundFileList.append(scanPath);
            //qDebug()<<"file scanPath: " << scanPath <<endl;
            possibleFilesCount ++;
            emit emitPossibleFilesCount(possibleFilesCount);
            emit emitIncludedFile(scanPath);
        }
        else
        {
            notIncludedList.append(scanPath);
            emit emitNotIncludedFile(scanPath);
        }
    }
    foreach (QString subDir, dir.entryList(QDir::Dirs
                                           | QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {
        scanPath = dir.absoluteFilePath(subDir);
        if(!stopped == true && !dirfilterstringlist.contains(subDir))
            openPath(scanPath);
    }
}

void DirectoryThreadWorker::openFilteredPath(QString path)
{
    QDir dir(path);
    QString scanPath;
    QString fileutf8string;
    QStringList dirfilterstringlist;
    QStringList filterstringlist;
    filterstringlist << "wav" << "aif" << "sd2" << "mp3" << ""
		     << "L" << "R" << "C" << "LF" << "Ls" << "Rs"
		     << "A1" <<"A2" << "A3" << "A4"<<"A4"<<"A6"<<"A7"<<"A8";
    #ifdef WIN32
        QString absolutePath;
        dirfilterstringlist << "Programm Files (x86)"
                        << "Program Files" << "ProgramData" << "Windows" << "tools" << "Intel";
    #else
        dirfilterstringlist << "System" << "Library" << "Applications" << "system" << "Contents"<< "MacOS";
    #endif

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotDot | QDir::NoDotAndDotDot );

    foreach (QString file, dir.entryList())
    {
        //qDebug() << dir.entryList().size();
        allFilesCount++;
        emit emitAllFilesCount(allFilesCount);
        fileutf8string = file.toUtf8();

        QString extension = QFileInfo(file).suffix();
        //qDebug()<< "extension" << extension;
        #ifdef WIN32
            absolutePath = dir.filePath(fileutf8string);
            scanPath = dir.toNativeSeparators(absolutePath);
        #else
            scanPath = dir.absoluteFilePath(fileutf8string);
        #endif

        if( (filterstringlist.contains(extension,Qt::CaseInsensitive)) )
        {

            QFileInfo finfo(scanPath);
            if(finfo.exists())
            {
                //qDebug() << "finfo: " << finfo.lastRead();
                directorySoundFileList.append(scanPath);
                //qDebug()<<"file scanPath: " << scanPath <<endl;
                possibleFilesCount ++;
                emit emitPossibleFilesCount(possibleFilesCount);
                emit emitIncludedFile(scanPath);
            }
            else
            {
                notIncludedList.append(scanPath);
                emit emitNotIncludedFile(scanPath);
            }
        }
        else
        {
            notIncludedList.append(scanPath);
            emit emitNotIncludedFile(scanPath);
        }
    }//foreach

    foreach (QString subDir, dir.entryList(QDir::Dirs
					   | QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {

        scanPath = dir.absoluteFilePath(subDir);
        if(!stopped == true && !dirfilterstringlist.contains(subDir))
        {
            //qDebug() << "subDir: " << subDir;
            openFilteredPath(scanPath);
        }
    }
}

void DirectoryThreadWorker::setFilesInDB(const QVariantList &archiv)
{
    filesInDB.clear();

    //! get the Id for a given  Archivname
    //QString Archiv_Id = sqLiteDriver->idforRowTableArchiv(currentGlobalConnection(_connectionName),archiv);
    //QString Archiv_Id = archiv.at(1).toString();
    int  Archiv_Id = archiv.at(1).toInt();
    //qDebug() << "Archiv_Id: " << Archiv_Id;

    //! get the Path for all Stored Files for a given Archiv
    //! Proof auslagern ins Model - damit schnellerer Scan?
#ifndef NO_DATABASECONNECTION
    filesInDB = sqLiteDriver->selectSoundfilesAllRowsForArchiv(
                currentGlobalConnection(_connectionName),"Path",Archiv_Id);
#endif
    //qDebug() << "filesInDB from SqLite: " << filesInDB;
    QStringList columns;
    columns.append("Path");
    columns.append("Description");
    columns.append("Userdescription");
    //QList<QStringList> extendedFilesInDb;
    //extendedFilesInDb = sqLiteDriver->selectSoundfilesAllRowsColumnsForArchiv(
    //                currentGlobalConnection(_connectionName),columns,Archiv_Id);

}



QStringList DirectoryThreadWorker::getNotIncludedList()
{
    return notIncludedList;
}

void DirectoryThreadWorker::fileCount(QString path)
{
    QDir dir(path);
    QString absolutePath;
    allFilesCount = allFilesCount + (dir.entryList(QDir::Files | QDir::NoSymLinks).length());

    foreach (QString subDir, dir.entryList(QDir::Dirs
                                           | QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {
        absolutePath = dir.absoluteFilePath(subDir);
        fileCount(absolutePath);
    }

}

int DirectoryThreadWorker::getFileCount()
{
    int filecount = allFilesCount;
    allFilesCount = 0;
    return filecount;


}

QStringList DirectoryThreadWorker::getdirectorySoundFileList()
{
    return directorySoundFileList;
}



bool DirectoryThreadWorker::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}


void DirectoryThreadWorker::setFilterState(int state)
{
    //qDebug()<< " directorythreadworker filterState: " << state;
    filterstate = state;
}

void DirectoryThreadWorker::setUpdateState(int state)
{
    //qDebug()<< " directorythreadworker filterState: " << state;
    updatestate = state;
}

void DirectoryThreadWorker::closeConnection()
{
#ifndef NO_DATABASECONNECTION

    cout << "scanThreadWorker closeConnectionCalled" << endl;
    //! PROOF
    if(_connectionName == "directorythreadDB"){
        closeGlobalConnection(_connectionName);
        removeGlobalConnection(_connectionName);
    }
#endif
}


