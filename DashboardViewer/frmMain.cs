using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using libDataLogging.DataLogWrapper;
using libLDFWrapper;
using VideoDashboard;

namespace DashboardViewer
{
    delegate void ReloadFileHandler();

    public partial class frmMain : Form
    {
        private DashboardLayout _ldf;
        private CDataLog _dataLogger;
        private Bitmap dashImg;
        private string curDashName; // filename of currently active dashboard

        private FileSystemWatcher _watch;
        private ReloadFileHandler _reloadFile;

        bool _showBoundingBoxes = false;
        private Timer _sampleTimer;
        private bool _running;

        public frmMain()
        {
            InitializeComponent();

            this.Text = "DashViewer";
            _watch = new FileSystemWatcher();
            _watch.Changed += _fileWatch_Changed; // new FileSystemEventHandler();
            _reloadFile = new ReloadFileHandler(ReloadFile);
            _sampleTimer = new Timer();
            _sampleTimer.Tick += _sampleTimer_Tick;
        }

        void _sampleTimer_Tick(object sender, EventArgs e)
        {
            if (sliderSampleIndex.Value <= sliderSampleIndex.Maximum &&
                sliderSampleIndex.Value >= sliderSampleIndex.Minimum)
            {
                sliderSampleIndex.Value++;
                sliderSampleIndex_Scroll(this, new EventArgs());
            }
        }

        private void layoutFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = false;
            ofd.Filter = "VideoDashboard layout files|*.layout";

            if (ofd.ShowDialog() == DialogResult.OK)
            {

                try
                {
                    if (_ldf == null)
                        _ldf = new DashboardLayout();

                    _ldf.ParseLayoutFile(ofd.FileName);
                    string s = ofd.FileName;
                    int pos = s.LastIndexOf('\\');
                    s = s.Substring(0, pos);
                    curDashName = ofd.FileName;
                    _watch.Path = s;
                    _watch.NotifyFilter = NotifyFilters.LastWrite;
                    _watch.Filter = "*.layout";
                    _watch.EnableRaisingEvents = true;

                    if (_dataLogger != null)
                        _ldf.SetDataLogger(_dataLogger.getDataLoggerObjPtr());

                    _ldf.ShowBoundingBoxes(_showBoundingBoxes);
                    if (dashImg != null)
                    {
                        dashImg.Dispose();
                        dashImg = null;
                    }
                    if (_dataLogger != null)
                        dashImg = _ldf.RenderDashboard("", sliderSampleIndex.Value);
                    else
                        dashImg = _ldf.RenderDashboard(ofd.FileName, 1);

                    this.Text = string.Format("DashViewer - {0}", _ldf.GetDashboardShortName(ofd.FileName));
                    previewWnd.Invalidate();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                    if (_ldf != null)
                    {
                        _ldf.Dispose();
                        _ldf = null;
                    }
                }
            }
        }

        void _fileWatch_Changed(object sender, FileSystemEventArgs e)
        {
            try
            {
                _watch.EnableRaisingEvents = false;
                if (e.ChangeType == WatcherChangeTypes.Changed)
                {
                    if (((frmMain)this).InvokeRequired)
                        BeginInvoke(_reloadFile);
                    else
                        _reloadFile();
                }
            }
            finally
            {
                _watch.EnableRaisingEvents = true;
            }
        }

        private void ReloadFile()
        {
            btReloadFile_Click(this, new EventArgs());
        }

        private void dataLogToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Multiselect = false;
            ofd.Filter = "iRacing binary telemetry files|*.ibt";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    if (_dataLogger == null)
                        _dataLogger = new CDataLog(ofd.FileName);
                    else
                    {
                        _dataLogger.Dispose();
                        _dataLogger = new CDataLog(ofd.FileName);
                    }

