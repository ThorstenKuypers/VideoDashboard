using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Data;
using System.Security.Cryptography;
using System.Text;
using DatabaseService;
using Base32;
using System.Net.Mail;

namespace Homepage
{
    public partial class RegisterBeta : System.Web.UI.Page
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(Page).Name);

        public static bool TesterExists(string name, string mail, string ip)
        {
            try
            {
                DataTable dt = null;

                try
                {
                    dt = DatabaseLayer.GetTable("BetaTesters");
                }
                catch (Exception ex)
                {
                    throw ex;
                }

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
                        //if (ip == null)
                        //{
                        //    log.Info(string.Format("Beta tester {0} - {1} manually added", name, mail));
                        //    return false; // ignore ip check if IP is null (tester is added by hand!)
                        //}

                        if (ip == "::1")
                        {
                            // ONLY FOR DEBUGGING PURPOSES
                            log.Info("beta tester added by hand");
                            return false;
                        }
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
        public static bool CreateNewTester(string name, string email, string IPused)
        {
            try
            {
                // create SHA512 hash from user data              
                string hashStr = string.Format("#{0}-{1}@{2}--{3}-{4}", new object[] { name, email, IPused, DateTime.Now.ToShortDateString(), DateTime.Now.ToShortTimeString() });
                byte[] hash = Encoding.ASCII.GetBytes(hashStr);
                SHA512CryptoServiceProvider sha = new SHA512CryptoServiceProvider();
                byte[] sha512 = sha.ComputeHash(hash);

                Random rnd = new Random((int)DateTime.Now.ToFileTime());
                hash = new byte[5];
                for (int i = 0; i < hash.Length; i++)
                {
                    int x = rnd.Next(sha512.Length);
                    hash[i] = sha512[x];
                }
                hashStr = null;
                sha512 = null;
                sha.Dispose();
                sha = null;
                //string code = Convert.ToBase64String(hash); // new string(Encoding.UTF8.GetChars(hash));
                string code = Base32.Base32Encoder.Encode(hash);

                hash = null;

                BetaTester t = new BetaTester(name, email, IPused, code, DateTime.Now);
                code = null;

                DatabaseLayer.AddBetaTester(t);

                log.Info(string.Format("Beat tester added successfully ({0} - {1} @ {2}", name, email, IPused));
                // TODO: send mail to user
                SmtpClient mc = new SmtpClient(email);

                mc.Dispose();
                mc = null;

                return true;
            }
            catch (Exception ex)
            {
                log.Error("Method: CreateNewTester()", ex);
                return false;
            }
        }


        protected void Page_Load(object sender, EventArgs e)
        {
            this.RegisterStatus.Visible = false;

            string css = panelRegFormName.CssClass;

            int x = 0;
            if (Request.Form.Keys.Count != 0)
            {
                string name = Request.Form.Get(Request.Form.Keys[0]);
                string mail = Request.Form.Get(Request.Form.Keys[1]);

                if (name == "")
                {
                    this.panelRegFormName.CssClass += " has-error";
                }
                else
                    this.panelRegFormName.CssClass = css;

                if (mail == "")
                    this.panelRegFormEmail.CssClass += " has-error";
                else
                    this.panelRegFormEmail.CssClass = css;

                if (name != "" && mail != "")
                {
                    try
                    {
                        // first check if user is already registered
                        if (!TesterExists(name, mail, Request.UserHostAddress))
                        {
                            CreateNewTester(name, mail, Request.UserHostAddress);
                            string msg = "Thank you for registering as a beta tester. An e-mail with your tester discount code has been sent to: <br/>" +
                                         "<p class=\"text-info text-center\" style=\"margin:2% 0;\">{0}</p> <br/>" +
                                         "Please make sure that you also check your spam folder(s)";

                            this.RegisterStatus.Text = string.Format(msg, mail);
                            this.RegisterStatus.Visible = true;
                        }
                        else
                        {
                            this.RegisterStatus.CssClass = "h4 text-danger";
                            this.RegisterStatus.Text = "Sorry but no beta tester discount code could be created. Either your name or your e-mail" +
                                "address were already used to create a code. <br>" +
                                "If you are sure that you haven't registered already please contact <a href='mailto:support@video-dashboard.com'>support</a>.";
                            this.RegisterStatus.Visible = true;
                        }
                    }
                    catch (Exception ex)
                    {
                        log.Error("", ex);
                    }
                }
            }

            x = 0;
        }
    }
}