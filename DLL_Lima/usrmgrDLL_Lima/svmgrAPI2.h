//***************************************************************************
//*
//*  SV32 - Copyright (c) ARC INFORMATIQUE, All Rights Reserved.
//*
//*--------------------------------------------------------------------------
//*
//*     File :  svmgrAPI2.h
//*
//*  Purpose :  This header file contains definitions of the interface
//*             methods that must be implemented by a SV manager and the API
//*             provided by SV32 to write a SV manager.
//*
//***************************************************************************



#ifndef _SVMGRAPI2_INCLUDED_  // Only include it once
#define _SVMGRAPI2_INCLUDED_



#ifndef __cplusplus
#error svmgrAPI2 requires C++ compilation
#endif



/////////////////////////////////////////////////////////////////////////////
//
//  Definitions and constants
//
/////////////////////////////////////////////////////////////////////////////

#ifndef INTERNAL_PCVUE_VERSION_90_000

#define INTERNAL_PCVUE_VERSION_90_000   	90000	// Correspond  la version 9.0	du 15/05/09
#define INTERNAL_PCVUE_VERSION_100_000 		100000	// Correspond  la version 10.0	
#define INTERNAL_PCVUE_VERSION_100_010      100010	// Correspond à la version 10.0 SP1	du 21/07/11

#endif

#define SVMGR_API_VERSION INTERNAL_PCVUE_VERSION_100_010


#define SVMGRAPI2        virtual BOOL
#define SVMGRAPI2_(type) virtual type

#pragma pack (push, 1)

/////////////////////////////////////////////////////////////////////////////
//
//  Type and interface definitions
//
/////////////////////////////////////////////////////////////////////////////


// Version information :

typedef enum
{
	cimwayquality_Bad,
	cimwayquality_Good

} CIMWAYQUALITY;

typedef struct
{
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;

} SVMGR_VERSION_INFO;

// String attributes information :

typedef enum
{
	ExtStringAttribute_Domain = 0,
	ExtStringAttribute_Nature = 1,
	ExtStringAttribute_3      = 2,
	ExtStringAttribute_4      = 3,
	ExtStringAttribute_5      = 4,
	ExtStringAttribute_6      = 5,
	ExtStringAttribute_7      = 6,
	ExtStringAttribute_8      = 7,
	ExtStringAttribute_9      = 8,
	ExtStringAttribute_10     = 9,
	ExtStringAttribute_11     = 10,
	ExtStringAttribute_12     = 11,
	ExtStringAttribute_13     = 12,
	ExtStringAttribute_14     = 13,
	ExtStringAttribute_15     = 14,
	ExtStringAttribute_16     = 15

} _svmgrExtStringAttributeIds;

typedef enum
{
	ExtBinaryAttribute_1      = 0

} _svmgrExtBinaryAttributeIds;

typedef union
{
	DWORD  dwRaw;
	struct
	{
		DWORD bit0: 1;
		DWORD bit1: 1;
		DWORD bit2: 1;
		DWORD bit3: 1;
		DWORD bit4: 1;
		DWORD bit5: 1;
		DWORD bit6: 1;
		DWORD bit7: 1;
		DWORD bit8: 1;
		DWORD bit9: 1;
		DWORD bit10: 1;
		DWORD bit11: 1;
		DWORD bit12: 1;
		DWORD bit13: 1;
		DWORD bit14: 1;
		DWORD bit15: 1;
		DWORD bit16: 1;
		DWORD bit17: 1;
		DWORD bit18: 1;
		DWORD bit19: 1;
		DWORD bit20: 1;
		DWORD bit21: 1;
		DWORD bit22: 1;
		DWORD bit23: 1;
		DWORD bit24: 1;
		DWORD bit25: 1;
		DWORD bit26: 1;
		DWORD bit27: 1;
		DWORD bit28: 1;
		DWORD bit29: 1;
		DWORD bit30: 1;
		DWORD bit31: 1;
	} bits;

} _svmgrExtBinaryAttributeValue;

#define SVMGR_NB_STRING_ATTRIBUTES  16 
#define SVMGR_STRING_ATTRIBUTES_LEN 257
#define SVMGR_NB_BINARY_ATTRIBUTES  1

/////////////////////////////////////////////////////////////////////////////

typedef enum
{
	blk_modUNK     = -1,  // Modification non-défini
	blk_modADD     = 0,   // Ajout d'un objet
	blk_modMDF     = 1,   // Modification d'un objet
	blk_modDEL     = 2    // Suppression d'un objet

} blkModificationType;

/////////////////////////////////////////////////////////////////////////////
//
//  Structure definitions
//
/////////////////////////////////////////////////////////////////////////////

typedef enum
{
	svmgr_vtNONE,
	svmgr_vtLOG,
	svmgr_vtALARM,
	svmgr_vtANA,
	svmgr_vtTXT

} _svmgrVarType;

