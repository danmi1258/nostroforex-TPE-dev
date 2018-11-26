//+------------------------------------------------------------------+
//|                                         MetaTrader 5 API Report  |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "..\..\Common\MT5APIConstants.h"
#include "..\..\Common\MT5APILogger.h"
#include "..\..\Common\MT5APITools.h"
#include "..\..\Common\Config\MT5APIConfigReport.h"
#include "..\..\Common\Config\MT5APIConfigCommon.h"
#include "..\..\Common\Config\MT5APIConfigTime.h"
#include "..\..\Common\Config\MT5APIConfigHoliday.h"
#include "..\..\Common\Config\MT5APIConfigSymbol.h"
#include "..\..\Common\Config\MT5APIConfigGroup.h"
#include "..\..\Common\Config\MT5APIConfigManager.h"
#include "..\..\Common\Config\MT5APIConfigNetwork.h"
#include "..\..\Common\Config\MT5APIConfigPlugin.h"
#include "..\..\Common\Config\MT5APIConfigFeeder.h"
#include "..\..\Common\Config\MT5APIConfigGateway.h"
#include "..\..\Common\Config\MT5APIConfigParam.h"
#include "..\..\Common\Bases\MT5APIUser.h"
#include "..\..\Common\Bases\MT5APIAccount.h"
#include "..\..\Common\Bases\MT5APIDeal.h"
#include "..\..\Common\Bases\MT5APIOrder.h"
#include "..\..\Common\Bases\MT5APIPosition.h"
#include "..\..\Common\Bases\MT5APIDaily.h"
#include "..\..\Common\Bases\MT5APITick.h"
#include "..\..\Common\Bases\MT5APIChart.h"
//+------------------------------------------------------------------+
//| Report API version                                               |
//+------------------------------------------------------------------+
#define MTReportAPIVersion       1755
#define MTReportAPIDate          L"18 Jan 2018"
//+------------------------------------------------------------------+
//| Report constants                                                 |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Standard report parameter names                                  |
//+------------------------------------------------------------------+
#define MTAPI_PARAM_GROUPS              L"Groups"         // Groups list
#define MTAPI_PARAM_SYMBOLS             L"Symbols"        // Symbols list
#define MTAPI_PARAM_FROM                L"From"           // From
#define MTAPI_PARAM_TO                  L"To"             // To
#define MTAPI_PARAM_IE_VERSION          L"IE_Version"     // IE version
#define MTAPI_PARAM_SCR_WIDTH           L"Screen_Width"   // Screen width
#define MTAPI_PARAM_SCR_HEIGHT          L"Screen_Height"  // Screen height
//+------------------------------------------------------------------+
//| Standard report colors of accounts                               |
//+------------------------------------------------------------------+
#define MTAPI_COLOR_ACOUNTS_REAL        0xE1441D          // Color of real accounts
#define MTAPI_COLOR_ACOUNTS_PRELIMINARY 0x20F070          // Color of preliminary accounts
#define MTAPI_COLOR_ACOUNTS_DEMO        0xAECF39          // Color of demo accounts
#define MTAPI_COLOR_ACOUNTS_CONTEST     0x0382CE          // Color of contest accounts
#define MTAPI_COLOR_ACOUNTS_COVERAGE    0xFCC503          // Color of coverage accounts
#define MTAPI_COLOR_ACOUNTS_MANAGER     0xAF6734          // Color of manager accounts
#define MTAPI_COLOR_ACOUNTS_OTHER       0x97A1B2          // Color of other accounts
//+------------------------------------------------------------------+
//| Standard report colors of profit and loss                        |
//+------------------------------------------------------------------+
#define MTAPI_COLOR_PROFIT              0x9cd29e          // Color of profit
#define MTAPI_COLOR_LOSS                0xe79590          // Color of loss
#define MTAPI_COLOR_NET_PROFIT          0x115cae          // Color of net profit
//+------------------------------------------------------------------+
//| Standard report colors of terminal types                         |
//+------------------------------------------------------------------+
#define MTAPI_COLOR_TERMINAL_CLIENT     0x115cae          // Color of Client terminal
#define MTAPI_COLOR_TERMINAL_IPHONE     0x9cd29e          // Color of IPhone terminal
#define MTAPI_COLOR_TERMINAL_ANDROID    0xce9203          // Color of Android terminal
//+------------------------------------------------------------------+
//| Common colors to draw charts and graphs                          |
//+------------------------------------------------------------------+
static UINT ExtChartColors[]= {
   0x6E90D0,
   0x6165A9,
   0xF2B742,
   0xC07281,
   0xF66843,
   0x966BAC,
   0xABA6AA,
   0x5ED1F0,
   0x8CC56F,
   0xA2A5AF,
   0xF78B45
   };
