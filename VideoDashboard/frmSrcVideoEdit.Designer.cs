namespace VideoDashboard
{
    partial class frmSrcVideoEdit
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btVidInfo = new System.Windows.Forms.Button();
            this.btMoveVideoUp = new System.Windows.Forms.Button();
            this.btMoveVideoDown = new System.Windows.Forms.Button();
            this.btRemoveVideo = new System.Windows.Forms.Button();
            this.btAddVideo = new System.Windows.Forms.Button();
            this.lvSrcVideoFiles = new System.Windows.Forms.ListView();
            this.chFileName = new System.Windows.Forms.ColumnHeader();
            this.chDuration = new System.Windows.Forms.ColumnHeader();
            this.chFileSize = new System.Windows.Forms.ColumnHeader();
            this.btCancel = new System.Windows.Forms.Button();
            this.btOk = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btVidInfo);
            this.groupBox1.Controls.Add(this.btMoveVideoUp);
            this.groupBox1.Controls.Add(this.btMoveVideoDown);
            this.groupBox1.Controls.Add(this.btRemoveVideo);
            this.groupBox1.Controls.Add(this.btAddVideo);
            this.groupBox1.Controls.Add(this.lvSrcVideoFiles);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(388, 225);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Source Video(s)";
            // 
            // btVidInfo
            // 
            this.btVidInfo.Location = new System.Drawing.Point(357, 162);
            this.btVidInfo.Name = "btVidInfo";
            this.btVidInfo.Size = new System.Drawing.Size(25, 25);
            this.btVidInfo.TabIndex = 5;
            this.btVidInfo.Text = "i";
            this.btVidInfo.UseVisualStyleBackColor = true;
            this.btVidInfo.Click += new System.EventHandler(this.btVidInfo_Click);
            // 
            // btMoveVideoUp
            // 
            this.btMoveVideoUp.Location = new System.Drawing.Point(357, 81);
            this.btMoveVideoUp.Name = "btMoveVideoUp";
            this.btMoveVideoUp.Size = new System.Drawing.Size(25, 25);
            this.btMoveVideoUp.TabIndex = 4;
            this.btMoveVideoUp.Text = "^";
            this.btMoveVideoUp.UseVisualStyleBackColor = true;
            // 
            // btMoveVideoDown
            // 
            this.btMoveVideoDown.Location = new System.Drawing.Point(357, 112);
            this.btMoveVideoDown.Name = "btMoveVideoDown";
            this.btMoveVideoDown.Size = new System.Drawing.Size(25, 25);
            this.btMoveVideoDown.TabIndex = 3;
            this.btMoveVideoDown.Text = "v";
            this.btMoveVideoDown.UseVisualStyleBackColor = true;
            // 
            // btRemoveVideo
            // 
            this.btRemoveVideo.Location = new System.Drawing.Point(357, 50);
            this.btRemoveVideo.Name = "btRemoveVideo";
            this.btRemoveVideo.Size = new System.Drawing.Size(25, 25);
            this.btRemoveVideo.TabIndex = 2;
            this.btRemoveVideo.Text = "-";
            this.btRemoveVideo.UseVisualStyleBackColor = true;
            this.btRemoveVideo.Click += new System.EventHandler(this.btRemoveVideo_Click);
            // 
            // btAddVideo
            // 
            this.btAddVideo.Location = new System.Drawing.Point(357, 19);
            this.btAddVideo.Name = "btAddVideo";
            this.btAddVideo.Size = new System.Drawing.Size(25, 25);
            this.btAddVideo.TabIndex = 1;
            this.btAddVideo.Text = "+";
            this.btAddVideo.UseVisualStyleBackColor = true;
            this.btAddVideo.Click += new System.EventHandler(this.btAddVideo_Click);
            // 
            // lvSrcVideoFiles
            // 
            this.lvSrcVideoFiles.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.chFileName,
            this.chDuration,
            this.chFileSize});
            this.lvSrcVideoFiles.FullRowSelect = true;
            this.lvSrcVideoFiles.GridLines = true;
            this.lvSrcVideoFiles.Location = new System.Drawing.Point(6, 19);
            this.lvSrcVideoFiles.Name = "lvSrcVideoFiles";
            this.lvSrcVideoFiles.Size = new System.Drawing.Size(345, 200);
            this.lvSrcVideoFiles.TabIndex = 0;
            this.lvSrcVideoFiles.UseCompatibleStateImageBehavior = false;
            this.lvSrcVideoFiles.View = System.Windows.Forms.View.Details;
            // 
            // chFileName
            // 
            this.chFileName.Text = "Filename";
            this.chFileName.Width = 220;
            // 
            // chDuration
            // 
            this.chDuration.Text = "Duration";
            this.chDuration.Width = 62;
            // 
            // chFileSize
            // 
            this.chFileSize.Text = "Size";
            this.chFileSize.Width = 57;
            // 
            // btCancel
            // 
            this.btCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btCancel.Location = new System.Drawing.Point(12, 243);
            this.btCancel.Name = "btCancel";
            this.btCancel.Size = new System.Drawing.Size(75, 23);
            this.btCancel.TabIndex = 1;
            this.btCancel.Text = "Cancel";
            this.btCancel.UseVisualStyleBackColor = true;
            // 
            // btOk
            // 
            this.btOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btOk.Location = new System.Drawing.Point(319, 243);
            this.btOk.Name = "btOk";
            this.btOk.Size = new System.Drawing.Size(75, 23);
            this.btOk.TabIndex = 2;
            this.btOk.Text = "OK";
            this.btOk.UseVisualStyleBackColor = true;
            this.btOk.Click += new System.EventHandler(this.btOk_Click);
            // 
            // frmSrcVideoEdit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(782, 278);
            this.Controls.Add(this.btOk);
            this.Controls.Add(this.btCancel);
            this.Controls.Add(this.groupBox1);
            this.Name = "frmSrcVideoEdit";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "frmSrcVideoEdit";
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListView lvSrcVideoFiles;
        private System.Windows.Forms.ColumnHeader chFileName;
        private System.Windows.Forms.ColumnHeader chDuration;
        private System.Windows.Forms.ColumnHeader chFileSize;
        private System.Windows.Forms.Button btRemoveVideo;
        private System.Windows.Forms.Button btAddVideo;
        private System.Windows.Forms.Button btMoveVideoUp;
        private System.Windows.Forms.Button btMoveVideoDown;
        private System.Windows.Forms.Button btCancel;
        private System.Windows.Forms.Button btOk;
        private System.Windows.Forms.Button btVidInfo;
    }
}