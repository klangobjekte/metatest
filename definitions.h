#ifndef PSFDEFINITIONS_H
#define PSFDEFINITIONS_H
#include <QString>
#include <QStringList>


#include <QDir>
#include <vector>
#include <map>



#define __P_USE_TOOLBAR
#define __P_USE_WAVEFORM_TOOLBAR
#define __P_USE_SCROLLBAR_WIDGETS
#define __P_USE_AGREGATIONMODEL
#define __P_USE_AGREGATIONFUNCTIONS
#define __P_SQLITE_DB
#define __P_TEMPORARYPATH "./ProSoundFinder2.app/tmp/";
//#define __P_USE_WAVEFORMLABEL
//#define __P_LOADMP3__WAVE

#define __P_USE_DEPLOY_SAFE_VERSION
#define __P_USE_LIGHT_VERSION
#define __P_USE_SCANDIALOGSHORT
//#define __P_USE_SET_ROW_DRAGGED_ATTRIBUTE
#define __P_USE_PATHWATCHER_ACTION
#define __P_USE_EXPORT_ARCHIV

//namespace psf
//{

extern QString currentStyleSheet;



//! Global static Variables
extern bool historyExists;
extern QStringList openWindowsList;
extern QString documentsPath;
extern QString dateTimeFormat;

static QString versionString = "ProSoundFinder2 -  1.06bpre16";

static QString historyWindowName = " History";
static QString mainWindowName = "ProSoundFinder";

//static QString _connectionName;
static QString  targetApp;



static QString doubleClickedWindowName;

static QStringList recentFiles;
static QStringList currentWindowsList;
static QStringList lastOpenWindowsList;


static QStringList currentArchivesList;
static QStringList selectedArchivesList;
static QStringList lastSelectedArchivesList;

static QString table_files          = "files";
static QString table_soundfiles     = "soundfiles";
static QString table_bextinfo       = "bextinfo";
static QString table_soundinfo     = "soundinfo";
static QString table_bincontent     = "bincontent";
static QString table_musicmetadata  = "musicmetadata";
static QString table_archiv         = "archiv";
static QString table_bins           = "bins";
static QString table_custommetadata = "custommetadata";
static QString table_genres         = "genres";
static QString table_kind           = "kind";
static QString table_users          = "users";
static QString table_userrights     = "rights";
static QString table_categorys      = "categorys";
static QString table_emotions       = "emotions";




//! ERROR STRINGS
static QString errorBextNotPossible =
        "Conversion to Broadcast Wave not supported for Filetype.\nDeselect \"Bwf\"!";
static QString errorCopyNotPossible =
        "Renaming not supported for Filetype.\n Deselect \"Both/Description\"";

static volatile bool dialogFlag;
static volatile bool isStyleSheetEditorOpenFlag;




#ifdef WIN32

#ifdef __MINGW32__
//#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif

    const int   MAXPATHNAMELENGTH   = 259;

    static QString tmpPathFolder = "/psftmp";
    static QString documentsfolder = "/My Documents/ProSoundFinder/";
    static QString databaseString = "soundarchivII.db";
    //! UPDATE BEGINN
    static QString databaseTodeleteString = "psflocalarchiv.db";
    static QString database2String = "psflocalarchivpre5.db";
    //! UPDATE END
    static QString imagepath = "./images/";
    //static QString qsspath = "./qss/";
    static QString qsspath = ":/qss/";
    static QString noScanResult = "./noScanResult.txt";
    static QString notincluded = "./notincluded.txt";
#else
    const int   MAXPATHNAMELENGTH   = 512;

    static QString tmpPathFolder = "/psftmp";
    static QString databaseString = "soundarchivII.db";

    //! UPDATE BEGINN
    static QString databaseTodeleteString = "psflocalarchiv.db";
    static QString database2String = "psflocalarchivpre5.db";
    //! UPDATE END
    static QString documentsfolder = "/Documents/ProSoundFinder/";
    static QString imagepath = "./ProSoundFinder2.app/Contents/images/";

    static QString qsspath = ":/qss/";

    static QString noScanResult = "./ProSoundFinder2.app/Contents/noScanResult.txt";
    static QString notincluded = "./ProSoundFinder2.app/Contents/notincluded.txt";

#endif

    enum{
        P_CREATEWAVEFORM_REQUEST_TYPE_EFAULT            =   0,
        P_CREATEWAVEFORM_REQUEST_TYPE_LOADVISUALS       =   1,
        P_CREATEWAVEFORM_REQUEST_TYPE_TOOLBAR           =   2,
        P_CREATEWAVEFORM_REQUEST_TYPE_SNDFILE           =   3,
        P_CREATEWAVEFORM_REQUEST_TYPE_VISIBILITYCHANGED =   4,
        P_CREATEWAVEFORM_REQUEST_TYPE_UPDATE            =   5,
        P_CREATEWAVEFORM_REQUEST_TYPE_UPDATE_TOOLBAR    =   6,
        P_CREATEWAVEFORM_REQUEST_TYPE_CUSTOM            =   7
    };

    enum{
        P_WAVEFORMCURSORSHAPE_ARROW     =   0,
        P_WAVEFORMCURSORSHAPE_IBEAM     =   1,
        P_WAVEFORMCURSORSHAPE_OPENHAND  =   2
    };

//! filterWidget logicComboBox
    enum{
        P_LOGIC_INVALID     =   -1,
        P_LOGIC_EMPTY       =   0,
        P_LOGIC_OR          =   1,
        P_LOGIC_AND         =   2,
        P_LOGIC_AND_NOT     =   3
    };

