using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

using libDataLogging.DataLogWrapper;
//using LayoutDefinitionFormat;
using libLDFWrapper;
using libds;
using DShow;

namespace VideoDashboard
{
    /*******************************************************************************************************/
    #region "Event Args Structures"

    #endregion
    /*******************************************************************************************************/

    /*******************************************************************************************************/
    #region "Delegates"

    public delegate void CloseProjectFormEventHandler(CloseProjectEventArgs e);

    #endregion
    /*******************************************************************************************************/

    /***********************************************************************************************/
    #region "Structures"

    public struct DashboardOverlayPreviewSettings
    {
        //public CDashboardLayout dashObj;
        public Bitmap dashboard;
        public Point Position;
        public Size Size;
        public float sizeScale;
        public float transparency;

        public bool isEnabled; // whether dashboard preview is checked
    };

    #endregion
    /***********************************************************************************************/

    public partial class frmProject : Form, IDisposable
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(frmProject).Name);

        private class DShowEventArgs : EventArgs
        {

            private long m_eventCode;
            private IntPtr m_param1;
            private IntPtr m_param2;
            private IntPtr m_graphId;

            public DShowEventArgs(long evCode, IntPtr param1, IntPtr param2, IntPtr GraphId)
            {
                m_eventCode = evCode;
                m_param1 = param1;
                m_param2 = param2;
                m_graphId = GraphId;
            }

            public long EventCode
            {

                get
                {
                    return m_eventCode;
                }
            }
            public IntPtr Param1
            {

                get
                {
                    return m_param1;
                }
            }
            public IntPtr Param2
            {

                get
                {
                    return m_param2;
                }
            }
            public IntPtr FilterGraphId
            {

                get
                {
                    return m_graphId;
                }
            }
        };

        private delegate void DShowEventHandler(Object sender, DShowEventArgs e);

        /***********************************************************************************************/
        #region "Events"

        public event CloseProjectFormEventHandler m_closeProjectEvent;
        private event DShowEventHandler m_dshowEvent;

        #endregion
        /***********************************************************************************************/

        /***********************************************************************************************/
        #region "Class Constructor/Destructor/Cleanup"

        public frmProject(Project project)
        {
            InitializeComponent();
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            this.Dock = DockStyle.Fill;

            m_project = project;
            m_curLapIdx = 0;
            m_curLapNumber = 0;
            m_dataLog = null;

            m_disposed = false;

            m_dshowEvent += new DShowEventHandler(DshowNotify);

            m_refSampleIdx = 0;

//            log4net.Config.XmlConfigurator.Configure();
        }

        #endregion
        /***********************************************************************************************/
        public Project m_project;

        /***********************************************************************************************/
        #region "Public Methods"

        public void ChangeProjectSettings()
        {
            toolStripBtEditProjectSettings_Click(this, new EventArgs());
        }

        public void RenderPreview()
        {
            toolStripBtRenderQuickPreview_Click(this, new EventArgs());
        }

        public void RenderToFile()
        {
            toolStripBtRenderToFile_Click(this, new EventArgs());
        }

        public void SaveProjectAs()
        {
            ProjectInfo pi = m_project.curProjectInfo;

            if (m_dashLayoutParser != null)
            {
                //pi.outVideoData.overlayCfg.name = m_curSelOverlay.GetDashboardPrettyName(); // DashboardName;
                pi.outVideoData.overlayCfg.posX = sliderOverlayPosX.Value;
                pi.outVideoData.overlayCfg.posY = sliderOverlayPosY.Value;
                pi.outVideoData.overlayCfg.scale = sliderOverlayScale.Value;
                pi.outVideoData.overlayCfg.transp = sliderTransparency.Value;
            }
            m_project.curProjectInfo = pi;

            try
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.FileName = pi.projectName;
                sfd.Filter = "VideoDashboard project files|*.vdprj";
                sfd.FilterIndex = 0;
                sfd.DefaultExt = "vdprj";
                sfd.AddExtension = true;
                //sfd.InitialDirectory = pi.appSet.projectPath;
                sfd.OverwritePrompt = true;
                sfd.Title = "Save project as...";

                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    pi.projectFileName = sfd.FileName;
                    m_project.curProjectInfo = pi;

                    m_project.Save(true);
                }
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        public void SaveProject()
        {
            ProjectInfo pi = m_project.curProjectInfo;

            try
            {
                if (m_dashLayoutParser != null)
                {
                    //pi.outVideoData.overlayCfg.name = m_curSelOverlay.DashboardName;
                    pi.outVideoData.overlayCfg.posX = sliderOverlayPosX.Value;
                    pi.outVideoData.overlayCfg.posY = sliderOverlayPosY.Value;
                    pi.outVideoData.overlayCfg.scale = sliderOverlayScale.Value;
                    pi.outVideoData.overlayCfg.transp = sliderTransparency.Value;
                }

                m_project.curProjectInfo = pi;

                if (string.IsNullOrEmpty(pi.projectFileName))
                {
                    SaveProjectAs();
                    return;
                }

                m_project.Save(true);
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        public void LoadProject()
        {
            try
            {
                loadVideoFile();
                loadDataLog();

                ProjectInfo pi = m_project.curProjectInfo;
             
                sliderOverlayPosX.Value = pi.outVideoData.overlayCfg.posX;
                sliderOverlayPosY.Value = pi.outVideoData.overlayCfg.posY;
                if (pi.outVideoData.overlayCfg.scale == 0)
                    sliderOverlayScale.Value = 100;
                else
                    sliderOverlayScale.Value = pi.outVideoData.overlayCfg.scale;


                if (pi.outVideoData.overlayCfg.transp == 0)
                    sliderTransparency.Value = 100;
                else
                    sliderTransparency.Value = pi.outVideoData.overlayCfg.transp;

                lblPosX.Text = sliderOverlayPosX.Value.ToString();
                lblPosY.Text = sliderOverlayPosY.Value.ToString();

                lblSyncEndMarkerFrame.Text = pi.outVideoData.syncData.endFrame.ToString();
                lblSyncEndMarkerSample.Text = pi.outVideoData.syncData.endSample.ToString();
                lblSyncStartMarkerFrame.Text = pi.outVideoData.syncData.startFrame.ToString();
                lblSyncStartMarkerSample.Text = pi.outVideoData.syncData.startSample.ToString();

                string tt = m_project.OutVideoData.syncData.endSample.ToString();

                int l = pi.outVideoData.syncData.endFrame - pi.outVideoData.syncData.startFrame;
                if (l < 0)
                    l = 0;
                lblOutVidLength.Text = l.ToString();

                l = pi.outVideoData.syncData.endSample - pi.outVideoData.syncData.startSample;
                if (l < 0)
                    l = 0;
                lblOutVidDataLength.Text = l.ToString();

                sliderDataViewerPos.Value = pi.outVideoData.syncData.startSample;

                if (pi.outVideoData.syncData.startFrame == 0)
                {
                    m_videoPlayer.CurrentVideoPos = 1;
                }
                else
                    m_videoPlayer.CurrentVideoPos = pi.outVideoData.syncData.startFrame;

                bool f = false;
                foreach (string s in cbOverlayList.Items)
                {
                    if (pi.dashboardFileName != "" && pi.dashboardFileName != null)
                    {
                        if (m_dashboardMap[s] == pi.dashboardFileName)
                        {
                            cbOverlayList.SelectedItem = s;
                            pi.outVideoData.overlayCfg.name = s;
                            f = true;
                            break;
                        }
                    }
                }

                m_project.curProjectInfo = pi;

                if (!f)
                    MessageBox.Show("The dashboard saved in the project file could not be found", "Mising overlay in project", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }

            string str = string.Format("Project: {0} loaded successfully from file {1}", m_project.ProjectName, m_project.ProjectFileName);
            log.Info(str);
        }

        public void NewProject()
        {
            string str = string.Format("New project \"{0}\" started", m_project.ProjectName, m_project.ProjectFileName);
            log.Info(str);
        }

        #endregion
        /***********************************************************************************************/

        /***********************************************************************************************/
        #region  "private variables"

        private bool m_videoFileOpen;
        private bool m_dataLogFileOpen;
        private CDataLog m_dataLog;
        private int m_curLapNumber;
        private int m_curLapIdx;
        private int m_sampleIdx;
        private LapInfo[] m_lapsArray;
        private bool m_isVideoLatched;
        private DashboardLayout m_dashLayoutParser;
        private Bitmap m_curActiveDashboard;
        private DashboardOverlayPreviewSettings m_dashPreviewSettings;
        private Dictionary<string, string> m_dashboardMap; // mapping of dashboard display names to file names

        private CVideoEncoder m_vidEncoder;

        private bool m_disposed;

        private Int64 m_encoderStartTime;
        private Int64 m_encoderElapsedTime;

        frmRenderSettings m_frmRender;

        int m_refSampleIdx; // reference sample index; when latch was enabled
        int m_refVidFrame;  //          "   only for video

        int m_playStartFrame; // frame at start of playback

        #endregion
        /***********************************************************************************************/

        /***********************************************************************************************/
        #region IDisposable Members

        void IDisposable.Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion
        /***********************************************************************************************/

        /***********************************************************************************************/
        #region "Data-Viewer"

        private void updateDataViewer()
        {
            try
            {
                if (m_dataLog == null)
                    return;

                if (dataViewerTickTimer.Enabled)
                    sliderDataViewerPos.Value = m_sampleIdx;
                else
                    m_sampleIdx = sliderDataViewerPos.Value;

                DataLogSample sample = m_dataLog.GetSample(m_sampleIdx);
                if (sample != null)
                {
                    m_curLapNumber = sample.LapNo;
                    m_curLapIdx = m_dataLog.GetLapIndex(m_curLapNumber);
                    lblCurLapIdx.Text = m_curLapIdx.ToString();
                    lblCurLap.Text = m_curLapNumber.ToString();

                    // draw session time counter
                    drawSessionTime(sample);
                    drawSpeed(sample);
                    drawRpm(sample);
                    drawLatG(sample);
                    drawLongG(sample);
                    drawLapPercent(sample);
                    drawGear(sample);
                    updateLapTimingInfo(m_curLapIdx);
                }
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void drawSessionTime(DataLogSample s)
        {
            try
            {
                TimeSpan ts = new TimeSpan(0, 0, (int)s.time);
                string text = string.Format("{0}:{1:00}:{2:00}", ts.Hours, ts.Minutes, ts.Seconds);

                lblSessionTime.Text = text;
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }

            //this.lblSessionTime.Refresh();

            //Graphics gfx = Graphics.FromHwnd(this.lblSessionTime.Handle);
            //TimeSpan ts = new TimeSpan(0, 0, (int)s.time);
            //string text = string.Format("{0}:{1}:{2}", ts.Hours, ts.Minutes, ts.Seconds);
            //Font f = new Font("Microsoft Sans Serif", 12.0f);
            //SolidBrush br = new SolidBrush(Color.Red);

            //SizeF text_size = gfx.MeasureString(text, f);
            //SizeF pnl_size = new SizeF(this.lblSessionTime.Width, this.lblSessionTime.Height);

            //float dx = ((pnl_size.Width - text_size.Width) / 2);
            //float dy = ((pnl_size.Height - text_size.Height) / 2);

            //gfx.DrawString(text, f, br, dx, dy);

            //br.Dispose();
            //f.Dispose();
            //gfx.Dispose();
        }

        private void drawSpeed(DataLogSample s)
        {
            try
            {
                float speed = s.groundSpeed;

                if (m_project.curProjectInfo.appSet.units == Units.Units_Metric)
                {
                    speed *= 3.6f;
                }
                if (m_project.curProjectInfo.appSet.units == Units.Units_English)
                {
                    speed *= 2.23694f;
                }

                int v = (int)speed;

                string str = string.Format("{0}", v);
                lblSpeed.Text = str;
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }

        }

        private void drawRpm(DataLogSample s)
        {
            float rpm = s.engineRpm;
            int r = (int)rpm;
            string str = string.Format("{0}", r);

            lblRpm.Text = str;
        }

        private void drawLatG(DataLogSample s)
        {
            //lblLatG.Refresh();

            try
            {
                SizeF latGbar = new SizeF(lblLatG.Width, lblLatG.Height);
                Bitmap bmp = new Bitmap((int)latGbar.Width, (int)latGbar.Height); // bitmap to draw the element in
                Graphics g = Graphics.FromImage(bmp);
                float cp = latGbar.Width / 2; // center point (y=0)
                float g_perPix = 3.0f / cp; // Gs per pixel (max +/- 3G)

                g.FillRectangle(new SolidBrush(Color.White), 0, 0, bmp.Width, bmp.Height);

                // draw centerline
                Pen p = new Pen(Color.Red);
                g.DrawLine(p, cp, 0, cp, latGbar.Height);

                float gLat = s.latG / 9.81f;
                float x = gLat / g_perPix;
                if (x < 0)
                {
                    g.FillRectangle(new SolidBrush(Color.Red), cp + x, 0, (x * (-1)), bmp.Height);
                }
                else
                {
                    g.FillRectangle(new SolidBrush(Color.Red), cp, 0, x, bmp.Height);
                }
                g.Dispose();
                g = null;

                Graphics gfx = Graphics.FromHwnd(lblLatG.Handle);
                gfx.DrawImage(bmp, 0, 0, bmp.Width, bmp.Height);
                gfx.Dispose();
                gfx = null;

                bmp.Dispose();
                bmp = null;
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void drawLongG(DataLogSample s)
        {
            //lblLongG.Refresh();

            try
            {
                SizeF longGbar = new SizeF(lblLongG.Width, lblLongG.Height);
                Bitmap bmp = new Bitmap((int)longGbar.Width, (int)longGbar.Height);
                Graphics g = Graphics.FromImage(bmp);
                float cp = longGbar.Width / 2; // center point (y=0)
                float g_perPix = 3.0f / cp; // Gs per pixel (max +/- 3G)

                g.FillRectangle(new SolidBrush(Color.White), 0, 0, bmp.Width, bmp.Height);

                // draw centerline
                Pen p = new Pen(Color.Green);
                g.DrawLine(p, cp, 0, cp, longGbar.Height);

                float gLong = s.longG / 9.81f;
                float x = gLong / g_perPix;
                if (x < 0)
                {
                    g.FillRectangle(new SolidBrush(Color.Green), cp + x, 0, (x * (-1)), longGbar.Height);
                }
                else
                {
                    g.FillRectangle(new SolidBrush(Color.Green), cp, 0, x, longGbar.Height);
                }
                g.Dispose();
                g = null;

                Graphics gfx = Graphics.FromHwnd(lblLongG.Handle);
                gfx.DrawImage(bmp, 0, 0, bmp.Width, bmp.Height);
                gfx.Dispose();
                gfx = null;

                bmp.Dispose();
                bmp = null;
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void drawGear(DataLogSample s)
        {
            lblGear.Text = s.gear.ToString();
        }

        private void updateLapTimingInfo(int lapIdx)
        {
            LapInfo li = m_dataLog.GetLapInfo(lapIdx);
            lblCurLapTime.Text = formatLapTimeString((float)li.lapTime);
        }

        private void drawLapPercent(DataLogSample s)
        {
            float p = s.lapPct * 100.0f;

            string str = string.Format("{0:f2}", p);
            lblCurLapPercent.Text = str;
        }

        private string formatLapTimeString(float time)
        {
            int min = 0;
            float sec = 0;

            sec = time % 60;
            min = (int)((time - sec) / 60);

            return string.Format("{0}:{1:00.000}", min, sec);
        }

        private void sliderDataViewerPos_ValueChanged(object sender, EventArgs e)
        {
            bool en = false; // was enabled before disabling the timer?
            if (dataViewerTickTimer.Enabled)
            {
                dataViewerTickTimer.Stop();
                en = true;
            }

            lblCurDataPos.Text = sliderDataViewerPos.Value.ToString();

            //m_sampleIdx = sliderDataViewerPos.Value;
            updateDataViewer();

            if (en)
                dataViewerTickTimer.Start();

        }

        private void dataViewerTickTimer_Tick(object sender, EventArgs e)
        {
            m_sampleIdx++;
            sliderDataViewerPos.Value = m_sampleIdx;

            updateDataViewer();
        }

        #endregion
        /***********************************************************************************************/

        /***********************************************************************************************/
        #region "Overlay Methods"

        private void chkBxEnableOverlayPreview_CheckedChanged(object sender, EventArgs e)
        {
            if (m_videoPlayer != null)
            {
                if (m_curActiveDashboard != null)
                {
                    m_dashPreviewSettings.dashboard = m_curActiveDashboard;
                    m_dashPreviewSettings.Position.X = sliderOverlayPosX.Value;
                    m_dashPreviewSettings.Position.Y = sliderOverlayPosY.Value;
                    m_dashPreviewSettings.transparency = sliderTransparency.Value;
                    m_dashPreviewSettings.sizeScale = sliderOverlayScale.Value;

                    m_dashPreviewSettings.isEnabled = chkBxEnableOverlayPreview.Checked;

                    m_videoPlayer.EnableOverlayPreview(chkBxEnableOverlayPreview.Checked, m_dashPreviewSettings);
                }
            }
        }

        private void sliderOverlayPosX_Scroll(object sender, EventArgs e)
        {
            if (m_curActiveDashboard != null)
            {
                m_dashPreviewSettings.Position.X = sliderOverlayPosX.Value;
                ProjectInfo pi = m_project.curProjectInfo;
                pi.outVideoData.overlayCfg.posX = sliderOverlayPosX.Value;
                m_project.curProjectInfo = pi;

                lblPosX.Text = sliderOverlayPosX.Value.ToString();

                if (chkBxEnableOverlayPreview.Checked)
                {
                    m_videoPlayer.UpdateOverlayPreview(m_dashPreviewSettings);
                }
            }
        }

        private void sliderOverlayPosY_Scroll(object sender, EventArgs e)
        {
            if (m_curActiveDashboard != null)
            {
                m_dashPreviewSettings.Position.Y = sliderOverlayPosY.Value;
                ProjectInfo pi = m_project.curProjectInfo;
                pi.outVideoData.overlayCfg.posY = sliderOverlayPosY.Value;
                m_project.curProjectInfo = pi;

                lblPosY.Text = sliderOverlayPosY.Value.ToString();

                if (chkBxEnableOverlayPreview.Checked)
                {
                    m_videoPlayer.UpdateOverlayPreview(m_dashPreviewSettings);
                }
            }
        }

        private void sliderOverlayScale_Scroll(object sender, EventArgs e)
        {
            if (m_curActiveDashboard != null)
            {
                m_dashPreviewSettings.sizeScale = sliderOverlayScale.Value;

                int max = 0;

                sliderOverlayPosX.Minimum = 0;
                float w = ((float)m_curActiveDashboard.Width * ((float)(sliderOverlayScale.Value / 100.0)));
                max = m_videoPlayer.VideoInfo.width - ((int)w);
                sliderOverlayPosX.Maximum = (int)max;
                if (sliderOverlayPosX.Value > ((int)max))
                    sliderOverlayPosX.Value = (int)max;

                sliderOverlayPosY.Minimum = 0;
                float h = ((float)m_curActiveDashboard.Height * ((float)(sliderOverlayScale.Value / 100.0)));
                max = m_videoPlayer.VideoInfo.height - ((int)h);
                sliderOverlayPosY.Maximum = (int)max;
                if (sliderOverlayPosY.Value > ((int)max))
                    sliderOverlayPosY.Value = (int)max;

                m_dashPreviewSettings.Position.X = sliderOverlayPosX.Value;
                m_dashPreviewSettings.Position.Y = sliderOverlayPosY.Value;

                ProjectInfo pi = m_project.curProjectInfo;
                pi.outVideoData.overlayCfg.scale = sliderOverlayScale.Value;
                m_project.curProjectInfo = pi;

                if (chkBxEnableOverlayPreview.Checked)
                {
                    m_videoPlayer.UpdateOverlayPreview(m_dashPreviewSettings);
                }
            }
        }

        private void sliderTransparency_Scroll(object sender, EventArgs e)
        {
            if (m_curActiveDashboard != null)
            {
                ProjectInfo pi = m_project.curProjectInfo;
                pi.outVideoData.overlayCfg.transp = sliderTransparency.Value;
                m_project.curProjectInfo = pi;

                m_dashPreviewSettings.transparency = sliderTransparency.Value;

                if (chkBxEnableOverlayPreview.Checked)
                {
                    m_videoPlayer.UpdateOverlayPreview(m_dashPreviewSettings);
                }
            }
        }

        private void resetOverlaySettings_Click(object sender, EventArgs e)
        {
            sliderOverlayPosX.Value = 0;
            sliderOverlayPosY.Value = 0;

            lblPosX.Text = sliderOverlayPosX.Value.ToString();
            lblPosY.Text = sliderOverlayPosY.Value.ToString();

            sliderTransparency.Value = sliderTransparency.Maximum;
            sliderOverlayScale.Value = 100;

            m_dashPreviewSettings.Position.X = sliderOverlayPosX.Value;
            m_dashPreviewSettings.Position.Y = sliderOverlayPosY.Value;
            m_dashPreviewSettings.transparency = sliderTransparency.Value;
            m_dashPreviewSettings.sizeScale = sliderOverlayScale.Value;

            if (chkBxEnableOverlayPreview.Checked)
            {
                m_videoPlayer.UpdateOverlayPreview(m_dashPreviewSettings);
            }
        }

        #endregion
        /***********************************************************************************************/

        /***********************************************************************************************/
        #region "public properties"

        public CDataLog Logger
        {
            get
            {
                return m_dataLog;
            }
        }
        #endregion
        /***********************************************************************************************/


        /***********************************************************************************************/
        #region "Event Handler Methods"

        void DshowNotify(object sender, DShowEventArgs e)
        {
            long code = e.EventCode;

            if (e.FilterGraphId.ToInt32() == m_vidEncoder.GraphId_Renderer)
            {
                switch (code)
                {
                    case 0x01:
                        m_vidEncoder.Cancel();
                        CancelCleanup();
                        break;

                    case 0x02:
                        m_vidEncoder.Cancel();
                        CancelCleanup();
                        break;

                    case 0x03:
                        m_vidEncoder.Cancel();
                        MessageBox.Show(String.Format("EC_ERRORABORT in WMV encoder graph {0}", e.Param1), "Error");
                        CancelCleanup();
                        break;

                    case 0x251:
                        // EC_WMT_EVENT_BASE - message is sent by WMV-encoder if user cancels the encoding process
                        CancelCleanup();
                        break;

                    default:
                        break;
                }
            }

            e = null;
        }

        private void OnVideoLatchStateChanged(LatchEventArgs e)
        {
            if (e.state)
            {
                sliderDataViewerPos.Enabled = false;
                btDataViewerPause.Enabled = false;
                btDataViewerPlay.Enabled = false;
                btGotoSampleStartMarker.Enabled = false;
                btGotoSampleEndMarker.Enabled = false;
                btSetDataStartPos.Enabled = false;
                btSetDataEndPos.Enabled = false;

                btPrevLap.Enabled = false;
                btNextLap.Enabled = false;

                m_isVideoLatched = true;
                //...

                m_refSampleIdx = m_sampleIdx;
                m_refVidFrame = (int)m_videoPlayer.CurrentVideoPos;

            }
            else
            {
                sliderDataViewerPos.Enabled = true;
                btDataViewerPause.Enabled = true;
                btDataViewerPlay.Enabled = true;
                btGotoSampleStartMarker.Enabled = true;
                btGotoSampleEndMarker.Enabled = true;

                btNextLap.Enabled = true;
                btPrevLap.Enabled = true;

                m_isVideoLatched = false;

            }
        }
        private void OnVideoPlay(VideoStartPlaybackArgs e)
        {
            m_playStartFrame = (int)m_videoPlayer.CurrentVideoPos;

            timer1.Start();
        }
        private void OnVideoPause(EventArgs e)
        {
            timer1.Stop();
        }
        private void OnVideoUpdate(VideoUpdateEventArgs e)
        {
            int curVidPos = (int)m_videoPlayer.CurrentVideoPos;

            int frameInc = curVidPos - m_refVidFrame;
            float newSamplePos = 0.0f;

            if (m_isVideoLatched)
            {
                if (m_videoPlayer != null && m_dataLog != null)
                {
                    //*********************
                    // v0.8.2 - test code
                    //*********************

                    if (m_refSampleIdx == 0)
                        m_refSampleIdx = m_sampleIdx;

                    ProjectInfo pi = m_project.curProjectInfo;

                    float n = (float)((double)frameInc * (((double)1.0f / (double)pi.inVideoData.framerate) / ((double)1.0f / (double)m_dataLog.TickRate)));
                    newSamplePos = n;
                    m_sampleIdx = m_refSampleIdx + (int)Math.Round((double)newSamplePos);

                    sliderDataViewerPos.Value = m_sampleIdx;
                    updateDataViewer();
                }
            }
        }
        private void OnSetVidStartFrame(VideoStartFrameArgs e)
        {
            if (m_dataLogFileOpen)
            {
                ProjectInfo pi = m_project.curProjectInfo;
                lblSyncStartMarkerFrame.Text = e.StartFrame.ToString();
                if (m_isVideoLatched)
                {
                    lblSyncStartMarkerSample.Text = m_sampleIdx.ToString();
                    pi.outVideoData.syncData.startSample = m_sampleIdx;

                    int d = pi.outVideoData.syncData.endSample - pi.outVideoData.syncData.startSample + 1;
                    lblOutVidDataLength.Text = d.ToString();
                }
                pi.outVideoData.syncData.startFrame = (int)e.StartFrame;

                int f = pi.outVideoData.syncData.endFrame - pi.outVideoData.syncData.startFrame + 1;
                lblOutVidLength.Text = f.ToString();

                m_project.curProjectInfo = pi;
            }
            else
            {
            } return;
        }
        private void OnSetVidEndFrame(VideoEndFrameArgs e)
        {
            if (m_dataLogFileOpen)
            {
                ProjectInfo pi = m_project.curProjectInfo;
                lblSyncEndMarkerFrame.Text = e.EndFrame.ToString();
                if (m_isVideoLatched)
                {
                    lblSyncEndMarkerSample.Text = m_sampleIdx.ToString();
                    pi.outVideoData.syncData.endSample = m_sampleIdx;

                    int d = pi.outVideoData.syncData.endSample - pi.outVideoData.syncData.startSample + 1;
                    lblOutVidDataLength.Text = d.ToString();
                }
                pi.outVideoData.syncData.endFrame = (int)e.EndFrame;

                int f = pi.outVideoData.syncData.endFrame - pi.outVideoData.syncData.startFrame + 1;
                lblOutVidLength.Text = f.ToString();

                m_project.curProjectInfo = pi;
            }
            else
            {
                return;
            }
        }

        #endregion
        /***********************************************************************************************/
        protected override void WndProc(ref Message m)
        {
            if (m_vidEncoder != null)
            {
                if (m.Msg == m_vidEncoder.DsNotifyWindowsMessageId_Renderer)
                {
                    IntPtr param1 = (IntPtr)0;
                    IntPtr param2 = (IntPtr)0;

                    try
                    {
                        long code = m_vidEncoder.DShowEventCode;
                        IntPtr graphId = (IntPtr)m.LParam;

                        DShowEventArgs e = new DShowEventArgs(code, param1, param2, graphId);
                        m_dshowEvent(this, e);
                    }
                    catch (Exception e)
                    {
                        string msg = string.Format("Error in:\"{0}\"\n", e.TargetSite);
                        ErrorBox(msg);

                        log.Error(msg, e);
                    }
                    return;
                }
            }
            try
            {
                base.WndProc(ref m);
            }
            catch (Exception e)
            {
                string msg = string.Format("Error in:\"{0}\"\n", e.TargetSite);
                ErrorBox(msg);

                log.Error(msg, e);
                return;
            }
        }
        /***********************************************************************************************/
        #region "Private Class Methods"

        private void ErrorBox(string text)
        {
            MessageBox.Show(text, "Error in VideoDashboard", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void loadDataLog()
        {
            if (m_dataLog != null && m_dataLogFileOpen)
            {
                closeDatalog();
            }

            if (string.IsNullOrEmpty(m_project.curProjectInfo.dataFile))
                return;

            try
            {
                ProjectInfo pi = m_project.curProjectInfo;
                this.tbDataLogFile.Text = pi.dataFile;

                m_dataLog = new CDataLog(pi.dataFile);

                pi.DataLoggerObj = m_dataLog.getDataLoggerObjPtr();

                btGotoSampleEndMarker.Enabled = true;
                btGotoSampleStartMarker.Enabled = true;
                btDataViewerPause.Enabled = true;
                btDataViewerPlay.Enabled = true;
                btDetailedSessionInfo.Enabled = true;
                btLapTimingDetail.Enabled = true;
                btNextLap.Enabled = true;
                btNextLap.Enabled = true;

                groupBox3.Enabled = true;
                groupBox6.Enabled = true;
                groupBox11.Enabled = true;

                m_lapsArray = m_dataLog.LapsArray;
                sliderDataViewerPos.Minimum = 0;
                sliderDataViewerPos.Maximum = m_dataLog.SampleCount - 1;
                sliderDataViewerPos.Value = 0;

                m_sampleIdx = sliderDataViewerPos.Value;

                LapInfo li = m_dataLog.GetFastestLap();
                lblFastestLap.Text = li.lapNo.ToString();
                lblFastestLapTime.Text = formatLapTimeString(li.lapTime);
                lblTotalLaps.Text = string.Format("{0}", m_dataLog.SessionLapCount);

                m_dataLogFileOpen = true;

                //if (m_videoFileOpen)
                //    pi.outVideoData.syncData.dataSampleInc = (int)(m_dataLog.TickRate / m_videoPlayer.VideoInfo.framerate);

                dataViewerTickTimer.Interval = (int)Math.Round(((float)1.0f / (float)m_dataLog.TickRate) * 1000);

                m_project.curProjectInfo = pi;
                updateDataViewer();

                if (m_dashLayoutParser != null)
                {
                    m_dashLayoutParser.SetDataLogger(pi.DataLoggerObj);
                }

                int r = m_dataLog.ShiftRPM;
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void btOpenDataLogFile_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Filter = "iRacing Binary Telemetry files (*.ibt)|*.ibt"; //|CSV file (*.csv)|*.csv";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    ProjectInfo pi = this.m_project.curProjectInfo;
                    pi.dataFile = ofd.FileName;
                    m_project.curProjectInfo = pi;

                    loadDataLog();
                }
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void closeDatalog()
        {
            if (m_dataLog != null)
            {
                m_dataLog.Dispose();
                m_dataLog = null;

                m_dataLogFileOpen = false;
            }
        }

        private void loadVideoFile()
        {
            try
            {
                ProjectInfo pi = m_project.curProjectInfo;

                if (!File.Exists(pi.inVideoData.fileName))
                    throw new FileNotFoundException(string.Format("File {0} could not be found!", pi.inVideoData.fileName));

                tbVideoFileName.Text = pi.inVideoData.fileName;
                m_videoPlayer.OpenVideoFile(pi.inVideoData.fileName);
                pi.inVideoData = m_videoPlayer.VideoInfo;

                m_videoFileOpen = true;
                groupBox7.Enabled = true;
                groupBox4.Enabled = true;

                m_project.curProjectInfo = pi;
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void btOpenVideoFile_Click(object sender, EventArgs e)
        {
            try
            {
                ProjectInfo pi = m_project.curProjectInfo;

                //    frmSrcVideoEdit frm = new frmSrcVideoEdit();
                //    frm.ShowDialog();

                //    if (frm.DialogResult == DialogResult.OK)
                //    {
                //        m_srcVideoFiles = new List<string>(frm.SrcFileList);
                //        if (m_srcVideoFiles.Count == 1)
                //        {
                //            pi.inVideoData.fileName = m_srcVideoFiles[0];

                //            m_project.curProjectInfo = pi;

                //            loadVideoFile();
                //        }

                //        // else go the new SrcVideoEditor class path
                //    }

                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Filter = "Video Files (*.avi)|*.avi";
                ofd.FilterIndex = 0;
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    pi.inVideoData.fileName = ofd.FileName;

                    m_project.curProjectInfo = pi;

                    loadVideoFile();
                }
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void btPrevLap_Click(object sender, EventArgs e)
        {
            if (m_curLapIdx == 1)
                return;
            else
            {
                if (m_lapsArray[m_curLapIdx - 1].sampleIdx != m_sampleIdx)
                {
                    m_sampleIdx = m_lapsArray[m_curLapIdx - 1].sampleIdx;
                    sliderDataViewerPos.Value = m_sampleIdx;
                    updateDataViewer();
                    return;
                }
                else
                {
                    m_curLapIdx--;

                    m_sampleIdx = m_lapsArray[m_curLapIdx - 1].sampleIdx;
                    sliderDataViewerPos.Value = m_sampleIdx;
                    updateDataViewer();
                    return;
                }
            }
        }

        private void btNextLap_Click(object sender, EventArgs e)
        {
            if (m_curLapIdx == m_dataLog.SessionLapCount)
                return;
            else
            {
                m_curLapIdx++;

                m_sampleIdx = m_lapsArray[m_curLapIdx - 1].sampleIdx;
                sliderDataViewerPos.Value = m_sampleIdx;
                updateDataViewer();
            }
        }

        private void btDataViewerPlay_Click(object sender, EventArgs e)
        {
            dataViewerTickTimer.Start();
        }

        private void btDataViewerPause_Click(object sender, EventArgs e)
        {
            dataViewerTickTimer.Stop();
        }

        private void frmProject_Load(object sender, EventArgs e)
        {
            Text = string.Format("{0}", m_project.ProjectName);
            m_dashboardMap = new Dictionary<string, string>();

            grpBoxProgress.Visible = false;

            groupBox3.Enabled = false;
            groupBox6.Enabled = false;
            groupBox7.Enabled = false;
            groupBox4.Enabled = false;
            groupBox11.Enabled = false;

            lblSpeed.Text = "";
            lblRpm.Text = "";
            lblTotalLaps.Text = "";
            lblCurLap.Text = "";
            lblCurLapTime.Text = "";
            lblFastestLap.Text = "";
            lblFastestLapTime.Text = "";
            m_isVideoLatched = false;

            btGotoSampleEndMarker.Enabled = false;
            btGotoSampleStartMarker.Enabled = false;
            btDataViewerPause.Enabled = false;
            btDataViewerPlay.Enabled = false;
            btDetailedSessionInfo.Enabled = false;
            btLapTimingDetail.Enabled = false;
            btNextLap.Enabled = false;
            btNextLap.Enabled = false;

            m_videoPlayer.ParentFormInst = this;
            m_videoPlayer.projectInfo = m_project.curProjectInfo;

            m_videoPlayer.m_latchEvent += new LatchDataEventHandler(OnVideoLatchStateChanged);
            m_videoPlayer.m_playEvent += new PlayEventHandler(OnVideoPlay);
            m_videoPlayer.m_pauseEvent += new PauseEventHandler(OnVideoPause);
            m_videoPlayer.m_videoUpdateEvent += new VideoUpdateEventHandler(OnVideoUpdate);
            m_videoPlayer.m_setVidStartFrame += new SetVideoStartFrameEventHandler(OnSetVidStartFrame);
            m_videoPlayer.m_setVidEndFrame += new SetVideoEndFrameEventHandler(OnSetVidEndFrame);
            m_videoPlayer.m_gotoVidStartMarker += new GotoVideoStartMarkerEventHandler(OnGotoStartSampleMarker);
            m_videoPlayer.m_gotoVidEndMarker += new GotoVideoEndMarkerEventHandler(OnGotoEndSampleMarker);

            m_dashPreviewSettings = new DashboardOverlayPreviewSettings();

            m_videoFileOpen = false;
            m_dataLogFileOpen = false;

            lblPosX.Text = sliderOverlayPosX.Value.ToString();
            lblPosY.Text = sliderOverlayPosY.Value.ToString();

            //m_updateTimer = new UpdateTimer(new VideoUpdateTick(), new DataUpdateTick());
            try
            {
                // try to enumerate all available dashboards in all paths
                foreach (string path in m_project.curProjectInfo.appSet.dashPaths)
                {
                    try
                    {
                        DirectoryInfo di = new DirectoryInfo(path);
                        FileInfo[] files = di.GetFiles("*.layout");
                        m_dashLayoutParser = new DashboardLayout();

                        for (int i = 0; i < files.Length; i++)
                        {
                            int pos = files[i].Name.IndexOf('.');
                            if (pos != -1)
                            {
                                try
                                {
                                    string fname = files[i].Name.Remove(pos);
                                    m_dashLayoutParser.ParseLayoutFile(path + "\\" + files[i].Name);
                                    string s = m_dashLayoutParser.GetDashboardShortName(path + "\\" + files[i].Name);
                                    cbOverlayList.Items.Add(s);
                                    m_dashboardMap.Add(s, path + "\\" + files[i].Name);
                                }
                                catch (Exception ex)
                                {
                                    ErrorBox(ex.Message);
                                    log.Error(ex.Message, ex);
                                }
                            }
                        }
                        log.Info(string.Format("* added {0} overlays from path {1}", files.Length, path));
                    }
                    catch (DirectoryNotFoundException)
                    {
                        continue;
                    }
                }
            }
            catch (Exception ex)
            {
                string msg;
                msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);

                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void cbOverlayList_SelectedIndexChanged(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;

            int idx = cbOverlayList.SelectedIndex;
            int max = 0;

            try
            {
                if (m_videoPlayer != null)
                {
                    string file = m_dashboardMap[(string)cbOverlayList.SelectedItem]; //pi.appSet.dashPath + "\\" + cbOverlayList.SelectedItem + ".layout";
                    pi.dashboardFileName = file;

                    m_dashLayoutParser.ParseLayoutFile(file);

                    if (m_curActiveDashboard != null)
                    {
                        m_curActiveDashboard.Dispose();
                        m_curActiveDashboard = null;
                    }

                    Bitmap dash = null;
                    if (m_dataLogFileOpen)
                        dash = m_dashLayoutParser.RenderDashboard("", sliderDataViewerPos.Value);
                    else
                        dash = m_dashLayoutParser.RenderDashboard("", 1); // GetDashboardImage(m_project.curProjectInfo.appSet.imgPath);

                    if (dash != null)
                    {
                        m_curActiveDashboard = dash;

                        sliderOverlayPosX.Minimum = 0;
                        float w = ((float)dash.Width * ((float)(sliderOverlayScale.Value / 100.0)));
                        max = m_videoPlayer.VideoInfo.width - ((int)w);
                        sliderOverlayPosX.Maximum = max;
                        if (sliderOverlayPosX.Value > max)
                            sliderOverlayPosX.Value = max;

                        sliderOverlayPosY.Minimum = 0;
                        float h = ((float)dash.Height * ((float)(sliderOverlayScale.Value / 100.0)));
                        max = m_videoPlayer.VideoInfo.height - ((int)h);
                        sliderOverlayPosY.Maximum = max;
                        if (sliderOverlayPosY.Value > max)
                            sliderOverlayPosY.Value = max;

                        //m_dashPreviewSettings.dashObj = m_curSelOverlay;
                        if (m_dashPreviewSettings.dashboard != null)
                        {
                            m_dashPreviewSettings.dashboard.Dispose();
                            m_dashPreviewSettings.dashboard = null;
                        }

                        m_dashPreviewSettings.dashboard = m_curActiveDashboard;
                        m_dashPreviewSettings.Position.X = sliderOverlayPosX.Value;
                        m_dashPreviewSettings.Position.Y = sliderOverlayPosY.Value;
                        m_dashPreviewSettings.Size.Width = dash.Width;
                        m_dashPreviewSettings.Size.Height = dash.Height;
                        m_dashPreviewSettings.transparency = sliderTransparency.Value;
                        m_dashPreviewSettings.sizeScale = sliderOverlayScale.Value;

                        pi.outVideoData.overlayCfg.name = m_dashboardMap[(string)cbOverlayList.SelectedItem];
                        pi.outVideoData.overlayCfg.posX = sliderOverlayPosX.Value;
                        pi.outVideoData.overlayCfg.posY = sliderOverlayPosY.Value;
                        pi.outVideoData.overlayCfg.scale = sliderOverlayScale.Value;
                        pi.outVideoData.overlayCfg.transp = sliderTransparency.Value;

                        m_project.curProjectInfo = pi;
                    }
                    m_videoPlayer.UpdateOverlayPreview(m_dashPreviewSettings);
                }
            }
            catch (Exception ex)
            {
                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                ErrorBox(msg);

                log.Error(msg, ex);
                return;
            }
        }

        private void frmProject_Shown(object sender, EventArgs e)
        {
            //LoadProject();
            lblProjectName.Text = m_project.curProjectInfo.projectName;
            lblProjectNotes.Text = m_project.curProjectInfo.projectDescription;

            m_frmRender = new frmRenderSettings(ref m_project);
        }

        private void btSetDataStartPos_Click(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            lblSyncStartMarkerSample.Text = m_sampleIdx.ToString();
            pi.outVideoData.syncData.startSample = m_sampleIdx;

            //if ((pi.outVideoData.syncData.endSample = !0) && (pi.outVideoData.syncData.startSample != 0))
            //{
            int d = pi.outVideoData.syncData.endSample - pi.outVideoData.syncData.startSample + 1;
            lblOutVidDataLength.Text = d.ToString();
            //}
            m_project.curProjectInfo = pi;
        }

        private void btSetDataEndPos_Click(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            lblSyncEndMarkerSample.Text = m_sampleIdx.ToString();
            pi.outVideoData.syncData.endSample = m_sampleIdx;

            //if ((pi.outVideoData.syncData.endSample = !0) && (pi.outVideoData.syncData.startSample != 0))
            //{
            int d = pi.outVideoData.syncData.endSample - pi.outVideoData.syncData.startSample + 1;
            lblOutVidDataLength.Text = d.ToString();
            //}

            m_project.curProjectInfo = pi;
        }

        private void toolStripBtEditProjectNotes_Click(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;

            frmNewProject f = new frmNewProject(pi.projectName, pi.projectDescription);
            if (f.ShowDialog() == DialogResult.OK)
            {

                if (f.m_projectName != "")
                {
                    pi.projectName = f.m_projectName;
                    lblProjectName.Text = f.m_projectName;
                }
                if (f.m_projectNotes != "")
                    pi.projectDescription = f.m_projectNotes;
            }
            m_project.curProjectInfo = pi;

            f.Dispose();
            f = null;
        }

        public void frmProject_FormClosed(object sender, FormClosedEventArgs e)
        {
        }

        private void toolStripBtEditProjectSettings_Click(object sender, EventArgs e)
        {
            frmProjectSettings f = new frmProjectSettings(m_project);

            f.ShowDialog();

            if (f.DialogResult == DialogResult.OK)
            {
                updateDataViewer();
            }

            f.Dispose();
            f = null;
        }

        private void btGotoSampleStartMarker_Click(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            m_sampleIdx = pi.outVideoData.syncData.startSample;
            if (m_sampleIdx == 0)
                return;

            sliderDataViewerPos.Value = m_sampleIdx;
            updateDataViewer();
        }

        private void btGotoSampleEndMarker_Click(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;
            m_sampleIdx = pi.outVideoData.syncData.endSample;
            if (m_sampleIdx == 0)
                return;

            sliderDataViewerPos.Value = m_sampleIdx;
            updateDataViewer();
        }

        private void OnGotoStartSampleMarker()
        {
            if (m_isVideoLatched)
            {
                ProjectInfo pi = m_project.curProjectInfo;
                m_sampleIdx = pi.outVideoData.syncData.startSample;
                sliderDataViewerPos.Value = m_sampleIdx;
                updateDataViewer();

                m_refVidFrame = (int)m_videoPlayer.CurrentVideoPos;
                m_refSampleIdx = m_sampleIdx;
            }
        }

        private void OnGotoEndSampleMarker()
        {
            if (m_isVideoLatched)
            {
                ProjectInfo pi = m_project.curProjectInfo;
                m_sampleIdx = pi.outVideoData.syncData.endSample;
                sliderDataViewerPos.Value = m_sampleIdx;
                updateDataViewer();

                m_refVidFrame = (int)m_videoPlayer.CurrentVideoPos;
                m_refSampleIdx = m_sampleIdx;
            }
        }

        private void toolStripBtRenderQuickPreview_Click(object sender, EventArgs e)
        {
            CWM9_ProfileManager wmvProfiler;

            if (m_vidEncoder != null)
            {
                m_vidEncoder.Dispose();
                m_vidEncoder = null;
            }

            if (m_videoFileOpen)
            {
                if (m_dataLogFileOpen)
                {
                    if (cbOverlayList.SelectedIndex >= 0 && m_dashLayoutParser != null)
                    {
                        m_frmRender.ShowDialog();
                        ProjectInfo pi = m_project.curProjectInfo;

                        if (m_frmRender.DialogResult == DialogResult.OK)
                        {
                            chkBxEnableOverlayPreview.Checked = false;
                            VideoEncoderConfig encCfg = new VideoEncoderConfig();
                            try
                            {
                                switch (pi.renderCfg.encoder)
                                {
                                    case VideoEncoderType.VideoEncoder_WMV:

                                        wmvProfiler = m_frmRender.WMV_ProfilerObj;
                                        encCfg.wm9_profilerObj = wmvProfiler.GetHandle();
                                        break;

                                    case VideoEncoderType.VideoEncoder_RAW:

                                        encCfg.videoCompressor = (IntPtr)m_frmRender.CompressionFilter;
                                        break;

                                    default:
                                        break;
                                }

                                encCfg.appPath = pi.appSet.appPath;
                                encCfg.srcVideoFileName = pi.inVideoData.fileName;
                                encCfg.outVideoFileName = pi.outVideoData.fileName;
                                encCfg.videoWidth = pi.outVideoData.width;
                                encCfg.videoHeight = pi.outVideoData.height;
                                encCfg.aspectRatio = (float)((float)pi.outVideoData.width / (float)pi.outVideoData.height);
                                encCfg.overlayCfg_posX = pi.outVideoData.overlayCfg.posX;
                                encCfg.overlayCfg_posY = pi.outVideoData.overlayCfg.posY;
                                encCfg.overlayCfg_transp = (float)((float)pi.outVideoData.overlayCfg.transp / 100.0f);
                                encCfg.overlayCfg_scale = (float)((float)pi.outVideoData.overlayCfg.scale / 100.0f);

                                encCfg.dashboardlayoutObj = m_dashLayoutParser.GetInstancePtr();
                                encCfg.dataLogTickRate = m_dataLog.TickRate;

                                encCfg.syncCfg_startFrame = pi.outVideoData.syncData.startFrame;
                                encCfg.syncCfg_startSample = pi.outVideoData.syncData.startSample;

                                /*****************************************************************************************************/
                                // limit to 30 seconds only if enough frames are selected. if start/end-markers are lower use
                                // these as original!

                                // video duration to render
                                int dur = pi.outVideoData.syncData.endFrame - pi.outVideoData.syncData.startFrame;
                                // preview duration
                                int pv_dur = pi.inVideoData.framerate * 30;    // 30 seconds

                                if (dur > pv_dur)
                                {
                                    pi.outVideoData.syncData.endFrame = pi.outVideoData.syncData.startFrame + pv_dur;
                                }
                                /****************************************************************************************************/

                                encCfg.syncCfg_endFrame = pi.outVideoData.syncData.endFrame;
                                encCfg.syncCfg_endSample = pi.outVideoData.syncData.endSample;

                                encCfg.parentWnd = Handle;
                                encCfg.preview = true;

                                encCfg.encoderType = (int)pi.renderCfg.encoder;

                                m_vidEncoder = new CVideoEncoder();
                                m_vidEncoder.ProgressUpdate += new ProgressUpdateHandler(OnEncodingProgressChanged);
                                m_vidEncoder.SetConfig(encCfg);

                                grpBoxProgress.Visible = true;
                                progressBar1.Visible = true;

                                int x = (pi.outVideoData.syncData.endFrame - pi.outVideoData.syncData.startFrame) + 1;
                                progressBar1.Minimum = 0;
                                progressBar1.Maximum = x;

                                groupBox1.Enabled = false;
                                groupBox2.Enabled = false;
                                groupBox6.Enabled = false;
                                groupBox11.Enabled = false;
                                groupBox7.Enabled = false;
                                groupBox4.Enabled = false;

                                m_videoPlayer.DisablePlayerControls();

                                m_vidEncoder.Start();
                            }
                            catch (Exception ex)
                            {
                                if (ex.InnerException != null)
                                {
                                    log.Error("unmanaged exception", ex.InnerException);
                                }
                                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                                ErrorBox(msg);

                                log.Error(msg, ex);

                                grpBoxProgress.Visible = false;

                                groupBox1.Enabled = true;
                                groupBox2.Enabled = true;
                                groupBox6.Enabled = true;
                                groupBox11.Enabled = true;
                                groupBox7.Enabled = true;
                                groupBox4.Enabled = true;

                                m_videoPlayer.EnablePlayerControls();

                                ex = null;

                                return;
                            }
                            m_encoderStartTime = DateTime.Now.Ticks;
                        }
                    }
                    else
                    {
                        MessageBox.Show("No overlay was selected or an internal error occured. Please select an overlay from the list first.");
                        return;
                    }
                }
                else
                {
                    MessageBox.Show("No Data Log file is open. Please open one before you continue");
                    return;
                }
            }
            else
            {
                MessageBox.Show("No Video File is currently open. Please open one before you continue!");
                return;
            }

            wmvProfiler = null;
        }

        private void OnEncodingProgressChanged(ProgressEventArgs e)
        {
            if (this.InvokeRequired)
            {
                object[] arg = new object[] { e };
                this.BeginInvoke(new ProgressUpdateHandler(OnProgressUpdateUI), arg);
            }
        }

        void OnProgressUpdateUI(ProgressEventArgs e)
        {
            float pct = 0;
            int max = progressBar1.Maximum;
            int x = e.CurrentProgress;

            pct = (float)((float)x / (float)max);
            pct = (float)(pct * 100.0f);

            //toolStripLabel1.Text = String.Format("{0}% of {1} Frames", (int)pct, max);
            lblProgressFrames.Text = string.Format("{0} / {1}", x, max);
            lblProgressPercent.Text = string.Format("{0}%", (int)pct);

            m_encoderElapsedTime = DateTime.Now.Ticks;
            TimeSpan ts = new TimeSpan(m_encoderElapsedTime - m_encoderStartTime);
            lblElapsedTime.Text = String.Format("{0:d2}:{1:d2}:{2:d2}", ts.Hours, ts.Minutes, ts.Seconds);

            // calculate estimated time; rudimentary
            if ((x % 4) == 0)
            {
                float f = (float)((float)ts.TotalSeconds / (float)x);
                double estTime = (double)(f * (float)max);
                TimeSpan est = TimeSpan.FromSeconds(estTime);
                lblEstimatedTime.Text = String.Format("{0:d2}:{1:d2}:{2:d2}", est.Hours, est.Minutes, est.Seconds);
            }

            progressBar1.Value = x;
        }

        private void btCancelRenderer_Click(object sender, EventArgs e)
        {
            m_vidEncoder.Cancel();
            Thread.Sleep(650);

            CancelCleanup();
        }

        private void CancelCleanup()
        {
            ProjectInfo pi = m_project.curProjectInfo;

            if (m_vidEncoder != null)
            {
                m_vidEncoder.Dispose();
                m_vidEncoder = null;
            }

            pi.DataLoggerObj = (IntPtr)0;
            grpBoxProgress.Visible = false;

            progressBar1.Minimum = 0;
            progressBar1.Maximum = 100;
            progressBar1.Value = 0;

            groupBox1.Enabled = true;
            groupBox2.Enabled = true;
            groupBox6.Enabled = true;
            groupBox11.Enabled = true;
            groupBox7.Enabled = true;
            groupBox4.Enabled = true;

            m_videoPlayer.EnablePlayerControls();

            GC.Collect();
        }

        private void toolStripBtRenderToFile_Click(object sender, EventArgs e)
        {
            CWM9_ProfileManager wmvProfiler;

            if (m_vidEncoder != null)
            {
                m_vidEncoder.Dispose();
                m_vidEncoder = null;
            }

            if (m_videoFileOpen)
            {
                if (m_dataLogFileOpen)
                {
                    if (cbOverlayList.SelectedIndex >= 0 && m_curActiveDashboard != null)
                    {
                        m_frmRender.ShowDialog();
                        ProjectInfo pi = m_project.curProjectInfo;

                        if (m_frmRender.DialogResult == DialogResult.OK)
                        {
                            chkBxEnableOverlayPreview.Checked = false;
                            VideoEncoderConfig encCfg = new VideoEncoderConfig();

                            try
                            {
                                switch (pi.renderCfg.encoder)
                                {
                                    case VideoEncoderType.VideoEncoder_WMV:

                                        wmvProfiler = m_frmRender.WMV_ProfilerObj;
                                        encCfg.wm9_profilerObj = wmvProfiler.GetHandle();
                                        break;

                                    case VideoEncoderType.VideoEncoder_RAW:

                                        encCfg.videoCompressor = (IntPtr)m_frmRender.CompressionFilter;
                                        break;

                                    default:
                                        break;
                                }

                                encCfg.appPath = pi.appSet.appPath;                               
                                encCfg.srcVideoFileName = pi.inVideoData.fileName;
                                encCfg.outVideoFileName = pi.outVideoData.fileName;
                                encCfg.videoWidth = pi.outVideoData.width;
                                encCfg.videoHeight = pi.outVideoData.height;
                                encCfg.aspectRatio = (float)((float)pi.outVideoData.width / (float)pi.outVideoData.height);
                                encCfg.overlayCfg_posX = pi.outVideoData.overlayCfg.posX;
                                encCfg.overlayCfg_posY = pi.outVideoData.overlayCfg.posY;
                                encCfg.overlayCfg_transp = (float)((float)pi.outVideoData.overlayCfg.transp / 100.0f);
                                encCfg.overlayCfg_scale = (float)((float)pi.outVideoData.overlayCfg.scale / 100.0f);

                                encCfg.dashboardlayoutObj = m_dashLayoutParser.GetInstancePtr();
                                encCfg.dataLogTickRate = m_dataLog.TickRate;
                                encCfg.syncCfg_startFrame = pi.outVideoData.syncData.startFrame;
                                encCfg.syncCfg_startSample = pi.outVideoData.syncData.startSample;
                                encCfg.syncCfg_endFrame = pi.outVideoData.syncData.endFrame;
                                encCfg.syncCfg_endSample = pi.outVideoData.syncData.endSample;

                                encCfg.parentWnd = Handle;
                                encCfg.preview = true;

                                encCfg.encoderType = (int)pi.renderCfg.encoder;

                                m_vidEncoder = new CVideoEncoder();
                                m_vidEncoder.ProgressUpdate += new ProgressUpdateHandler(OnEncodingProgressChanged);
                                m_vidEncoder.SetConfig(encCfg);

                                grpBoxProgress.Visible = true;

                                int x = (pi.outVideoData.syncData.endFrame - pi.outVideoData.syncData.startFrame) + 1;
                                progressBar1.Minimum = 0;
                                progressBar1.Maximum = x;

                                groupBox1.Enabled = false;
                                groupBox2.Enabled = false;
                                groupBox6.Enabled = false;
                                groupBox11.Enabled = false;
                                groupBox7.Enabled = false;
                                groupBox4.Enabled = false;

                                m_videoPlayer.DisablePlayerControls();

                                m_vidEncoder.Start();
                            }
                            catch (Exception ex)
                            {
                                if (ex.InnerException != null)
                                {
                                    log.Error("unmanaged exception", ex.InnerException);
                                }
                                string msg = string.Format("Error in:\"{0}\"\n", ex.TargetSite);
                                ErrorBox(msg);

                                log.Error(msg, ex);

                                grpBoxProgress.Visible = false;

                                groupBox1.Enabled = true;
                                groupBox2.Enabled = true;
                                groupBox6.Enabled = true;
                                groupBox11.Enabled = true;
                                groupBox7.Enabled = true;
                                groupBox4.Enabled = true;

                                m_videoPlayer.EnablePlayerControls();

                                ex = null;
                            }

                            m_encoderStartTime = DateTime.Now.Ticks;
                        }
                    }
                    else
                    {
                        MessageBox.Show("No overlay was selected or an internal error occured. Please select an overlay from the list first");
                        return;
                    }
                }
                else
                {
                    MessageBox.Show("No Data Log file is open. Please open one before you continue");
                    return;
                }
            }
            else
            {
                MessageBox.Show("No Video File is currently open. Please open one before you continue!");
                return;
            }

            wmvProfiler = null;
        }


        private void lblProjectName_DoubleClick(object sender, EventArgs e)
        {
            frmNewProject f = new frmNewProject(m_project.ProjectName, m_project.ProjectDescription);
            if (f.ShowDialog() == DialogResult.OK)
            {
                m_project.ProjectName = f.m_projectName;
                m_project.ProjectDescription = f.m_projectNotes;

                lblProjectName.Text = f.m_projectName;
                lblProjectNotes.Text = f.m_projectNotes;
            }

            f.Dispose();
            f = null;
        }

        private void lblProjectNotes_DoubleClick(object sender, EventArgs e)
        {
            frmNewProject f = new frmNewProject(m_project.ProjectName, m_project.ProjectDescription);
            if (f.ShowDialog() == DialogResult.OK)
            {
                m_project.ProjectName = f.m_projectName;
                m_project.ProjectDescription = f.m_projectNotes;

                lblProjectName.Text = f.m_projectName;
                lblProjectNotes.Text = f.m_projectNotes;
            }

            f.Dispose();
            f = null;
        }

        private void btLapTimingDetail_Click(object sender, EventArgs e)
        {
            frmLapInfo f = new frmLapInfo(m_lapsArray);
            f.ShowDialog();

            f.Dispose();
            f = null;
        }

        #endregion

        private void frmProject_FormClosing(object sender, FormClosingEventArgs e)
        {
            bool keepLogFile = false;

            if (m_project.compareFileHash() == false)
            {
                DialogResult res = MessageBox.Show("Would you like to save your current project before closing?", "Save Project?", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (res == DialogResult.Yes)
                {
                    keepLogFile = true;
                    SaveProject();
                }
                else if (res == DialogResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }
                else
                {
                    // project not saved; so delete logfile
                    keepLogFile = false;
                }
            }

            if (m_dataLog != null)
            {
                m_dataLog.Dispose();
                m_dataLog = null;
            }


            //if (m_wmvProfiler != null)
            //{
            //    m_wmvProfiler.Dispose();
            //    m_wmvProfiler = null;
            //}

            if (m_vidEncoder != null)
            {
                m_vidEncoder.Dispose();
                m_vidEncoder = null;
            }

            //if (m_overlayList != null)
            //{
            //    for (int i = 0; i < m_overlayList.Count; i++)
            //    {
            //        if (m_overlayList[i] != null)
            //        {
            //            m_overlayList[i].Dispose();
            //            m_overlayList[i] = null;
            //        }
            //    }
            //    m_overlayList.Clear();
            //    m_overlayList = null;
            //}

            if (m_videoPlayer != null)
            {
                m_videoPlayer.Dispose();
                m_videoPlayer = null;
            }

            if (m_frmRender != null)
            {
                m_frmRender.Dispose();
                m_frmRender = null;
            }

            //if (m_logStream != null)
            //{
            //    m_logStream.Close();
            //    m_logStream.Dispose();
            //}

            if (!keepLogFile)
            {
                //ProjectInfo pi = m_project.curProjectInfo;
                //string fn = pi.appSet.projectPath + "\\" + pi.projectName + ".log";

                //if (File.Exists(fn))
                //    File.Delete(fn);
            }

            CloseProjectEventArgs args = new CloseProjectEventArgs(m_project.curProjectInfo.projectName, e.CloseReason);

            if (((frmMain)m_closeProjectEvent.Target).InvokeRequired)
            {
                m_closeProjectEvent.BeginInvoke(args, null, null);
            }
            else
            {
                m_closeProjectEvent.Invoke(args);
            }

            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
            //GC.Collect(2);

        }

        private void ONVideoProgress_tick(object sender, EventArgs e)
        {
            int curVidPos = (int)m_videoPlayer.CurrentVideoPos;

            int frameInc = curVidPos - m_refVidFrame;
            float newSamplePos = 0.0f;

            if (m_isVideoLatched)
            {
                if (m_videoPlayer != null && m_dataLog != null)
                {
                    if (m_refSampleIdx == 0)
                        m_refSampleIdx = m_sampleIdx;

                    ProjectInfo pi = m_project.curProjectInfo;

                    float n = (float)((double)frameInc * (((double)1.0f / (double)pi.inVideoData.framerate) / ((double)1.0f / (double)m_dataLog.TickRate)));
                    newSamplePos = n;
                    m_sampleIdx = m_refSampleIdx + (int)Math.Round((double)newSamplePos);

                    sliderDataViewerPos.Value = m_sampleIdx;
                    updateDataViewer();
                }
            }
        }

        /***********************************************************************************************/
    }
}
