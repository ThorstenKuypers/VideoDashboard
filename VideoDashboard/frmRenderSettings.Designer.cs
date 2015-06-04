namespace VideoDashboard
{
    partial class frmRenderSettings
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

            if (m_wmvProfiler != null)
            {
                m_wmvProfiler.Dispose();
                m_wmvProfiler = null;
            }

            if (m_vidCompEnum != null)
            {
                m_vidCompEnum.Dispose();
                m_vidCompEnum = null;
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
            this.btOk = new System.Windows.Forms.Button();
            this.btCancel = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btBrowseFile = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.cbEncoder = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.pnlRawEncCtrl = new customControls.pnlRawEncCtrl();
            this.pnlWmvControl = new customControls.pnlWmvControl();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.lblResVidSize = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.cbOutputWidth = new System.Windows.Forms.ComboBox();
            this.cbAspectRatio = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // btOk
            // 
            this.btOk.Location = new System.Drawing.Point(287, 468);
            this.btOk.Name = "btOk";
            this.btOk.Size = new System.Drawing.Size(85, 23);
            this.btOk.TabIndex = 0;
            this.btOk.Text = "Render to File";
            this.btOk.UseVisualStyleBackColor = true;
            this.btOk.Click += new System.EventHandler(this.btOk_Click);
            // 
            // btCancel
            // 
            this.btCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btCancel.Location = new System.Drawing.Point(12, 468);
            this.btCancel.Name = "btCancel";
            this.btCancel.Size = new System.Drawing.Size(75, 23);
            this.btCancel.TabIndex = 1;
            this.btCancel.Text = "Cancel";
            this.btCancel.UseVisualStyleBackColor = true;
            this.btCancel.Click += new System.EventHandler(this.btCancel_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.groupBox4);
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(360, 450);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btBrowseFile);
            this.groupBox2.Controls.Add(this.textBox1);
            this.groupBox2.Location = new System.Drawing.Point(6, 395);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(348, 49);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "File";
            // 
            // btBrowseFile
            // 
            this.btBrowseFile.Location = new System.Drawing.Point(308, 17);
            this.btBrowseFile.Name = "btBrowseFile";
            this.btBrowseFile.Size = new System.Drawing.Size(34, 23);
            this.btBrowseFile.TabIndex = 1;
            this.btBrowseFile.Text = "...";
            this.btBrowseFile.UseVisualStyleBackColor = true;
            this.btBrowseFile.Click += new System.EventHandler(this.btBrowseFile_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(6, 19);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(296, 20);
            this.textBox1.TabIndex = 0;
            this.textBox1.Leave += new System.EventHandler(this.textBox1_Leave);
            this.textBox1.Enter += new System.EventHandler(this.textBox1_Enter);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.cbEncoder);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Controls.Add(this.pnlRawEncCtrl);
            this.groupBox4.Controls.Add(this.pnlWmvControl);
            this.groupBox4.Location = new System.Drawing.Point(6, 19);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(348, 289);
            this.groupBox4.TabIndex = 11;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Video Format";
            // 
            // cbEncoder
            // 
            this.cbEncoder.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbEncoder.FormattingEnabled = true;
            this.cbEncoder.Items.AddRange(new object[] {
            "WindowsMedia 9 (default)",
            "RAW"});
            this.cbEncoder.Location = new System.Drawing.Point(65, 19);
            this.cbEncoder.Name = "cbEncoder";
            this.cbEncoder.Size = new System.Drawing.Size(239, 21);
            this.cbEncoder.TabIndex = 3;
            this.cbEncoder.SelectedIndexChanged += new System.EventHandler(this.cbEncoder_SelectedIndexChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(9, 22);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(50, 13);
            this.label12.TabIndex = 2;
            this.label12.Text = "Encoder:";
            // 
            // pnlRawEncCtrl
            // 
            this.pnlRawEncCtrl.Location = new System.Drawing.Point(6, 46);
            this.pnlRawEncCtrl.Name = "pnlRawEncCtrl";
            this.pnlRawEncCtrl.Size = new System.Drawing.Size(333, 237);
            this.pnlRawEncCtrl.TabIndex = 5;
            // 
            // pnlWmvControl
            // 
            this.pnlWmvControl.Bitrate = ((uint)(0u));
            this.pnlWmvControl.FrameRate = 0;
            this.pnlWmvControl.KeyframeInterval = ((long)(0));
            this.pnlWmvControl.Location = new System.Drawing.Point(6, 46);
            this.pnlWmvControl.Name = "pnlWmvControl";
            this.pnlWmvControl.Quality = ((uint)(50u));
            this.pnlWmvControl.Size = new System.Drawing.Size(333, 237);
            this.pnlWmvControl.TabIndex = 4;
            this.pnlWmvControl.VideoHeigth = 0;
            this.pnlWmvControl.VideoWidth = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.lblResVidSize);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.cbOutputWidth);
            this.groupBox3.Controls.Add(this.cbAspectRatio);
            this.groupBox3.Location = new System.Drawing.Point(6, 314);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(348, 77);
            this.groupBox3.TabIndex = 10;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Video Size";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(192, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Aspect Ratio:";
            // 
            // lblResVidSize
            // 
            this.lblResVidSize.AutoSize = true;
            this.lblResVidSize.Location = new System.Drawing.Point(122, 52);
            this.lblResVidSize.Name = "lblResVidSize";
            this.lblResVidSize.Size = new System.Drawing.Size(24, 13);
            this.lblResVidSize.TabIndex = 8;
            this.lblResVidSize.Text = "0x0";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(6, 24);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(68, 13);
            this.label11.TabIndex = 0;
            this.label11.Text = "Video Width:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(9, 52);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(107, 13);
            this.label14.TabIndex = 7;
            this.label14.Text = "Resulting Video Size:";
            // 
            // cbOutputWidth
            // 
            this.cbOutputWidth.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbOutputWidth.FormattingEnabled = true;
            this.cbOutputWidth.Items.AddRange(new object[] {
            "keep input width",
            "320",
            "480",
            "640",
            "720",
            "800",
            "1280",
            "1920"});
            this.cbOutputWidth.Location = new System.Drawing.Point(80, 19);
            this.cbOutputWidth.Name = "cbOutputWidth";
            this.cbOutputWidth.Size = new System.Drawing.Size(106, 21);
            this.cbOutputWidth.TabIndex = 1;
            this.cbOutputWidth.SelectedIndexChanged += new System.EventHandler(this.cbOutputWidth_SelectedIndexChanged);
            // 
            // cbAspectRatio
            // 
            this.cbAspectRatio.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbAspectRatio.FormattingEnabled = true;
            this.cbAspectRatio.Items.AddRange(new object[] {
            "Keep Original",
            "4:3 (SD)",
            "16:9 (HD)"});
            this.cbAspectRatio.Location = new System.Drawing.Point(269, 19);
            this.cbAspectRatio.Name = "cbAspectRatio";
            this.cbAspectRatio.Size = new System.Drawing.Size(70, 21);
            this.cbAspectRatio.TabIndex = 6;
            this.cbAspectRatio.SelectedIndexChanged += new System.EventHandler(this.cbAspectRatio_SelectedIndexChanged);
            // 
            // frmRenderSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 498);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btCancel);
            this.Controls.Add(this.btOk);
            this.Name = "frmRenderSettings";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Video Settings";
            this.Load += new System.EventHandler(this.frmRenderSettings_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btOk;
        private System.Windows.Forms.Button btCancel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblResVidSize;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox cbOutputWidth;
        private System.Windows.Forms.ComboBox cbAspectRatio;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ComboBox cbEncoder;
        private System.Windows.Forms.Label label12;
        private customControls.pnlWmvControl pnlWmvControl;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btBrowseFile;
        private System.Windows.Forms.TextBox textBox1;
        private customControls.pnlRawEncCtrl pnlRawEncCtrl;
    }
}