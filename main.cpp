#include <QtCore>

#include "coutput.h"
#include <iostream>
#include <clocale>
#include <string>
#include <cstdlib>

using namespace std;
using namespace meta;

//static int argument = MTOOLSCOMMENT;
static int argument = DIGICOMMENT;

QStringList directorySoundFileList;
QList<QVariant>Ldataentry;
int allFilesCount;
bool stopped = false;


#ifdef __APPLE__
#define PROBLEM1 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_sd2/oknotba.wav.sd2"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_wav_test"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_aiff test"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_sd2"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_sd2/quarantaene"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_Time"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder/mp3"
#define PROBLEM2 "/Users/Admin/Music/iTunes"
//#define PROBLEM2 "/Volumes/MacGo/Musik Archiv"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/HannaBarbera"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder 3/_Soundformate/Pt OSX mit comment/quarantaene"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles"
//#define PROBLEM2 "/Users/Admin/Documents/python/_testfiles/testfolder 7"
//#define PROBLEM2 "/Volumes/Little/Archive24"
//#define PROBLEM2 "/Volumes/Macintosh HD"
//#define PROBLEM2 "/Volumes/Archiv_Ex"


bool openPath(QString path)
{
    //! Segmentation FAULT
    //qDebug() << "ScanHelper Path:" << path;

    if(path.length()>=MAX_META_PATHLEN)
    {
	qDebug() << "ERROR: PathName to Long main 42" << path << endl;
	return false;
    }

    //! Path must be UTF String!
    string strAscii = path.toStdString();
    char *srcpath;
    srcpath=NULL;
    int len = strlen(strAscii.c_str());
    srcpath  = new  (nothrow)char[len +1];
    if (srcpath == NULL)
    {
        cout << "Error: main (63) memory could not be allocated" << endl;
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
	    //qDebug() << "ScanHelper: cant open" << srcpath;
	    if(srcpath)
		delete []srcpath;
	    return false;
    }
    fclose(pfile);

    COutput *cOutput;
    cOutput = new COutput;


    //int argument = DIGICOMMENT;
    //cout << "ScanHelper ascii :" << srcpath <<  endl;

    Ldataentry.clear();
    char *arraypointer;

    arraypointer=NULL;
    arraypointer = cOutput->getMetaData(argument,srcpath);
    if(arraypointer == NULL)
    {
        cout << "No ARRAY>" << endl;
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
	Ldataentry.append(data.left(data.indexOf("\n")));
	data.remove(0,data.indexOf("\n")+1);
    }

    if(cOutput)
	delete cOutput;
    if(srcpath)
	delete []srcpath;
  return true;
}

QList<QVariant> getLdataEntry()
{
    //qDebug() << "Ldataentry: " << Ldataentry << endl;
    return Ldataentry;
}

