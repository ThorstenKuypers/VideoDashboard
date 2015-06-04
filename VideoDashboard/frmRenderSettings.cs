using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using customControls;
using libds;

namespace VideoDashboard
{
    public enum VideoEncoderType
    {
        VideoEncoder_none = -1,
        VideoEncoder_WMV = 1,
        VideoEncoder_RAW = 2
    };

    public partial class frmRenderSettings : Form
    {
        private Project m_project;
        private string m_curWmvProfile;
        private CWM9_ProfileManager m_wmvProfiler;
        private VideoEncoderType m_selectedEncoder;
        private VideoCompressorEnum m_vidCompEnum;
        private List<VideoCompressor> m_compressorList;
        private IntPtr m_selCompressionFilter;

        private int wmvProfileIdx;

        public frmRenderSettings(ref Project prj)
        {
            InitializeComponent();
            
            try
            {
                m_project = prj;

                pnlWmvControl.SelectedProfileChanged += new SelectedProfileChangedEventHandler(OnSelectedProfileChanged);
                pnlWmvControl.appPath = m_project.curProjectInfo.appSet.appPath;
                pnlWmvControl.ProfilesPath = m_project.curProjectInfo.appSet.videoProfilesPath;
                pnlWmvControl.EnumProfiles();
                m_wmvProfiler = new CWM9_ProfileManager();

                m_vidCompEnum = new VideoCompressorEnum();
                m_vidCompEnum.Enumerate();
                m_compressorList = m_vidCompEnum.GetCompressorList();

                int i = 0;
                for (; i < m_compressorList.Count; i++)
                {
                    pnlRawEncCtrl.AddCompressorToList(i, m_compressorList[i].name);
                }
                pnlRawEncCtrl.AddCompressorToList(i, "none (uncompressed)");

                pnlRawEncCtrl.ConfigureCompressor += new ConfigureCompressorEventHandler(ConfigureVideoCompressor);
                pnlRawEncCtrl.SelectedCompressorChanged += new SelectedCompressorChanged(OnSelectedCompressorChanged);
            }
            catch (Exception ex)
            {
                string s = string.Format("Error in Method:\"frmRenderSettings()\"\n -->{0}", ex.Message);
                ErrorBox(s);

                return;
            }
        }

        public CWM9_ProfileManager WMV_ProfilerObj
        {
            get
            {
                return m_wmvProfiler;
            }
        }

        public IntPtr CompressionFilter
        {
            get
            {
                return m_selCompressionFilter;
            }
        }

