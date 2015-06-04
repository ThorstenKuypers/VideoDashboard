using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace customControls
{
    public partial class pnlRawEncCtrl : UserControl
    {
        #region "Events"

        public event ConfigureCompressorEventHandler ConfigureCompressor;
        public event SelectedCompressorChanged SelectedCompressorChanged;

        #endregion

        #region "Public Methods"

        public pnlRawEncCtrl()
        {
            InitializeComponent();
        }

        public void SelectCompressor(int idx)
        {
            cbCompressorList.SelectedIndex = idx;
        }

        public void AddCompressorToList(int index, string name)
        {
            cbCompressorList.Items.Add(name);
        }

        #endregion

        #region "Properties"

        public int OutputWidth
        {
            set
            {
                lblTargetWidth.Text = value.ToString();
            }
        }

        public int OutputHeight
        {
            set
            {
                lblTargetHeight.Text = value.ToString();
            }
        }

        public int OutputFramerate
        {
            set
            {
                lblTargetFrameRate.Text = value.ToString();
            }
        }

        public int SelectedCompressor
        {
            get
            {
                return m_selCompressor;
            }
        }

        #endregion

        private int m_selCompressor;

        #region "Protected Methods + Overrides"

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
        }

        #endregion

        #region "Private Methods"

        private void btConfigCompressor_Click(object sender, EventArgs e)
        {
            CompressorConfigEventArgs args = new CompressorConfigEventArgs();

            m_selCompressor = cbCompressorList.SelectedIndex;
            args.index = m_selCompressor;
            args.name = (string)cbCompressorList.SelectedItem;

            ConfigureCompressor.Invoke(args);
        }

        private void cbCompressorList_SelectedIndexChanged(object sender, EventArgs e)
        {
            string str = (string)cbCompressorList.SelectedItem;
            if (str.StartsWith("none"))
                btConfigCompressor.Enabled = false;
            else
                btConfigCompressor.Enabled = true;

            m_selCompressor = cbCompressorList.SelectedIndex;

            CompressorConfigEventArgs ea=new CompressorConfigEventArgs();
            ea.index =cbCompressorList.SelectedIndex;
            ea.name=str;

            SelectedCompressorChanged(ea);
        }

        #endregion

    }

    public struct CompressorConfigEventArgs
    {
        public int index;
        public string name;
    };

    public delegate void ConfigureCompressorEventHandler(CompressorConfigEventArgs e);
    public delegate void SelectedCompressorChanged(CompressorConfigEventArgs e);

}
