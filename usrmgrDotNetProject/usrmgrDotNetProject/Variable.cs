using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace usrmgrDotNetProject
{
    class Variable
    {
    
        protected string m_name;
        protected uint m_adviseN;

        public void Main(string name, uint adviseN)
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
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable" + m_name + " (" + m_adviseN + ") abonnée.");
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

        public virtual void WriteVar()
        {

        }
    }

    class TypeBool : Variable
    {

        private SvMgrObjects.VarValue m_value;

        public void Main(string name, uint adviseN, SvMgrObjects.VarValue value)
        {
            m_name = name;
            m_adviseN = adviseN;
            m_value = value;

            if(adviseN != 0)
            {
                this.Advise();
                //Ajout vecteur pour comptage
            }
        }

        public void SetVar(SvMgrObjects.VarValue value)
        {
        m_value = value;
            
        }

        public SvMgrObjects.VarValue GetVarValue()
        {
            return m_value;
        }

        public virtual void WriteVar(SvMgrObjects.VarValue value)
        {
            if (value != m_value)
            {
                SvMgrAPI.VarWrite(m_name, value, 0); 
            }
        }
    }
}
