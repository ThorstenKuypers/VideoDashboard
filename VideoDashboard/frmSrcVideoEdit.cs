using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using libds;

namespace VideoDashboard
{
    public partial class frmSrcVideoEdit : Form
    {
        private List<string> fileList;

        public List<string> SrcFileList
        {
            get
            {
                return fileList;
            }
        }

        public frmSrcVideoEdit()
        {
            InitializeComponent();
        }

        private void btAddVideo_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter ="AVI Files (*.avi)|*.avi";
            ofd.Multiselect = false;
            ofd.Title = "Add source video";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                string fname = ofd.FileName;
                FileInfo srcFileInfo = new FileInfo(fname);
                
                ListViewItem lvi = new ListViewItem(srcFileInfo.FullName);
                lvi.SubItems.Add("0");
                lvi.SubItems.Add(srcFileInfo.Length.ToString());
                lvSrcVideoFiles.Items.Add(lvi);
            }
        }

        private void btRemoveVideo_Click(object sender, EventArgs e)
        {
            if (lvSrcVideoFiles.SelectedItems.Count != 0)
            {
                lvSrcVideoFiles.SelectedItems[0].Remove();
            }
        }

        private void btVidInfo_Click(object sender, EventArgs e)
        {
            if (lvSrcVideoFiles.SelectedItems.Count != 0)
            {
                SourceVideoFileInfo svi = new SourceVideoFileInfo(lvSrcVideoFiles.SelectedItems[0].Text);

                svi.Dispose();
                svi = null;
            }
        }

        private void btOk_Click(object sender, EventArgs e)
        {
            if (lvSrcVideoFiles.Items.Count == 0)
            {
                MessageBox.Show("You have not selected at least one source video file!", "Missing source file(s)", MessageBoxButtons.OK, MessageBoxIcon.Error);

                DialogResult = DialogResult.None;
                return;
            }

            fileList = new List<string>();
            for (int i = 0; i < lvSrcVideoFiles.Items.Count; i++)
            {
                fileList.Add(lvSrcVideoFiles.Items[i].Text);
            }
        }
    }
}
