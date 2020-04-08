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

            TypeBool Test = new TypeBool("Mx.Test", 1, false);
            TypeBool Test1 = new TypeBool("Mx.Test1", 2, false);
            TypeAna Ana = new TypeAna("Mx.Ana", 3, 0);
            TypeAna Ana1 = new TypeAna("Mx.Ana1", 4, 0);
            TypeString Txt = new TypeString("Mx.Txt", 5, "");
            TypeString Txt1 = new TypeString("Mx.Txt1", 6, "");

            /*
            if (SvMgrAPI.VarAdvise("Mx.Test1", 2))
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable déclarée");
            }
            else
            {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Warning, "Variable non déclarée");
            }
            */
        }

        private void SvMgrAPI_OnDataChange2(SvMgrObjects.VariableResult[] ArrayVarResult)
        {

            uint iVal = ArrayVarResult[0].clientHandle;
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable modifiée : " + iVal);

            //throw new NotImplementedException();
        }

        public void Dispose()
        {
            
        }
    }
}
