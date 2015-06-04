namespace VideoDashboard
{
    partial class videoPlayer
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
            if (m_disposed)
            {
                throw new System.ObjectDisposedException("Object: videoPlayer already disposed!");
            }

            if (disposing && components != null)
            {
                CloseVideo();

                if (m_previewVidImage != null)
                {
                    m_previewVidImage.Dispose();
                    m_previewVidImage = null;
                }
                m_curVideoFile = null;

                if (timerVpos != null)
                {
                    timerVpos.Dispose();
                    timerVpos = null;
                }

                //                if (m_updateTimer != null)
                //                {
                ////                    m_updateTimer.Dispose();
                //                    m_updateTimer = null;
                //                }

                foreach (GotoVideoEndMarkerEventHandler h in m_gotoVidEndMarker.GetInvocationList())
                {
                    m_gotoVidEndMarker -= h;
                }
                foreach (GotoVideoStartMarkerEventHandler h in m_gotoVidStartMarker.GetInvocationList())
                {
                    m_gotoVidStartMarker -= h;
                }
                foreach (LatchDataEventHandler h in m_latchEvent.GetInvocationList())
                {
                    m_latchEvent -= h;
                }
                foreach (PauseEventHandler h in m_pauseEvent.GetInvocationList())
                {
                    m_pauseEvent -= h;
                }
                foreach (PlayEventHandler h in m_playEvent.GetInvocationList())
                {
                    m_playEvent -= h;
                }
                foreach (SetVideoEndFrameEventHandler h in m_setVidEndFrame.GetInvocationList())
                {
                    m_setVidEndFrame -= h;
                }
                foreach (SetVideoStartFrameEventHandler h in m_setVidStartFrame.GetInvocationList())
                {
                    m_setVidStartFrame -= h;
                }
                foreach (VideoUpdateEventHandler h in m_videoUpdateEvent.GetInvocationList())
                {
                    m_videoUpdateEvent -= h;
                }

                m_disposed = true;
            }
            base.Dispose(disposing);

            if (components != null)
                components.Dispose();

        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.videoDisplay = new System.Windows.Forms.Label();
            this.sliderVideoPos = new System.Windows.Forms.TrackBar();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.btSetVideoStartPos = new System.Windows.Forms.Button();
            this.btSetVideoEndPos = new System.Windows.Forms.Button();
            this.lblCurVideoPos = new System.Windows.Forms.Label();
            this.btGotoFrameEndMarker = new System.Windows.Forms.Button();
            this.btVideoPause = new System.Windows.Forms.Button();
            this.btVideoPlay = new System.Windows.Forms.Button();
            this.btGotoFrameStartMarker = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.sliderVideoPos)).BeginInit();
            this.SuspendLayout();
            // 
            // videoDisplay
            // 
            this.videoDisplay.BackColor = System.Drawing.Color.Black;
            this.videoDisplay.Location = new System.Drawing.Point(0, 0);
            this.videoDisplay.Margin = new System.Windows.Forms.Padding(0);
            this.videoDisplay.Name = "videoDisplay";
            this.videoDisplay.Size = new System.Drawing.Size(455, 256);
            this.videoDisplay.TabIndex = 0;
            this.videoDisplay.Text = "label1";
            this.videoDisplay.Paint += new System.Windows.Forms.PaintEventHandler(this.videoDisplay_Paint);
            // 
            // sliderVideoPos
            // 
            this.sliderVideoPos.AutoSize = false;
            this.sliderVideoPos.LargeChange = 25;
            this.sliderVideoPos.Location = new System.Drawing.Point(3, 259);
            this.sliderVideoPos.Margin = new System.Windows.Forms.Padding(0);
            this.sliderVideoPos.Maximum = 100;
            this.sliderVideoPos.Name = "sliderVideoPos";
            this.sliderVideoPos.Size = new System.Drawing.Size(449, 23);
            this.sliderVideoPos.TabIndex = 1;
            this.sliderVideoPos.TickStyle = System.Windows.Forms.TickStyle.None;
            this.sliderVideoPos.ValueChanged += new System.EventHandler(this.sliderVideoPos_ValueChanged);
            this.sliderVideoPos.Scroll += new System.EventHandler(this.sliderVideoPos_Scroll);
            this.sliderVideoPos.MouseDown += new System.Windows.Forms.MouseEventHandler(this.sliderVideoPos_MouseDown);
            this.sliderVideoPos.MouseUp += new System.Windows.Forms.MouseEventHandler(this.sliderVideoPos_MouseUp);
            this.sliderVideoPos.KeyDown += new System.Windows.Forms.KeyEventHandler(this.sliderVideoPos_KeyDown);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(6, 327);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(79, 17);
            this.checkBox1.TabIndex = 6;
            this.checkBox1.Text = "Latch Data";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // btSetVideoStartPos
            // 
            this.btSetVideoStartPos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btSetVideoStartPos.Location = new System.Drawing.Point(91, 322);
            this.btSetVideoStartPos.Name = "btSetVideoStartPos";
            this.btSetVideoStartPos.Size = new System.Drawing.Size(25, 25);
            this.btSetVideoStartPos.TabIndex = 23;
            this.btSetVideoStartPos.Text = "S";
            this.btSetVideoStartPos.UseVisualStyleBackColor = true;
            this.btSetVideoStartPos.Click += new System.EventHandler(this.btSetVideoStartPos_Click);
            // 
            // btSetVideoEndPos
            // 
            this.btSetVideoEndPos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btSetVideoEndPos.Location = new System.Drawing.Point(353, 322);
            this.btSetVideoEndPos.Name = "btSetVideoEndPos";
            this.btSetVideoEndPos.Size = new System.Drawing.Size(25, 25);
            this.btSetVideoEndPos.TabIndex = 24;
            this.btSetVideoEndPos.Text = "E";
            this.btSetVideoEndPos.UseVisualStyleBackColor = true;
            this.btSetVideoEndPos.Click += new System.EventHandler(this.btSetVideoEndPos_Click);
            // 
            // lblCurVideoPos
            // 
            this.lblCurVideoPos.Location = new System.Drawing.Point(204, 294);
            this.lblCurVideoPos.Name = "lblCurVideoPos";
            this.lblCurVideoPos.Size = new System.Drawing.Size(50, 15);
            this.lblCurVideoPos.TabIndex = 25;
            this.lblCurVideoPos.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btGotoFrameEndMarker
            // 
            this.btGotoFrameEndMarker.Image = global::VideoDashboard.Properties.Resources.lastframe;
            this.btGotoFrameEndMarker.Location = new System.Drawing.Point(250, 322);
            this.btGotoFrameEndMarker.Name = "btGotoFrameEndMarker";
            this.btGotoFrameEndMarker.Size = new System.Drawing.Size(25, 25);
            this.btGotoFrameEndMarker.TabIndex = 5;
            this.btGotoFrameEndMarker.UseVisualStyleBackColor = true;
            this.btGotoFrameEndMarker.Click += new System.EventHandler(this.btGotoFrameEndMarker_Click);
            // 
            // btVideoPause
            // 
            this.btVideoPause.Image = global::VideoDashboard.Properties.Resources.pause;
            this.btVideoPause.Location = new System.Drawing.Point(227, 322);
            this.btVideoPause.Name = "btVideoPause";
            this.btVideoPause.Size = new System.Drawing.Size(25, 25);
            this.btVideoPause.TabIndex = 4;
            this.btVideoPause.UseVisualStyleBackColor = true;
            this.btVideoPause.Click += new System.EventHandler(this.btVideoPause_Click);
            // 
            // btVideoPlay
            // 
            this.btVideoPlay.Image = global::VideoDashboard.Properties.Resources.play;
            this.btVideoPlay.Location = new System.Drawing.Point(204, 322);
            this.btVideoPlay.Name = "btVideoPlay";
            this.btVideoPlay.Size = new System.Drawing.Size(25, 25);
            this.btVideoPlay.TabIndex = 3;
            this.btVideoPlay.UseVisualStyleBackColor = true;
            this.btVideoPlay.Click += new System.EventHandler(this.btVideoPlay_Click);
            // 
            // btGotoFrameStartMarker
            // 
            this.btGotoFrameStartMarker.Image = global::VideoDashboard.Properties.Resources.firstframe;
            this.btGotoFrameStartMarker.Location = new System.Drawing.Point(181, 322);
            this.btGotoFrameStartMarker.Name = "btGotoFrameStartMarker";
            this.btGotoFrameStartMarker.Size = new System.Drawing.Size(25, 25);
            this.btGotoFrameStartMarker.TabIndex = 2;
            this.btGotoFrameStartMarker.UseVisualStyleBackColor = true;
            this.btGotoFrameStartMarker.Click += new System.EventHandler(this.btGotoFrameStartMarker_Click);
            // 
            // videoPlayer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lblCurVideoPos);
            this.Controls.Add(this.btSetVideoEndPos);
            this.Controls.Add(this.btSetVideoStartPos);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.btGotoFrameEndMarker);
            this.Controls.Add(this.btVideoPause);
            this.Controls.Add(this.btVideoPlay);
            this.Controls.Add(this.btGotoFrameStartMarker);
            this.Controls.Add(this.sliderVideoPos);
            this.Controls.Add(this.videoDisplay);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "videoPlayer";
            this.Size = new System.Drawing.Size(455, 350);
            this.Load += new System.EventHandler(this.videoPlayer_Load);
            ((System.ComponentModel.ISupportInitialize)(this.sliderVideoPos)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label videoDisplay;
        private System.Windows.Forms.TrackBar sliderVideoPos;
        private System.Windows.Forms.Button btGotoFrameStartMarker;
        private System.Windows.Forms.Button btVideoPlay;
        private System.Windows.Forms.Button btVideoPause;
        private System.Windows.Forms.Button btGotoFrameEndMarker;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Button btSetVideoStartPos;
        private System.Windows.Forms.Button btSetVideoEndPos;
        private System.Windows.Forms.Label lblCurVideoPos;
    }
}
