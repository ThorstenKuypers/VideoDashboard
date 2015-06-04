using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace customControls
{
    public partial class pnlWmvControl : UserControl
    {
        public event SelectedProfileChangedEventHandler SelectedProfileChanged;
        
        #region // Private Variables
        private int m_videoWidth;
        private int m_videoHeight;
        private int m_frameRate;
        private UInt32 m_bitrate;
        private Int64 m_keyframes;
        private UInt32 m_quality;
        private string m_appPath;
        private string m_profilesPath;

        #endregion
        
        public pnlWmvControl()
        {
            InitializeComponent();
        }

        #region // Public Methods

        public void SelectProfile(int idx)
        {
            comboBox1.SelectedIndex = idx;
        }

        public void EnumProfiles()
        {
            string profilePath = m_profilesPath;// m_appPath + "\\VideoProfiles";
            try
            {
                DirectoryInfo dir = new DirectoryInfo(profilePath);
                FileInfo[] files = dir.GetFiles();

                foreach (FileInfo fi in files)
                {
                    string s = fi.Name.Remove(fi.Name.LastIndexOf("."));
                    this.comboBox1.Items.Add(s);
                }

                dir = null;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error");
            }
        }

        #endregion
        #region // overrides

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            this.lblQuality.Text = this.trackBar1.Value.ToString();
        }
        
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
        }
        #endregion
        #region // Private Methods

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedProfileChangedEventArgs ea = new SelectedProfileChangedEventArgs();
            ea.index = -1;

            if (this.comboBox1.SelectedIndex != -1)
            {
                ea.ProfileName = (string)this.comboBox1.SelectedItem; // string.Format("{0}\\VideoProfiles\\{1}.prx", m_appPath, this.comboBox1.SelectedItem);
                ea.index = comboBox1.SelectedIndex;
            }
            else
            {
                ea.ProfileName = null;
            }
            
            this.SelectedProfileChanged.Invoke(ea);
        }
        #endregion

        #region // Public Properties
        //public String SelectedProfileName
        //{
        //    get
        //    {
        //        return m_selProfName;
        //    }
        //}

        public int VideoWidth
        {
            get
            {
                return m_videoWidth;
            }
            set
            {
                m_videoWidth = value;
                this.lblTargetWidth.Text = string.Format("{0}", m_videoWidth);
            }
        }

        public int VideoHeigth
        {
            get
            {
                return m_videoHeight;
            }
            set
            {
                m_videoHeight = value;
                this.lblTargetHeight.Text = string.Format("{0}", m_videoHeight);
            }
        }

        public string appPath
        {
            set
            {
                m_appPath = value;
            }
        }

        public string ProfilesPath
        {
            set
            {
                m_profilesPath = value;
            }
        }

        public int FrameRate
        {
            get
            {
                return m_frameRate;
            }
            set
            {
                m_frameRate = value;
                lblTargetFrameRate.Text = string.Format("{0}", m_frameRate);
            }
        }
        public UInt32 Bitrate
        {
            get
            {
                //if (tbBitrate.Text.EndsWith("M")
                UInt32.TryParse(tbBitrate.Text, out m_bitrate);
                m_bitrate = m_bitrate * 1000;
                return m_bitrate;
            }
            set
            {
                m_bitrate = value / 1000;
                tbBitrate.Text = m_bitrate.ToString();
            }
        }
        public Int64 KeyframeInterval
        {
            get
            {
                Int64.TryParse(tbKeyframe.Text, out m_keyframes);
                m_keyframes = m_keyframes * 10000000;
                return m_keyframes;
            }
            set
            {
                m_keyframes = value / 10000000;
                tbKeyframe.Text = m_keyframes.ToString();
            }
        }
        public UInt32 Quality
        {
            get
            {
                m_quality = (UInt32)trackBar1.Value;
                return m_quality;
            }
            set
            {
                m_quality = value;
                trackBar1.Value = (int)m_quality;
                lblQuality.Text = m_quality.ToString();
            }
        }

        #endregion

        private void pnlWmvControl_Load(object sender, EventArgs e)
        {
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            this.lblQuality.Text = this.trackBar1.Value.ToString();
        }
    }

    public delegate void SelectedProfileChangedEventHandler(SelectedProfileChangedEventArgs e);
    public struct SelectedProfileChangedEventArgs
    {
        public int index;
        public String ProfileName;
    }


}
