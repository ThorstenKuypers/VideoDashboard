using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using libDataLogging.DataLogWrapper;

namespace VideoDashboard
{
    public partial class frmChannelsList : Form
    {
        private CDataLog _logger;

        public frmChannelsList(CDataLog logger)
        {
            InitializeComponent();

            listView1.HeaderStyle = ColumnHeaderStyle.Nonclickable;
            _logger = logger;
        }

        private void frmChannelsList_Load(object sender, EventArgs e)
        {
            if (_logger == null)
            {
                MessageBox.Show("No log file is currently open. Please open a log file first!", "Error missing log file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            string[] channels = _logger.ExportedChannelNames;
            string[] mathCh = _logger.SpecialChannelNames;

            for(int i=0;i<channels.Length;i++)
            {
                listView1.Items.Add(channels[i]);

                listView1.Items[i].SubItems.Add(_logger.GetChannelUnit(channels[i]));
                listView1.Items[i].SubItems.Add(_logger.GetChannelDescription(channels[i]));
            }

            foreach (string s in mathCh)
            {
                string c = string.Format("{0} (M)", s);
                listView1.Items.Add(c);
            }

            mathCh = null;
            channels = null;
        }
    }
}
