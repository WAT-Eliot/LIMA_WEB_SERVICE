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

            /*----------------------------- Déclaration des variables ----------------------------------------*/
            
            // Variable bool
            TypeBool Test = new TypeBool("Mx.Test", 1, false);
            Variable.vVariableBool[0] = Test;
            TypeBool Test1 = new TypeBool("Mx.Test1", 2, false);
            Variable.vVariableBool[1] = Test1;

            //Variable Ana
            TypeAna Ana = new TypeAna("Mx.Ana", 3, 0);
            Variable.vVariableAna[0] = Ana;
            TypeAna Ana1 = new TypeAna("Mx.Ana1", 4, 0);
            Variable.vVariableAna[1] = Ana1;

            //Variable String
            TypeString Txt = new TypeString("Mx.Txt", 5, "");
            Variable.vVariableString[0] = Txt;
            TypeString Txt1 = new TypeString("Mx.Txt1", 6, "");
            Variable.vVariableString[1] = Txt1;

            /*----------------------------- Fin Déclaration des variables ----------------------------------------*/
        }

        private void SvMgrAPI_OnDataChange2(SvMgrObjects.VariableResult[] ArrayVarResult)
        {
            
            for (int i = 0; i < ArrayVarResult.Length; i++) {
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Variable modifiée : " + ArrayVarResult[i].clientHandle);
                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, "Type de la Variable modifiée : " + ArrayVarResult[i].varValue.vt);

                switch (ArrayVarResult[i].varValue.vt)
                {
                    case SvMgrEnums.VarType.svmgr_vtNONE:

                    case SvMgrEnums.VarType.svmgr_vtLOG:
                        for (int iTailleTab = 0; iTailleTab < Variable.vVariableBool.Length; iTailleTab++)
                        {
                            if((Variable.vVariableBool[iTailleTab].GetAdviseNumber() == ArrayVarResult[i].clientHandle))
                            {
                                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, Variable.vVariableBool[iTailleTab].GetVarName() + " = " + ArrayVarResult[i].varValue.Log);
                                Variable.vVariableBool[iTailleTab].SetVar(ArrayVarResult[i].varValue.Log);
                            }
                        }
                        break;
                    case SvMgrEnums.VarType.svmgr_vtANA:
                        for (int iTailleTab = 0; iTailleTab < Variable.vVariableAna.Length; iTailleTab++)
                        {
                            if ((Variable.vVariableAna[iTailleTab].GetAdviseNumber() == ArrayVarResult[i].clientHandle))
                            {
                                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, Variable.vVariableAna[iTailleTab].GetVarName() + " = " + ArrayVarResult[i].varValue.Ana);
                                Variable.vVariableAna[iTailleTab].SetVar(ArrayVarResult[i].varValue.Ana);
                            }
                        }
                        break;
                    case SvMgrEnums.VarType.svmgr_vtTXT:
                        for (int iTailleTab = 0; iTailleTab < Variable.vVariableString.Length; iTailleTab++)
                        {
                            if ((Variable.vVariableString[iTailleTab].GetAdviseNumber() == ArrayVarResult[i].clientHandle))
                            {
                                SvMgrAPI.LogMessage(SvMgrEnums.LogMessageLevel.Info, Variable.vVariableString[iTailleTab].GetVarName() + " = " + ArrayVarResult[i].varValue.Txt);
                                Variable.vVariableString[iTailleTab].SetVar(ArrayVarResult[i].varValue.Txt);
                            }
                        }
                        break;
                }
            }
        }

        public void Dispose()
        {
            
        }

    }
}
