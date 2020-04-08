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
        }

        private void SvMgrAPI_StartProject()
        {
            SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "DLL démarrée");
        }

        public void Dispose()
        {
            
        }
    }
}