typedef enum
{ 
	svmgr_lvlUSERPROG1,  // prog level 1
	svmgr_lvlUSERPROG2,  // prog level 2
	svmgr_lvlUSERPROG3,  // prog level 3
	svmgr_lvlUSERPROG4,  // prog level 4
	svmgr_lvlOPERATOR,   // operator
	svmgr_lvlINHIB,      // inhibited
	svmgr_lvlALARM       // alarm - obsolete mask

} _svmgrLevelMask;

typedef enum
{
	svmgr_dsCACHE,
	svmgr_dsDEVICE

} _svmgrDataSource;

typedef struct
{
	char          *szName;
	char          *szAttrib[ 14];
	_svmgrVarType vt;
	
} _svmgrVarEnum;

typedef struct
{
	int iStructSize;

	_svmgrExtBinaryAttributeValue BinaryAttributes[ SVMGR_NB_BINARY_ATTRIBUTES];
	char  StringAttributes[ SVMGR_NB_STRING_ATTRIBUTES][ SVMGR_STRING_ATTRIBUTES_LEN];

} _svmgrExtAttributes;

typedef struct
{
	int iStructSize;

	int iAlarmLevel;

} _svmgrAlarmAttributes;

typedef struct
{
	int    iStructSize;

	LPSTR  szFormat;
	LPSTR  szUnit;
	LPSTR  szDescription;
	double dMinimumValue;
	double dMaximumValue;
	double dControlMinimumValue;
	double dControlMaximumValue;
	double dThreshod1Value;
	double dThreshod2Value;
	double dThreshod3Value;
	double dThreshod4Value;

} _svmgrVariableAttributes;

typedef struct
{
	DWORD dwSize;
	DWORD dwVarCount;

} _svmgrStreamHeader;

typedef struct
{
	ULONG    ulClientHandle;
	DWORD    dwValueOffset;
	FILETIME ftTimeStamp;
	BYTE     bQuality;
	LONG     lQualityEx;
	BOOL     bResult;

} _svmgrStreamVarHeader;

typedef struct
{
	_svmgrVarType vt;
	union
	{
		BOOL   bLog;
		double dAna;
		char   cTxt;
		
		struct
		{
			BOOL bAlarmValue;
			BOOL bLogValue;
			BOOL bAck;

		} alarm;

	} val;

	BOOL     __stdcall bLog( void)    { return vt == svmgr_vtALARM? val.alarm.bLogValue: val.bLog; }
	double   __stdcall dAna( void)    { return val.dAna; }
	char   * __stdcall szTxt( void)   { return &(val.cTxt); }

	BOOL   __stdcall bAlarm( void)    { return val.alarm.bAlarmValue; }
	BOOL   __stdcall bAlarmLog( void) { return val.alarm.bLogValue; }
	BOOL   __stdcall bAlarmAck( void) { return val.alarm.bAck; }
	
} _svmgrVarValue;

typedef struct
{
	_svmgrVarType vt;
	union
	{
		BOOL     bLog;
		double   dAna;
		char   * szTxt;
		
		struct
		{
			BOOL bAlarmValue;
			BOOL bLogValue;
			BOOL bAck;

		} alarm;

	} val;

	BOOL     __stdcall bLog( void)    { return vt == svmgr_vtALARM? val.alarm.bLogValue: val.bLog; }
	double   __stdcall dAna( void)    { return val.dAna; }
	char   * __stdcall szTxt( void)   { return val.szTxt; }

	BOOL   __stdcall bAlarm( void)    { return val.alarm.bAlarmValue; }
	BOOL   __stdcall bAlarmLog( void) { return val.alarm.bLogValue; }
	BOOL   __stdcall bAlarmAck( void) { return val.alarm.bAck; }
	
} _svmgrVarValue2;

// Definition of TimestampKind property of _svmgrVarStatus union
//
#define SVMGR_TIMESTAMP_SV             0
#define SVMGR_TIMESTAMP_PLC_OK         1
#define SVMGR_TIMESTAMP_PLC_NOK        2
#define SVMGR_TIMESTAMP_HISTO_OK       3
#define SVMGR_TIMESTAMP_HISTO_NOK      4

