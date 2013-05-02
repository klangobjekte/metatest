
#include "scanThreadWorker.h"
#include "scanHelper.h"
#ifndef NO_DATABASECONNECTION
//#include "psfdefinitions.h"
//#include "multiconnection.h"
//#include "processhelper.h"
    #include "sqldriver.h"
    #include <QThread>
    #include "customtablemodel.h"
#endif


ScanThreadWorker::ScanThreadWorker( QString connectionName,QObject* parent )
    : QObject( parent )
{
    stopped = false;
    count =0;
    addedCount=0;
    directorySoundFileListLength = 0;
    mToolsCheckedState = MTOOLSCOMMENT;


#ifdef __P_SQLITE_DB

    QSqlDatabase db = QSqlDatabase::database(activeDatabase);
    if(db.driverName() == "QSQLITE"){
        _connectionName = "scanthreadDB";
        //_connectionName = "browser";
    }
     else if (db.driverName() == "QMYSQL"){
        //_connectionName = "browser";
        _connectionName = connectionName;
    }
#else
    _connectionName = connectionName;
#endif


    scanHelper = new ScanHelper();
#ifndef NO_DATABASECONNECTION
    sqLiteDriver = new SqLiteDriver(_connectionName);
#endif
    directorySoundFileList.clear();
    _choosenArchiv.clear();

}

ScanThreadWorker::~ScanThreadWorker()
{
    if (scanHelper)
	delete scanHelper;
    #ifndef NO_DATABASECONNECTION
    delete sqLiteDriver;
    #endif
    closeConnection();
}

void ScanThreadWorker::setDataEntryList(const QStringList &list, const  QList<QVariant> &archiv)
{
   directorySoundFileList = list;
   _choosenArchiv = archiv;
   directorySoundFileListLength = directorySoundFileList.length();
}

void ScanThreadWorker::setMToolsChecked(int state)
{
    mToolsCheckedState = state;
}

//! im Thread wird der Worker  nachdem er
//! erzeugt wurde initialisiert. Es wird
//! eine Connection zur datenbank erzeugt,
//! eine Transaktion und eine neue Query erteugt
void ScanThreadWorker::init()
{
    createConnection();
    transaction();
}

//! ausgeloest von init
void ScanThreadWorker:: createConnection()
{
#ifndef NO_DATABASECONNECTION
    QSqlDatabase db = QSqlDatabase::database(activeDatabase);
    if(db.driverName() == "QSQLITE"){
        if(_connectionName =="scanthreadDB"){
            //qDebug() << db.databaseName();
            QSqlError err = addGLobalConnection("QSQLITE",db.databaseName(),_connectionName);
            qDebug() << "ScanThread createConnection: " << err;
        }
        else{
           _connectionName = db.connectionName();
        }
    }
    else if(db.driverName() == "QMYSQL"){
        //qDebug() << db.databaseName();
        _connectionName = db.connectionName();
        //qDebug() << _connectionName;
    }
#endif
}


//! ausgeloest von init
void ScanThreadWorker::transaction()
{
#ifndef NO_DATABASECONNECTION

    qDebug()<< "Scan Thread transacion" << currentGlobalConnection(_connectionName).transaction();
#endif
}


bool ScanThreadWorker::commit()
{
    bool check = false;
    #ifndef NO_DATABASECONNECTION
    while(query->next())
    {
        qDebug()<< "Commit: query has next";
    }
    check =  currentGlobalConnection(_connectionName).commit();
    qDebug()<< "db->lastError nach Commit: " << currentGlobalConnection(_connectionName).lastError();
    #endif
    return check;
}

bool ScanThreadWorker::rollback()
{
    bool check = false;
#ifndef NO_DATABASECONNECTION
    while(query->next())
    {
        qDebug()<< "Rollback: query has next";
    }
    check =  currentGlobalConnection(_connectionName).rollback();
    qDebug()<< "db->lastError nach Rollback" << currentGlobalConnection(_connectionName).lastError();
#endif
    return check;
}

//! setzt den Querystring - ausgeloest vom thread
bool ScanThreadWorker::queryPrepare(const QList<QVariant> &currentarchivList)
{
#ifndef NO_DATABASECONNECTION
    query = sqLiteDriver->prepareTableSoundFile(currentGlobalConnection(_connectionName),currentarchivList);
    QSqlError err = query->lastError();
     if (err.type() != QSqlError::NoError)
        return true;
#endif
     return false;

}

//! Query die vom Loop ausgefuerht wird
bool ScanThreadWorker::queryExecute( QList<QVariant> LdataEntry,const QList<QVariant> &currentarchivList)
{

    bool ret = false;
#ifndef NO_DATABASECONNECTION
    ret = sqLiteDriver->bindValueTableSoundFile(currentGlobalConnection(_connectionName),LdataEntry,currentarchivList);
    //qDebug() << "queryExeute: " << ret << "LdataEntry" << LdataEntry << "currentarchivList" << currentarchivList;
#endif
    return ret;
}

void ScanThreadWorker::closeConnection()
{
#ifndef NO_DATABASECONNECTION

    qDebug() << "scanThreadWorker closeConnectionCalled";
    //! PROOF
    if(_connectionName == "scanthreadDB"){
        closeGlobalConnection(_connectionName);
        removeGlobalConnection(_connectionName);
    }
#endif
}



//! hier wird der Loop ausgefuehrt
void ScanThreadWorker::executeScanHelper()
{
    addedCount = 0;
    //! PROOF !!!!!!!!!!!!!!!!!!!!!!!

   // scanHelper->mutex.lock();


    for(int count =0; count < directorySoundFileListLength; ++count){
        if(!stopped == false){
            #ifndef NO_DATABASECONNECTION
            query->finish();
            #endif
            stopped = false;
            return;
        }
            //! PROOF !!!!!!!!!!!!!!!!!!!!!!!
        #ifndef NO_DATABASECONNECTION
        mutex.tryLock();
        #endif
        emit addedpath(directorySoundFileList[count]);

        scanHelper->openPath(mToolsCheckedState,directorySoundFileList[count]);
        #ifndef NO_DATABASECONNECTION
        mutex.unlock();
        #endif
        QList<QVariant> mydataEntryList = scanHelper->getLdataEntry();
        //qDebug()<< "mydataEntryList" << mydataEntryList;
        //qDebug()<<"DirectorySoundFileList - count" << directorySoundFileList[count] <<  endl << "----------------------"<<endl;
        //if(!(dataEntryList[0].isNull()) && !(dataEntryList.isEmpty()) && !(dataEntryList.contains("\n")) && !(dataEntryList.length() < 3))
        //qDebug()<< "mydataEntryList" << mydataEntryList;
        //! Write to Database:
        if(!(mydataEntryList.isEmpty()) && !(mydataEntryList.contains("\n")) && !(mydataEntryList.length() < 3))
        {
            if(queryExecute(mydataEntryList,_choosenArchiv))
            {
            //qDebug()<< "dataEntryList" << dataEntryList;
            //qDebug()<< "mydataEntryList" << mydataEntryList;
            //qDebug()<< "choosenArchiv" << _choosenArchiv;
            ++addedCount;
            emit totalCount(count);
            }
        }
    } //! for
    //commit();
    #ifndef NO_DATABASECONNECTION
    query->finish();
    #endif
    emit itemsAdded(addedCount);
    //qDebug() << "addedCount: " << addedCount;
    emit stopThread();// ruft stop im Scan thread auf
}


void ScanThreadWorker::setStopped()
{
    stopped = true;
}

QStringList ScanThreadWorker::getNoScanDataFoundList()
{
    return scanHelper->getNoScanDataFoundList();
}
