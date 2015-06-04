using System;
using System.Collections.Specialized;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using System.Configuration;

namespace VideoDashboard
{
    public struct CloseProjectEventArgs
    {
        public CloseProjectEventArgs(string projectName, CloseReason reason)
        {
            m_projectName = projectName;
            m_reason = reason;
        }

        public string m_projectName;
        public CloseReason m_reason;
    }


    public partial class frmMain : Form
    {
        private ApplicationSettings applicationConfig;

        #region "log4net instance"
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(typeof(frmMain).Name);
        #endregion

        #region "Error MessageBox"
        private void MsgBoxError(Exception ex)
        {
#if DEBUG
            string msg = string.Format("Error in {0}\n -- {1}\n -- {2}", ex.TargetSite.Name, ex.Message, ex.StackTrace);
#else
            string msg = string.Format("Error in {0}\n -- {1}\n", ex.TargetSite.Name, ex.Message);
#endif
            MessageBox.Show(msg, ex.Source, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
        #endregion

        public frmMain()
        {
            InitializeComponent();

            try
            {
                applicationConfig = new ApplicationSettings();
                applicationConfig.dashPaths = new List<string>();
                loadAppConfig();

                string[] args = Environment.GetCommandLineArgs();
                if (args.Length > 1)
                {
#if DEBUG
                    if (args[1] == "-development")
                    {
                        applicationConfig.appPath = Application.StartupPath;
                        applicationConfig.dashPaths.Clear();
                        applicationConfig.dashPaths.Add(applicationConfig.appPath + "\\dashboards");
                        applicationConfig.videoProfilesPath = applicationConfig.appPath + "\\VideoProfiles";
                    }
#endif
                }


                log4net.GlobalContext.Properties["LogFilePathName"] = applicationConfig.appPath + "\\VideoDashboard.log";
                string s = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\" + Application.ProductName + "\\" + Application.ProductName + ".exe.Config";

                log4net.Config.XmlConfigurator.Configure(new FileInfo(s));
                log.Info("VideoDashboard started");

            }
            catch (Exception ex)
            {
                log.Error("frmMain()", ex);
                MsgBoxError(ex);

                ex = null;
            }
        }

        private void OnCloseProject(CloseProjectEventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                frmProject frm = (frmProject)ActiveMdiChild;
                if (frm != null)
                {
                    if (MdiChildren.Length == 1)
                    {// last project form has closed
                        Text = "VideoDashboard";
                        showChannelsListToolStripMenuItem.Enabled = false;
                    }

                    frm.m_project = null;
                    frm.Dispose();
                    frm = null;

                    //GC.Collect(0);
                    //GC.Collect(1);
                    //GC.Collect(2);
                }
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            frmAboutBox ab = new frmAboutBox();
            ab.ShowDialog();
        }

        private void frmMain_MdiChildActivate(object sender, EventArgs e)
        {
            frmProject frm = (frmProject)ActiveMdiChild;

            if (frm != null)
            {
                Text = string.Format("VideoDashboard - {0}", frm.m_project.ProjectName);
                showChannelsListToolStripMenuItem.Enabled = true;
            }
        }

        private void btNewProject_Click(object sender, EventArgs e)
        {
            frmNewProject f = new frmNewProject();
            if (f.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Project prj = new Project(f.m_projectName, f.m_projectNotes, applicationConfig);

                    Text = string.Format("VideoDashboard - {0}", f.m_projectName);

                    frmProject pnlMain = new frmProject(prj);
                    pnlMain.m_closeProjectEvent += OnCloseProject;

                    f.Dispose();
                    f = null;

                    pnlMain.MdiParent = this;
                    pnlMain.Show();
                    pnlMain.NewProject();
                }
                catch (Exception ex)
                {
                    log.Error("btNewProject_Click()", ex);
                    MsgBoxError(ex);
                    ex = null;
                }
            }
        }

        private void btOpenProject_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.CheckFileExists = true;
                ofd.Filter = "VideoDashboard project files|*.vdprj";
                ofd.Multiselect = false;
                ofd.Title = "Open Project file...";

                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    Project prj = new Project(null, null, applicationConfig);
                    //prj.LoadFromFile(ofd.FileName);
                    prj.Load(ofd.FileName);

                    string s = prj.ProjectName;
                    this.Text = string.Format("VideoDashboard - {0}", s);

                    frmProject frm = new frmProject(prj);
                    int w = frm.Width;
                    int h = frm.Height;
                    int cw = this.ClientRectangle.Width;
                    int ch = this.ClientRectangle.Height;

                    frm.MdiParent = this;
                    frm.m_closeProjectEvent += OnCloseProject;
                    frm.Show();
                    frm.LoadProject();

                }

