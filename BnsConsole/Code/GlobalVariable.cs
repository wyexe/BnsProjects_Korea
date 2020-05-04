using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BnsConsole.Code
{
    public class CGlobalVariable
    {
        public static List<CCard> g_WorkCardList = new List<CCard>();
        public static List<CAccount> g_AccountList = new List<CAccount>();
        public static bool g_bStartGame = false;
    }
}
