using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace VideoDashboard
{
    public partial class frmProjectSettings : Form
    {
        private Project m_project;

        public frmProjectSettings(Project project)
        {
            InitializeComponent();

            m_project = project;
            ProjectInfo pi = m_project.curProjectInfo;

            //tbDefaultprojectPath.Text =pi.appSet.projectPath;
            foreach (string path in pi.appSet.dashPaths)
            {
                if (tbDashFolder.Text == "")
                    tbDashFolder.Text = path;
                tbDashFolder.Text += ";" + path;
            }
            tbVidProfilesFolder.Text = pi.appSet.videoProfilesPath;

            comboBox1.SelectedIndex = 0;

            m_project.curProjectInfo = pi;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void tbDefaultprojectPath_Leave(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ProjectInfo pi = m_project.curProjectInfo;

            switch (comboBox1.SelectedIndex)
            {
                case 0:
                    pi.appSet.units = Units.Units_Metric;
                    break;
                case 1:
                    pi.appSet.units = Units.Units_English;
                    break;

                default:
                    pi.appSet.units = Units.Units_undefined;
                    break;
            }

            //pi.appSet.projectPath = tbDefaultprojectPath.Text;
            if (tbDashFolder.Text != "")
            {
                string p = tbDashFolder.Text;
                string[] paths = p.Split(new char[] { ';' });
                foreach (string s in paths)
                    pi.appSet.dashPaths.Add(s);
            }
            pi.appSet.videoProfilesPath = tbVidProfilesFolder.Text;

            m_project.curProjectInfo = pi;
        }
    }
}
