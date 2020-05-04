using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CreateCard
{
    class Program
    {
        public static ulong[] Crc32Table = new ulong[256];
        public static int GetCrc(string strMsg)
        {
            byte[] buffer = System.Text.ASCIIEncoding.ASCII.GetBytes(strMsg); ulong value = 0xffffffff;
            int len = buffer.Length;
            for (int i = 0; i < len; i++)
            {
                value = (value >> 8) ^ Crc32Table[(value & 0xFF) ^ buffer[i]];
            }
            return (int)(value ^ 0xffffffff);
        }
        public static void MakeTable()
        {
            ulong Crc;
            Crc32Table = new ulong[256];
            int i, j;
            for (i = 0; i < 256; i++)
            {
                Crc = (ulong)i;
                for (j = 8; j > 0; j--)
                {
                    if ((Crc & 1) == 1)
                        Crc = (Crc >> 1) ^ 0xEDB88320;
                    else
                        Crc >>= 1;
                }
                Crc32Table[i] = Crc;
            }
        }

        static int FindArgs(string[] args, string str)
        {
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == str)
                    return i;
            }
            return -1;
        }

        static int GetNumber(string[] args, string str)
        {
            try
            {
                int nIndex = FindArgs(args, str);
                if (nIndex == -1)
                {
                    Console.WriteLine("where is the " + str + "? make sure input '" + str + "'");
                    return -1;
                }

                int nNumber = 0;
                string strCount = args[nIndex + 1];
                if (!int.TryParse(strCount, out nNumber))
                {
                    Console.WriteLine("Can't Convert " + strCount + " to Number!");
                    return -1;
                }
                return nNumber;
            }
            catch (Exception ex)
            {
                Console.WriteLine("GetNumber:" + ex.Message);
            }
            return -1;
        }

        static string GetText(string[] args, string str)
        {
            try
            {
                int nIndex = FindArgs(args, str);
                if (nIndex == -1)
                {
                    Console.WriteLine("where is the " + str + "? make sure input '" + str + "'");
                    return "";
                }

                return args[nIndex + 1];
            }
            catch (Exception ex)
            {
                Console.WriteLine("GetText:" + ex.Message);
            }
            return "";
        }

        static bool WriteFile(List<string> lst)
        {
            bool bRetCode = false;
            using (StreamWriter SW = new StreamWriter(System.Environment.CurrentDirectory + @"\NewCard.txt",false,Encoding.Default))
            {
                for (int i = 0; i < lst.Count; ++i)
                    SW.WriteLine(lst[i]);

                bRetCode = true;
                SW.Close();
            }
            return bRetCode;
        }

        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("at least 1 parm!");
                return;
            }
            if (args[0] == "?")
            {
                Console.WriteLine("CreateNewCard -Count ? -Hour ? -Format Output|File");
            }
            else if (args[0] == "CreateNewCard")
            {
                int nCount = GetNumber(args, "-Count");
                if (nCount == -1)
                    return;

                int nHour = GetNumber(args, "-Hour");
                if (nHour == -1)
                    return;

                string strFormat = GetText(args, "-Format");
                if (strFormat == "")
                    return;

                MakeTable();
                Random rd = new Random();
                List<string> lst = new List<string>();
                for (int i = 0; i < nCount; i++)
                {
                    string strCard = string.Empty;
                    strCard += nHour.ToString("D4");
                    strCard += DateTime.Now.Year.ToString("D4");
                    strCard += DateTime.Now.Month.ToString("D2");
                    strCard += DateTime.Now.Day.ToString("D2");
                    strCard += rd.Next(Int32.MaxValue).ToString("X8");
                    strCard += GetCrc(strCard).ToString("X8");
                    lst.Add(strCard);
                }

                if (strFormat == "Output")
                {
                    if (WriteDB(ref lst))
                    {
                        for (int i = 0; i < lst.Count; ++i)
                            Console.WriteLine(lst[i]);
                    }
                   
                }
                else if (strFormat == "File")
                {
                    if (WriteDB(ref lst))
                    {
                        if (!WriteFile(lst))
                        {
                            Console.WriteLine("WriteFile Fiald!");
                            return;
                        }
                    }
                    
                }
            }
            else
            {
                Console.WriteLine("invalid parm by:" + args[0]);
            }

        }

        public static bool WriteDB(ref List<string> lst)
        {
            try
            {
                for (int i = 0; i < lst.Count; i++)
                {
                    string strCardCrc = GetCrc(lst[i]).ToString("X8");
                    string strSQL = "exec Proc_InsertCard '" + lst[i] + "','" + strCardCrc + "'";
                    DataSet ds = Query(strSQL, 3000);
                    if (ds.Tables.Count > 0 && ds.Tables[0].Rows.Count > 0)
                    {
                        string strRetValue = ds.Tables[0].Rows[0][0] as string;
                        if (strRetValue.Trim() == "NO")
                        {
                            Console.WriteLine("CardNo Repeat, Card:" + lst[i] + ",invalid");
                            lst.RemoveAt(i--);
                            continue;
                        }
                    }
                    else
                    {
                        Console.WriteLine("con db Faild!, Card:" + lst[i] + ",invalid");
                        lst.RemoveAt(i--);
                    }
                } 
                Console.WriteLine("Write CardNo in DB for count was:" + lst.Count.ToString());
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("WriteDB:" + ex.Message);
            }
            return false;
        }

        public static DataSet Query(string SQLString, int Times)
        {
            using (SqlConnection connection = new SqlConnection("server=localhost;database=BnsKorea;uid=GameUser;pwd=lg_db_korea"))
            {
                DataSet ds = new DataSet();
                try
                {
                    connection.Open();
                    SqlDataAdapter command = new SqlDataAdapter(SQLString, connection);
                    command.SelectCommand.CommandTimeout = Times;
                    command.Fill(ds, "ds");
                }
                catch (System.Data.SqlClient.SqlException ex)
                {
                    Console.WriteLine("Query:" + ex.Message);
                    throw new Exception(ex.Message);
                }
                return ds;
            }
        }


    }
}