        private void ErrorBox(string text)
        {
            MessageBox.Show(text, "Error in VirtualDashboard", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void frmRenderSettings_Load(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;

            if (pi.outVideoData.fileName != null && pi.outVideoData.fileName != "")
            {
                textBox1.Text = pi.outVideoData.fileName;
            }

            cbAspectRatio.SelectedIndex = pi.renderCfg.cbAspectRatioIdx;
            cbOutputWidth.SelectedIndex = pi.renderCfg.cbOutputWidthIdx;
            switch (pi.renderCfg.encoder)
            {
                case VideoEncoderType.VideoEncoder_WMV:
                    cbEncoder.SelectedIndex = 0;
                    pnlWmvControl.SelectProfile(pi.renderCfg.wmvProfileIndex);
                    break;

                case VideoEncoderType.VideoEncoder_RAW:
                    cbEncoder.SelectedIndex = 1;
                    pnlRawEncCtrl.SelectCompressor(pi.renderCfg.rawCompressorIndex);
                    break;

                default:
                    cbEncoder.SelectedIndex = 0;
                    break;
            }
        }

        private void ConfigureVideoCompressor(CompressorConfigEventArgs e)
        {
            try
            {
                if (e.index < 0 || e.index >= m_compressorList.Count)
                    return;

                if (m_vidCompEnum != null)
                {
                    m_vidCompEnum.ShowConfigDialog(m_compressorList[e.index].index, Handle);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void cbOutputWidth_SelectedIndexChanged(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            int w = pi.inVideoData.width, h = 0;
            float ar = pi.inVideoData.aspectR;

            if (cbOutputWidth.SelectedIndex > 0)
            {
                int.TryParse(cbOutputWidth.SelectedItem.ToString(), out w);
            }

            switch (cbAspectRatio.SelectedIndex)
            {
                case 1:
                    ar = (float)(4.0 / 3.0); // SD Aspect Ratio
                    break;
                case 2:
                    ar = (float)(16.0 / 9.0); // HD Aspect Ratio
                    break;
                default:
                    ar = pi.inVideoData.aspectR;
                    break;
            }
            pi.renderCfg.cbOutputWidthIdx = cbOutputWidth.SelectedIndex;

            h = (int)Math.Round(((float)w / (float)ar));
            lblResVidSize.Text = string.Format("{0}x{1}", w, h);

            pi.outVideoData.width = w;
            pi.outVideoData.height = h;
            pi.outVideoData.aspectR = ar;

            m_project.curProjectInfo = pi;

            pnlRawEncCtrl.OutputWidth = w;
            pnlRawEncCtrl.OutputHeight = h;
            pnlRawEncCtrl.OutputFramerate = pi.inVideoData.framerate;
        }

        private void cbAspectRatio_SelectedIndexChanged(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            int w = pi.inVideoData.width, h = 0;
            float ar = pi.inVideoData.aspectR;

            if (cbOutputWidth.SelectedIndex > 0)
            {
                int.TryParse(cbOutputWidth.SelectedItem.ToString(), out w);
            }

            switch (cbAspectRatio.SelectedIndex)
            {
                case 1:
                    ar = (float)(4.0 / 3.0); // SD Aspect Ratio
                    break;
                case 2:
                    ar = (float)(16.0 / 9.0); // HD Aspect Ratio
                    break;
                default:
                    ar = pi.inVideoData.aspectR;
                    break;
            }
            pi.renderCfg.cbAspectRatioIdx = cbAspectRatio.SelectedIndex;

            h = (int)Math.Round(((float)w / (float)ar));
            lblResVidSize.Text = string.Format("{0}x{1}", w, h);

            pi.outVideoData.width = w;
            pi.outVideoData.height = h;
            pi.outVideoData.aspectR = ar;

            m_project.curProjectInfo = pi;

            pnlRawEncCtrl.OutputWidth = w;
            pnlRawEncCtrl.OutputHeight = h;
            pnlRawEncCtrl.OutputFramerate = pi.inVideoData.framerate;
        }

        private void cbEncoder_SelectedIndexChanged(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            string file = textBox1.Text;

            switch (cbEncoder.SelectedIndex)
            {
                case 0: // WMV encoder
                    pnlRawEncCtrl.Visible = false;
                    pnlWmvControl.Visible = true;

                    m_selectedEncoder = VideoEncoderType.VideoEncoder_WMV;

                    cbAspectRatio.Enabled = false;
                    cbOutputWidth.Enabled = false;
                    lblResVidSize.Visible = false;

                    pi.renderCfg.rawCompressorIndex = -1;
                    pi.renderCfg.rawCompressorName = null;

                    if (file != null && file != "")
                    {
                        // change file name ext to wmv
                        int i = file.LastIndexOf('.');
                        if (i > 0)
                        {
                            string s = file.Remove(i);
                            string str = string.Format("{0}.wmv", s);
                            file = str;
                            textBox1.Text = str;
                        }
                    }

                    break;

                case 1: // RAW (AVI) encoder
                    pnlWmvControl.Visible = false;
                    pnlRawEncCtrl.Visible = true;

                    m_selectedEncoder = VideoEncoderType.VideoEncoder_RAW;

                    cbAspectRatio.Enabled = true;
                    cbOutputWidth.Enabled = true;
                    lblResVidSize.Visible = true;

                    pi.renderCfg.wmvProfileFileName = null;
                    pi.renderCfg.wmvProfileIndex = -1;

                    if (file != null && file != "")
                    {
                        // change file name ext to avi
                        int i = file.LastIndexOf('.');
                        if (i > 0)
                        {
                            string s = file.Remove(i);
                            string str = string.Format("{0}.avi", s);
                            file = str;
                            textBox1.Text = str;
                        }
                    }

                    break;

                default:
                    m_selectedEncoder = VideoEncoderType.VideoEncoder_none;
                    break;
            }

            pi.renderCfg.encoder = m_selectedEncoder;
            pi.outVideoData.fileName = file;
            m_project.curProjectInfo = pi;
        }

        private void OnSelectedProfileChanged(SelectedProfileChangedEventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;

            m_curWmvProfile = e.ProfileName;
            wmvProfileIdx = e.index;

            try
            {
                string vpPath = string.Format("{0}\\{1}.prx", m_project.curProjectInfo.appSet.videoProfilesPath, e.ProfileName);
                m_wmvProfiler.CloseCurrentProfile();
                m_wmvProfiler.LoadProfile(vpPath);

                pnlWmvControl.VideoWidth = m_wmvProfiler.ProfileVideoWidth;
                pnlWmvControl.VideoHeigth = m_wmvProfiler.ProfileVideoHeight;
                pnlWmvControl.FrameRate = (int)m_wmvProfiler.ProfileFramerate;
                pnlWmvControl.Bitrate = m_wmvProfiler.ProfileBitrate;
                pnlWmvControl.KeyframeInterval = m_wmvProfiler.ProfileKeyframes;
                pnlWmvControl.Quality = m_wmvProfiler.ProfileQuality;

                pi.renderCfg.wmvProfileIndex = wmvProfileIdx;
                pi.renderCfg.wmvProfileFileName = m_curWmvProfile;

                pi.outVideoData.width = m_wmvProfiler.ProfileVideoWidth;
                pi.outVideoData.height = m_wmvProfiler.ProfileVideoHeight;
                pi.outVideoData.framerate = (int)m_wmvProfiler.ProfileFramerate;
                pi.outVideoData.aspectR = (float)((float)m_wmvProfiler.ProfileVideoWidth / (float)m_wmvProfiler.ProfileVideoHeight);
                pi.outVideoData.avgTimeFrame = (int)(10000000 / m_wmvProfiler.ProfileFramerate);

                m_project.curProjectInfo = pi;
            }
            catch (Exception ex)
            {
                string s = string.Format("Error in Method:\"OnSelectedProfileChanged()\"\n -->{0}", ex.Message);
                ErrorBox(s);

                return;
            }
        }

        private void OnSelectedCompressorChanged(CompressorConfigEventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;

            pi.renderCfg.rawCompressorIndex = pnlRawEncCtrl.SelectedCompressor;
            if (pnlRawEncCtrl.SelectedCompressor == m_compressorList.Count)
            {
                pi.renderCfg.rawCompressorName = "none";
                m_selCompressionFilter = (IntPtr)0;
            }
            else
            {
                pi.renderCfg.rawCompressorName = m_compressorList[pnlRawEncCtrl.SelectedCompressor].name;
            }
            pi.outVideoData.framerate = pi.inVideoData.framerate;
            pi.outVideoData.avgTimeFrame = pi.inVideoData.avgTimeFrame;

            m_project.curProjectInfo = pi;
        }

        private void btCancel_Click(object sender, EventArgs e)
        {
        }

        private void btBrowseFile_Click(object sender, EventArgs e)
        {
            try
            {
                ProjectInfo pi = m_project.curProjectInfo;
                SaveFileDialog f = new SaveFileDialog();

                if (cbEncoder.SelectedIndex == 0)
                {
                    f.Filter = "WindowsMedia Files (*.wmv)|*.wmv";
                }
                if (cbEncoder.SelectedIndex == 1)
                {
                    f.Filter = "Raw Video (*.avi)|*.avi";
                }

                if (textBox1.Text != "")
                {
                    f.FileName = textBox1.Text;
                }

                if (f.ShowDialog() == DialogResult.OK)
                {
                    textBox1.Text = f.FileName;
                    pi.outVideoData.fileName = f.FileName;
                }

                m_project.curProjectInfo = pi;

                f.Dispose();
                f = null;
            }
            catch (Exception ex)
            {
                string s = string.Format("Error in Method:\"btBrowseFile_Click()\"\n -->{0}", ex.Message);
                ErrorBox(ex.Message);
            }
        }

        private void textBox1_Leave(object sender, EventArgs e)
        {
            //m_videoFileName = textBox1.Text;
            ProjectInfo pi = m_project.curProjectInfo;
            pi.outVideoData.fileName = textBox1.Text;
            m_project.curProjectInfo = pi;
        }

        private void textBox1_Enter(object sender, EventArgs e)
        {
            textBox1.BackColor = Color.White;
        }

        private void btOk_Click(object sender, EventArgs e)
        {
            try
            {
                ProjectInfo pi = m_project.curProjectInfo;

                if (string.IsNullOrEmpty(textBox1.Text))
                {
                    MessageBox.Show("Please enter a valid file name!");
                    textBox1.BackColor = Color.Yellow;
                    return;
                }

                // check if file exists and ask for overwrite
                if (System.IO.File.Exists(textBox1.Text))
                {
                    DialogResult dr =MessageBox.Show(string.Format("File {0} already exists.\nDo you want to replace it?", textBox1.Text), "Confirm Save As", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation);
                    if (dr == DialogResult.No)
                        return;
                }

                pi.outVideoData.fileName = textBox1.Text;

                //pi.renderCfg.encoder = m_selectedEncoder;
                if (m_selectedEncoder == VideoEncoderType.VideoEncoder_WMV)
                {
                    //pi.renderCfg.wmvProfileIndex = wmvProfileIdx;
                    //pi.renderCfg.wmvProfileFileName = m_curWmvProfile;

                    //pi.outVideoData.width = m_wmvProfiler.ProfileVideoWidth;
                    //pi.outVideoData.height = m_wmvProfiler.ProfileVideoHeight;
                    //pi.outVideoData.framerate = (int)m_wmvProfiler.ProfileFramerate;
                    //pi.outVideoData.aspectR = (float)((float)m_wmvProfiler.ProfileVideoWidth / (float)m_wmvProfiler.ProfileVideoHeight);
                }
                if (m_selectedEncoder == VideoEncoderType.VideoEncoder_RAW)
                {
                    //pi.renderCfg.rawCompressorIndex = pnlRawEncCtrl.SelectedCompressor;
                    //if (pnlRawEncCtrl.SelectedCompressor == m_compressorList.Count)
                    //{
                    //    pi.renderCfg.rawCompressorName = "none";
                    //}
                    //else
                    //{
                    //    pi.renderCfg.rawCompressorName = m_compressorList[pnlRawEncCtrl.SelectedCompressor].name;
                    //}
                    //pi.outVideoData.framerate = pi.inVideoData.framerate;
                    if (pi.renderCfg.rawCompressorName == "none")
                    {
                        m_selCompressionFilter = (IntPtr)0;
                    }
                    else
                    {
                        m_selCompressionFilter = m_vidCompEnum.GetCompressorFilter(m_compressorList[pnlRawEncCtrl.SelectedCompressor].index);
                    }
                }

                m_project.curProjectInfo = pi;

                DialogResult = DialogResult.OK;
            }
            catch (Exception ex)
            {
                string s = string.Format("Error in Method:\"btOk_Click()\"\n -->{0}", ex.Message);
                ErrorBox(ex.Message);
            }
        }
    }
}