typedef union __svmgrVarStatus
{
	LONG lRaw;
	struct
	{
		union
		{
			BYTE bRaw;
			struct
			{
				BYTE  NonSignifiant     : 1;
				BYTE  NonAccessible     : 1;
				BYTE  Inhibited         : 1;
				BYTE  LowLimitExceeded  : 1;
				BYTE  HighLimitExceeded : 1;
				BYTE  BadFormat         : 1;
				BYTE  Watchdog          : 1;
				BYTE  NotUsed           : 1;
			} bits;
		} State;

		union
		{
			BYTE bRaw;
			struct
			{
				BYTE  ProgLevel1 : 1;
				BYTE  ProgLevel2 : 1;
				BYTE  ProgLevel3 : 1;
				BYTE  ProgLevel4 : 1;
				BYTE  Operator   : 1;
				BYTE  Dependency : 1;
				BYTE  AlarmLevel : 1;
				BYTE  Expression : 1;
			} bits;
		} Mask;

		union
		{
			WORD wRaw;
			struct
			{
				WORD  NotUsed1      : 8;
				WORD  TimestampKind : 3;
				WORD  NotUsed2      : 5;
			} bits;
		} Misc;

	} Status;

	BOOL __stdcall IsValidState( void) { return Status.State.bRaw == 0? TRUE: FALSE; }
	BOOL __stdcall IsMasked( void)     { return Status.Mask.bRaw == 0? FALSE: TRUE; }
	BOOL __stdcall IsInhibited( void)  { return Status.State.bits.Inhibited == 0? FALSE: TRUE; }
	BOOL __stdcall IsTimeStampSV( void) { return Status.Misc.bits.TimestampKind	== SVMGR_TIMESTAMP_SV ? TRUE:  FALSE; }

	__svmgrVarStatus( long lQualityEx = 0): lRaw( lQualityEx) {}

} _svmgrVarStatus;

typedef enum
{
	svmgrVariableAttribute_Unknown               = -1,

	svmgrVariableAttribute_Description           = 0,
	svmgrVariableAttribute_Format                = 1,
	svmgrVariableAttribute_Unit                  = 2,
	svmgrVariableAttribute_MinimumValue          = 3,
	svmgrVariableAttribute_MaximumValue          = 4,
	svmgrVariableAttribute_ControlMinimumValue   = 5,
	svmgrVariableAttribute_ControlMaximumValue   = 6,
	svmgrVariableAttribute_Threshold1Value       = 7,
	svmgrVariableAttribute_Threshold2Value       = 8,
	svmgrVariableAttribute_Threshold3Value       = 9,
	svmgrVariableAttribute_Threshold4Value       = 10,

} _svmgrVariableAttributeIds;

typedef enum
{
	svmgrAlarmAttribute_Unknown					= -1,

	svmgrAlarmAttribute_AlarmLevel				= 0,

} _svmgrAlarmAttributeIds;


typedef enum
{
	svmgrQuality_Good = 0x00,
	svmgrQuality_Bad = 0x80,
	svmgrQuality_Bad_CommunicationFailure = 0x81,
	svmgrQuality_Bad_BadFormattingValue= 0x82,
	svmgrQuality_Bad_WatchdogFailure = 0x84

} _svmgrQuality;

typedef struct
{
	LPCSTR        szVariableName;
	_svmgrQuality qQuality;
	FILETIME      ftTimestamp;
	VARIANT       vValue;

} _svmgrDataSet;

typedef struct
{
	LPCSTR        szVariableName;
	BOOL          bEnableSimulated;
} _svmgrSimulatedVariable;

typedef struct
{
	int iStructSize;

	double  dDeadbandValue; // Pourcent of [min-max] range - Value: 0-100
	bool	bWithExtendedAttributes;
	bool	bAlarmAsLog;					// If true, Value return by OnDataChange is a log even if the variable is an alarm

} _svmgrAdviseOptions;

typedef enum
{
	svmgrProjectDirectory_Root, 
	svmgrProjectDirectory_Cfg,

} _svmgrProjectDirectory;


typedef struct
{
	LPCSTR        szVariableName;
	_svmgrQuality qQuality;
	FILETIME      ftTimestamp;
	VARIANT       vValue;

} _svmgrVarRecord;

typedef struct
{
	int iStructSize;

	// When only timestamp change (value and quality are the same ones), the blank manager call OnDataChange2 or 
	// OnDataChange methods only when this property is set to TRUE.
	BOOL	bNotifyOnTimestampChange; 

	// Do not call the method OnDataChange on a variable which was forced from the toolkit manager as long as the forced 
	// value did not change.
	BOOL	bDisableOnDataChangeAfterSetOrWrite; 

} _svmgrGlobalSettings;

typedef struct 
{
	DWORD dwPeriod;
} _svmgrAddOPCGroupParameters;

typedef enum
{
	svmgrRecipeType_Bloc,
	svmgrRecipeType_Multiple,
} _svmgrRecipeType;

typedef enum
{
	svmgrOpcRecipeExecuteMode_OptimizedSerialization = 0,
	svmgrOpcRecipeExecuteMode_FullSerialization = 1,
	svmgrOpcRecipeExecuteMode_NoOptimization = 2,
	svmgrOpcRecipeExecuteMode_FullOptimization = 3,

} _svmgrOpcRecipeExecuteMode;


///////////////////////////////////////////////////////////////////////////
// SVMgr interface :