//+------------------------------------------------------------------+
//| Structures                                                       |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Report parameter structure                                       |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct MTReportParam
  {
   //--- parameter types
   enum ParamType
     {
      TYPE_STRING    =0,                              // string
      TYPE_INT       =1,                              // integer
      TYPE_FLOAT     =2,                              // floating
      TYPE_TIME      =3,                              // time only
      TYPE_DATE      =4,                              // date only
      TYPE_DATETIME  =5,                              // date & time
      TYPE_GROUPS    =6,                              // groups list
      TYPE_SYMBOLS   =7,                              // symbols list
      //---
      TYPE_FIRST     =TYPE_STRING,
      TYPE_LAST      =TYPE_SYMBOLS
     };
   //---
   UINT              type;                            // type
   wchar_t           name[64];                        // name
   wchar_t           defvalue[256];                   // default value
   UINT              reserved[16];                    // reserved
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Report about structure                                           |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct MTReportInfo
  {
   //--- snapshot mode flags
   enum EnSnapshots
     {
      SNAPSHOT_NONE          =0x0,                    // without snapshots
      SNAPSHOT_USERS         =0x1,                    // users database snapshot for request
      SNAPSHOT_USERS_FULL    =0x2,                    // full users database snapshot
      SNAPSHOT_ACCOUNTS      =0x4,                    // trade account states snapshot
      SNAPSHOT_ACCOUNTS_FULL =0x8,                    // trade account states snapshot for request
      SNAPSHOT_ORDERS        =0x10,                   // orders database snapshot
      SNAPSHOT_ORDERS_FULL   =0x20,                   // orders database snapshot for request
      SNAPSHOT_POSITIONS     =0x40,                   // positions database snapshot
      SNAPSHOT_POSITIONS_FULL=0x80,                   // positions database snapshot for request
      //---
      SNAPSHOT_ALL           =SNAPSHOT_USERS|SNAPSHOT_ACCOUNTS|SNAPSHOT_ORDERS|SNAPSHOT_POSITIONS,
      SNAPSHOT_ALL_FULL      =SNAPSHOT_USERS_FULL|SNAPSHOT_ACCOUNTS_FULL|SNAPSHOT_ORDERS_FULL|SNAPSHOT_POSITIONS_FULL,
     };
   //--- types reports
   enum EnTypes
     {
      TYPE_NONE              =0x0,                    // no support
      TYPE_HTML              =0x1,                    // hypertext
      TYPE_TABLE             =0x2,                    // binary table
      //---
      TYPE_ALL               =TYPE_HTML|TYPE_TABLE
     };
   //--- minimal IE version
   enum EnIEVersion
     {
      IE_VERSION_ANY         =0x0000,                 // any IE version
      IE_VERSION_9           =0x0900,                 // IE 9 required
      //---
      IE_VERSION_FIRST       =IE_VERSION_ANY,
      IE_VERSION_LAST        =IE_VERSION_9,
     };
   //--- information
   UINT              version;                         // report version
   UINT              version_api;                     // report API version
   UINT              version_ie;                      // minimal IE version
   wchar_t           name[64];                        // report name
   wchar_t           copyright[128];                  // copyright
   wchar_t           description[256];                // description
   UINT              snapshots;                       // snapshot modes
   UINT              types;                           // report types
   UINT              common_reserved[32];             // common reserved
   MTReportParam     params[64];                      // request parameters
   UINT              params_total;                    // request parameters total
   MTReportParam     config[64];                      // config parameters
   UINT              config_total;                    // config parameters total
   UINT              reserved[64];                    // reserved
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Server description structure                                     |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct MTReportServerInfo
  {
   wchar_t           platform_name[64];               // platform name
   wchar_t           platform_owner[128];             // platform owner
   UINT              server_version;                  // server version
   UINT              server_build;                    // server build
   UINT              server_type;                     // server type
   UINT64            server_id;                       // server id
   UINT              reserved[32];                    // reserved
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Report column interface                                          |
//+------------------------------------------------------------------+
class IMTReportColumn
  {
public:
   //--- column data types
   enum EnType
     {
      //--- base types
      TYPE_INT8               =0,                     // Integer (8 bits)
      TYPE_UINT8              =1,                     // Unsigned Integer (8 bits)
      TYPE_INT16              =2,                     // Integer (16 bits)
      TYPE_UINT16             =3,                     // Unsigned Integer (16 bits)
      TYPE_INT32              =4,                     // Integer (32 bits)
      TYPE_UINT32             =5,                     // Unsigned Integer (32 bits)
      TYPE_INT64              =6,                     // Integer (64 bits)
      TYPE_UINT64             =7,                     // Unsigned Integer (64 bits)
      TYPE_DOUBLE             =8,                     // Double
      TYPE_MONEY              =9,                     // Money (Double)
      TYPE_STRING             =10,                    // Unicode String
      TYPE_DATE               =11,                    // Date (Int64)
      TYPE_TIME               =12,                    // Time (Int64)
      TYPE_DATETIME           =13,                    // Datetime (Int64)
      //--- prices
      TYPE_PRICE              =100,                   // Price (Double)
      TYPE_PRICES             =101,                   // Bid/Ask (Double[2])
      TYPE_PRICE_POSITION     =102,                   // Price for positions (Double)
      //--- volumes
      TYPE_VOLUME             =200,                   // Volume (Int64)
      TYPE_VOLUME_ORDER       =201,                   // Initial Volume/Current Volume (UINT64[2])
      //--- positions
      TYPE_POSITION_TYPE      =300,                   // Position Type (UInt)
      //--- orders
      TYPE_ORDER_TYPE         =400,                   // Order Type (UInt)
      TYPE_ORDER_TYPE_TIME    =401,                   // Order Type by Time (UInt)
      TYPE_ORDER_TYPE_REASON  =402,                   // Order Type by Reason (UInt)
      TYPE_ORDER_STATUS       =403,                   // Order Status (UInt)
      TYPE_ORDER_FILLING      =404,                   // Order Filling (UInt)
      //--- deals
      TYPE_DEAL_ACTION        =500,                   // Deal Action (UInt)
      TYPE_DEAL_ENTRY         =501,                   // Deal Entry (UInt)
      //--- accounts
      TYPE_USER_LOGIN         =600,                   // Account Login (UInt64)
      TYPE_USER_LEVERAGE      =601,                   // Account Leverage (UInt)
     };
   //--- column flags
   enum EnFlags
     {
      FLAG_NONE               =0x0,                   // none flags
      FLAG_PRIMARY            =0x1,                   // primary integer column
      FLAG_HIDDEN_VIEW        =0x2,                   // hidden in grid view
      FLAG_HIDDEN_SAVE        =0x4,                   // hidden in saved file
      FLAG_HIDDEN             =FLAG_HIDDEN_VIEW|FLAG_HIDDEN_SAVE, // hidden in grid and file
      FLAG_LEFT               =0x8,                   // force align left
      FLAG_RIGHT              =0x10,                  // force align right
      FLAG_CENTER             =FLAG_LEFT|FLAG_RIGHT,  // force align center
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTReportColumn *column)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- name
   virtual LPCWSTR   Name(void) const=0;
   virtual MTAPIRES  Name(LPCWSTR name)=0;
   //--- column id
   virtual UINT      ColumnID(void) const=0;
   virtual MTAPIRES  ColumnID(const UINT column_id)=0;
   //--- type IMTReportColumn::EnType
   virtual UINT      Type(void) const=0;
   virtual MTAPIRES  Type(const UINT type)=0;
   //--- column relative width
   virtual UINT      Width(void) const=0;
   virtual MTAPIRES  Width(const UINT width)=0;
   //--- column max width in pixels
   virtual UINT      WidthMax(void) const=0;
   virtual MTAPIRES  WidthMax(const UINT width_max)=0;
   //--- default digits
   virtual UINT      Digits(void) const=0;
   virtual MTAPIRES  Digits(const UINT digits)=0;
   //--- column digits reference
   virtual UINT      DigitsColumn(void) const=0;
   virtual MTAPIRES  DigitsColumn(const UINT column_id)=0;
   //--- flags IMTReportColumn::EnFlags
   virtual UINT64    Flags(void) const=0;
   virtual MTAPIRES  Flags(const UINT64 flags)=0;
   //--- offset
   virtual UINT      Offset(void) const=0;
   virtual MTAPIRES  Offset(const UINT offset)=0;
   //--- size
   virtual UINT      Size(void) const=0;
   virtual MTAPIRES  Size(const UINT size)=0;
  };
//+------------------------------------------------------------------+
//| Report summary interface                                         |
//+------------------------------------------------------------------+
class IMTReportSummary
  {
public:
   //--- summary data types
   enum EnType
     {
      //--- base types
      TYPE_INT                =0,                     // Integer (64 bits)
      TYPE_UINT               =1,                     // Unsigned Integer (64 bits)
      TYPE_DOUBLE             =2,                     // Double
      TYPE_MONEY              =3,                     // Money
      TYPE_STRING             =4,                     // Unicode String
      TYPE_DATE               =5,                     // Date
      TYPE_TIME               =6,                     // Time
      TYPE_DATETIME           =7,                     // Datetime
      //--- prices
      TYPE_PRICE              =100,                   // Price
      TYPE_PRICES             =101,                   // Bid/Ask
      //--- volumes
      TYPE_VOLUME             =200,                   // Volume
      TYPE_VOLUME_ORDER       =201,                   // Initial Volume/Current Volume (UINT64[2])
     };
   //--- summary flags
   enum EnFlags
     {
      FLAG_NONE               =0x0,                   // none flag
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTReportSummary *summary)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- column id
   virtual UINT      ColumnID(void) const=0;
   virtual MTAPIRES  ColumnID(const UINT column_id)=0;
   //--- line index
   virtual UINT      Line(void) const=0;
   virtual MTAPIRES  Line(const UINT line)=0;
   //--- column for merging (merging from ColumnID to MergeColumn)
   virtual UINT      MergeColumn(void) const=0;
   virtual MTAPIRES  MergeColumn(const UINT column_id)=0;
   //--- text color
   virtual UINT      Color(void) const=0;
   virtual MTAPIRES  Color(const UINT color)=0;
   //--- flags IMTReportSummary::EnFlags
   virtual UINT64    Flags(void) const=0;
   virtual MTAPIRES  Flags(const UINT64 flags)=0;
   //--- type IMTReportSummary::EnType
   virtual UINT      Type(void) const=0;
   //--- digits
   virtual UINT      Digits(void) const=0;
   virtual MTAPIRES  Digits(const UINT digits)=0;
   //--- integer
   virtual INT64     ValueInt(void) const=0;
   virtual MTAPIRES  ValueInt(const INT64 value)=0;
   //--- unsigned integer
   virtual UINT64    ValueUInt(void) const=0;
   virtual MTAPIRES  ValueUInt(const UINT64 value)=0;
   //--- double
   virtual double    ValueDouble(void) const=0;
   virtual MTAPIRES  ValueDouble(const double value)=0;
   //--- money
   virtual double    ValueMoney(void) const=0;
   virtual MTAPIRES  ValueMoney(const double value)=0;
   //--- string
   virtual LPCWSTR   ValueString(void) const=0;
   virtual MTAPIRES  ValueString(LPCWSTR value)=0;
   //--- date
   virtual INT64     ValueDate(void) const=0;
   virtual MTAPIRES  ValueDate(const INT64 value)=0;
   //--- time
   virtual INT64     ValueTime(void) const=0;
   virtual MTAPIRES  ValueTime(const INT64 value)=0;
   //--- datetime
   virtual INT64     ValueDateTime(void) const=0;
   virtual MTAPIRES  ValueDateTime(const INT64 value)=0;
   //--- price
   virtual double    ValuePrice(void) const=0;
   virtual MTAPIRES  ValuePrice(const double value)=0;
   //--- prices
   virtual double    ValuePricesBid(void) const=0;
   virtual double    ValuePricesAsk(void) const=0;
   virtual MTAPIRES  ValuePrices(const double value_bid,const double value_ask)=0;
   //--- volume
   virtual UINT64    ValueVolume(void) const=0;
   virtual MTAPIRES  ValueVolume(const UINT64 value)=0;
   //--- order volume
   virtual UINT64    ValueVolumeInitial(void) const=0;
   virtual UINT64    ValueVolumeCurrent(void) const=0;
   virtual MTAPIRES  ValueVolume(const UINT64 value_initial,const UINT64 value_current)=0;
  };
//+------------------------------------------------------------------+
//| Report chart series interface                                    |
//+------------------------------------------------------------------+
class IMTReportSeries
  {
public:
   //--- series type
   enum EnSeriesType
     {
      TYPE_TITLE              =0,                     // title
      //---
      TYPE_LINE               =100,                   // line
      TYPE_HISTOGRAM          =101,                   // histogram
      TYPE_BAR                =102,                   // bar
      TYPE_AREA               =103,                   // area
      TYPE_PIECE              =104,                   // piece for pie
     };
   //--- flags
   enum EnSeriesFlags
     {
      FLAG_SHOW_VALUES        =1,                     // show series values on chart
     };

public:
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTReportSeries *series)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- title
   virtual LPCWSTR   Title(void) const=0;
   virtual MTAPIRES  Title(LPCWSTR title)=0;
   //--- type
   virtual UINT      Type(void) const=0;
   virtual MTAPIRES  Type(const UINT type)=0;
   //--- flags IMTReportSeries::EnSeriesFlags
   virtual UINT64    Flags(void) const=0;
   virtual MTAPIRES  Flags(const UINT64 flags)=0;
   //--- color
   virtual UINT      Color(void) const=0;
   virtual MTAPIRES  Color(const UINT color)=0;
   //--- tooltip format
   virtual LPCWSTR   Tooltip(void) const=0;
   virtual MTAPIRES  Tooltip(LPCWSTR tooltip)=0;
   //--- values management
   virtual MTAPIRES  ValueClear(void)=0;
   virtual UINT      ValueTotal(void) const=0;
   virtual MTAPIRES  ValueAdd(LPCWSTR value)=0;
   virtual MTAPIRES  ValueAddInt(const INT64 value)=0;
   virtual MTAPIRES  ValueAddDouble(const double value)=0;
   virtual MTAPIRES  ValueUpdate(const UINT pos,LPCWSTR value)=0;
   virtual MTAPIRES  ValueUpdateInt(const UINT pos,const INT64 value)=0;
   virtual MTAPIRES  ValueUpdateDouble(const UINT pos,const double value)=0;
   virtual LPCWSTR   ValueNext(const UINT pos) const=0;
   virtual MTAPIRES  ValueNextInt(const UINT pos,INT64& value) const=0;
   virtual MTAPIRES  ValueNextDouble(const UINT pos,double& value) const=0;
   virtual MTAPIRES  ValueShift(const UINT pos,const int shift)=0;
   virtual MTAPIRES  ValueDelete(const UINT pos)=0;
   virtual MTAPIRES  ValueDescription(const UINT pos,LPCWSTR descr)=0;
   virtual LPCWSTR   ValueDescription(const UINT pos) const=0;
  };
//+------------------------------------------------------------------+
//| Report chart interface                                           |
//+------------------------------------------------------------------+
class IMTReportChart
  {
public:
   //--- chart type
   enum EnChartType
     {
      TYPE_GRAPH              =0,                     // classical chart
      TYPE_GRAPH_ACCUMULATION =1,                     // classical chart with accumulation
      TYPE_GRAPH_NORMALIZED   =2,                     // classical chart with normalization
      TYPE_GRAPH_STACKED      =3,                     // classical chart with stack
      //---
      TYPE_BAR                =100,                   // line bar chart
      TYPE_BAR_ACCUMULATION   =101,                   // line bar chart with accumulation
      TYPE_BAR_NORMALIZED     =102,                   // line bar chart with normalization
      TYPE_BAR_STACKED        =103,                   // line bar chart with stack
      //---
      TYPE_PIE                =200,                   // pie chart
     };
   //--- flags
   enum EnChartFlags
     {
      FLAG_ACCUMULATED_VALUES =1,                     // show accumulated values
      FLAG_SHOW_TABLE         =2,                     // show value table
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTReportChart *chart)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- title
   virtual LPCWSTR   Title(void) const=0;
   virtual MTAPIRES  Title(LPCWSTR title)=0;
   //--- type
   virtual UINT      Type(void) const=0;
   virtual MTAPIRES  Type(const UINT type)=0;
   //--- digits
   virtual UINT      Digits(void) const=0;
   virtual MTAPIRES  Digits(const UINT digits)=0;
   //--- flags
   virtual UINT64    Flags(void) const=0;
   virtual MTAPIRES  Flags(const UINT64 flags)=0;
   //--- bar height
   virtual UINT      BarHeight(void) const=0;
   virtual MTAPIRES  BarHeight(const UINT height)=0;
   //--- piece tooltip format
   virtual LPCWSTR   PieceTooltip(void)=0;
   virtual MTAPIRES  PieceTooltip(LPCWSTR tooltip)=0;
   //--- piece description format
   virtual LPCWSTR   PieceDescription(void)=0;
   virtual MTAPIRES  PieceDescription(LPCWSTR description)=0;
   //--- series management
   virtual MTAPIRES  SeriesClear(void)=0;
   virtual MTAPIRES  SeriesAdd(IMTReportSeries *series)=0;
   virtual MTAPIRES  SeriesAddCopy(const IMTReportSeries *series)=0;
   virtual MTAPIRES  SeriesDelete(const UINT pos)=0;
   virtual IMTReportSeries* SeriesDetach(const UINT pos)=0;
   virtual MTAPIRES  SeriesUpdate(const UINT pos,IMTReportSeries* series)=0;
   virtual MTAPIRES  SeriesShift(const UINT pos,const int shift)=0;
   virtual UINT      SeriesTotal(void) const=0;
   virtual IMTReportSeries* SeriesNext(const UINT pos)=0;
  };
//+------------------------------------------------------------------+
//| Report API interface                                             |
//+------------------------------------------------------------------+
class IMTReportAPI
  {
public:
   //--- HtmlWriteSafe flags
   enum EnHtmlSafeFlags
     {
      HTML_SAFE_NONE          =0x0,    // no flags
      HTML_SAFE_USENOBSP      =0x1,    // replace blank to &nbsp;
     };
   //--- HtmlTplProcess flags
   enum EnTplProcessFlags
     {
      TPL_PROCESS_NONE        =0x0,    // no flags
     };

public:
   //--- server description
   virtual MTAPIRES  About(MTReportServerInfo& info)=0;
   //--- license check
   virtual MTAPIRES  LicenseCheck(LPCWSTR license_name)=0;
   //--- memory management
   virtual void*     Allocate(const UINT size)=0;
   virtual void      Free(void* ptr)=0;
   //--- clear result
   virtual MTAPIRES  Clear(void)=0;
   virtual MTAPIRES  FunctionReserved1(void)=0;
   virtual MTAPIRES  FunctionReserved2(void)=0;
   virtual MTAPIRES  FunctionReserved3(void)=0;
   virtual MTAPIRES  FunctionReserved4(void)=0;
   virtual MTAPIRES  FunctionReserved5(void)=0;
   virtual MTAPIRES  FunctionReserved6(void)=0;
   //--- common flags functions
   virtual MTAPIRES  IsStopped(void)=0;
   virtual MTAPIRES  IsReserved1(void)=0;
   virtual MTAPIRES  IsReserved2(void)=0;
   virtual MTAPIRES  IsReserved3(void)=0;
   virtual MTAPIRES  IsReserved4(void)=0;
   virtual MTAPIRES  IsReserved5(void)=0;
   virtual MTAPIRES  IsReserved6(void)=0;
   //--- server log functions
   virtual MTAPIRES  LoggerOut(const UINT code,LPCWSTR msg,...)=0;
   virtual MTAPIRES  LoggerRequest(const UINT mode,const UINT type,const INT64 from,const INT64 to,LPCWSTR filter,MTLogRecord*& records,UINT& records_total)=0;
   virtual void      LoggerFlush(void)=0;
   virtual MTAPIRES  LoggerReserved1(void)=0;
   virtual MTAPIRES  LoggerReserved2(void)=0;
   virtual MTAPIRES  LoggerReserved3(void)=0;
   virtual MTAPIRES  LoggerReserved4(void)=0;
   //--- report parameters
   virtual IMTConParam* ParamCreate(void)=0;
   virtual UINT      ParamTotal(void)=0;
   virtual MTAPIRES  ParamGet(LPCWSTR name,IMTConParam *param)=0;
   virtual MTAPIRES  ParamNext(const UINT pos,IMTConParam *param)=0;
   virtual MTAPIRES  ParamReserved1(void)=0;
   virtual MTAPIRES  ParamReserved2(void)=0;
   virtual MTAPIRES  ParamReserved3(void)=0;
   virtual MTAPIRES  ParamReserved4(void)=0;
   //--- report parameters helpers
   virtual MTAPIRES  ParamLogins(UINT64*& logins,UINT& total)=0;
   virtual INT64     ParamFrom(void)=0;
   virtual INT64     ParamTo(void)=0;
   virtual LPCWSTR   ParamGroups(void)=0;
   virtual LPCWSTR   ParamSymbols(void)=0;
   virtual UINT      ParamIEVersion(void)=0;
   virtual UINT      ParamScreenWidth(void)=0;
   virtual UINT      ParamScreenHeight(void)=0;
   virtual MTAPIRES  ParamHelperReserved1(void)=0;
   virtual MTAPIRES  ParamHelperReserved2(void)=0;
   virtual MTAPIRES  ParamHelperReserved3(void)=0;
   virtual MTAPIRES  ParamHelperReserved4(void)=0;
   //--- HTML management
   virtual MTAPIRES  HtmlWrite(LPCWSTR format,...)=0;
   virtual MTAPIRES  HtmlWriteSafe(LPCWSTR html,const UINT flags)=0;
   virtual MTAPIRES  HtmlWriteReserved1(void)=0;
   virtual MTAPIRES  HtmlWriteReserved2(void)=0;
   virtual MTAPIRES  HtmlTplLoad(LPCWSTR templstr)=0;
   virtual MTAPIRES  HtmlTplLoadFile(LPCWSTR templname)=0;
   virtual MTAPIRES  HtmlTplLoadResource(const UINT resid,LPCWSTR restype)=0;
   virtual MTAPIRES  HtmlTplNext(MTAPISTR& tag,UINT *counter)=0;
   virtual MTAPIRES  HtmlTplProcess(const UINT flags)=0;
   virtual MTAPIRES  HtmlTplReserved1(void)=0;
   virtual MTAPIRES  HtmlTplReserved2(void)=0;
   virtual MTAPIRES  HtmlReserved1(void)=0;
   virtual MTAPIRES  HtmlReserved2(void)=0;
   virtual MTAPIRES  HtmlReserved3(void)=0;
   virtual MTAPIRES  HtmlReserved4(void)=0;
   //--- table column management
   virtual IMTReportColumn *TableColumnCreate(void)=0;
   virtual MTAPIRES  TableColumnClear(void)=0;
   virtual MTAPIRES  TableColumnAdd(const IMTReportColumn *column)=0;
   virtual MTAPIRES  TableColumnDelete(const UINT pos)=0;
   virtual UINT      TableColumnTotal(void)=0;
   virtual UINT      TableColumnSize(void)=0;
   virtual MTAPIRES  TableColumnNext(const UINT pos,IMTReportColumn *column)=0;
   virtual MTAPIRES  TableColumnReserved1(void)=0;
   virtual MTAPIRES  TableColumnReserved2(void)=0;
   //--- table row management
   virtual MTAPIRES  TableRowClear(void)=0;
   virtual MTAPIRES  TableRowWrite(const void *data,const UINT size)=0;
   virtual UINT      TableRowTotal(void)=0;
   virtual MTAPIRES  TableRowReserved1(void)=0;
   virtual MTAPIRES  TableRowReserved2(void)=0;
   //--- table summary management
   virtual IMTReportSummary *TableSummaryCreate(void)=0;
   virtual MTAPIRES  TableSummaryClear(void)=0;
   virtual MTAPIRES  TableSummaryAdd(const IMTReportSummary *summary)=0;
   virtual MTAPIRES  TableSummaryDelete(const UINT pos)=0;
   virtual MTAPIRES  TableSummaryNext(const UINT pos,IMTReportSummary *summary)=0;
   virtual UINT      TableSummaryTotal(void)=0;
   virtual MTAPIRES  TableSummaryReserved1(void)=0;
   virtual MTAPIRES  TableSummaryReserved2(void)=0;
   virtual MTAPIRES  TableSummaryReserved3(void)=0;
   //--- table reserved functions
   virtual MTAPIRES  TableReserved1(void)=0;
   virtual MTAPIRES  TableReserved2(void)=0;
   virtual MTAPIRES  TableReserved3(void)=0;
   virtual MTAPIRES  TableReserved4(void)=0;
   virtual MTAPIRES  TableReserved5(void)=0;
   virtual MTAPIRES  TableReserved6(void)=0;
   //--- chart management
   virtual IMTReportChart *ChartCreate(void)=0;
   virtual IMTReportSeries *ChartCreateSeries(void)=0;
   virtual MTAPIRES  ChartWriteHtml(const IMTReportChart *chart)=0;
   virtual MTAPIRES  ChartReserved1(void)=0;
   virtual MTAPIRES  ChartReserved2(void)=0;
   virtual MTAPIRES  ChartReserved3(void)=0;
   virtual MTAPIRES  ChartReserved4(void)=0;
   //--- report configuration
   virtual IMTConReport *ReportCreate(void)=0;
   virtual MTAPIRES  ReportCurrent(IMTConReport *report)=0;
   virtual MTAPIRES  ReportReserved1(void)=0;
   virtual MTAPIRES  ReportReserved2(void)=0;
   virtual MTAPIRES  ReportReserved3(void)=0;
   virtual MTAPIRES  ReportReserved4(void)=0;
   //--- common configuration 
   virtual IMTConCommon* CommonCreate(void)=0;
   virtual MTAPIRES  CommonGet(IMTConCommon* common)=0;
   virtual MTAPIRES  CommonReserved1(void)=0;
   virtual MTAPIRES  CommonReserved2(void)=0;
   virtual MTAPIRES  CommonReserved3(void)=0;
   virtual MTAPIRES  CommonReserved4(void)=0;
   //--- network configuration
   virtual IMTConServer* NetServerCreate(void)=0;
   virtual IMTConServerRange* NetServerRangeCreate(void)=0;
   virtual UINT      NetServerTotal(void)=0;
   virtual MTAPIRES  NetServerNext(const UINT pos,IMTConServer *server)=0;
   virtual MTAPIRES  NetServerGet(const UINT64 id,IMTConServer *server)=0;
   virtual MTAPIRES  NetServerReserved1(void)=0;
   virtual MTAPIRES  NetServerReserved2(void)=0;
   virtual MTAPIRES  NetServerReserved3(void)=0;
   virtual MTAPIRES  NetServerReserved4(void)=0;
   //--- time configuration
   virtual IMTConTime* TimeCreate(void)=0;
   virtual INT64     TimeCurrent(void)=0;
   virtual INT64     TimeGeneration(void)=0;
   virtual MTAPIRES  TimeGet(IMTConTime *time)=0;
   virtual MTAPIRES  TimeReserved1(void)=0;
   virtual MTAPIRES  TimeReserved2(void)=0;
   virtual MTAPIRES  TimeReserved3(void)=0;
   virtual MTAPIRES  TimeReserved4(void)=0;
   //--- holiday configuration
   virtual IMTConHoliday* HolidayCreate()=0;
   virtual UINT      HolidayTotal(void)=0;
   virtual MTAPIRES  HolidayNext(const UINT pos,IMTConHoliday *holiday)=0;
   virtual MTAPIRES  HolidayReserved1(void)=0;
   virtual MTAPIRES  HolidayReserved2(void)=0;
   virtual MTAPIRES  HolidayReserved3(void)=0;
   virtual MTAPIRES  HolidayReserved4(void)=0;
   //--- clients group configuration
   virtual IMTConGroup* GroupCreate(void)=0;
   virtual IMTConGroupSymbol* GroupSymbolCreate(void)=0;
   virtual IMTConCommission* GroupCommissionCreate(void)=0;
   virtual IMTConCommTier* GroupTierCreate(void)=0;
   virtual UINT      GroupTotal(void)=0;
   virtual MTAPIRES  GroupNext(const UINT pos,IMTConGroup *group)=0;
   virtual MTAPIRES  GroupGet(LPCWSTR name,IMTConGroup *group)=0;
   virtual MTAPIRES  GroupGetLight(LPCWSTR name,IMTConGroup *group)=0;
   virtual MTAPIRES  GroupReserved1(void)=0;
   virtual MTAPIRES  GroupReserved2(void)=0;
   virtual MTAPIRES  GroupReserved3(void)=0;
   virtual MTAPIRES  GroupReserved4(void)=0;
   //--- symbols configuration
   virtual IMTConSymbol* SymbolCreate(void)=0;
   virtual IMTConSymbolSession* SymbolSessionCreate(void)=0;
   virtual UINT      SymbolTotal(void)=0;
   virtual MTAPIRES  SymbolNext(const UINT pos,IMTConSymbol *symbol)=0;
   virtual MTAPIRES  SymbolGet(LPCWSTR name,IMTConSymbol *symbol)=0;
   virtual MTAPIRES  SymbolGetLight(LPCWSTR name,IMTConSymbol *symbol)=0;
   virtual MTAPIRES  SymbolGet(LPCWSTR name,const IMTConGroup *group,IMTConSymbol *symbol)=0;
   virtual MTAPIRES  SymbolExist(const IMTConSymbol* symbol,const IMTConGroup* group)=0;
   virtual MTAPIRES  SymbolReserved1(void)=0;
   virtual MTAPIRES  SymbolReserved2(void)=0;
   virtual MTAPIRES  SymbolReserved3(void)=0;
   virtual MTAPIRES  SymbolReserved4(void)=0;
   //--- managers configuration
   virtual IMTConManager* ManagerCreate(void)=0;
   virtual IMTConManagerAccess* ManagerAccessCreate(void)=0;
   virtual MTAPIRES  ManagerCurrent(IMTConManager *manager)=0;
   virtual UINT      ManagerTotal(void)=0;
   virtual MTAPIRES  ManagerNext(const UINT pos,IMTConManager *manager)=0;
   virtual MTAPIRES  ManagerGet(const UINT64 login,IMTConManager *manager)=0;
   virtual MTAPIRES  ManagerReserved1(void)=0;
   virtual MTAPIRES  ManagerReserved2(void)=0;
   virtual MTAPIRES  ManagerReserved3(void)=0;
   virtual MTAPIRES  ManagerReserved4(void)=0;
   //--- clients database
   virtual IMTUser*  UserCreate(void)=0;
   virtual IMTAccount* UserCreateAccount(void)=0;
   virtual MTAPIRES  UserGet(const UINT64 login,IMTUser *user)=0;
   virtual MTAPIRES  UserGetLight(const UINT64 login,IMTUser *user)=0;
   virtual MTAPIRES  UserLogins(LPCWSTR group,UINT64*& logins,UINT &total)=0;
   virtual MTAPIRES  UserAccountGet(const UINT64 login,IMTAccount *account)=0;
   virtual MTAPIRES  UserReserved1(void)=0;
   virtual MTAPIRES  UserReserved2(void)=0;
   virtual MTAPIRES  UserReserved3(void)=0;
   virtual MTAPIRES  UserReserved4(void)=0;
   //--- trade orders database
   virtual IMTOrder* OrderCreate(void)=0;
   virtual IMTOrderArray* OrderCreateArray(void)=0;
   virtual MTAPIRES  OrderGet(const UINT64 ticket,IMTOrder *order)=0;
   virtual MTAPIRES  OrderGet(const UINT64 login,IMTOrderArray *orders)=0;
   virtual MTAPIRES  OrderReserved1(void)=0;
   virtual MTAPIRES  OrderReserved2(void)=0;
   virtual MTAPIRES  OrderReserved3(void)=0;
   virtual MTAPIRES  OrderReserved4(void)=0;
   //--- trade history orders databases
   virtual MTAPIRES  HistoryGet(const UINT64 ticket,IMTOrder *order)=0;
   virtual MTAPIRES  HistoryGet(const UINT64 login,const INT64 from,const INT64 to,IMTOrderArray *orders)=0;
   virtual MTAPIRES  HistoryReserved1(void)=0;
   virtual MTAPIRES  HistoryReserved2(void)=0;
   virtual MTAPIRES  HistoryReserved3(void)=0;
   virtual MTAPIRES  HistoryReserved4(void)=0;
   //--- trade deals database
   virtual IMTDeal  *DealCreate(void)=0;
   virtual IMTDealArray *DealCreateArray(void)=0;
   virtual MTAPIRES  DealGet(const UINT64 ticket,IMTDeal *deal)=0;
   virtual MTAPIRES  DealGet(const UINT64 login,const INT64 from,const INT64 to,IMTDealArray *deals)=0;
   virtual MTAPIRES  DealReserved1(void)=0;
   virtual MTAPIRES  DealReserved2(void)=0;
   virtual MTAPIRES  DealReserved3(void)=0;
   virtual MTAPIRES  DealReserved4(void)=0;
   //--- trade positions database
   virtual IMTPosition* PositionCreate(void)=0;
   virtual IMTPositionArray* PositionCreateArray(void)=0;
   virtual MTAPIRES  PositionGet(const UINT64 login,LPCWSTR symbol,IMTPosition *position)=0;
   virtual MTAPIRES  PositionGet(const UINT64 login,IMTPositionArray *positions)=0;
   virtual MTAPIRES  PositionGetByTicket(const UINT64 ticket,IMTPosition *position)=0;
   virtual MTAPIRES  PositionReserved1(void)=0;
   virtual MTAPIRES  PositionReserved2(void)=0;
   virtual MTAPIRES  PositionReserved3(void)=0;
   //--- trade daily database
   virtual IMTDaily* DailyCreate(void)=0;
   virtual IMTDailyArray* DailyCreateArray(void)=0;
   virtual MTAPIRES  DailyGet(const UINT64 login,const INT64 datetime,IMTDaily *daily)=0;
   virtual MTAPIRES  DailyGetLight(const UINT64 login,const INT64 datetime,IMTDaily *daily)=0;
   virtual MTAPIRES  DailyGet(const UINT64 login,const INT64 from,const INT64 to,IMTDailyArray *daily)=0;
   virtual MTAPIRES  DailyGetLight(const UINT64 login,const INT64 from,const INT64 to,IMTDailyArray *daily)=0;
   virtual MTAPIRES  DailyReserved1(void)=0;
   virtual MTAPIRES  DailyReserved2(void)=0;
   virtual MTAPIRES  DailyReserved3(void)=0;
   virtual MTAPIRES  DailyReserved4(void)=0;
   //--- trade methods
   virtual MTAPIRES  TradeProfit(LPCWSTR group,LPCWSTR symbol,const UINT type,const UINT64 volume,const double price_open,const double price_close,double& profit,double& profit_rate)=0;
   virtual MTAPIRES  TradeRateBuy(LPCWSTR base,LPCWSTR currency,double& rate,LPCWSTR group=NULL,LPCWSTR symbol=NULL,const double price=0)=0;
   virtual MTAPIRES  TradeRateSell(LPCWSTR base,LPCWSTR currency,double& rate,LPCWSTR group=NULL,LPCWSTR symbol=NULL,const double price=0)=0;
   virtual MTAPIRES  TradeReserved1(void)=0;
   virtual MTAPIRES  TradeReserved2(void)=0;
   virtual MTAPIRES  TradeReserved3(void)=0;
   virtual MTAPIRES  TradeReserved4(void)=0;
   //--- plugins configuration
   virtual IMTConPlugin* PluginCreate(void)=0;
   virtual IMTConPluginModule* PluginModuleCreate(void)=0;
   virtual IMTConParam* PluginParamCreate(void)=0;
   virtual UINT      PluginTotal(void)=0;
   virtual MTAPIRES  PluginNext(const UINT pos,IMTConPlugin *plugin)=0;
   virtual MTAPIRES  PluginGet(const UINT64 server,LPCWSTR name,IMTConPlugin *plugin)=0;
   virtual UINT      PluginModuleTotal(void)=0;
   virtual MTAPIRES  PluginModuleNext(const UINT pos,IMTConPluginModule *module)=0;
   virtual MTAPIRES  PluginModuleGet(const UINT64 server,LPCWSTR name,IMTConPluginModule *module)=0;
   virtual MTAPIRES  PluginReserved1(void)=0;
   virtual MTAPIRES  PluginReserved2(void)=0;
   virtual MTAPIRES  PluginReserved3(void)=0;
   virtual MTAPIRES  PluginReserved4(void)=0;
   //--- datafeeds configuration
   virtual IMTConFeeder* FeederCreate(void)=0;
   virtual IMTConFeederModule* FeederModuleCreate(void)=0;
   virtual IMTConParam* FeederParamCreate(void)=0;
   virtual IMTConFeederTranslate* FeederTranslateCreate(void)=0;
   virtual UINT      FeederTotal(void)=0;
   virtual MTAPIRES  FeederNext(const UINT pos,IMTConFeeder *feeder)=0;
   virtual MTAPIRES  FeederGet(LPCWSTR name,IMTConFeeder *feeder)=0;
   virtual UINT      FeederModuleTotal(void)=0;
   virtual MTAPIRES  FeederModuleNext(const UINT pos,IMTConFeederModule *module)=0;
   virtual MTAPIRES  FeederModuleGet(LPCWSTR name,IMTConFeederModule *module)=0;
   virtual MTAPIRES  FeederReserved1(void)=0;
   virtual MTAPIRES  FeederReserved2(void)=0;
   virtual MTAPIRES  FeederReserved3(void)=0;
   virtual MTAPIRES  FeederReserved4(void)=0;
   //--- gateways configuration
   virtual IMTConGateway* GatewayCreate(void)=0;
   virtual IMTConGatewayModule* GatewayModuleCreate(void)=0;
   virtual IMTConParam* GatewayParamCreate(void)=0;
   virtual IMTConGatewayTranslate* GatewayTranslateCreate(void)=0;
   virtual UINT      GatewayTotal(void)=0;
   virtual MTAPIRES  GatewayNext(const UINT pos,IMTConGateway *gateway)=0;
   virtual MTAPIRES  GatewayGet(LPCWSTR name,IMTConGateway *gateway)=0;
   virtual UINT      GatewayModuleTotal(void)=0;
   virtual MTAPIRES  GatewayModuleNext(const UINT pos,IMTConGatewayModule *module)=0;
   virtual MTAPIRES  GatewayModuleGet(LPCWSTR name,IMTConGatewayModule *module)=0;
   virtual MTAPIRES  GatewayReserved1(void)=0;
   virtual MTAPIRES  GatewayReserved2(void)=0;
   virtual MTAPIRES  GatewayReserved3(void)=0;
   virtual MTAPIRES  GatewayReserved4(void)=0;
   //--- ticks
   virtual MTAPIRES  TickHistoryGet(LPCWSTR symbol,const INT64 from,const INT64 to,MTTickShort*& ticks,UINT& ticks_total)=0;
   virtual MTAPIRES  TickHistoryGetRaw(LPCWSTR symbol,const INT64 from,const INT64 to,MTTickShort*& ticks,UINT& ticks_total)=0;
   virtual MTAPIRES  TickReserved1(void)=0;
   virtual MTAPIRES  TickReserved2(void)=0;
   virtual MTAPIRES  TickReserved3(void)=0;
   virtual MTAPIRES  TickReserved4(void)=0;
   //--- chart methods
   virtual MTAPIRES  ChartHistoryGet(LPCWSTR symbol,const INT64 from,const INT64 to,MTChartBar*& bars,UINT& bars_total)=0;
   virtual MTAPIRES  ChartHistoryReserved1(void)=0;
   virtual MTAPIRES  ChartHistoryReserved2(void)=0;
   virtual MTAPIRES  ChartHistoryReserved3(void)=0;
   virtual MTAPIRES  ChartHistoryReserved4(void)=0;
  };
//+------------------------------------------------------------------+
//| Report context interface                                         |
//+------------------------------------------------------------------+
class IMTReportContext
  {
public:
   virtual void      Release(void)=0;
   //--- report generation method
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api)=0;
  };
//+------------------------------------------------------------------+
//| Report DLL entry points                                          |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES  MTReportAbout(const UINT index,MTReportInfo& info);
MTAPIENTRY MTAPIRES  MTReportCreate(const UINT index,const UINT apiversion,IMTReportContext **context);
//+------------------------------------------------------------------+