                    if (_dataLogger != null)
                    {
                        MessageBox.Show(string.Format("Successfully loaded {0} samples from data log file", _dataLogger.SampleCount), "Data Log loaded", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    sliderSampleIndex.Minimum = 0;
                    sliderSampleIndex.Maximum = _dataLogger.SampleCount;
                    sliderSampleIndex.Value = 0;

                    btStart.Enabled = true;

                    if (_ldf != null)
                        _ldf.SetDataLogger(_dataLogger.getDataLoggerObjPtr());
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "DashboardViewer", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }

        }

        private void sliderSampleIndex_Scroll(object sender, EventArgs e)
        {
            double t = 0;
            Object obj = null;
            if (_dataLogger != null)
            {
                obj = _dataLogger.GetChannelData("SessionTime", sliderSampleIndex.Value);
                t = (double)obj;
            }
            tbSessionTime.Text = t.ToString("######.##");

            if (dashImg != null)
            {
                dashImg.Dispose();
                dashImg = null;
            }
            if (_ldf != null)
            {
                dashImg = _ldf.RenderDashboard("", sliderSampleIndex.Value);
                previewWnd.Invalidate();
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (dashImg != null)
            {
                dashImg.Dispose();
                dashImg = null;
            }
            if (_dataLogger != null)
            {
                _dataLogger.Dispose();
                _dataLogger = null;
            }
            if (_ldf != null)
            {
                _ldf.Dispose();
                _ldf = null;
            }
        }

        private void previewWnd_Paint(object sender, PaintEventArgs e)
        {
            if (dashImg != null)
            {
                try
                {
                    Graphics gfx = e.Graphics;

                    gfx.DrawImage(dashImg, 0, 0, dashImg.Width, dashImg.Height);

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btReloadFile_Click(object sender, EventArgs e)
        {
            if (curDashName == null)
            {
                MessageBox.Show("No layout file opened. Please open a file first.", "DashboardViewer", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            try
            {
                if (dashImg != null)
                {
                    dashImg.Dispose();
                    dashImg = null;
                }

                if (_ldf != null)
                {
                    _ldf.ParseLayoutFile(curDashName);
                    _ldf.ShowBoundingBoxes(_showBoundingBoxes);
                    dashImg = _ldf.RenderDashboard("", sliderSampleIndex.Value);

                    previewWnd.Invalidate();
                }
                //else
                //{
                //    _ldf = new DashboardLayout();
                //    _ldf.ParseLayoutFile(curDashName);
                //    _ldf.ShowBoundingBoxes(_showBoundingBoxes);
                //    dashImg = _ldf.RenderDashboard("", sliderSampleIndex.Value);

                //    previewWnd.Invalidate();
                //}

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Layout Format Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                //if (_ldf != null)
                //{
                //    _ldf.Dispose();
                //    _ldf = null;
                //}
            }
        }

        private void showBoundingBoxesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (showBoundingBoxesToolStripMenuItem.Checked)
            {
                showBoundingBoxesToolStripMenuItem.Checked = false;
                _showBoundingBoxes = showBoundingBoxesToolStripMenuItem.Checked;
            }
            else
            {
                showBoundingBoxesToolStripMenuItem.Checked = true;
                _showBoundingBoxes = showBoundingBoxesToolStripMenuItem.Checked;
            }

            if (_ldf != null)
            {
                _ldf.ShowBoundingBoxes(_showBoundingBoxes);
                btReloadFile_Click(this, new EventArgs());
            }
        }

        private void btStart_Click(object sender, EventArgs e)
        {
            if (!_running)
            {
                int i = (int)Math.Round(((float)1000 / (float)_dataLogger.TickRate));
                _sampleTimer.Interval = i;
                _sampleTimer.Start();
                btStart.Text = "Stop";
                _running = true;
            }
            else
            {
                _sampleTimer.Stop();
                btStart.Text = "Start";
                _running = false;
            }
        }

        private void channelsListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_dataLogger != null)
            {
                frmChannelsList f = new frmChannelsList(_dataLogger);
                f.ShowDialog();

                f.Dispose();
                f = null;
            }
            else
            {
                MessageBox.Show("No data log is open. Please open a log file first", "No data log file open", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
        }

    }
}
