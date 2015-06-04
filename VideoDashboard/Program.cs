using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Reflection;
using System.Resources;

namespace VideoDashboard
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // all related files must be installed or written to the APPDATA folder because the programs files folder
            // need elevated privileges
            string path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\" + Application.ProductName + "\\";
            // delete logfile before application start; so only the most recent errors get logged (de-cluttering logfile)
            if (File.Exists(path + "VideoDashboard.log"))
                File.Delete(path + "VideoDashboard.log");

#if __BETA
            // pre-start conditions for beta testing version
            // Check license file
            byte[] b = VideoDashboard.Properties.Resources.license;
            string publicKey = VideoDashboard.Properties.Resources.publicKey;
            string lic = new string(System.Text.Encoding.ASCII.GetChars(b));
            File.WriteAllText(path + "license.lic", lic);

            try
            {
                Rhino.Licensing.LicenseValidator licVal = new Rhino.Licensing.LicenseValidator(publicKey, path + "license.lic");
                licVal.AssertValidLicense();
                licVal.DisableFutureChecks();

                licVal = null;
                publicKey = null;
                lic = null;
                if (File.Exists(path + "license.lic"))
                {
                    File.Delete(path + "license.lic");
                }
            }
            catch (Rhino.Licensing.LicenseExpiredException)
            {
                MessageBox.Show("The beta license has expired. Beta phase is officially over.", "Beta has expired", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            catch (Rhino.Licensing.LicenseFileNotFoundException)
            {
                MessageBox.Show("The beta license could not be found. Please reinstall VideoDashboard.", "License file not found", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            catch (Rhino.Licensing.LicenseNotFoundException)
            {
                MessageBox.Show("Invalid license. Please reinstall VideoDashboard.", "Invalid license", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("An unexpected error occured. Please reinstall VideoDashboard or contact support\n\n", ex.Message), "Unexpected error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

#endif
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new frmMain());
        }
    }
}
