#ifndef DIRECTORYTHREADWORKER_H
#define DIRECTORYTHREADWORKER_H
#include <QObject>
#include <QDir>
#include <QStringList>
#include <QVariantList>
#include "definitions.h"

#define NO_DATABASECONNECTION

#ifndef NO_DATABASECONNECTION
#include "psfdefinitions.h"
#include "multiconnection.h"
#include "sqldriver.h"
#endif



class DirectoryThreadWorker : public QObject
{
    Q_OBJECT

public:
    DirectoryThreadWorker(QString connectionName = "directorythreadDB",QObject *parent = 0);
    ~DirectoryThreadWorker();

    void setStopped(bool stopOpenPath);
    void cleardirectorySoundFileList();
    void clearAllFilesCount();
    void openPath(QString path);
    void openFilteredPath(QString path);
    QStringList getdirectorySoundFileList();

    //void getDirectorysfromTable();



    void fileCount(QString path);
    int getFileCount();
    bool removeDir(const QString &dirName);
    QStringList getNotIncludedList();
    void setFilterState(int state);

    void setUpdateState(int state);



private:

#ifndef NO_DATABASECONNECTION
    //QMutex mutex;
    SqLiteDriver *sqLiteDriver;
#endif
    QStringList filesInDB;

    void closeConnection();
    void setFilesInDB(const QVariantList &archiv);
    void deleteOrphantEntries(const QVariantList &archiv);

    QStringList directorySoundFileList;
    QStringList orphantList;
    QStringList notIncludedList;

    qlonglong completeFileCount;
    long allFilesCount;
    long possibleFilesCount;
    volatile bool stopped;
    int filterstate;
    int updatestate;

    QString _connectionName;


signals:
    void finished();
    void emitAllFilesCount(const long &allFilesCount);
    void emitPossibleFilesCount(const long &possibleFilesCount);

    void emitIncludedFile(QString absolutePath);
    void emitNotIncludedFile(QString absolutePath);


public slots:
    void runOpenPath(QString path,const QVariantList &archiv);

};

#endif // DIRECTORYVIEWER_H
