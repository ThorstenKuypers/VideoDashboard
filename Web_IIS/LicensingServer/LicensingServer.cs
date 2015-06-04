using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;
using System.IO;

using Activatar;
using Activatar.Providers;
using Activatar.Server;
using Rhino.Licensing;

namespace LicensingServer
{
    public class LicenseServer
    {

        private ProductActivation _activator;
        private string _privateXmlKey;
        private ProductKeyPublisher _keyGen;
        LicenseGenerator _gen;

        public LicenseServer(string privateKeyPath)
        {
            _privateXmlKey = File.ReadAllText(privateKeyPath);
            _activator = new ProductActivation(_privateXmlKey);
            _keyGen = new ProductKeyPublisher(_privateXmlKey);
            _gen = new LicenseGenerator(_privateXmlKey);
        }

        /// <summary>
        /// Method to create a new unique license key for the customer,
        /// based on the payment transaction data supplied
        /// </summary>
        public string CreateNewLicenseKey(string firstName, string lastName, string txn_id, int custId)
        {
            string date = DateTime.Now.ToString();
            Guid guid = Guid.NewGuid();
            string str = string.Format("{0} {1}|{2}@{3}", firstName, lastName, custId, date);
            string key = _keyGen.GenerateProductKey(str, guid);

            return key;
        }

        public string GenerateNewLicenseFile(string licenseKey, string machineId)
        {
            return null;
        }

        /// <summary>
        /// Activates a license key      
        /// </summary>
        /// <param name="licenseKey"></param>
        /// <returns>license file content or null in case of error</returns>
        public string ActivateNewLicense(string name, string licenseKey, string encodedHwid)
        {
            try
            {
                if (licenseKey != null && licenseKey != "")
                {
                    Dictionary<string, string> p = new Dictionary<string, string>();
                    p.Add("hid", encodedHwid);

                    string lic = _gen.Generate(name, Guid.NewGuid(), DateTime.MaxValue, p, LicenseType.Personal);

                    return lic;
                }
            }
            catch
            {
                return null;
            }

            return null;
        }

        public string GenerateNewTrialLicense(byte[] machineId)
        {
            Random rnd = new Random();
            int r = rnd.Next(int.MaxValue);
            string str = string.Format("$$TrialLicense@{0}/{1}$$", DateTime.Now.ToString(), r);

            string key = _keyGen.GenerateProductKey(str, Guid.NewGuid());
            byte[] kb = new System.Security.Cryptography.SHA1CryptoServiceProvider().ComputeHash(Encoding.ASCII.GetBytes(key));
            key = Convert.ToBase64String(kb);

            str = Encoding.ASCII.GetString(machineId);
            byte[] b = Convert.FromBase64String(str);


            Dictionary<string, string> p = new Dictionary<string, string>();
            p.Add("hid", Encoding.ASCII.GetString(machineId));

            return _gen.Generate(key, Guid.NewGuid(), DateTime.Now.AddDays(3), p, LicenseType.Trial); ;
        }
    }
}