struct ISVMgr
    {
    virtual DWORD __stdcall GetApiVersion ();

    virtual void __stdcall InitInstance ();
    virtual void __stdcall ExitInstance ();

    virtual void __stdcall StaticInit ();
    virtual void __stdcall StaticEnd ();

    virtual void __stdcall StartProject ();
    virtual void __stdcall StopProject ();
    virtual void __stdcall DelProject ();

	virtual void __stdcall OnDataChange ( char *Buffer);
	virtual void __stdcall OnReadCompleted ( char *Buffer);

	virtual void __stdcall OnWriteCompleted ( BOOL bResult, ULONG ulClientHandle);

	virtual void __stdcall OnNotify ( ULONG ulClientHandle);
	virtual void __stdcall OnTimerElapsed ( ULONG ulClientHandle);

    virtual void __stdcall OnAckAlarmCompleted( BOOL bResult, ULONG ulClientHandle);

    virtual void __stdcall OnMaskVarCompleted( BOOL bResult, ULONG ulClientHandle);

    virtual void __stdcall OnUnMaskVarCompleted( BOOL bResult, ULONG ulClientHandle);

	virtual void __stdcall OnExtentedAttributesChange(
			ULONG                           ulClientHandle,
			WORD                            wNbExtStringAttributes,
			_svmgrExtStringAttributeIds   * peExtStringAttributeIds,
			LPSTR                         * pszExtStringAttributeValues,
			WORD                            wNbExtBinaryAttributes,
			_svmgrExtBinaryAttributeIds   * peExtBinaryAttributeIds,
			_svmgrExtBinaryAttributeValue * puExtBinaryAttributeValues);

	virtual void __stdcall OnSetExtendedStringAttribute(
			BOOL                        bResult, 
			ULONG                       ulClientHandle);

	virtual void __stdcall OnSetExtendedBinaryAttribute(
			BOOL                        bResult, 
			ULONG                       ulClientHandle);

	virtual void __stdcall OnSetDataSetCompleted(
			BOOL      bResult,
			ULONG     ulClientHandle);

	virtual void __stdcall OnSetVariableAttribute(
			BOOL      bResult,
			ULONG     ulClientHandle);

	virtual void __stdcall OnSetSimulatedVariablesCompleted(
			BOOL      bResult,
			ULONG     ulClientHandle);

	// The blank manager signals that pending read command is completed.
	virtual void __stdcall OnReadFrameCompleted (
	    BOOL			bResult,
		ULONG 			ulClientHandle,
		USHORT			ErrorCode,
		FILETIME		ftTimeStamp,
		DWORD			dwSize,
		BYTE			*pbyBuffer,
		ULONG			ulFrameStatus,
		ULONG			ulFrameComplementaryStatus);

	// The blank manager signals that pending write command is completed.
	virtual void __stdcall OnWriteFrameCompleted (
		BOOL		bResult,
		ULONG 		ulClientHandle,
		USHORT		ErrorCode,
		ULONG		ulFrameStatus,
		ULONG 		ulFrameComplementaryStatus);

	// The blank manager signals that pending modification frame address command is completed.
	virtual void __stdcall OnModifyFrameAddressCompleted (
		BOOL		bResult,
		ULONG 	    ulClientHandle,
		USHORT		ErrorCode);

	// The blank manager signals that pending modification equipment address command is completed.
	virtual void __stdcall OnModifyEqtAddressCompleted (
		BOOL		bResult,
		ULONG 	    ulClientHandle,
		USHORT		ErrorCode);

	virtual void __stdcall OnVariableConfigurationChange(
		blkModificationType  mdfType,
		LPCTSTR              szVariableName);

	// The blank manager signals that pending modification alarm attribute is completed.
	virtual void __stdcall OnSetAlarmAttribute(
			BOOL      bResult,
			ULONG     ulClientHandle);

	virtual BOOL __stdcall OnDataChange2(
		DWORD               dwCount,
		ULONG             * pulClientHandles,
		BOOL              * pbResults,
		_svmgrVarValue2  ** pValues,
		FILETIME          * pftTimestamps,
		_svmgrVarStatus   * pStatus);

	virtual BOOL __stdcall OnReadCompleted2(
		DWORD               dwCount,
		ULONG             * pulClientHandles,
		BOOL              * pbResults,
		_svmgrVarValue2  ** pValues,
		FILETIME          * pftTimestamps,
		_svmgrVarStatus   * pStatus);

	virtual BOOL __stdcall OnReadGroupCompleted(
		DWORD               dwCount,
        ULONG               ulClientHandle,
		LPTSTR            * pszVariables,
		BOOL              * pbResults,
		_svmgrVarValue2  ** pValues,
		FILETIME          * pftTimestamps,
		_svmgrVarStatus   * pStatus);

	virtual void __stdcall OnAddOPCGroupCompleted(BOOL bResult, ULONG ulClientHandle, ULONG ulGroupServerHandle, HRESULT hrErrorCode);
	virtual void __stdcall OnReadOPCItemCompleted(BOOL bResult, ULONG ulClientHandle, VARIANT value, FILETIME ftTimeStamp, USHORT usQuality, HRESULT hrErrorCode);
	virtual void __stdcall OnWriteOPCItemCompleted(BOOL bResult, ULONG ulClientHandle, HRESULT hrErrorCode);
	virtual void __stdcall OnRemoveOPCGroupCompleted(BOOL bResult, ULONG ulClientHandle, HRESULT hrErrorCode);
	virtual void __stdcall OnSubscribeOPCItemCompleted(BOOL bResult, ULONG ulClientHandle, HRESULT hrErrorCode);
	virtual void __stdcall OnUnsubscribeOPCItemCompleted(BOOL bResult, ULONG ulClientHandle, HRESULT hrErrorCode);
	virtual void __stdcall OnOPCItemChange(ULONG ulClientHandle, VARIANT value, FILETIME ftTimeStamp, USHORT usQuality, HRESULT hrErrorCode);

	virtual BOOL __stdcall OnSendRecipeCompleted(BOOL bResult, ULONG ulClientHandle);
	virtual BOOL __stdcall OnSetGroupQualityCompleted(BOOL bResult, ULONG ulClientHandle);
};

