using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace usrmgrDotNetProject
{
    public class CollectBusiness
    {
        public Variable.sNavetteCollecte navetteCollecte;

        public CollectBusiness()
        {
            navetteCollecte = new Variable.sNavetteCollecte();
        }

        public void Echange1(Variable.sTapingOutputConveyorNumber tocn)
        {
            navetteCollecte.TapingOutputConveyorNumber = tocn;
        }
    }
}
