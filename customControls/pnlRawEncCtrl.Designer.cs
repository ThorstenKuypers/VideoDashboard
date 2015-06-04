namespace customControls
{
    partial class pnlRawEncCtrl
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lblInfo = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btConfigCompressor = new System.Windows.Forms.Button();
            this.cbCompressorList = new System.Windows.Forms.ComboBox();
            this.lblTargetFrameRate = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lblTargetHeight = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lblTargetWidth = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btApply = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lblInfo);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.lblTargetFrameRate);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.lblTargetHeight);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.lblTargetWidth);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(319, 200);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            // 
            // lblInfo
            // 
            this.lblInfo.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblInfo.Location = new System.Drawing.Point(6, 105);
            this.lblInfo.Name = "lblInfo";
            this.lblInfo.Size = new System.Drawing.Size(307, 92);
            this.lblInfo.TabIndex = 7;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btConfigCompressor);
            this.groupBox2.Controls.Add(this.cbCompressorList);
            this.groupBox2.Location = new System.Drawing.Point(6, 48);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(307, 54);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Compressor";
            // 
            // btConfigCompressor
            // 
            this.btConfigCompressor.Location = new System.Drawing.Point(241, 19);
            this.btConfigCompressor.Name = "btConfigCompressor";
            this.btConfigCompressor.Size = new System.Drawing.Size(60, 23);
            this.btConfigCompressor.TabIndex = 7;
            this.btConfigCompressor.Text = "Configure";
            this.btConfigCompressor.UseVisualStyleBackColor = true;
            this.btConfigCompressor.Click += new System.EventHandler(this.btConfigCompressor_Click);
            // 
            // cbCompressorList
            // 
            this.cbCompressorList.FormattingEnabled = true;
            this.cbCompressorList.Location = new System.Drawing.Point(6, 19);
            this.cbCompressorList.Name = "cbCompressorList";
            this.cbCompressorList.Size = new System.Drawing.Size(229, 21);
            this.cbCompressorList.TabIndex = 6;
            this.cbCompressorList.SelectedIndexChanged += new System.EventHandler(this.cbCompressorList_SelectedIndexChanged);
            // 
            // lblTargetFrameRate
            // 
            this.lblTargetFrameRate.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTargetFrameRate.Location = new System.Drawing.Point(260, 17);
            this.lblTargetFrameRate.Name = "lblTargetFrameRate";
            this.lblTargetFrameRate.Size = new System.Drawing.Size(25, 15);
            this.lblTargetFrameRate.TabIndex = 5;
            this.lblTargetFrameRate.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(189, 16);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Frame-Rate:";
            // 
            // lblTargetHeight
            // 
            this.lblTargetHeight.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTargetHeight.Location = new System.Drawing.Point(138, 16);
            this.lblTargetHeight.Name = "lblTargetHeight";
            this.lblTargetHeight.Size = new System.Drawing.Size(35, 15);
            this.lblTargetHeight.TabIndex = 3;
            this.lblTargetHeight.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(91, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Height:";
            // 
            // lblTargetWidth
            // 
            this.lblTargetWidth.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTargetWidth.Location = new System.Drawing.Point(50, 16);
            this.lblTargetWidth.Name = "lblTargetWidth";
            this.lblTargetWidth.Size = new System.Drawing.Size(35, 15);
            this.lblTargetWidth.TabIndex = 1;
            this.lblTargetWidth.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Width:";
            // 
            // btApply
            // 
            this.btApply.Location = new System.Drawing.Point(247, 209);
            this.btApply.Name = "btApply";
            this.btApply.Size = new System.Drawing.Size(75, 23);
            this.btApply.TabIndex = 7;
            this.btApply.Text = "Apply";
            this.btApply.UseVisualStyleBackColor = true;
            this.btApply.Visible = false;
            // 
            // pnlRawEncCtrl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.btApply);
            this.Controls.Add(this.groupBox1);
            this.Name = "pnlRawEncCtrl";
            this.Size = new System.Drawing.Size(333, 237);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label lblTargetFrameRate;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lblTargetHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblTargetWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btApply;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btConfigCompressor;
        private System.Windows.Forms.ComboBox cbCompressorList;
        private System.Windows.Forms.Label lblInfo;
    }
}
