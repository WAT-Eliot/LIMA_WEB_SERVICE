using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SvMgrObjects;

namespace usrmgrDotNetProject
{
    class Variable
    {
        public static List<TypeBool> vVariableBool = new List<TypeBool>();
        public static List<TypeAna> vVariableAna = new List<TypeAna>();
        public static List<TypeString> vVariableString = new List<TypeString>();
        protected string m_name;
        protected uint m_adviseN;

        public Variable()
        {
        }

        public Variable(string name, uint adviseN)
        {
            m_name = name;
            m_adviseN = adviseN;
        }

        public string GetVarName()
        {
            return m_name;
        }

        public void Advise()
        {
            if (SvMgrAPI.VarAdvise(m_name, m_adviseN))
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable " + m_name + " (n° " + m_adviseN + ") abonnée.");
            }
            else
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable" + m_name + " (" + m_adviseN + ") non abonnée.");
            }
        }

        public void Unadvise()
        {
            SvMgrAPI.VarUnadvise(m_name, m_adviseN);
        }

        public uint GetAdviseNumber()
        {
            return m_adviseN;
        }

        public virtual void WriteVar()
        {

        }
    }

    class TypeBool : Variable
    {
        
        private bool m_value;

        public TypeBool()
        {
        }

        public TypeBool(string name, uint adviseN, bool value)
        {
            m_name = name;
            m_adviseN = adviseN;
            m_value = value;

            if(adviseN != 0)
            {
                this.Advise();

                //Ajout liste pour comptage
                Variable.vVariableBool.Add(this);
            }
            
        }

        public void SetVar(bool value)
        {
            m_value = value;
            
        }

        public bool GetVarValue()
        {
            return m_value;
        }

        public virtual void WriteVar(bool value)
        {
            SvMgrObjects.VarValue varValue = new SvMgrObjects.VarValue();
            varValue.vt = SvMgrEnums.VarType.svmgr_vtLOG;
            varValue.Log = value;

            if (varValue.Log != m_value)
            {
                SvMgrAPI.VarWrite(m_name, varValue, 0);
            }
        }
    }

    class TypeAna : Variable
    {
        private double m_value;

        public TypeAna()
        {
        }

        public TypeAna(string name, uint adviseN, double value)
        {
            m_name = name;
            m_adviseN = adviseN;
            m_value = value;

            if (adviseN != 0)
            {
                this.Advise();

                //Ajout liste pour comptage
                Variable.vVariableAna.Add(this);
            }
        }

        public void SetVar(double value)
        {
            m_value = value;

        }

        public double GetVarValue()
        {
            return m_value;
        }

        public virtual void WriteVar(double value)
        {
            SvMgrObjects.VarValue varValue = new SvMgrObjects.VarValue();
            varValue.vt = SvMgrEnums.VarType.svmgr_vtANA;
            varValue.Ana = value;


            if (varValue.Ana != m_value)
            {
                SvMgrAPI.VarWrite(m_name, varValue, 0);
            }
        }
    }

    class TypeString : Variable
    {

        private string m_value;

        public TypeString()
        {
        }

        public TypeString(string name, uint adviseN, string value)
        {
            m_name = name;
            m_adviseN = adviseN;
            m_value = value;

            if (adviseN != 0)
            {
                this.Advise();

                //Ajout liste pour comptage
                Variable.vVariableString.Add(this);
            }
        }

        public void SetVar(string value)
        {
            m_value = value;

        }

        public string GetVarValue()
        {
            return m_value;
        }

        public virtual void WriteVar(string value)
        {
            SvMgrObjects.VarValue varValue = new SvMgrObjects.VarValue();
            varValue.vt = SvMgrEnums.VarType.svmgr_vtTXT;
            varValue.Txt = value;

            if (varValue.Txt != m_value)
            {
                SvMgrAPI.VarWrite(m_name, varValue, 0);
            }
        }
    }
}
