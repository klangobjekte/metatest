#ifndef SQLWORKER_H
#define SQLWORKER_H
#define NO_DATABASECONNECTION

#include <QObject>
#include <QStringList>
#ifndef NO_DATABASECONNECTION
#include <QtSql>
#include <QMutex>
QT_FORWARD_DECLARE_CLASS(SqLiteDriver)
class CustomTableModel;
class QSqlError;
#endif

QT_FORWARD_DECLARE_CLASS(ScanHelper)






class ScanThreadWorker : public QObject
{
    Q_OBJECT
    public:
        ScanThreadWorker( QString connectionName,QObject* parent = 0);
        ~ScanThreadWorker();
        void setDataEntryList(const QStringList &list, const QList<QVariant> &archiv);
	void setMToolsChecked(int state);
        void init();

        void createConnection();

        void transaction();

        bool queryPrepare(const QList<QVariant> &currentarchivList);
        bool queryExecute(QList<QVariant> LdataEntry,const QList<QVariant> &currentarchivList);
        void closeConnection();
	QStringList getNoScanDataFoundList();

    public slots:

        void executeScanHelper();
        bool commit();
        bool rollback();
        void setStopped();

    signals:
        void stopThread();
        void itemsAdded(const long &addedCount);
        void totalCount(const long &count);
        void addedpath(QString addedpath);
        void publishThreadFilesAdded(const QList<QVariant>&,
        const QList<QVariant>&);

    protected:


    private:
        //SqLiteDriver *sqLiteDriver;
        //QSqlQuery *query;
        ScanHelper *scanHelper;
        QString _connectionName;
        //QMutex mutex;
        int mToolsCheckedState;

        bool dbOpen;
        volatile bool stopped;

        int directorySoundFileListLength;
	long count;
	long addedCount;
    int queryat;

        QString messageStr;
        QStringList directorySoundFileList;
        //QList<QVariant> dataEntryList;
        QList<QVariant> _choosenArchiv;

};



#endif // SQLWORKER_H
