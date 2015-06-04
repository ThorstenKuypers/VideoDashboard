using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Security.Cryptography;
using System.Text;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;

using log4net;
//using Base32;

namespace LicensingService
{
    public class BetaTesterRegisterService
    {
        //private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(BetaTesterRegisterService).Name);
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger("BetaTesterRegisterService");
        public static bool TesterExists(string name, string mail, string ip)
        {
            try
            {
                SqlConnection sql = new SqlConnection(_conStr);
                SqlDataAdapter da = new SqlDataAdapter("SELECT * FROM BetaTesters", sql);
                DataSet ds = new DataSet("BetaTesters");
                da.Fill(ds, "BetaTesters");
                DataTable dt = ds.Tables["BetaTesters"];

                foreach (DataRow r in dt.Rows)
                {
                    if (!Convert.IsDBNull(r["name"]))
                    {
                        if (((string)r["name"]).Trim() == name)
                        {                            
                            log.Warn(string.Format("Name duplicate detected from IP: {0}", ip));
                            return true;
                        }
                    }

                    if (!Convert.IsDBNull(r["email"]))
                    {
                        if (((string)r["email"]).Trim() == mail)
                        {                            
                            log.Warn(string.Format("E-mail duplicate detected from IP: {0}", ip));
                            return true;
                        }
                    }

                    if (!Convert.IsDBNull(r["ip"]))
                    {
                        if (ip == null) 
                        {
                            log.Info(string.Format("Beta tester {0} - {1} manually added", name, mail));
                            return false; // ignore ip check if IP is null (tester is added by hand!)
                        }

#if __DEBUG
                        if (ip =="::1")
                        {
                            // ONLY FOR DEBUGGING PURPOSES
                            log.Debug("beta tester by localhost!");
                            return false;
                        }
#endif
                        if (((string)r["ip"]).Trim() == ip)
                        {
                            DateTime t = DateTime.Now;
                            DateTime tl = new DateTime();
                            if (!Convert.IsDBNull(r["registerDate"]))
                                tl = DateTime.FromBinary((long)r["registerDate"]);
                            TimeSpan ts = t - tl;

                            // if the same IP address was already used to register a new beta tester within the last 24 hours
                            // consider this a re-registration attempt
                            if (ts.Hours < 24) 
                            {
                                log.Warn(string.Format("IP duplicate detected for IP: {0} last time {1}h ago", ip, ts.Hours.ToString()));
                                return true;
                            }

                            return false;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                log.Error("Method: TesterExists()", ex);
                return true;
            }

            return false;
        }
        //public static bool CreateNewTester(string name, string email, string IPused)
        //{
        //    try
        //    {
        //        // create SHA512 hash from user data              
        //        string hashStr = string.Format("#{0}-{1}@{2}--{3}-{4}", new object[] { name, email, IPused, DateTime.Now.ToShortDateString(), DateTime.Now.ToShortTimeString() });
        //        byte[] hash = Encoding.ASCII.GetBytes(hashStr);
        //        SHA512CryptoServiceProvider sha = new SHA512CryptoServiceProvider();
        //        byte[] sha512 = sha.ComputeHash(hash);

        //        Random rnd = new Random((int)DateTime.Now.ToFileTime());
        //        hash = new byte[5];
        //        for (int i = 0; i < hash.Length; i++)
        //        {
        //            int x = rnd.Next(sha512.Length);
        //            hash[i] = sha512[x];
        //        }
        //        hashStr = null;
        //        sha512 = null;
        //        sha.Dispose();
        //        sha = null;
        //        //string code = Convert.ToBase64String(hash); // new string(Encoding.UTF8.GetChars(hash));
        //        string code = Base32.Base32Encoder.Encode(hash);
                
        //        hash = null;

        //        BetaTester t = new BetaTester(name, email, IPused, code, DateTime.Now);
        //        code = null;

        //        AddTesterToDB(t);

        //        log.Info(string.Format("Beat tester added successfully ({0} - {1} @ {2}", name, email, IPused));
        //        // TODO: send mail to user

        //        return true;
        //    }
        //    catch (Exception ex)
        //    {
        //        log.Error("Method: CreateNewTester()", ex);
        //        return false;
        //    }
        //}

        public static void DeleteTester(String name)
        {

        }

        public static void DeleteTester(BetaTester tester)
        {

        }

        /*********************************************************************/
        #region "Database related stuff"

        private static string _conStr = "Data Source=(LocalDB)\\v11.0;AttachDbFilename=D:\\Development\\Projects\\VD_LIC\\Homepage\\App_Data\\VideoDashboardDB.mdf;Integrated Security=True";

        public static void AddTesterToDB(BetaTester t)
        {
            if (t != null)
            {
                try
                {
                    SqlConnection sql = new SqlConnection(_conStr);
                    SqlDataAdapter sda = new SqlDataAdapter("SELECT * FROM BetaTesters", sql);
                    SqlCommandBuilder scb = new SqlCommandBuilder(sda);
                    sda.InsertCommand = scb.GetInsertCommand();

                    DataSet ds = new DataSet("BetaTesters");
                    sda.Fill(ds, "BetaTesters");

                    DataTable dt = ds.Tables["BetaTesters"];
                    DataRow row = dt.NewRow();
                    row["name"] = t.Name;
                    row["email"] = t.Email;
                    row["code"] = t.Code;
                    row["ip"] = t.IP;
                    row["registerDate"] = t.RegistrationDate.ToBinary();

                    dt.Rows.Add(row);
                    sda.Update(ds, "BetaTesters");
                }
                catch (Exception ex)
                {
                    log.Error("Method: AddTesterToDB()", ex);
                }
            }
        }

        public static BetaTester GetTesterFromDB(string name)
        {
            try
            {
                SqlConnection sql = new SqlConnection(_conStr);
                string cmd = string.Format("SELECT * FROM BetaTesters WHERE name='{0}'", name);
                SqlDataAdapter sda = new SqlDataAdapter(cmd, sql);

                DataSet ds = new DataSet();
                sda.Fill(ds);

                DataTable dt = ds.Tables["BetaTesters"];
                DataRow row = dt.Rows[0];

                string email = "";
                if (!Convert.IsDBNull(row["email"]))
                    email = (string)((string)row["email"]).Trim();

                string ip = "";
                if (!Convert.IsDBNull(row["email"]))
                    ip = (string)((string)row["email"]).Trim();

                string code = "";
                if (!Convert.IsDBNull(row["email"]))
                    code = (string)((string)row["email"]).Trim();

                DateTime t = new DateTime();
                if (!Convert.IsDBNull(row["registerDate"]))
                    t = DateTime.FromBinary((long)row["registerDate"]);

                return new BetaTester(name, email, ip, code, t);
            }
            catch (Exception ex)
            {
                log.Error("Method: GetTesterFromDB()", ex);
            }

            return null;
        }

        public static void RemoveTesterFromDB(string name)
        {

        }

        public static void UpdateTesterInDB(BetaTester t)
        {

        }

        #endregion
        /*********************************************************************/
    }
}