//! filterWidget behaviourComboBox
    enum{
        P_BEHAVIOUR_INVALID     =   -1,
        P_BEHAVIOUR_CONTAINS    =   0,
        P_BEHAVIOUR_START_SWITH =   1,
        P_BEHAVIUOR_ENDS_WITH   =   2,
        P_BEHAVIOUR_COMPARE     =   3,
        P_BEHAVIOUR_IS_EXACTLY  =   4,
        P_BAHAVIOUR_LOCALAWARE  =   5
    };

//! Playermessage
enum{
    PLAYER_MESSAGE_NONE = 0,
    PLAYER_MESSAGE_LOADEDNOWAVEFORM = 1,
    PLAYER_MESSAGE_CANTFIND = 2,
    PLAYER_MESSAGE_INVALIDFILE = 3,
    PLAYER_MESSAGE_LOADED = 4};

//! filesystem
enum{
    FILESYSTEM_UNDEFINED = 0,
    FILESYSTEM_NTFS = 1,
    FILESYSTEM_HFS = 2};

//! FILTERSTATE
enum{
    SCAN_NOTFILTERED            = 0,
    SCAN_FILTERED               = 1};

//! UPDATESTATE
enum{

    SCAN_NOUPDATE     = 0,
    SCAN_UPDATE        = 1};

//! DragBehaviourStateFlag
enum{
    DRAGBEHAVIOUR_FILE = 1,
    DRAGBEHAVIOUR_BOTH = 2,
    DRAGBEHAVIOUR_COMMENT = 3,
    DRAGBEHAVIOUR_FILE_BWF = 4,
    DRAGBEHAVIOUR_BOTH_BWF = 5,
    DRAGBEHAVIOUR_COMMENT_BWF = 6};

//! messageColor
enum{
    GREEN_MESSAGE = 0,
    RED_MESSAGE = 2,
    BLUE_MESSAGE = 1};

//! Filetyp
enum { MagicNumber = 0x7F51C883,
     MagicBinNumberPSFB = 0x50534642,
     MagicArchivNumberPSFA = 0x50534641};

//! Filetyp
enum{
    BIN_FILE = 0,
    ARCHIV_FILE = 1,
    PROJECT_FILE = 2};

enum{
    TIME_LABEL	    = 0,
    BEGINN_LABEL	= 1,
    END_LABEL	    = 2,
    DURATION_LABEL  = 3};

enum {
    SOUNDFILE_OK_NOT        = 0,
    SOUNDFILE_OK_MP3        = 1,
    SOUNDFILE_OK_NOWAVE     = 2,
    SOUNDFILE_OK_EMPTY      = 3,
    SOUNDFILE_OK_NOTFOUND   = 4,
    SOUNDFILE_OK_ZEROLENGTH = 5,
    SOUNDFILE_OK_ALL        = 6,
    SOUDFILE_OK_URL         = 7,
    SOUNDFILE_OK_PHONON_ERROR =8};

enum {
    WAVEFORMCOLOR_BLACK     = 0,
    WAVEFORMCOLOR_BLUE      = 1,
    WAVEFORMCOLOR_RED       = 2,
    WAVEFORMCOLOR_GREEN     = 3,
    WAVEFORMCOLOR_SILVER    = 4};

enum { MaxRecentFiles = 5 };
enum { MaxRecentWindows = 25};
enum { MaxRecentArchives = 25};
enum { MaxRecentColumns = 100 };
enum { MaxRecentTableRowSizes = 8};
enum { MaxRecentChannels = 12};
enum { MaxRecentWatchFolders = 200};

//! BinContent
enum {
   Bincontent_SoundFile_Id =0,
   Bincontent_Bin_Id = 1};

//! Bins
enum {
    Bins_BinId=0,
    Bins_Binname=1};

//! Archiv
enum {
    P_Archiv_Id=0,
    P_Archiv =1,
    P_Archivpath = 2,
    P_Watched = 3};

#ifdef WIN32
//#include <memory.h>
#include <wchar.h>
#include <stdlib.h>
// for gcc 4.3!!
//#include <cstdlib>
//#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES  1
#define _CRT_SECURE_DEPRECATE_MEMORY
//#define __FORCE_CLEAR_SETTINGS__

typedef wchar_t         psf_char;
#define psf_cscpy       wcscpy
#define psf_strcmp      wcscmp
#define psf_sprintf     swprintf
#define psf_printf      wprintf
#define psf_strrchr     wcsrchr
#define psf_strstr      wcsstr
#define psf_strdup      wcsdup
#define psf_strlen      wcslen
#define psf_atol       _wtol
#define psf_memcpy      wmemcpy
#define psf_memset      wmemset
#define psf_sf_open     sf_wchar_open
#define psf_string      wstring
#define psf_tostring    toStdWString
#define psf_cout        wcout
#define TSTR(text) L##text
#define psf_f_open sf_wchar_open

#else

#define kNeedsHelpErr			1
#define kInvalidArgumentErr		2
#define kInvalidDestinationErr	3
#define kCouldNotCreateCFString	4
#define kCouldNotGetStringData	5

typedef char psf_char;
#define psf_cscpy       strcpy
#define psf_strcmp      strcmp
#define psf_sprintf     sprintf
#define psf_printf      printf
#define psf_strrchr     strrchr
#define psf_strstr      strstr
#define psf_strdup      strdup
#define psf_strlen      strlen
#define psf_atol        atol
#define psf_memcpy      memcpy
#define psf_memset      memset
#define psf_tostring    toStdstring
#define psf_cout        cout
#define psf_sf_open     sf_open
#define psf_string      string
#define TSTR(text)      text
#define psf_f_open      sf_open
#endif



//}//! namespace psf
#endif // PSFDEFINITIONS_H
