using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

//using libds;
//using LayoutDefinitionFormat;
using libds;

namespace VideoDashboard
{
    /*****************************************************************************************************/
    #region "Event Args Structures"

    public struct LatchEventArgs
    {
        public LatchEventArgs(bool initialState, Int32 fr, Int32 frame)
        {
            state = initialState;
            frameRate = fr;
            isVideoOpen = false;

            curVidFrame = frame;
        }

        public bool state;
        public bool isVideoOpen;
        public Int32 frameRate;
        public Int32 curVidFrame;
    };

    public struct VideoStartFrameArgs
    {
        public VideoStartFrameArgs(long startFrame)
        {
            StartFrame = startFrame;
        }

        public long StartFrame;
    };

    public struct VideoEndFrameArgs
    {
        public VideoEndFrameArgs(long endFrame)
        {
            EndFrame = endFrame;
        }

        public long EndFrame;
    };

    public struct VideoUpdateEventArgs
    {
        public VideoUpdateEventArgs(Int32 FrameInc)
        {
            frameInc = FrameInc;
        }

        public Int32 frameInc;
    };

    public struct VideoStartPlaybackArgs
    {
        public VideoStartPlaybackArgs(Int32 startFrame)
        {
            frame =startFrame;
        }

        public Int32 frame;
    }

    #endregion
    /*****************************************************************************************************/

    /*****************************************************************************************************/
    #region "Delegates"

    public delegate void LatchDataEventHandler(LatchEventArgs e);
    public delegate void PlayEventHandler(VideoStartPlaybackArgs e);
    public delegate void PauseEventHandler(EventArgs e);
    public delegate void VideoUpdateEventHandler(VideoUpdateEventArgs e);
    public delegate void SetVideoStartFrameEventHandler(VideoStartFrameArgs e);
    public delegate void SetVideoEndFrameEventHandler(VideoEndFrameArgs e);
    public delegate void GotoVideoStartMarkerEventHandler();
    public delegate void GotoVideoEndMarkerEventHandler();

    #endregion
    /*****************************************************************************************************/

    public partial class videoPlayer : UserControl, IDisposable
    {
        #region IDisposable Members

        void IDisposable.Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion

        #region "Events"

        public event LatchDataEventHandler m_latchEvent;
        public event PlayEventHandler m_playEvent;
        public event PauseEventHandler m_pauseEvent;
        public event VideoUpdateEventHandler m_videoUpdateEvent;
        public event SetVideoStartFrameEventHandler m_setVidStartFrame;
        public event SetVideoEndFrameEventHandler m_setVidEndFrame;
        public event GotoVideoStartMarkerEventHandler m_gotoVidStartMarker;
        public event GotoVideoEndMarkerEventHandler m_gotoVidEndMarker;

        #endregion

        /*****************************************************************************************************/
        #region "Private Vars"

        private Bitmap m_previewVidImage;
        private frmProject m_parentFormInst;
        private InputVideoPlayer m_player;
        private string m_curVideoFile;
        private InputVideoInfo inVideoData;
        private Timer timerVpos; // video Position slider update timer
        private ProjectInfo m_projectInfo;

        private bool m_disposed;
        private bool m_isLatched;

        private bool sliderThumbClicked;
        private int m_scrollPos;


        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Class Constructor"
        public videoPlayer()
        {
            InitializeComponent();
            m_disposed = false;

            sliderThumbClicked = false;
            m_scrollPos = 0;

        }

        ~videoPlayer()
        {
        }

        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Properties"

        public ProjectInfo projectInfo
        {
            set
            {
                m_projectInfo = value;
            }
        }

        public frmProject ParentFormInst
        {
            set
            {
                m_parentFormInst = value;
            }
        }

        public long CurrentVideoPos
        {
            get
            {
                if (m_player != null)
                {
                    return m_player.VideoPosition;
                }

                return 0;
            }

            set
            {
                sliderVideoPos.Value = (int)value;
                if (m_player != null)
                    m_player.VideoPosition = value;
            }
        }

        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Public Methods"

