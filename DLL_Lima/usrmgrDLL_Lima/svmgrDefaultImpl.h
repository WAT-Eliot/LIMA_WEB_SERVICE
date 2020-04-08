////////////////////////////////////////////////////////////////////////////////
// Default implementation of the ISVMgr interface.

DWORD ISVMgr::GetApiVersion ()
{
    return SVMGR_API_VERSION;
}

void ISVMgr::InitInstance ()
{
}

void ISVMgr::ExitInstance ()
{
}

void ISVMgr::StaticInit ()
{
}

void ISVMgr::StaticEnd ()
{
}

void ISVMgr::StartProject ()
{
}

void ISVMgr::StopProject ()
{
}

void ISVMgr::DelProject ()
{
}

void ISVMgr::OnReadCompleted (char * pBuffer)
{
}

void ISVMgr::OnWriteCompleted (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnNotify (ULONG ulClientHandle)
{
}

void ISVMgr::OnTimerElapsed (ULONG ulClientHandle)
{
}

void ISVMgr::OnAckAlarmCompleted (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnMaskVarCompleted (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnUnMaskVarCompleted (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnSetExtendedStringAttribute (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnSetExtendedBinaryAttribute (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnSetVariableAttribute (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnSetAlarmAttribute (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnSetDataSetCompleted (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnSetSimulatedVariablesCompleted (BOOL bResult, ULONG ulClientHandle)
{
}

void ISVMgr::OnVariableConfigurationChange (blkModificationType mdfType, LPCTSTR szVariableName)
{
}

void ISVMgr::OnDataChange (char * pBuffer)
{
}

void ISVMgr::OnExtentedAttributesChange (ULONG ulClientHandle,
                                         WORD wNbExtStringAttributes,
                                         _svmgrExtStringAttributeIds * peExtStringAttributeIds,
                                         LPSTR * pszExtStringAttributeValues,
                                         WORD wNbExtBinaryAttributes,
                                         _svmgrExtBinaryAttributeIds * peExtBinaryAttributeIds,
                                         _svmgrExtBinaryAttributeValue * puExtBinaryAttributeValues)
{
}

void ISVMgr::OnReadFrameCompleted (BOOL bResult,
                                   ULONG ulClientHandle,
                                   USHORT usErrorCode,
                                   FILETIME ftTimeStamp,
                                   DWORD dwSize,
                                   BYTE * pBuffer,
                                   ULONG ulFrameStatus,
                                   ULONG ulFrameComplementaryStatus)
{
}

void ISVMgr::OnWriteFrameCompleted (BOOL bResult,
                                    ULONG ulClientHandle,
                                    USHORT usErrorCode,
                                    ULONG ulFrameStatus,
                                    ULONG ulFrameComplementaryStatus)
{
}

void ISVMgr::OnModifyFrameAddressCompleted (BOOL bResult,
                                            ULONG ulClientHandle,
                                            USHORT usErrorCode)
{
}

void ISVMgr::OnModifyEqtAddressCompleted (BOOL bResult,
                                          ULONG ulClientHandle,
                                          USHORT usErrorCode) 
{
}

BOOL ISVMgr::OnDataChange2 (DWORD dwCount,
						    ULONG * pulClientHandle,
						    BOOL * pbErrors,
						    _svmgrVarValue2 ** pValues,
						    FILETIME * pftTimestamps,
						    _svmgrVarStatus * pStatus) 
{
    return FALSE;
}

BOOL ISVMgr::OnReadCompleted2 (DWORD dwCount,
                               ULONG * pulClientHandles,
                               BOOL * pbResults,
                               _svmgrVarValue2 ** pValues,
                               FILETIME * pftTimestamps,
                               _svmgrVarStatus * pStatus)
{
    return FALSE;
}

BOOL ISVMgr::OnReadGroupCompleted ( DWORD               dwCount,
                                    ULONG               ulClientHandle,
		                            LPTSTR            * pszVariables,
		                            BOOL              * pbResults,
		                            _svmgrVarValue2  ** pValues,
		                            FILETIME          * pftTimestamps,
		                            _svmgrVarStatus   * pStatus)
{
    return FALSE;
}

void ISVMgr::OnAddOPCGroupCompleted (BOOL bResult,
                                     ULONG ulClientHandle,
                                     ULONG ulGroupHandle,
                                     HRESULT hrErrorCode)
{
}

void ISVMgr::OnReadOPCItemCompleted (BOOL bResult,
                                     ULONG ulClientHandle,
                                     VARIANT value,
                                     FILETIME ftTimeStamp,
                                     USHORT usQuality,
                                     HRESULT hrErrorCode)
{
}

void ISVMgr::OnWriteOPCItemCompleted (BOOL bResult,
                                      ULONG ulClientHandle,
                                      HRESULT hrErrorCode)
{
}

void ISVMgr::OnRemoveOPCGroupCompleted (BOOL bResult,
                                        ULONG ulClientHandle,
                                        HRESULT hrErrorCode)
{
}

void ISVMgr::OnSubscribeOPCItemCompleted (BOOL bResult,
                                          ULONG ulClientHandle,
                                          HRESULT hrErrorCode)
{
}

void ISVMgr::OnUnsubscribeOPCItemCompleted (BOOL bResult,
                                            ULONG ulClientHandle,
                                            HRESULT hrErrorCode)
{
}

void ISVMgr::OnOPCItemChange (ULONG ulClientHandle,
                              VARIANT value,
                              FILETIME ftTimeStamp,
                              USHORT usQuality,
                              HRESULT hrErrorCode)
{
}

BOOL ISVMgr::OnSendRecipeCompleted (BOOL bResult, ULONG ulClientHandle)
{
    return FALSE;
}

BOOL ISVMgr::OnSetGroupQualityCompleted (BOOL bResult, ULONG ulClientHandle)
{
    return FALSE;
}
