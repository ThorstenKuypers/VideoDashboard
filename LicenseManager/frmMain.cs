using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Configuration;
using System.Data.Sql;
using System.Data.SqlClient;
using System.Security.Cryptography;
using Base32;

using DatabaseService;
using System.ServiceModel;

namespace LicenseManager
{
    public partial class frmMain : Form
    {
        private IDatabaseService _dbClient;
        private ChannelFactory<IDatabaseService> _channels;

        public frmMain()
        {
            InitializeComponent();

            try
            {
#if DEBUG
                _channels = new ChannelFactory<IDatabaseService>("dbg_DatabaseEndpoint");
#else
            _channels = new ChannelFactory<IDatabaseService>("DatabaseEndpoint");
#endif
                _dbClient = _channels.CreateChannel(); //new DatabaseService.DatabaseServiceClient("WSHttpBinding_IDatabaseService");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void loadBetaTestersTable()
        {
            try
            {
                if (_dbClient == null)
                    _dbClient = _channels.CreateChannel();

                DataTable tbl = _dbClient.GetBetaTestersTable();
                if (tbl != null)
                {
                    int x = 0;

                    BindingSource bs = new BindingSource();
                    bs.DataSource = tbl;
                    dataGridViewBetaTesters.DataSource = bs;

                }

            }
            catch (FaultException<DatabaseService.DatabaseServiceException> ex)
            {
                if (ex.Detail.Message == "Unauthorized access")
                {
                    if (!_dbClient.Login("$$secret_passwd$$"))
                    {
                        MessageBox.Show("can't login to database service");
                        //_channels.Close();
                        _dbClient = null;
                        return;
                    }

                    loadBetaTestersTable();
                }
                else
                {
                    MessageBox.Show(ex.Detail.Message, ex.Reason.ToString());
                    //_channels.Close();
                    _dbClient = null;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

                // in case of exception consider the communication broken and close 
                // the channel and session
                //_channels.Close();
                _dbClient = null;
            }
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            loadBetaTestersTable();
        }

        private void btRefreshTable_Click(object sender, EventArgs e)
        {
            loadBetaTestersTable();
        }

        private void btUpdateTable_Click(object sender, EventArgs e)
        {
            if (mainTabControl.SelectedTab.Text == "BETA")
            {
                BindingSource bs = (BindingSource)dataGridViewBetaTesters.DataSource;
                DataTable dt = (DataTable)bs.DataSource;

                if (dt != null)
                {
                    try
                    {
                        if (_dbClient == null)
                            _dbClient = _channels.CreateChannel();

                        if (!_dbClient.Login("$$secret_passwd$$"))
                        {
                            MessageBox.Show("can't login to database service");
                            //_channels.Close();
                            _dbClient = null;
                            return;
                        }

                        _dbClient.UpdateBetaTestersTable(dt);
                    }
                    catch (FaultException<DatabaseServiceException> ex)
                    {
                        MessageBox.Show(ex.Detail.Message, ex.Reason.ToString());
                        //_channels.Close();
                        _dbClient = null;
                    }
                    catch (FaultException ex)
                    {
                        MessageBox.Show(ex.Message, ex.Reason.ToString());
                        //_channels.Close();
                        _dbClient = null;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "ERROR");
                        //_channels.Close();
                        _dbClient = null;
                    }
                }
            }
        }

        private void dataGridViewBetaTesters_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            if (mainTabControl.SelectedTab.Text == "BETA")
                MessageBox.Show("yes");

            DialogResult res = MessageBox.Show("WARNING: Really remove the current row from the data set? Can't be undone!", "WARNING", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);
            if (res == System.Windows.Forms.DialogResult.No)
            {
                e.Cancel = true;
                return;
            }
        }

        private void dataGridViewBetaTesters_UserDeletedRow(object sender, DataGridViewRowEventArgs e)
        {
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {

            _channels.Close();
            _channels = null;
        }

        private string generateNewBetaDiscountCode(string name, string email, string IPused)
        {
            string code = null;

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
                code = Base32.Base32Encoder.Encode(hash);

                hash = null;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "EEROR");
            }

            return code;
        }

        private void btAddNewTester_Click(object sender, EventArgs e)
        {
            frmNewBetaTester f = new frmNewBetaTester();
            DialogResult ret = f.ShowDialog();

            if (ret == DialogResult.OK)
            {
                BetaTester bt = new BetaTester();

                bt.ID = -1;
                bt.Name = f.UserName;
                bt.Email = f.Email;
                bt.IP = "::1";
                bt.Code = generateNewBetaDiscountCode(f.UserName, f.Email, bt.IP);
                bt.RegistrationDate = DateTime.Now;

                BindingSource bs = (BindingSource)dataGridViewBetaTesters.DataSource;
                DataTable dt = (DataTable)bs.DataSource;

                if (dt != null)
                {
                    DataRow r = dt.NewRow();
                    r.BeginEdit();
                    r["name"] = bt.Name;
                    r["email"] = bt.Email;
                    r["code"] = bt.Code;
                    r["ip"] = bt.IP;
                    r["registerDate"] = bt.RegistrationDate.ToBinary();
                    r.EndEdit();

                    dt.Rows.Add(r);

                    try
                    {
                        if (_dbClient == null)
                            _dbClient = _channels.CreateChannel(); //new DatabaseService.DatabaseServiceClient("WSHttpBinding_IDatabaseService");

                        if (!_dbClient.Login("$$secret_passwd$$"))
                        {
                            MessageBox.Show("can't login to database service");
                            //_channels.Close();
                            _dbClient = null;
                            return;
                        }

                        _dbClient.UpdateBetaTestersTable(dt);
                    }
                    catch (FaultException<DatabaseServiceException> ex)
                    {
                        MessageBox.Show(ex.Detail.Message, ex.Reason.ToString());
                        //_channels.Close();
                        _dbClient = null;
                    }
                    catch (FaultException ex)
                    {
                        MessageBox.Show(ex.Message, ex.Reason.ToString());
                        //_channels.Close();
                        _dbClient = null;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message, "ERROR");
                        //_channels.Close();
                        _dbClient = null;
                    }
                }
            }
        }
    }
}