void openDir(QString path)
{
    QDir dir(path);
    QString absolutePath;
    QString fileutf8string;
    QStringList filterstringlist;
    filterstringlist << "wav" << "aif" << "sd2" << "mp3" << "m4a" << "" << "L" << "R";
    //qDebug()<<"dir.entryList: " << dir.entryList(QDir::Files | QDir::NoSymLinks) <<endl;
    foreach (QString file, dir.entryList(QDir::Files | QDir::NoSymLinks))
    {
	QString extension = QFileInfo(file).suffix();
	//qDebug()<< "extension" << extension;
	if( (filterstringlist.contains(extension,Qt::CaseInsensitive)) )
	{
	    //QString absolutePath;
	    fileutf8string = file.toUtf8();
        //qDebug() << "fileutf8string: " << fileutf8string;
	    absolutePath = dir.absoluteFilePath(fileutf8string);
        //qDebug()<<"file absolutePath: " << absolutePath <<endl;
	    QFileInfo finfo(absolutePath);
	    if(finfo.exists())
	    {
		directorySoundFileList.append(absolutePath);
        //qDebug()<<"file absolutePath finfo exists: " << absolutePath <<endl;
		//allFilesCount++;
		//emit emitAllFilesCount(allFilesCount);
	    }
	}
    }

    foreach (QString subDir, dir.entryList(QDir::Dirs
			| QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {
	QString absolutePath;
	absolutePath = dir.absoluteFilePath(subDir);
	if(!stopped == true)
	    openDir(absolutePath);
    }


}

void scanDir()
{

    //qDebug() << "directorySoundFileList" << directorySoundFileList << endl;
    for(int count =0; count< directorySoundFileList.length(); ++count)
    {
    qDebug() << endl << "directorySoundFileList:" << directorySoundFileList.at(count) << "at: " << count;
	if(openPath(directorySoundFileList[count]))
	{
        qDebug() << "Path opened: True " << endl;
	    QList<QVariant> mydataEntryList = getLdataEntry();
	    //if(!(dataEntryList[0].isNull()) && !(dataEntryList.isEmpty()) && !(dataEntryList.contains("\n")) && !(dataEntryList.length() < 3))
	    if(!(mydataEntryList.isEmpty()) && !(mydataEntryList.contains("\n")) && !(mydataEntryList.length() < 3))
	    {
		//if(queryExecute(dataEntryList,choosenArchiv))
		//{
		   // qDebug()<< "mydataEntryList" << mydataEntryList;
		    //qDebug()<< "choosenArchiv" << choosenArchiv;
		    //++addedCount;
		//}
            foreach(QVariant entry, mydataEntryList)
                qDebug() << entry.toString();
            qDebug() << endl;
	    }

	    //emit totalCount(count);
	    //scanHelper->cleanUp();
	}
    }

}

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    char askpath[256]=PROBLEM2;
    char* srcPath;

    //pfile = sndfile->getFile();
    //! No file name on command line?
    if(argc == 1)
    {
    //qDebug() << "REACHED = 1" ;
    // Prompt for input:
    //printf("Please enter a Filepath: ");
    // Get the file name entered
    //fgets(askpath, MAXPATHLEN, stdin);

    int len = strlen(askpath);
    //Remove the newline if it's there
    if(askpath[len-1] == '\n')
    {
        askpath[len-1] = '\0';
    }
    }
    // Get 2nd command line string
    else
      strcpy(askpath, argv[1]);
    /**
    if(!(pfile = fopen(askpath, "rb")))
    {
    // Test auf einen gueltigen Pfad.
    printf("Sorry, can't open %s\n", askpath);
    return -1;
    }
    fclose(pfile);
    */
    srcPath =  askpath;
    //cout << "askpath: " << askpath << endl;
    QString  stringpath;
    stringpath = QString::fromAscii((const char*)askpath);
    //cout << "open: " << openPath(stringpath) << endl;
    /**
    if (openPath(stringpath))
    {
    //getMetaData();
    cleanUp();
    }
    */
    openDir(stringpath);
    //qDebug() << "directorySoundFileList" << directorySoundFileList << endl;
    scanDir();
    //return a.exec();
}

#endif



#ifdef WIN32
#include <windows.h>
#define PROBLEM1 "G:\Users\Admin\Documents\python\_testfiles\testfolder\_no_prob/_sd2\oknotba.wav.sd2"
//#define PROBLEM2 L"G:\\Users\\Admin\\Documents\\python\\_testfiles\\testfolder\\_no_prob\\\uc39cmlauttest"
#define PROBLEM2 L"G:/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/\uc39cmlauttest"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\_no_prob\\\u00dcmlauttest"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder"
//#define PROBLEM2 L"C:\\Qt_Projekte\\thereis"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\Drones"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\mp3"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\thereis"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\Wav 24 bit"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\_special"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\Fade Files"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\Unterordner_1"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\testfiles"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_aiff test"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_sd2"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_sd2/quarantaene"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/_Time"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/HannaBarbera"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder 3/_Soundformate/Pt OSX mit comment/quarantaene"
//#define PROBLEM2 L"G:/Users/Admin/Documents/python/_testfiles"
//#define PROBLEM2 L"C:\\Qt_Projekte\\testfolder\\no_prob\\wav_test\\testfühls"
//#define PROBLEM2 L"/Users/Admin/Documents/python/_testfiles/testfolder 7"
//#define PROBLEM2 L"/Volumes/Little/Archive24"
//#define PROBLEM2 L"/Volumes/Macintosh HD"
//#define PROBLEM2 L"/Volumes/Archiv_Ex"

bool openPath(QString path)
{
    //! Segmentation FAULT
    qDebug() << "ScanHelper Path:" << path;

    if(path.length()>=MAX_META_PATHLEN)
    {
        qDebug() << "ERROR: PathName to Long main 42" << path << endl;
        return false;
    }



    //! Path must be UTF String!
    wstring strw = path.toStdWString();
    /**
    wchar_t *srcpath;
    srcpath=NULL;
    int len = wcslen(strw.c_str());

    srcpath  = new  (nothrow)wchar_t[len +1];
    if (srcpath == NULL)
    {
        cout << "Error: main (48) memory could not be allocated" << endl;
        if(srcpath)
            delete[] srcpath;
        return false;
    }
    else
    {
        wcscpy(srcpath,strw.c_str());
    }
    */
    // Kopiere in std::vector inklusive Nullterminierung
    //std::vector<wchar_t> vec(strw.begin(), strw.end());
    //vec.push_back(L'\0');

    // Extrahiere wchar_t*
    //LPWSTR srcpath = &vec[0];
    //for(int i = 0; i< wcslen(srcpath);++i)
    //    wprintf(L"srcpath at: %d %d\n",i,srcpath[i]);

    int len = wcslen(strw.c_str());
    wchar_t *wpath = new  wchar_t [len+1];
    wcscpy(wpath,strw.c_str());

    wchar_t *srcpath;
    //srcpath = array;
    srcpath = wpath;

    //wcout << endl << "path fuer Ueberhabe: " << srcpath << endl;

    if(!(pfile = _wfopen(srcpath, L"r")))
    {
            //printf("Sorry, can't open %s\n", srcpath);
            //qDebug() << "ScanHelper: cant open" << srcpath;
        wcout << "Sorry, can't open"<< srcpath << endl;

            if(srcpath)
                delete []srcpath;
            return false;
    }
    fclose(pfile);

    COutput *cOutput;
    cOutput = new COutput;


    //int argument = DIGICOMMENT;
    //cout << "ScanHelper ascii :" << srcpath <<  endl;

    Ldataentry.clear();
    wchar_t *arraypointer=NULL;

    arraypointer = cOutput->getMetaDataV2(argument,srcpath);

    //cOutput->getMetaDataToConsole(argument,srcpath);

    if(arraypointer == NULL)
    {
        //cout << "No ARRAY>" << endl;
        if(cOutput)
            delete cOutput;
        if(srcpath)
            delete []srcpath;
        return false;
    }
    int arraylen = wcslen(arraypointer);
    //cout << "arraylen: " << arraylen << endl;
    //wcout << "arraypointer: " << cOutput->w_getMetaData(argument,srcpath) << endl;
    //QByteArray data;
    QString indata;
    indata = QString::fromWCharArray(arraypointer,arraylen);

    //! Comments werden richtig dekodiert:
    //! wenn text Codec auf UTF-8 in der Main
    //! Funktion und hier und QString to Latin steht:
    //! andernfalls werden dateinamen richtig dekodiert
    //QString data = indata.toLatin1();

    //! Filenamen werden immer richtig dekodier,
    //! wenn allerdings text Codec auf UTF-8 steht
    //! dann nur wenn hier to Utf8 steht:
     QString data = indata.toUtf8();

    //data.append(arraypointer);
    //for(int i=0;i<arraylen;++i)
    //{
    //    data[i]= arraypointer[i];
    //}

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

QList<QVariant> getLdataEntry()
{
    //qDebug() << "Ldataentry: " << Ldataentry << endl;
    return Ldataentry;
}

void openDir(QString path)
{
    qDebug() << "openDir: " << path;

    QDir dir(path.toUtf8());

    QString fileutf8string;
    QStringList filterstringlist;
    filterstringlist << "wav" << "aif" << "sd2" << "mp3" << "m4a" << "" << "L" << "R";
    //qDebug()<<"dir.entryList: " << dir.entryList(QDir::Files | QDir::NoSymLinks) <<endl;
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotDot | QDir::NoDotAndDotDot );
    foreach (QString file, dir.entryList())
    {
        QString extension = QFileInfo(file).suffix();
        //qDebug()<< "extension" << extension;
        //if( (filterstringlist.contains(extension,Qt::CaseInsensitive)) )
        //{
            //QString absolutePath;
            QString absolutePath;
            QString nativePath;
            fileutf8string = file.toUtf8();
            qDebug() << "fileutf8string: " << fileutf8string;
            absolutePath = dir.filePath(fileutf8string);
            nativePath = dir.toNativeSeparators(absolutePath);
            //wcout <<"file absolutePath: " << nativePath.toStdWString() <<endl;

            QFileInfo finfo(nativePath);
            if(finfo.exists())
            {
                directorySoundFileList.append(nativePath);
                //qDebug()<<"file absolutePath finfo exists: " << absolutePath <<endl;
                //allFilesCount++;
                //emit emitAllFilesCount(allFilesCount);
            }
        //}
    }

    foreach (QString subDir, dir.entryList(QDir::Dirs
                        | QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {
        QString absolutePath;
        absolutePath = dir.absoluteFilePath(subDir);
        openDir(absolutePath);
    }


}

void scanDir()
{

    qDebug() << "directorySoundFileList" << directorySoundFileList << endl;
    for(int count =0; count< directorySoundFileList.length(); ++count)
    {
        //qDebug() << "Path for Open Path:" << directorySoundFileList.at(count);
        if(openPath(directorySoundFileList[count]))
        {
            //qDebug() << "TRUE" << endl;
            QList<QVariant> mydataEntryList = getLdataEntry();
            //if(!(dataEntryList[0].isNull()) && !(dataEntryList.isEmpty()) && !(dataEntryList.contains("\n")) && !(dataEntryList.length() < 3))
            if(!(mydataEntryList.isEmpty()) && !(mydataEntryList.contains("\n")) && !(mydataEntryList.length() < 3))
            {
                //if(queryExecute(dataEntryList,choosenArchiv))
                //{
                   // qDebug()<< "mydataEntryList" << mydataEntryList;
                    //qDebug()<< "choosenArchiv" << choosenArchiv;
                    //++addedCount;
                //}
                foreach(QVariant entry, mydataEntryList)
                    qDebug() << entry.toString();
                qDebug() << endl;

            }
            //emit totalCount(count);
            //scanHelper->cleanUp();
        }
    }

}

int main(int argc, char *argv[])
{
    //setlocale(LC_CTYPE, "");
    //setlocale(LC_CTYPE, "");
    setlocale(LC_ALL, "german.utf8");
    //setlocale(LC_ALL, "en_US.utf8");
    //QCoreApplication a(argc, argv);
    //! damit die bext Comments richtig dekodiert werden:
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Latin-1"));

    wchar_t askpath[256];
    //askpath =PROBLEM2;

    wchar_t* srcPath;

    //wstring w_string = utfstring.toStdWString();
    //QString umlautstring = "G:\\Users\\Admin\\Documents\\python\\_testfiles";
    QString umlautstring = "G:\\Users\\Admin\\Music";
    //QString umlautstring = "C:\\Qt_Projekte\\testfolder";
    //QString umlautstring = "C:\\Qt_Projekte\\testfolder\\_no_prob\\simpleTest";
    //QString umlautstring = "C:\\Qt_Projekte\\testfolder\\_no_prob\\_wav_test";
    //QString umlautstring = "C:\\Qt_Projekte\\testfolder\\_no_prob\\\u00dcmlauttest";
    //QString umlautstring = "G:/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/\u00dcmlauttest";
    //QString umlautstring = "G:\\Users\\Admin\\Documents\\python\\_testfiles\\testfolder\\_no_prob\\\u00dcmlauttest";
    //QString umlautstring = "G:\\Users\\Admin\\Documents\\python\\_testfiles\\testfolder\\_no_prob\\\u00dcmlauttest";
    //QString umlautstring = "G:\\Users\\Admin\\Documents\\python\\_testfiles\\testfolder\\_no_prob\\Umlauttest";
    //QString umlautstring = "C:\\Qt_Projekte\\testfolder\\_no_prob\\Umlauttest";
    //QString umlautstring = "G:/Users/Admin/Documents/python/_testfiles/testfolder/_no_prob/Ümlauttest";

    //qDebug() << "umlautstring: " << umlautstring << endl;








    QString umlaututfsstring = umlautstring.toUtf8();
    //qDebug() << "umlaututfsstring: " << umlaututfsstring << endl;
    //int len = umlaututfsstring.size()*2;
    //qDebug() << "len" << len;
    //wchar_t *new_w_string;
    //new_w_string = new wchar_t(len);
    //for (int i=0; i< len; i++)
    //    new_w_string[i]= '\0';
    //const QChar *unicodechar;
    //const unsigned short *shortutf16;
    //unicodechar = umlaututfsstring.unicode();
    //shortutf16 = umlaututfsstring.utf16();
    //for(int i=0; i < len ;++i)
    //{
        //qDebug() << "  shortutf16 at  " <<i << "  " << shortutf16[i];
        //qDebug() << "= unicodechar at " <<i << "  " << unicodechar[i]<<endl;

    //}


    wstring w_string = umlaututfsstring.toStdWString();
    vector<wchar_t> vec(w_string.begin(), w_string.end());
    vec.push_back(L'\0');

    // Extrahiere wchar_t*
    srcPath = &vec[0];

   //wcout << "srcPath: " << srcPath << endl;

    //wchar_t *patharray;
    //path.toWCharArray(patharray);
    //for(int i = 0; i< wcslen(askpath); ++i)
    //    wprintf(L"askpath at: %d %d %c\n",i,askpath[i],askpath[i]);
        //qDebug() << path.at(i);


   //struct statfs s;

   //statfs("G", &s);
   //printf("Filesystem: %x \n" ,s.f_type);

       /**
    DWORD VolumenameSize = 256;
    wchar_t Volumename[256];
    DWORD outsize = 256;
    wchar_t Filesystenname[256];

    GetVolumeInformation(srcPath,Volumename,VolumenameSize,NULL, NULL,NULL,Filesystenname,outsize);
    //wcout << endl << L"Volumename:     " << Volumename <<
    //         endl << L"Filesystenname: " << Filesystenname << endl << endl;
    */


    //pfile = sndfile->getFile();
    //! No file name on command line?
    if(argc == 1)
    {
    //qDebug() << "REACHED = 1" ;
    // Prompt for input:
    //printf("Please enter a Filepath: ");
    // Get the file name entered
    //fgets(askpath, MAXPATHLEN, stdin);

        int len = wcslen(askpath);
    //Remove the newline if it's there
    if(askpath[len-1] == '\n')
    {
        askpath[len-1] = '\0';
    }
    }
    // Get 2nd command line string
    else
          //wcscpy(askpath, argv[1]);
    /**
    if(!(pfile = fopen(askpath, "rb")))
    {
    // Test auf einen gueltigen Pfad.
    printf("Sorry, can't open %s\n", askpath);
    return -1;
    }
    fclose(pfile);
    */
    //srcPath =  askpath;
    //cout << "askpath: " << askpath << endl;
    //QString  stringpath;
    //stringpath = QString::fromAscii((const char*)askpath);
    //QString stringpath = QString::fromWCharArray(askpath);
    //qDebug() << "AskPath nach QString stringpath: " << stringpath << endl;
    qDebug() << "umlaututfsstring: " << umlaututfsstring << endl;
    openDir(umlaututfsstring);
    //qDebug() << "directorySoundFileList: ";
    //foreach(QString listentry,directorySoundFileList)
    //    qDebug() << listentry;
    qDebug() << endl;
    scanDir();
    //return a.exec();
}

#endif




