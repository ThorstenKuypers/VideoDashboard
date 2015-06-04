namespace customControls
{
    partial class pnlWmvControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(pnlWmvControl));
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btSaveProfile = new System.Windows.Forms.Button();
            this.btLoadProfile = new System.Windows.Forms.Button();
            this.btDelProfile = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.lblQuality = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.label7 = new System.Windows.Forms.Label();
            this.tbKeyframe = new System.Windows.Forms.TextBox();
            this.tbBitrate = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.lblTargetFrameRate = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lblTargetHeight = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lblTargetWidth = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(51, 13);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(196, 21);
            this.comboBox1.TabIndex = 0;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(39, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Profile:";
            // 
            // btSaveProfile
            // 
            this.btSaveProfile.Image = ((System.Drawing.Image)(resources.GetObject("btSaveProfile.Image")));
            this.btSaveProfile.Location = new System.Drawing.Point(253, 13);
            this.btSaveProfile.Name = "btSaveProfile";
            this.btSaveProfile.Size = new System.Drawing.Size(21, 21);
            this.btSaveProfile.TabIndex = 2;
            this.btSaveProfile.UseVisualStyleBackColor = true;
            this.btSaveProfile.Visible = false;
            // 
            // btLoadProfile
            // 
            this.btLoadProfile.Image = ((System.Drawing.Image)(resources.GetObject("btLoadProfile.Image")));
            this.btLoadProfile.Location = new System.Drawing.Point(272, 13);
            this.btLoadProfile.Name = "btLoadProfile";
            this.btLoadProfile.Size = new System.Drawing.Size(21, 21);
            this.btLoadProfile.TabIndex = 3;
            this.btLoadProfile.UseVisualStyleBackColor = true;
            this.btLoadProfile.Visible = false;
            // 
            // btDelProfile
            // 
            this.btDelProfile.Image = ((System.Drawing.Image)(resources.GetObject("btDelProfile.Image")));
            this.btDelProfile.Location = new System.Drawing.Point(291, 13);
            this.btDelProfile.Name = "btDelProfile";
            this.btDelProfile.Size = new System.Drawing.Size(21, 21);
            this.btDelProfile.TabIndex = 4;
            this.btDelProfile.UseVisualStyleBackColor = true;
            this.btDelProfile.Visible = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.lblQuality);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.trackBar1);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.tbKeyframe);
            this.groupBox1.Controls.Add(this.tbBitrate);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.lblTargetFrameRate);
            this.groupBox1.Controls.Add(this.btDelProfile);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.btLoadProfile);
            this.groupBox1.Controls.Add(this.lblTargetHeight);
            this.groupBox1.Controls.Add(this.btSaveProfile);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.comboBox1);
            this.groupBox1.Controls.Add(this.lblTargetWidth);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(6, 34);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(319, 200);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(122, 90);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(30, 13);
            this.label9.TabIndex = 14;
            this.label9.Text = "kbps";
            // 
            // lblQuality
            // 
            this.lblQuality.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblQuality.Location = new System.Drawing.Point(132, 169);
            this.lblQuality.Name = "lblQuality";
            this.lblQuality.Size = new System.Drawing.Size(39, 15);
            this.lblQuality.TabIndex = 13;
            this.lblQuality.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(132, 124);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(39, 13);
            this.label8.TabIndex = 12;
            this.label8.Text = "Quality";
            // 
            // trackBar1
            // 
            this.trackBar1.AutoSize = false;
            this.trackBar1.Location = new System.Drawing.Point(73, 140);
            this.trackBar1.Maximum = 100;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(155, 26);
            this.trackBar1.SmallChange = 5;
            this.trackBar1.TabIndex = 11;
            this.trackBar1.TickFrequency = 50;
            this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar1.Value = 50;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(300, 90);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(12, 13);
            this.label7.TabIndex = 10;
            this.label7.Text = "s";
            // 
            // tbKeyframe
            // 
            this.tbKeyframe.Location = new System.Drawing.Point(244, 87);
            this.tbKeyframe.Name = "tbKeyframe";
            this.tbKeyframe.Size = new System.Drawing.Size(50, 20);
            this.tbKeyframe.TabIndex = 9;
            // 
            // tbBitrate
            // 
            this.tbBitrate.Location = new System.Drawing.Point(51, 87);
            this.tbBitrate.Name = "tbBitrate";
            this.tbBitrate.Size = new System.Drawing.Size(65, 20);
            this.tbBitrate.TabIndex = 8;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(179, 90);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(59, 13);
            this.label6.TabIndex = 7;
            this.label6.Text = "Keyframes:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(5, 90);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Bitrate:";
            // 
            // lblTargetFrameRate
            // 
            this.lblTargetFrameRate.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTargetFrameRate.Location = new System.Drawing.Point(273, 53);
            this.lblTargetFrameRate.Name = "lblTargetFrameRate";
            this.lblTargetFrameRate.Size = new System.Drawing.Size(40, 15);
            this.lblTargetFrameRate.TabIndex = 5;
            this.lblTargetFrameRate.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(202, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Frame-Rate:";
            // 
            // lblTargetHeight
            // 
            this.lblTargetHeight.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTargetHeight.Location = new System.Drawing.Point(156, 54);
            this.lblTargetHeight.Name = "lblTargetHeight";
            this.lblTargetHeight.Size = new System.Drawing.Size(40, 15);
            this.lblTargetHeight.TabIndex = 3;
            this.lblTargetHeight.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(109, 54);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Height:";
            // 
            // lblTargetWidth
            // 
            this.lblTargetWidth.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTargetWidth.Location = new System.Drawing.Point(51, 54);
            this.lblTargetWidth.Name = "lblTargetWidth";
            this.lblTargetWidth.Size = new System.Drawing.Size(40, 15);
            this.lblTargetWidth.TabIndex = 1;
            this.lblTargetWidth.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Width:";
            // 
            // pnlWmvControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "pnlWmvControl";
            this.Size = new System.Drawing.Size(333, 237);
            this.Load += new System.EventHandler(this.pnlWmvControl_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btSaveProfile;
        private System.Windows.Forms.Button btLoadProfile;
        private System.Windows.Forms.Button btDelProfile;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblTargetWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblTargetHeight;
        private System.Windows.Forms.Label lblTargetFrameRate;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox tbBitrate;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbKeyframe;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Label lblQuality;
        private System.Windows.Forms.Label label9;
    }
}
