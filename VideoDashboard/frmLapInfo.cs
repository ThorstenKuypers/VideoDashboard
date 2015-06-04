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
    public partial class frmLapInfo : Form
    {
        public frmLapInfo(LapInfo[] laps)
        {
            InitializeComponent();

            for (int i = 0; i < laps.Length; i++)
            {
                lapInfoList.Items.Add((i + 1).ToString());
                if (i == 0 || i == laps.Length - 1)
                    lapInfoList.Items[i].BackColor = Color.Wheat;

                lapInfoList.Items[i].SubItems.Add(laps[i].lapNo.ToString());
                lapInfoList.Items[i].SubItems.Add(formatLapTimeString(laps[i].lapTime));
                lapInfoList.Items[i].SubItems.Add(laps[i].time.ToString());
                lapInfoList.Items[i].SubItems.Add(laps[i].sampleIdx.ToString());
                int e = laps[i].sampleIdx + laps[i].sampleCnt;
                lapInfoList.Items[i].SubItems.Add(e.ToString());
                lapInfoList.Items[i].SubItems.Add(laps[i].sampleCnt.ToString());
            }
        }

        private string formatLapTimeString(float time)
        {
            int min = 0;
            float sec = 0;

            sec = time % 60;
            min = (int)((time - sec) / 60);

            return string.Format("{0}:{1:00.000}", min, sec);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