        public void EnableOverlayPreview(bool show, DashboardOverlayPreviewSettings overlayPreviewSettings)
        {
            Pause();

            if (m_player != null)
            {
                if (show)
                {
                    VideoFrame fr = m_player.GetCurrentVideoFrame();
                    m_previewVidImage = new Bitmap(fr.frame);
                    m_player.ReleaseVideoframe(fr);

                    Bitmap bmp = new Bitmap(m_previewVidImage);
                    Graphics g = Graphics.FromImage(bmp);
                    Bitmap dash = overlayPreviewSettings.dashboard;
                    //g.DrawImage(dash, 5, m_previewVidImage.Height - dash.Height - 10, dash.Width, dash.Height);

                    System.Drawing.Imaging.ImageAttributes ia = new System.Drawing.Imaging.ImageAttributes();
                    if (overlayPreviewSettings.transparency > 0)
                    {
                        System.Drawing.Imaging.ColorMatrix m = new System.Drawing.Imaging.ColorMatrix();
                        m.Matrix00 = 1.0f;
                        m.Matrix11 = 1.0f;
                        m.Matrix22 = 1.0f;
                        m.Matrix44 = 1.0f;
                        m.Matrix33 = (float)(overlayPreviewSettings.transparency / 100);
                        ia.SetColorMatrix(m);
                    }

                    float sx = 0, sy = 0;
                    if (overlayPreviewSettings.sizeScale > 0)
                    {
                        sx = (float)dash.Width * (overlayPreviewSettings.sizeScale / 100);
                        sy = (float)dash.Height * (overlayPreviewSettings.sizeScale / 100);
                    }
                    else
                    {
                        sx = dash.Width;
                        sy = dash.Height;
                    }
                    //g.DrawImage(dash, overlayPreviewSettings.Position.X, overlayPreviewSettings.Position.Y, dash.Width, dash.Height);
                    Rectangle dst = new Rectangle((int)overlayPreviewSettings.Position.X, (int)overlayPreviewSettings.Position.Y, (int)sx, (int)sy);
                    g.DrawImage(dash, dst, 0, 0, dash.Width, dash.Height, GraphicsUnit.Pixel, ia);

                    g.Dispose();
                    g = null;

                    g = Graphics.FromHwnd(videoDisplay.Handle);
                    g.DrawImage(bmp, 0, 0, videoDisplay.Width, videoDisplay.Height);
                    g.Dispose();
                    g = null;

                    //dash.Dispose();
                    //dash = null;
                    bmp.Dispose();
                    bmp = null;
                    ia.Dispose();
                }
                else
                {
                    if (m_previewVidImage != null)
                    {
                        m_previewVidImage.Dispose();
                        m_previewVidImage = null;

                        VideoFrame fr = m_player.GetCurrentVideoFrame();
                        Bitmap bmp = new Bitmap(fr.frame);
                        m_player.ReleaseVideoframe(fr);

                        Graphics g = Graphics.FromHwnd(videoDisplay.Handle);
                        g.DrawImage(bmp, 0, 0, videoDisplay.Width, videoDisplay.Height);
                        g.Dispose();
                        g = null;
                        bmp.Dispose();
                        bmp = null;
                    }
                }
            }
        }

        public void UpdateOverlayPreview(DashboardOverlayPreviewSettings dashSet)
        {
            if (m_previewVidImage != null)
            {
                Bitmap vid = new Bitmap(m_previewVidImage);

                try
                {
                    Bitmap dash = dashSet.dashboard;
                    Graphics g = Graphics.FromImage(vid);
                    //g.DrawImage(dash, dashSet.Position.X, dashSet.Position.Y, dash.Width, dash.Height);

                    System.Drawing.Imaging.ImageAttributes ia = new System.Drawing.Imaging.ImageAttributes();
                    if (dashSet.transparency > 0)
                    {
                        System.Drawing.Imaging.ColorMatrix m = new System.Drawing.Imaging.ColorMatrix();
                        m.Matrix00 = 1.0f;
                        m.Matrix11 = 1.0f;
                        m.Matrix22 = 1.0f;
                        m.Matrix44 = 1.0f;
                        m.Matrix33 = (float)(dashSet.transparency / 100);
                        ia.SetColorMatrix(m);
                    }

                    float sx = 0, sy = 0;
                    if (dashSet.sizeScale > 0)
                    {
                        sx = (float)dash.Width * (dashSet.sizeScale / 100);
                        sy = (float)dash.Height * (dashSet.sizeScale / 100);
                    }
                    else
                    {
                        sx = dash.Width;
                        sy = dash.Height;
                    }
                    //g.DrawImage(dash, overlayPreviewSettings.Position.X, overlayPreviewSettings.Position.Y, dash.Width, dash.Height);
                    Rectangle dst = new Rectangle((int)dashSet.Position.X, (int)dashSet.Position.Y, (int)sx, (int)sy);
                    g.DrawImage(dash, dst, 0, 0, dash.Width, dash.Height, GraphicsUnit.Pixel, ia);

                    g.Dispose();
                    g = Graphics.FromHwnd(videoDisplay.Handle);
                    g.DrawImage(vid, 0, 0, videoDisplay.Width, videoDisplay.Height);
                    g.Dispose();
                    g = null;
                    //dash.Dispose();
                    //dash = null;
                    vid.Dispose();
                    vid = null;
                    ia.Dispose();
                    ia = null;
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message, "ERROR");
                }
            }
        }

