using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LicenseManager
{
    public partial class frmNewBetaTester : Form
    {
        public string UserName { get { return textBox1.Text; } }
        public string Email { get { return textBox2.Text; } }


        public frmNewBetaTester()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text=="")
            {
                MessageBox.Show("Name is empty");
                return;
            }
            if (textBox2.Text=="")
            {
                MessageBox.Show("E-Mail is empty");
                return;
            }

            DialogResult = DialogResult.OK;
        }
    }
}
