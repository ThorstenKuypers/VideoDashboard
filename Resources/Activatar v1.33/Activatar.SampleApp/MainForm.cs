using System;
using System.Drawing;
using System.Windows.Forms;

namespace Activatar.SampleApp
{
    public partial class MainForm : Form
    {
        private ProductLicenseManager _licenseManager;

        #region MainForm constructor
        public MainForm()
        {
            InitializeComponent();
            lblLogo.Parent = picLogo;
        }
        #endregion

        private void InitializeLicenseManager()
        {
            string publicXmlKey = Helpers.GetResourceString("Activatar.SampleApp.RSAKeys.PublicKey.xml");
            _licenseManager = new ProductLicenseManager(publicXmlKey);
        }

        protected override void OnLoad(EventArgs e)
        {
            InitializeLicenseManager();

            ProductLicense license = _licenseManager.LoadLicense("SampleApp");

            switch (license.Status)
            {
                case LicenseStatus.Licensed:
                    lblMessage.ForeColor = Color.Green;
                    lblMessage.Text = "This product has a valid license. To try again, enter a new product key.";
                    break;
                case LicenseStatus.TrialVersion:
                    lblMessage.ForeColor = Color.Green;
                    lblMessage.Text = String.Format("This is a trial license. {0} To try again, enter a new product key.", license.StatusReason);
                    break;
                case LicenseStatus.Expired:
                    lblMessage.ForeColor = Color.Red;
                    lblMessage.Text = String.Format("The trial period for this product has expired. {0}", license.StatusReason);
                    break;
                case LicenseStatus.MachineHashMismatch:
                    lblMessage.ForeColor = Color.Red;
                    lblMessage.Text = "The current license is not valid for this machine.";
                    break;
                default:
                    lblMessage.ForeColor = Color.Red;
                    lblMessage.Text = "This product needs activation.";
                    break;
            }

            base.OnLoad(e);
        }

        private void OnActivateClick(object sender, LinkLabelLinkClickedEventArgs e)
        {
            lblMessage.Text = lblError.Text = String.Empty;

            this.Cursor = Cursors.WaitCursor;
            ProductLicense license = _licenseManager.ActivateProduct(txtProductKey.Text);
            this.Cursor = Cursors.Default;

            switch (license.Status)
            {
                case LicenseStatus.Licensed:
                    lblMessage.ForeColor = Color.Green;
                    lblMessage.Text = "The product key is correct! Press next to finish.";
                    groupBox.Visible = false;
                    cmdNext.Enabled = true;
                    _licenseManager.SaveLicense("SampleApp", license);
                    break;
                case LicenseStatus.TrialVersion:
                    lblMessage.ForeColor = Color.Green;
                    lblMessage.Text = String.Format("This product key has a trial license. {0}", license.StatusReason);
                    groupBox.Visible = false;
                    cmdNext.Enabled = true;
                    _licenseManager.SaveLicense("SampleApp", license);
                    break;
                case LicenseStatus.Expired:
                    lblMessage.ForeColor = Color.Red;
                    lblMessage.Text = String.Format("The trial period for this product has expired. {0}", license.StatusReason);
                    _licenseManager.SaveLicense("SampleApp", license);
                    break;
                case LicenseStatus.InternalError:
                    lblMessage.ForeColor = Color.Red;
                    lblMessage.Text = "There was an error trying to activate the product.";
                    lblError.Text = license.StatusReason;
                    break;
                default:
                    lblMessage.ForeColor = Color.Red;
                    lblMessage.Text = "The product key you entered is not valid.";
                    break;
            }
        }

        #region Form management
        private void OnGenerateClick(object sender, LinkLabelLinkClickedEventArgs e)
        {
            KeyGenerator keyGen = new KeyGenerator();
            bool generated = keyGen.OpenForm(this);
            lblMessage.Text = lblError.Text = txtProductKey.Text = String.Empty;
            if (generated)
            {
                string newProductKey = (string) Clipboard.GetData(DataFormats.Text);
                string key = String.Empty;
                for (int n = 0; n < newProductKey.Length; n++)
                {
                    key += newProductKey[n];
                    txtProductKey.Text = key;
                    System.Threading.Thread.Sleep(30);
                    this.Refresh();
                }
            }
        }

        private void OnNextClick(object sender, EventArgs e)
        {
            pnl2.Location = new Point(10, 90);
            pnl2.Visible = true;
            pnl1.Visible = false;
            cmdNext.Visible = false;
            cmdTryAgain.Visible = true;
            cmdExit.Visible = true;
            cmdCancel.Visible = false;
        }

        private void OnTryAgainClick(object sender, EventArgs e)
        {
            txtProductKey.Text = String.Empty;
            pnl2.Location = new Point(318, 90);
            pnl2.Visible = false;
            pnl1.Visible = true;
            cmdNext.Visible = true;
            cmdNext.Enabled = false;
            cmdTryAgain.Visible = false;
            cmdExit.Visible = false;
            cmdCancel.Visible = true;
            groupBox.Visible = true;
        }

        private void OnProductKeyTextChanged(object sender, EventArgs e)
        {
            if (!String.IsNullOrEmpty(txtProductKey.Text) && txtProductKey.Text.Length == 35)
            {
                lnkActivate.Enabled = true;
            }
            else
            {
                lnkActivate.Enabled = false;
            }
        }

        private void OnCancelClick(object sender, EventArgs e)
        {
            if (MessageBox.Show(this, "Are you sure you want to exit?", "Confirmation", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.OK)
            {
                Application.Exit();
            }
        }

        private void OnExitClick(object sender, EventArgs e)
        {
            Application.Exit();
        }
        #endregion
    }
}
