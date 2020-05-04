using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BnsConsole.Code
{
    public class SettingConfig
    {
        /// <summary>
        ///  清除7天前的日志
        /// </summary>
        public void ClearConfig()
        {
            ClearAccountLog();
            ClearPlayerDaily();
            ClearTaskUpgradeSchedule();
        }

        /// <summary>
        /// 从Text提取日期返回与当前日期相差的小时
        /// </summary>
        /// <param name="strText"></param>
        /// <returns></returns>
        private int GetAccountLogDateTime(string strText)
        {
            // DateTime Account,Pass:LogText
            var nIndex = strText.IndexOf(" ");
            if (nIndex == -1)
                return int.MaxValue;

            nIndex = strText.IndexOf(" ", nIndex + 1);
            if (nIndex == -1)
                return int.MaxValue;

            var strDate = strText.Substring(0, nIndex);
            DateTime dt;
            if (!DateTime.TryParse(strDate, out dt))
                return int.MaxValue;

            return (int)DateTime.Now.Subtract(dt).TotalHours;
        }

        /// <summary>
        /// 清除AccountLog.txt
        /// </summary>
        /// <returns></returns>
        private void ClearAccountLog()
        {
            string strAccountLogPath = System.Environment.CurrentDirectory + @"\Setting\AccountLog.txt";

            var lst = ConsoleFile.ReadFile_By_Path(strAccountLogPath);
            lst.RemoveAll(v => GetAccountLogDateTime(v) > 7 * 24);

            ConsoleFile.SaveFile_By_Path(strAccountLogPath, lst);
        }


        /// <summary>
        /// 从Text提取日期返回与当前日期相差的小时
        /// </summary>
        /// <param name="strText"></param>
        /// <returns></returns>
        private int GetPlayerDailyTime(string strText)
        {
            // iesitianac@hotmail.com,qiuqiud,1,163,2016-7-27 20:29:0
            var nIndex = strText.LastIndexOf(",");
            if (nIndex == -1)
                return int.MaxValue;

            var strDate = strText.Substring(nIndex + 1);
            DateTime dt;
            if (!DateTime.TryParse(strDate, out dt))
                return int.MaxValue;

            return (int)DateTime.Now.Subtract(dt).TotalHours;
        }

        /// <summary>
        /// 清除PlayerDaily.txt
        /// </summary>
        private void ClearPlayerDaily()
        {
            string strPlayerDailyPath = System.Environment.CurrentDirectory + @"\Setting\PlayerDaily.txt";

            var lst = ConsoleFile.ReadFile_By_Path(strPlayerDailyPath);
            lst.RemoveAll(v => GetPlayerDailyTime(v) > 7 * 24);

            ConsoleFile.SaveFile_By_Path(strPlayerDailyPath, lst);
        }

        /// <summary>
        /// 清除TaskUpgradeSchedule.txt
        /// </summary>
        private void ClearTaskUpgradeSchedule()
        {
            string strTaskUpgradeSchedulePath = System.Environment.CurrentDirectory + @"\Setting\TaskUpgradeSchedule.txt";
            var lst = ConsoleFile.ReadFile_By_Path(strTaskUpgradeSchedulePath);

            if (lst.Count > 30)
            {
                // 将前面的20条删了
                lst.RemoveRange(0, 20);
                ConsoleFile.SaveFile_By_Path(strTaskUpgradeSchedulePath, lst);
            }
        }
    }
}
