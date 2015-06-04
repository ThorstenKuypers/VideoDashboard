namespace VideoDashboard
{
    partial class frmLapInfo
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.lapInfoList = new System.Windows.Forms.ListView();
            this.headerLapNo = new System.Windows.Forms.ColumnHeader();
            this.headerLap = new System.Windows.Forms.ColumnHeader();
            this.headerLaptime = new System.Windows.Forms.ColumnHeader();
            this.headerStarttime = new System.Windows.Forms.ColumnHeader();
            this.headerStartIndex = new System.Windows.Forms.ColumnHeader();
            this.headerEndIndex = new System.Windows.Forms.ColumnHeader();
            this.headerLength = new System.Windows.Forms.ColumnHeader();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(400, 432);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Close";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lapInfoList
            // 
            this.lapInfoList.AutoArrange = false;
            this.lapInfoList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.headerLapNo,
            this.headerLap,
            this.headerLaptime,
            this.headerStarttime,
            this.headerStartIndex,
            this.headerEndIndex,
            this.headerLength});
            this.lapInfoList.FullRowSelect = true;
            this.lapInfoList.GridLines = true;
            this.lapInfoList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lapInfoList.Location = new System.Drawing.Point(12, 19);
            this.lapInfoList.Name = "lapInfoList";
            this.lapInfoList.Size = new System.Drawing.Size(394, 383);
            this.lapInfoList.TabIndex = 1;
            this.lapInfoList.UseCompatibleStateImageBehavior = false;
            this.lapInfoList.View = System.Windows.Forms.View.Details;
            // 
            // headerLapNo
            // 
            this.headerLapNo.Text = "#";
            this.headerLapNo.Width = 21;
            // 
            // headerLap
            // 
            this.headerLap.Text = "Lap";
            this.headerLap.Width = 34;
            // 
            // headerLaptime
            // 
            this.headerLaptime.Text = "Laptime";
            this.headerLaptime.Width = 73;
            // 
            // headerStarttime
            // 
            this.headerStarttime.Text = "SessionTime";
            this.headerStarttime.Width = 83;
            // 
            // headerStartIndex
            // 
            this.headerStartIndex.Text = "Start";
            this.headerStartIndex.Width = 59;
            // 
            // headerEndIndex
            // 
            this.headerEndIndex.Text = "End";
            // 
            // headerLength
            // 
            this.headerLength.Text = "Length";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lapInfoList);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(418, 414);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Laps";
            // 
            // frmLapInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(487, 463);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox1);
            this.Name = "frmLapInfo";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Lap Info";
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListView lapInfoList;
        private System.Windows.Forms.ColumnHeader headerLapNo;
        private System.Windows.Forms.ColumnHeader headerLaptime;
        private System.Windows.Forms.ColumnHeader headerStartIndex;
        private System.Windows.Forms.ColumnHeader headerStarttime;
        private System.Windows.Forms.ColumnHeader headerLength;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ColumnHeader headerEndIndex;
        private System.Windows.Forms.ColumnHeader headerLap;
    }
}