        public void OpenVideoFile(string file)
        {
            // is a video already open?
            if (m_curVideoFile != null)
            {
                CloseVideo();
                m_curVideoFile = null;
            }

            if (file !=null && file != "")
            {
                try
                {
                    m_player = new InputVideoPlayer(this.Handle, videoDisplay.Handle, new Rectangle(0, 0, videoDisplay.Width, videoDisplay.Height), file, null);
                    m_curVideoFile = file;

                    inVideoData = new InputVideoInfo();
                    inVideoData.framerate = m_player.VideoFrameRate;
                    inVideoData.avgTimeFrame = (int)m_player.AvgTimePerFrame;
                    inVideoData.aspectR = (float)((float)m_player.VideoSize.Width / (float)m_player.VideoSize.Height);
                    inVideoData.width = m_player.VideoSize.Width;
                    inVideoData.height = m_player.VideoSize.Height;
                    inVideoData.fileName = file;

                    timerVpos.Interval = 60;
                    //timerVpos.Interval = inVideoData.avgTimeFrame / 10000;                    

                    sliderVideoPos.Enabled = true;
                    btGotoFrameStartMarker.Enabled = true;
                    btVideoPlay.Enabled = true;
                    btVideoPause.Enabled = true;
                    btGotoFrameEndMarker.Enabled = true;
                    btSetVideoStartPos.Enabled = true;
                    btSetVideoEndPos.Enabled = true;

                    checkBox1.Enabled = true;

                    sliderVideoPos.Minimum = 1;
                    sliderVideoPos.Maximum = (int)m_player.Duration;
                    sliderVideoPos.Value = 1;
                    m_player.VideoPosition = sliderVideoPos.Value;
                    lblCurVideoPos.Text = sliderVideoPos.Value.ToString();
                    
                    Pause();
                }
                catch (Exception e)
                {
                    //MessageBox.Show(e.Message, "Video Player Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    throw e;
                }
            }
        }

        public void CloseVideo()
        {
            if (m_curVideoFile != "")
            {
                if (m_player != null)
                {
                    m_player.Dispose();
                    m_player = null;
                }
            }            
        }

        public void DisablePlayerControls()
        {
            checkBox1.Enabled = false;
            btGotoFrameEndMarker.Enabled = false;
            btGotoFrameStartMarker.Enabled = false;
            btSetVideoEndPos.Enabled = false;
            btSetVideoStartPos.Enabled = false;
            btVideoPause.Enabled = false;
            btVideoPlay.Enabled = false;
            sliderVideoPos.Enabled = false;
        }

        public void EnablePlayerControls()
        {
            checkBox1.Enabled = true;
            btGotoFrameEndMarker.Enabled = true;
            btGotoFrameStartMarker.Enabled = true;
            btSetVideoEndPos.Enabled = true;
            btSetVideoStartPos.Enabled = true;
            btVideoPause.Enabled = true;
            btVideoPlay.Enabled = true;
            sliderVideoPos.Enabled = true;

            
        }

        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Event Handler Methods"

        void updateTimer_Tick()
        {
        }