/////////////////////////////////////////////////////////////////////////////
//
//  SV Manager API
//
/////////////////////////////////////////////////////////////////////////////

struct IAPIMgr
{
LPVOID pvSvReserved;

SVMGRAPI2_(DWORD) __stdcall GetLastError ();

SVMGRAPI2 __stdcall GetVersion (
    SVMGR_VERSION_INFO * pVersionInfo);


// ==========================
// Check variables attributes
// ==========================
// VarIsExist - Check if a variable exist or not
SVMGRAPI2 __stdcall VarIsExist(LPCSTR pszVarName          // Variable name to check
						 );
// VarGetType - Get the type of a variable
SVMGRAPI2 __stdcall VarGetType(LPCSTR        pszVarName,  // Variable name to get type
						 _svmgrVarType &vt          // Type of the requested variable
						 );

// =====================
// Enumeration functions
// =====================
// CreateVarEnum - Create a variable enumeration
SVMGRAPI2 __stdcall CreateVarEnum(HANDLE &hEnum
							);
// CloseVarEnum - Close a variable enumeration
SVMGRAPI2 __stdcall CloseVarEnum(HANDLE hEnum
						   );
// VarEnum - Get a variable enumeration
SVMGRAPI2 __stdcall VarEnum(HANDLE        hEnum,
					  ULONG         ulNbRequestedVar,
					  _svmgrVarEnum *peVar,
					  ULONG         *pulNbVar
					  );
// ClearVarEnum - Clear a variable enumeration
SVMGRAPI2 __stdcall ClearVarEnum(ULONG         ulNbVar,
						   _svmgrVarEnum *peVar
						   );

// ==========================
// Advise variables functions
// ==========================
// VarAdvise - Begin an advise on the value, timestamp and quality of a variable
SVMGRAPI2 __stdcall VarAdvise(LPCSTR pszVarName,
						ULONG  ulClientHandle
						);
// VarUnadvise - Stop an advise on the value, timestamp and quality of a variable
SVMGRAPI2 __stdcall VarUnadvise(LPCSTR pszVarName,
						  ULONG  ulClientHandle
						  );

// ========================
// Read variables functions
// ========================
// VarRead - Get the value, timestamp and quality of a variable
SVMGRAPI2 __stdcall VarRead(LPCSTR           pszVarName,
					  ULONG            ulClientHandle,
					  _svmgrDataSource dsSource = svmgr_dsCACHE
					  );

// =========================
// Write variables functions
// =========================
// LogVarWrite - Write a logical value in a logical or alarm variable
SVMGRAPI2 __stdcall LogVarWrite(LPCSTR  pszVarName,
						  BOOL    bValue,
						  ULONG   ulClientHandle
						  );
// AnaVarWrite - Write an analogic value in an analogic variable
SVMGRAPI2 __stdcall AnaVarWrite(LPCSTR  pszVarName,
						  double  dValue,
						  ULONG   ulClientHandle
						  );
// TxtVarWrite - Write a text value in a text variable
SVMGRAPI2 __stdcall TxtVarWrite(LPCSTR  pszVarName,
						  LPCSTR  pszValue,
						  ULONG   ulClientHandle
						  );


// ================================
// Acquitment alarm variables functions
// ================================
SVMGRAPI2 __stdcall AckAlarm(		LPCSTR a_pszVarName,
							LPCSTR a_pszOperator,
							ULONG  a_ulClientHandle);

// ================================
// Mask variables functions
// ================================
SVMGRAPI2 __stdcall MaskVar(		_svmgrLevelMask a_MaskLevel,
							LPCSTR			a_pszVarName,
							LPCSTR			a_pszOperator,
							ULONG			a_ulClientHandle);

// ================================
// Unmask variables functions
// ================================
SVMGRAPI2 __stdcall UnMaskVar(	_svmgrLevelMask a_MaskLevel,
							LPCSTR			a_pszVarName,
							LPCSTR			a_pszOperator,
							ULONG			a_ulClientHandle);

// ================================
// Get current user functions
// ================================
SVMGRAPI2 __stdcall GetCurrentUser( LPSTR  a_pszUserName,
							  int    a_iSize);

// ================================
// Timer and notification functions
// ================================
// SetTimer - Start a timer and notify the User DLL when elapsed
SVMGRAPI2 __stdcall SetTimer(ULONG ulMsDelay,
					   ULONG ulClientHandle
					   );
// Notify - Notify the User DLL
SVMGRAPI2 __stdcall Notify(ULONG ulClientHandle
					 );

// =====================
// Log message functions
// =====================
// LogMessage - Log a formatted message in the event viewer
SVMGRAPI2 __stdcall LogMessage(USHORT usLevel,    // Message level
						 USHORT usDest,     // Message destination
						 LPCSTR pszFormat,  // Format control
						 ...                // Optional arguments
						 );

// Trace - Log a formatted message in the event viewer if
// the trace flag is active
SVMGRAPI2 __stdcall Trace(ULONG ulTraceFlag, // Trace flag
					LPCSTR pszFormat,  // Format control
					...                // Optional arguments
					);

// =============================
// Extended attributes functions
// =============================
// InitExtendedAttributesStructure - Initialize the structure used by
// the GetExtendedAttributes function
SVMGRAPI2 __stdcall InitExtendedAttributesStructure(int                   iStructSize,
										  _svmgrExtAttributes & sExtendedAttributes);
// FreeExtendedAttributesStructure - Free the structure used by
// the GetExtendedAttributes function
SVMGRAPI2 __stdcall FreeExtendedAttributesStructure(_svmgrExtAttributes & sExtendedAttributes);
// GetExtendedAttributes - Get the extended attributes for a variable
SVMGRAPI2 __stdcall GetExtendedAttributes(LPCSTR                szVarName,
								_svmgrExtAttributes & sExtendedAttributes);
// SetExtendedStringAttribute - Set a string attribute of a variable
SVMGRAPI2 __stdcall SetExtendedStringAttribute(LPCSTR                      szVarName,
									 _svmgrExtStringAttributeIds eExtAttributeId,
									 LPCSTR                      szExtAttributeValue,
									 ULONG                       ulClientHandle);
// SetExtendedBinaryAttribute - Set a binary attribute of a variable
SVMGRAPI2 __stdcall SetExtendedBinaryAttribute(LPCTSTR                       szName,
											   _svmgrExtBinaryAttributeIds   eExtAttributeId,
											   _svmgrExtBinaryAttributeValue uExtAttributeValue,
											   ULONG                         ulClientHandle);

// ==========================
// Alarm attributes functions
// ==========================
// InitAlarmAttributesStructure - Initialize the structure used by the GetAlarmAttributes function
SVMGRAPI2 __stdcall InitAlarmAttributesStructure(
										int						iStructSize,
									   _svmgrAlarmAttributes &	sAlarmAttributes);

// FreeAlarmAttributesStructure - Free the structure used by the GetAlarmAttributes function
SVMGRAPI2 __stdcall FreeAlarmAttributesStructure(
										_svmgrAlarmAttributes & sAlarmAttributes);

// GetAlarmAttributes - Get the alarm attributes for a alarm
SVMGRAPI2 __stdcall GetAlarmAttributes(
										LPCSTR                  szAlarmName,
										_svmgrAlarmAttributes &	sAlarmAttributes);

// SetAlamrAttribute - Set a alarm attribute for a alarm
SVMGRAPI2 __stdcall SetAlarmAttribute(
								LPCSTR						szAlarmName,
							   _svmgrAlarmAttributeIds		eAttributeId,
							   int							iAttributeValue,
							   ULONG						ulClientHandle);

// =============================
// Variable attributes functions
// =============================
// InitVariableAttributesStructure - Initialize the structure used by
// the GetVariableAttributes function
SVMGRAPI2 __stdcall InitVariableAttributesStructure(int                        iStructSize,
										  _svmgrVariableAttributes & sVariableAttributes);
// FreeVariableAttributesStructure - Free the structure used by
// the GetVariableAttributes function
SVMGRAPI2 __stdcall FreeVariableAttributesStructure(_svmgrVariableAttributes & sVariableAttributes);
// GetVariableAttributes - Get the variable attributes for a variable
SVMGRAPI2 __stdcall GetVariableAttributes(LPCSTR                     szVarName,
								_svmgrVariableAttributes & sVariableAttributes);
// SetVariableAttribute - Set a variable attribute for a variable
SVMGRAPI2 __stdcall SetVariableAttribute(LPCSTR                     szVarName,
							   _svmgrVariableAttributeIds eAttributeId,
							   LPCSTR                     szAttributeValue,
							   ULONG                      ulClientHandle);
SVMGRAPI2 __stdcall SetVariableAttribute(LPCSTR                     szVarName,
							   _svmgrVariableAttributeIds eAttributeId,
							   double                     dAttributeValue,
							   ULONG                      ulClientHandle);

//---------------------------------------------------------------------------------------------------------

// SetDataSeries - Set Value, Timestamp and quality for a set of variables
SVMGRAPI2 __stdcall SetDataSet(ULONG           ulSeriesSize,
					 _svmgrDataSet * pDataSets,
					 ULONG           ulClientHandle);

// GetDataSeriesMaxSize - Returns the maximum size of the array use in SetDataSeries function
SVMGRAPI2_(ULONG)__stdcall  GetDataSetMaxSize();

SVMGRAPI2 __stdcall SetSimulatedVariables(ULONG           ulSeriesSize,
					 _svmgrSimulatedVariable * pSimulatedVariables,
					 ULONG           ulClientHandle);

SVMGRAPI2 __stdcall GetProjectDirectory(LPSTR szProjectDirectory);

// Read a frame named pszFullFrameName
SVMGRAPI2 __stdcall CimwayReadFrame(
						LPCSTR	a_pszFullFrameName, // Contains the full name of the frame.
						ULONG	a_ulClientHandle,	// Associated client handle  
						BOOL	a_bUseCache);		// Force the reading to the device when this value is set to FALSE. If this value is set to TRUE, the reading can use the last cyclic refresh.

// Write a frame named a_pszFullFrameName with the buffer a_pbyBuffer.
SVMGRAPI2 __stdcall CimwayWriteFrame(         
						LPCSTR	a_pszFullFrameName,	// Contains the full name of the frame.
						ULONG	a_ulClientHandle,	// Associated client handle 
						ULONG	a_ulSize,			// 
						BYTE *	a_pbyBuffer);		// 

// Modify the address of the frame 
SVMGRAPI2 __stdcall CimwayModifyFrameAddress(			
						LPCSTR	a_pszFullFrameName,		// Contains the full name of the frame.
						ULONG	a_ulClientHandle,		// Associated client handle 
						LPCSTR	a_pszADFrameAddress);	// New address of the frame

// Modify the address of the equipment 
SVMGRAPI2 __stdcall CimwayModifyEqtAddress(			
						LPCSTR	a_pszFullEqtName,		// Contains the full name of the equipment.
						ULONG	a_ulClientHandle,		// Associated client handle 
						LPCSTR	a_pszADEqtAddress);		// New address of the equipment

SVMGRAPI2 __stdcall AdviseConfiguration(void);

SVMGRAPI2 __stdcall CancelAdviseConfiguration(void);

// ==========================
// Advise variables functions
// ==========================
// InitVariableAttributesStructure - Initialize the structure used by
// the VarAdvise function
SVMGRAPI2 __stdcall InitAdviseOptionsStructure(int                   iStructSize,
											   _svmgrAdviseOptions & sAdviseOptions);
// FreeVariableAttributesStructure - Free the structure used by
// the VarAdvise function
SVMGRAPI2 __stdcall FreeAdviseOptionsStructure(_svmgrAdviseOptions & sAdviseOptions);

// VarAdvise - Begin an advise (with options) on the value, timestamp and quality of a variable
SVMGRAPI2 __stdcall VarAdvise(
						LPCSTR		          pszVarName,       // [in] Name of variable
						_svmgrAdviseOptions & sAdviseOptions,        // [in] Options of the advise
						ULONG		          ulClientHandle);  // [in] Client handle

SVMGRAPI2 __stdcall GetProjectDirectory(_svmgrProjectDirectory eDirectory, LPSTR szProjectDirectory, int nSize);

// VarRecord - Record change value of variables
SVMGRAPI2 __stdcall VarRecords(
						ULONG				ulSeriesSize,		// [in] Number of Record
						_svmgrVarRecord *	pVarRecords);		// [in] Ptr of array of record variables

// GeVarRecordsMaxSize - Returns the maximum size of the array use in VarRecord function
SVMGRAPI2_(ULONG)__stdcall  GetVarRecordsMaxSize();

SVMGRAPI2 __stdcall CreateRecipe(HANDLE	&hRecipe, _svmgrRecipeType recipeType, _svmgrOpcRecipeExecuteMode opcRecipeExecuteMode);
SVMGRAPI2 __stdcall AddVariableToRecipe(HANDLE hRecipe, LPCTSTR	szVariableName,	bool bValue);
SVMGRAPI2 __stdcall AddVariableToRecipe(HANDLE hRecipe, LPCTSTR	szVariableName,	double dValue);
SVMGRAPI2 __stdcall AddVariableToRecipe(HANDLE hRecipe, LPCTSTR	szVariableName, LPCTSTR	szValue);
SVMGRAPI2 __stdcall SendRecipe(HANDLE hRecipe, ULONG ulClientHandle);
SVMGRAPI2 __stdcall CloseRecipe(HANDLE hRecipe);


// ===============
// Global settings
// ===============
// InitGlobalSettings - Initialize the structure used by GlobalSettings functions
SVMGRAPI2 __stdcall InitGlobalSettings(
										int						iStructSize,
										_svmgrGlobalSettings &	sGlobalSettings);

// FreeGlobaalSettings - Free the structure used by the GlobalSettings functions
SVMGRAPI2 __stdcall FreeGlobalSettings(
										_svmgrGlobalSettings & sGlobalSettings);

// GetGlobalSettings - Get GlobalSettings
SVMGRAPI2 __stdcall GetGlobalSettings(
										_svmgrGlobalSettings & sGlobalSettings);

// SetGlobalSettings - Set GlobalSettings
SVMGRAPI2 __stdcall SetGlobalSettings(
										_svmgrGlobalSettings & sGlobalSettings);


SVMGRAPI2 __stdcall AddOPCGroup(LPCTSTR pszServerID, LPCTSTR pszGroupID, _svmgrAddOPCGroupParameters &sAddOPCGroupParameters, ULONG ulClientHandle);
SVMGRAPI2 __stdcall ReadOPCItem(ULONG ulGroupServerHandle, LPCTSTR pszItemID, ULONG ulClientHandle);
SVMGRAPI2 __stdcall WriteOPCItem(ULONG ulGroupServerHandle, LPCTSTR pszItemID, const VARIANT & value, ULONG ulClientHandle);
SVMGRAPI2 __stdcall RemoveOPCGroup(ULONG ulGroupServerHandle, ULONG ulClientHandle);
SVMGRAPI2 __stdcall SubscribeOPCItem(ULONG ulGroupServerHandle, LPCTSTR pszItemID, ULONG ulClientHandle);
SVMGRAPI2 __stdcall UnsubscribeOPCItem(ULONG ulGroupServerHandle, ULONG ulClientHandle);

SVMGRAPI2 __stdcall CreateVariablesGroup(HANDLE	&hVariablesGroup);
SVMGRAPI2 __stdcall AddVariableToGroup(HANDLE hVariablesGroup, LPCTSTR szVariableName);
SVMGRAPI2 __stdcall SetGroupQuality(HANDLE hVariablesGroup, _svmgrQuality quality, ULONG	ulClientHandle);
SVMGRAPI2 __stdcall ReadGroup(HANDLE hVariablesGroup, ULONG	ulClientHandle);
SVMGRAPI2 __stdcall CloseVariablesGroup(HANDLE hVariablesGroup);
};

