using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Web_Service
{
    public class Class1
    {
        public int toto()
        {
            return 1;
        }
        public Class2 getClass2()
        {
            return new Class2();
        }
    }
    public class Class2
    {
        public int titi = 2;
        public int tutu { get; set; } = 3;
    }

    
}