        void OnTimerTick(object sender, EventArgs e)
        {
            if (sliderThumbClicked == false)
            {
                sliderVideoPos.Value = (int)m_player.VideoPosition;
                lblCurVideoPos.Text = sliderVideoPos.Value.ToString();
            }
        }

        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Overrides"
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
        }
        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Properties"

        public InputVideoInfo VideoInfo
        {
            get
            {
                return inVideoData;
            }
        }

        //public Bitmap CurrentVideoImage
        //{
        //    get
        //    {
        //        if (m_player != null)
        //        {
        //            Bitmap bmp = m_player.VideoImage;
        //            return bmp;
        //        }
        //    }
        //}
        #endregion
        /*****************************************************************************************************/

        /*****************************************************************************************************/
        #region "Private Methods"

        private void videoPlayer_Load(object sender, EventArgs e)
        {
            timerVpos = new Timer();
            timerVpos.Tick += new EventHandler(OnTimerTick);
            
            // disable all controls until video is opened 
            sliderVideoPos.Enabled = false;
            btGotoFrameEndMarker.Enabled = false;
            btVideoPause.Enabled = false;
            btVideoPlay.Enabled = false;
            btGotoFrameStartMarker.Enabled = false;
            checkBox1.Enabled = false;
            btSetVideoEndPos.Enabled = false;
            btSetVideoStartPos.Enabled = false;

            m_isLatched = false;
        }

        private void Play()
        {
            if (m_player != null && m_curVideoFile != null)
            {
                m_player.Play();
            }
        }

        private void Pause()
        {
            if (m_player != null && m_curVideoFile != null)
            {
                m_player.Pause();
            }
        }

        private void Stop()
        {
            if (m_player != null && m_curVideoFile != null)
            {
                m_player.Stop();
            }
        }

        private void RepaintVideo(Graphics g)
        {
            if (m_player != null)
            {
                VideoFrame frame = m_player.GetCurrentVideoFrame();
                // = Graphics.FromHwnd(videoDisplay.Handle);
                g.DrawImage(frame.frame, new Rectangle(0, 0, videoDisplay.Width, videoDisplay.Height));

                m_player.ReleaseVideoframe(frame);
                //g.Dispose();
                //g = null;
            }
        }

        private void btVideoPlay_Click(object sender, EventArgs e)
        {
            Play();

            if (((frmProject)m_playEvent.Target).InvokeRequired)
            {
                m_playEvent.BeginInvoke(new VideoStartPlaybackArgs(sliderVideoPos.Value), null, null);
            }
            else
            {
                m_playEvent(new VideoStartPlaybackArgs(sliderVideoPos.Value));
            }

            timerVpos.Start();
        }

        private void btVideoPause_Click(object sender, EventArgs e)
        {
            Pause();
            if (((frmProject)m_pauseEvent.Target).InvokeRequired)
            {
                m_pauseEvent.BeginInvoke(new EventArgs(), null, null);
            }
            else
            {
                m_pauseEvent.Invoke(new EventArgs());
            }

            timerVpos.Stop();
        }

        private void sliderVideoPos_Scroll(object sender, EventArgs e)
        {
            if (m_player == null)
                return;

            if (m_player.VideoPlaybackStatus == 1)
            {
                if (sliderThumbClicked == true)
                {
                    m_scrollPos = sliderVideoPos.Value;
                }
            }
            else
            {
                m_player.VideoPosition = (long)sliderVideoPos.Value;

                VideoUpdateEventArgs vuea = new VideoUpdateEventArgs();
                vuea.frameInc = sliderVideoPos.Value;

                if (((frmProject)m_videoUpdateEvent.Target).InvokeRequired)
                {
                    m_videoUpdateEvent.BeginInvoke(vuea, null, null);
                }
                else
                {
                    m_videoUpdateEvent.Invoke(vuea);
                }
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            // fire event to inform rest of the application that the checked state changed
            frmProject frm = (frmProject)m_latchEvent.Target;
            Int32 fr =0;
            bool vo = false;

            if (checkBox1.Checked)
                m_isLatched = true;
            else
                m_isLatched = false;

            if (m_curVideoFile != null && m_player != null)
            {
                fr = m_player.VideoFrameRate;
                vo = true;
            }
            LatchEventArgs lea = new LatchEventArgs(checkBox1.Checked, fr, sliderVideoPos.Value);
            lea.isVideoOpen = vo;

            if (frm.InvokeRequired)
            {
                m_latchEvent.BeginInvoke(lea, null, null);
            }
            else
            {
                m_latchEvent.Invoke(lea);
            }
        }
       
        private void sliderVideoPos_KeyDown(object sender, KeyEventArgs e)
        {
//            Keys k = e.KeyCode;
//            bool ctrl = false;
//            bool alt = false;
//            bool shift = false;
//            int frameInc = 0;

//            if (e.Control)
//                ctrl = true;
//            if (e.Shift)
//                shift = true;
//            if (e.Alt)
//                alt = true;

//            if (k == Keys.Right)
//            {
//                if (ctrl)
//                    frameInc = 10;
//                else if (alt)
//                    frameInc = 30;
//                else if (shift)
//                    frameInc = 100;
//                else
//                    frameInc = 1;
////                frameInc -= 1;
//            }
//            else if (k == Keys.Left)
//            {
//                if (ctrl)
//                    frameInc = -10;
//                else if (alt)
//                    frameInc = -30;
//                else if (shift)
//                    frameInc = -100;
//                else
//                    frameInc = -1;
////                frameInc += 1;
//            }

//            m_frameInc = frameInc;

        }

        private void btSetVideoStartPos_Click(object sender, EventArgs e)
        {
            m_projectInfo.outVideoData.syncData.startFrame = sliderVideoPos.Value;

            VideoStartFrameArgs arg = new VideoStartFrameArgs((long)sliderVideoPos.Value);

            if (((frmProject)m_setVidStartFrame.Target).InvokeRequired)
            {
                m_setVidStartFrame.BeginInvoke(arg, null, null);
            }
            else
            {
                m_setVidStartFrame.Invoke(arg);
            }
        }

        private void btSetVideoEndPos_Click(object sender, EventArgs e)
        {
            m_projectInfo.outVideoData.syncData.endFrame = sliderVideoPos.Value;

            VideoEndFrameArgs arg = new VideoEndFrameArgs((long)sliderVideoPos.Value);

            if (((frmProject)m_setVidEndFrame.Target).InvokeRequired)
            {
                m_setVidEndFrame.BeginInvoke(arg, null, null);
            }
            else
            {
                m_setVidEndFrame.Invoke(arg);
            }
        }

        private void videoDisplay_Paint(object sender, PaintEventArgs e)
        {
            RepaintVideo(e.Graphics);
        }

        private void btGotoFrameStartMarker_Click(object sender, EventArgs e)
        {
            if (m_projectInfo.outVideoData.syncData.startFrame != 0)
            {
                m_player.VideoPosition = m_projectInfo.outVideoData.syncData.startFrame;
                sliderVideoPos.Value = m_projectInfo.outVideoData.syncData.startFrame;
            }
            if (m_isLatched)
            {
                if (((frmProject)m_gotoVidStartMarker.Target).InvokeRequired)
                {
                    m_gotoVidStartMarker.BeginInvoke(null, null);
                }
                else
                {
                    m_gotoVidStartMarker.Invoke();
                }
            }
        }

        private void btGotoFrameEndMarker_Click(object sender, EventArgs e)
        {
            if (m_projectInfo.outVideoData.syncData.endFrame != 0)
            {
                m_player.VideoPosition = m_projectInfo.outVideoData.syncData.endFrame;
                sliderVideoPos.Value = m_projectInfo.outVideoData.syncData.endFrame;
            }
            if (m_isLatched)
            {
                if (((frmProject)m_gotoVidEndMarker.Target).InvokeRequired)
                {
                    m_gotoVidEndMarker.BeginInvoke(null, null);
                }
                else
                {
                    m_gotoVidEndMarker.Invoke();
                }
            }
        }

        private void sliderVideoPos_ValueChanged(object sender, EventArgs e)
        {
            lblCurVideoPos.Text = sliderVideoPos.Value.ToString();
        }

        #endregion

        private void sliderVideoPos_MouseDown(object sender, MouseEventArgs e)
        {
            sliderThumbClicked = true;

        }

        private void sliderVideoPos_MouseUp(object sender, MouseEventArgs e)
        {
            bool ve = false;
            bool te = false;


            //if (sliderVideoPos.ClientRectangle.Contains(e.Location.X, e.Location.Y))
            //{
            //    int w = sliderVideoPos.ClientRectangle.Width;
            //    long d = m_player.Duration;

            //    float p = (float)((float)w / (float)d);
            //    float p2 = (float)((float)e.X / (float)p);
            //    int v = (int)Math.Round((double)p2);

            //    if ((v >= sliderVideoPos.Minimum) && (v <= sliderVideoPos.Maximum))
            //    {
            //        m_scrollPos = v;
            //    }
            //}

            if (m_player.VideoPlaybackStatus == 1)
            {
                if (timerVpos.Enabled)
                {
                    timerVpos.Stop();
                    te = true;
                }

                m_player.Pause();
                ve = true;

                if (m_scrollPos != 0)
                {
                    sliderVideoPos.Value = m_scrollPos;
                    m_player.VideoPosition = m_scrollPos;
                }
                else
                {
                    m_player.VideoPosition = (long)sliderVideoPos.Value;
                }
                if (ve)
                    m_player.Play();
                if (te)
                    timerVpos.Start();
            }
            else
            {
                //sliderVideoPos.Value = m_scrollPos;
                m_player.VideoPosition = (long)sliderVideoPos.Value;

                VideoUpdateEventArgs vuea = new VideoUpdateEventArgs();
                vuea.frameInc = sliderVideoPos.Value;

                if (((frmProject)m_videoUpdateEvent.Target).InvokeRequired)
                {
                    m_videoUpdateEvent.BeginInvoke(vuea, null, null);
                }
                else
                {
                    m_videoUpdateEvent.Invoke(vuea);
                }
            }

            sliderThumbClicked = false;
        }

        /*****************************************************************************************************/


    }
}
