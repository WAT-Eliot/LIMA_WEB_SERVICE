#pragma once
////////////////////////////////////////////////////////////////////////////////

class CAdvisedVar2 : public CObject
{
    DECLARE_DYNAMIC(CAdvisedVar)

    CString m_strAdviseChange;
    CString m_strVarToChange;

public:
	
    CAdvisedVar2 (LPCSTR pszAdviseChange, LPCSTR pszVarToChange)
    {
        // pszAdviseChange: Name of the variable to read
        // pszVarToChange:  Name of the variable to write with the value of the read variable

        m_strAdviseChange = pszAdviseChange;
        m_strVarToChange = pszVarToChange;

        // Inside the constructor we advise the variable
        // Is not necessary does it here, you could call this function in another function

        svmgrAPI->VarAdvise (pszAdviseChange, (ULONG)this);
    }
	
    CString NameVar ()
    {
        return m_strAdviseChange;
    }
	
    CString VarToChange ()
    {
        return m_strVarToChange;
    }
};