// Values for the ulTraceFlag parameter of the svmgrTrace function
#define SVMGR_FLAG_BIT0        0x0001
#define SVMGR_FLAG_BIT1        0x0002
#define SVMGR_FLAG_BIT2        0x0004
#define SVMGR_FLAG_BIT3        0x0008
#define SVMGR_FLAG_BIT4        0x0010
#define SVMGR_FLAG_BIT5        0x0020
#define SVMGR_FLAG_BIT6        0x0040
#define SVMGR_FLAG_BIT7        0x0080
#define SVMGR_FLAG_BIT8        0x0100
#define SVMGR_FLAG_BIT9        0x0200
#define SVMGR_FLAG_BIT10       0x0400
#define SVMGR_FLAG_BIT11       0x0800
#define SVMGR_FLAG_BIT12       0x1000
#define SVMGR_FLAG_BIT13       0x2000
#define SVMGR_FLAG_BIT14       0x4000
#define SVMGR_FLAG_BIT15       0x8000
										//#MODIFDL 21/07/99
#define SVMGR_FLAG_BIT16       0x10000
#define SVMGR_FLAG_BIT17       0x20000
#define SVMGR_FLAG_BIT18       0x40000
#define SVMGR_FLAG_BIT19       0x80000
#define SVMGR_FLAG_BIT20       0x100000
#define SVMGR_FLAG_BIT21       0x200000
#define SVMGR_FLAG_BIT22       0x400000
#define SVMGR_FLAG_BIT23       0x800000
#define SVMGR_FLAG_BIT24       0x1000000
#define SVMGR_FLAG_BIT25       0x2000000
#define SVMGR_FLAG_BIT26       0x4000000
#define SVMGR_FLAG_BIT27       0x8000000
#define SVMGR_FLAG_BIT28       0x10000000
#define SVMGR_FLAG_BIT29       0x20000000
#define SVMGR_FLAG_BIT30       0x40000000
#define SVMGR_FLAG_BIT31       0x80000000

// Values for the usLevel parameter of the svmgrLogMessage function
#define SVMGR_LVL_INFO      1
#define SVMGR_LVL_WARNING   2
#define SVMGR_LVL_FATAL     3
// Values for the usDest parameter of the svmgrLogMessage function
#define SVMGR_DEST_FILET    1
#define SVMGR_DEST_VIEWER   2

#pragma pack (pop)

#endif  // _SVMGRAPI2_INCLUDED


