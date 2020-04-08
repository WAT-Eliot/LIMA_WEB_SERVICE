using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace usrmgrDotNetProject
{
    public class Main:IDisposable 
    {
        public Main()
        {
            SvMgrAPI.StartProject += SvMgrAPI_StartProject;
            SvMgrAPI.OnDataChange2 += SvMgrAPI_OnDataChange2;
        }

        private void SvMgrAPI_StartProject()
        {
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "DLL démarrée");

            TypeBool Test = new TypeBool("Mx.Test", 0, false);


            if (SvMgrAPI.VarAdvise("Mx.Test1", 0))
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable déclarée");
            }
            else
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Variable non déclarée");
            }
        }

        private void SvMgrAPI_OnDataChange2(SvMgrObjects.VariableResult[] ArrayVarResult)
        {
            throw new NotImplementedException();
        }

        public void Dispose()
        {
            
        }
    }
}