                ofd.Dispose();
                ofd = null;
            }
            catch (Exception ex)
            {
                log.Error("btOpenProject_Click()", ex);
                MsgBoxError(ex);
                ex = null;
            }
        }

        private void btCloseProject_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                ((frmProject)ActiveMdiChild).Close();
            }
        }

        private void btTools_Click(object sender, EventArgs e)
        {

        }

        private void btOptions_Click(object sender, EventArgs e)
        {
            frmProject frm = (frmProject)ActiveMdiChild;

            if (frm != null)
            {
                frm.ChangeProjectSettings();
            }
        }

        private void btRenderToPreview_Click(object sender, EventArgs e)
        {
            frmProject frm = (frmProject)ActiveMdiChild;

            if (frm != null)
            {
                frm.RenderPreview();
            }
        }

        private void btRenderToFile_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild != null)
            {
                ((frmProject)ActiveMdiChild).RenderToFile();
            }
        }

        private void menuSaveAs_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild == null)
            {
                return;
            }

            ((frmProject)ActiveMdiChild).SaveProjectAs();
        }

        private void menuSave_Click(object sender, EventArgs e)
        {
            if (ActiveMdiChild == null)
            {
                return;
            }

            ((frmProject)ActiveMdiChild).SaveProject();
        }

        private void loadAppConfig()
        {
            try
            {
                string s = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\" + Application.ProductName;
                string cfgFile = (s + "\\" + Application.ProductName + ".exe.Config");
                ExeConfigurationFileMap cfgMap = new ExeConfigurationFileMap();
                cfgMap.ExeConfigFilename = cfgFile;
                Configuration cfg = ConfigurationManager.OpenMappedExeConfiguration(cfgMap, ConfigurationUserLevel.None);

                KeyValueConfigurationCollection appSet = cfg.AppSettings.Settings;
                if (appSet["dashboardPaths"].Value != "")
                {
                    string p = appSet["dashboardPaths"].Value;
                    string[] toks = p.Split(new char[] { ';' });
                    if (toks.Length > 0)
                    {
                        foreach (string t in toks)
                        {
                            applicationConfig.dashPaths.Add(t);
                        }
                    }
                }
                else
                    applicationConfig.dashPaths.Add(s + "\\dashboards");

                if (appSet["videoProfilesPath"].Value != "")
                {
                    string t = appSet["videoProfilesPath"].Value;
                    applicationConfig.videoProfilesPath = t;
                }
                else
                    applicationConfig.videoProfilesPath = s + "\\VideoProfiles";

                if (appSet["wndPosX"].Value != "")
                {
                    string t = appSet["wndPosX"].Value;
                    int x = 0;
                    int.TryParse(t, out x);
                }
                if (appSet["wndPosY"].Value != "")
                {
                    string t = appSet["wndPosY"].Value;
                    int x = 0;
                    int.TryParse(t, out x);
                }
                if (appSet["wndWidth"].Value != "")
                {
                    string t = appSet["wndWidth"].Value;
                    int x = 0;
                    int.TryParse(t, out x);
                }
                if (appSet["wndHeight"].Value != "")
                {
                    string t = appSet["wndHeight"].Value;
                    int x = 0;
                    int.TryParse(t, out x);
                }
                if (appSet["units"].Value != "")
                {
                    string t = appSet["units"].Value;
                    if (t == "Units_Metric")
                        applicationConfig.units = Units.Units_Metric;
                    else if (t == "Units_English")
                        applicationConfig.units = Units.Units_English;
                    else
                        applicationConfig.units = Units.Units_undefined;
                }
                applicationConfig.appPath = s;
            }
            catch (Exception ex)
            {
                string s = "Can't load application configuration. Fall back to default config\n\n";
                MessageBox.Show(s, "VideoDashboard error");

#if __BETA||DEBUG
                log.Error("loadAppConfig()", ex);
                MsgBoxError(ex);
#endif
            }
        }

        private void saveConfig()
        {
            string p = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\" + Application.ProductName;
            string cfgFile = (p + "\\" + Application.ProductName + ".exe.Config");
            ExeConfigurationFileMap cfgMap = new ExeConfigurationFileMap();
            cfgMap.ExeConfigFilename = cfgFile;
            Configuration cfg = ConfigurationManager.OpenMappedExeConfiguration(cfgMap, ConfigurationUserLevel.None);

            KeyValueConfigurationCollection appSet = cfg.AppSettings.Settings;

            string path = null;

            if (applicationConfig.dashPaths.Count > 0)
            {
                foreach (string s in applicationConfig.dashPaths)
                {
                    if (path == null)
                        path = s;
                    else
                        path += ";" + s;
                }

                cfg.AppSettings.Settings["dashboardPaths"].Value = path;
            }

            cfg.AppSettings.Settings["videoProfilesPath"].Value = applicationConfig.videoProfilesPath;
            cfg.AppSettings.Settings["wndPosX"].Value = applicationConfig.wndPosX.ToString();
            cfg.AppSettings.Settings["wndPosY"].Value = applicationConfig.wndPosY.ToString();
            cfg.AppSettings.Settings["wndWidth"].Value = applicationConfig.wndWidth.ToString();
            cfg.AppSettings.Settings["wndHeight"].Value = applicationConfig.wndHeight.ToString();
            cfg.AppSettings.Settings["units"].Value = applicationConfig.units.ToString();

            cfg.Save(ConfigurationSaveMode.Modified);
        }

        private void showChannelsListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                frmProject fp = (frmProject)ActiveMdiChild;

                if (fp.Logger != null)
                {
                    frmChannelsList f = new frmChannelsList(fp.Logger);
                    f.ShowDialog();
                }
                else
                {
                    MessageBox.Show("No log file is currently open. Please open a log file first!", "Error missing log file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            catch (Exception ex)
            {
                log.Error("showChannelsListToolStripMenuItem_Click()", ex);
                MsgBoxError(ex);

                return;
            }
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
        }

        private void frmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            saveConfig();
        }
    }
}
