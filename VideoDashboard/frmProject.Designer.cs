namespace VideoDashboard
{
    partial class frmProject
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
                throw new System.ObjectDisposedException("Object: frmProject already disposed!");
            }

            if (disposing && components !=null)
            {
                if (m_curActiveDashboard != null)
                {
                    m_curActiveDashboard.Dispose();
                    m_curActiveDashboard = null;
                }
                if (m_videoPlayer != null)
                {
                    m_videoPlayer.Dispose();
                    m_videoPlayer = null;
                }
                if (m_dataLog != null)
                {
                    m_dataLog.Dispose();
                    m_dataLog = null;
                }

                m_lapsArray = null;
                m_dashLayoutParser = null;

                if (m_frmRender != null)
                {
                    m_frmRender.Dispose();
                    m_frmRender = null;
                }
                if (m_dshowEvent != null)
                {
                    foreach (DShowEventHandler h in m_dshowEvent.GetInvocationList())
                    {
                        m_dshowEvent -= h;
                    }
                }

                if (m_closeProjectEvent != null)
                {
                    foreach (CloseProjectFormEventHandler h in m_closeProjectEvent.GetInvocationList())
                    {
                        m_closeProjectEvent -= h;
                    }
                }
            }
            base.Dispose(disposing);

            if (components != null)
                components.Dispose();
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.MainPanelTable = new System.Windows.Forms.TableLayoutPanel();
            this.pnlInputVideoFile = new System.Windows.Forms.Panel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btShowInVideoInfo = new System.Windows.Forms.Button();
            this.btOpenVideoFile = new System.Windows.Forms.Button();
            this.tbVideoFileName = new System.Windows.Forms.TextBox();
            this.pnlDataLogger = new System.Windows.Forms.Panel();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btDetailedSessionInfo = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.lblCurDataPos = new System.Windows.Forms.Label();
            this.lblCurLapPercent = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.lblGear = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.btSetDataEndPos = new System.Windows.Forms.Button();
            this.lblSessionTime = new System.Windows.Forms.Label();
            this.btSetDataStartPos = new System.Windows.Forms.Button();
            this.lblLongG = new System.Windows.Forms.Label();
            this.lblLatG = new System.Windows.Forms.Label();
            this.lblRpm = new System.Windows.Forms.Label();
            this.lblSpeed = new System.Windows.Forms.Label();
            this.lblCurLap = new System.Windows.Forms.Label();
            this.sliderDataViewerPos = new System.Windows.Forms.TrackBar();
            this.label22 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.btGotoSampleEndMarker = new System.Windows.Forms.Button();
            this.btDataViewerPause = new System.Windows.Forms.Button();
            this.btDataViewerPlay = new System.Windows.Forms.Button();
            this.btGotoSampleStartMarker = new System.Windows.Forms.Button();
            this.btOpenDataLogFile = new System.Windows.Forms.Button();
            this.tbDataLogFile = new System.Windows.Forms.TextBox();
            this.m_videoPlayer = new VideoDashboard.videoPlayer();
            this.pnlOverlayConfig = new System.Windows.Forms.Panel();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.OverlayConfigCtxMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.resetOverlaySettings = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.sliderOverlayScale = new System.Windows.Forms.TrackBar();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.sliderTransparency = new System.Windows.Forms.TrackBar();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.lblPosY = new System.Windows.Forms.Label();
            this.lblPosX = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.sliderOverlayPosY = new System.Windows.Forms.TrackBar();
            this.sliderOverlayPosX = new System.Windows.Forms.TrackBar();
            this.pnlLapInfo = new System.Windows.Forms.Panel();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.lblCurLapTime = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.btNextLap = new System.Windows.Forms.Button();
            this.btPrevLap = new System.Windows.Forms.Button();
            this.btSectionMarkers = new System.Windows.Forms.Button();
            this.btLapTimingDetail = new System.Windows.Forms.Button();
            this.lblFastestLapTime = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.lblTotalLaps = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.lblCurLapIdx = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lblFastestLap = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.pnlOverlay = new System.Windows.Forms.Panel();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.chkBxEnableOverlayPreview = new System.Windows.Forms.CheckBox();
            this.cbOverlayList = new System.Windows.Forms.ComboBox();
            this.pnlProject = new System.Windows.Forms.Panel();
            this.groupBox10 = new System.Windows.Forms.GroupBox();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.lblProjectNotes = new System.Windows.Forms.Label();
            this.lblProjectName = new System.Windows.Forms.Label();
            this.pnlVideoDataSync = new System.Windows.Forms.Panel();
            this.groupBox11 = new System.Windows.Forms.GroupBox();
            this.lblOutVidDataLength = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.lblOutVidLength = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.lblSyncEndMarkerSample = new System.Windows.Forms.Label();
            this.lblSyncStartMarkerSample = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.lblSyncEndMarkerFrame = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.lblSyncStartMarkerFrame = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.pnlProgress = new System.Windows.Forms.Panel();
            this.grpBoxProgress = new System.Windows.Forms.GroupBox();
            this.btCancelRenderer = new System.Windows.Forms.Button();
            this.lblProgressPercent = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.lblProgressFrames = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.lblEstimatedTime = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.lblElapsedTime = new System.Windows.Forms.Label();
            this.dataViewerTickTimer = new System.Windows.Forms.Timer(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.MainPanelTable.SuspendLayout();
            this.pnlInputVideoFile.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.pnlDataLogger.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderDataViewerPos)).BeginInit();
            this.pnlOverlayConfig.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.OverlayConfigCtxMenu.SuspendLayout();
            this.groupBox9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderOverlayScale)).BeginInit();
            this.groupBox8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderTransparency)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderOverlayPosY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sliderOverlayPosX)).BeginInit();
            this.pnlLapInfo.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.pnlOverlay.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.pnlProject.SuspendLayout();
            this.groupBox10.SuspendLayout();
            this.groupBox12.SuspendLayout();
            this.pnlVideoDataSync.SuspendLayout();
            this.groupBox11.SuspendLayout();
            this.pnlProgress.SuspendLayout();
            this.grpBoxProgress.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainPanelTable
            // 
            this.MainPanelTable.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.MainPanelTable.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.MainPanelTable.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.MainPanelTable.ColumnCount = 3;
            this.MainPanelTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 28.07018F));
            this.MainPanelTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 42.1976F));
            this.MainPanelTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 29.73223F));
            this.MainPanelTable.Controls.Add(this.pnlInputVideoFile, 0, 0);
            this.MainPanelTable.Controls.Add(this.pnlDataLogger, 0, 1);
            this.MainPanelTable.Controls.Add(this.m_videoPlayer, 1, 1);
            this.MainPanelTable.Controls.Add(this.pnlOverlayConfig, 2, 1);
            this.MainPanelTable.Controls.Add(this.pnlLapInfo, 0, 2);
            this.MainPanelTable.Controls.Add(this.pnlOverlay, 2, 0);
            this.MainPanelTable.Controls.Add(this.pnlProject, 1, 0);
            this.MainPanelTable.Controls.Add(this.pnlVideoDataSync, 1, 2);
            this.MainPanelTable.Controls.Add(this.pnlProgress, 2, 2);
            this.MainPanelTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainPanelTable.Location = new System.Drawing.Point(0, 0);
            this.MainPanelTable.Margin = new System.Windows.Forms.Padding(0);
            this.MainPanelTable.Name = "MainPanelTable";
            this.MainPanelTable.RowCount = 3;
            this.MainPanelTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20.91503F));
            this.MainPanelTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 57.35294F));
            this.MainPanelTable.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 21.40523F));
            this.MainPanelTable.Size = new System.Drawing.Size(1088, 613);
            this.MainPanelTable.TabIndex = 0;
            // 
            // pnlInputVideoFile
            // 
            this.pnlInputVideoFile.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.pnlInputVideoFile.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.pnlInputVideoFile.Controls.Add(this.groupBox1);
            this.pnlInputVideoFile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlInputVideoFile.Location = new System.Drawing.Point(1, 1);
            this.pnlInputVideoFile.Margin = new System.Windows.Forms.Padding(0);
            this.pnlInputVideoFile.Name = "pnlInputVideoFile";
            this.pnlInputVideoFile.Size = new System.Drawing.Size(304, 127);
            this.pnlInputVideoFile.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox1.Controls.Add(this.btShowInVideoInfo);
            this.groupBox1.Controls.Add(this.btOpenVideoFile);
            this.groupBox1.Controls.Add(this.tbVideoFileName);
            this.groupBox1.Location = new System.Drawing.Point(9, 9);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(282, 115);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Video File";
            // 
            // btShowInVideoInfo
            // 
            this.btShowInVideoInfo.Location = new System.Drawing.Point(6, 48);
            this.btShowInVideoInfo.Name = "btShowInVideoInfo";
            this.btShowInVideoInfo.Size = new System.Drawing.Size(75, 23);
            this.btShowInVideoInfo.TabIndex = 2;
            this.btShowInVideoInfo.Text = "Video Info";
            this.btShowInVideoInfo.UseVisualStyleBackColor = true;
            this.btShowInVideoInfo.Visible = false;
            // 
            // btOpenVideoFile
            // 
            this.btOpenVideoFile.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.btOpenVideoFile.Location = new System.Drawing.Point(201, 45);
            this.btOpenVideoFile.Name = "btOpenVideoFile";
            this.btOpenVideoFile.Size = new System.Drawing.Size(75, 23);
            this.btOpenVideoFile.TabIndex = 1;
            this.btOpenVideoFile.Text = "Open";
            this.btOpenVideoFile.UseVisualStyleBackColor = true;
            this.btOpenVideoFile.Click += new System.EventHandler(this.btOpenVideoFile_Click);
            // 
            // tbVideoFileName
            // 
            this.tbVideoFileName.Location = new System.Drawing.Point(6, 19);
            this.tbVideoFileName.Name = "tbVideoFileName";
            this.tbVideoFileName.Size = new System.Drawing.Size(270, 20);
            this.tbVideoFileName.TabIndex = 0;
            // 
            // pnlDataLogger
            // 
            this.pnlDataLogger.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.pnlDataLogger.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.pnlDataLogger.Controls.Add(this.groupBox2);
            this.pnlDataLogger.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlDataLogger.Location = new System.Drawing.Point(1, 129);
            this.pnlDataLogger.Margin = new System.Windows.Forms.Padding(0);
            this.pnlDataLogger.Name = "pnlDataLogger";
            this.pnlDataLogger.Size = new System.Drawing.Size(304, 350);
            this.pnlDataLogger.TabIndex = 1;
            // 
            // groupBox2
            // 
            this.groupBox2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox2.Controls.Add(this.btDetailedSessionInfo);
            this.groupBox2.Controls.Add(this.groupBox3);
            this.groupBox2.Controls.Add(this.btOpenDataLogFile);
            this.groupBox2.Controls.Add(this.tbDataLogFile);
            this.groupBox2.Location = new System.Drawing.Point(9, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(282, 318);
            this.groupBox2.TabIndex = 0;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Data Viewer";
            // 
            // btDetailedSessionInfo
            // 
            this.btDetailedSessionInfo.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.btDetailedSessionInfo.Location = new System.Drawing.Point(6, 45);
            this.btDetailedSessionInfo.Name = "btDetailedSessionInfo";
            this.btDetailedSessionInfo.Size = new System.Drawing.Size(70, 25);
            this.btDetailedSessionInfo.TabIndex = 4;
            this.btDetailedSessionInfo.Text = "Sesion Info";
            this.btDetailedSessionInfo.UseVisualStyleBackColor = true;
            this.btDetailedSessionInfo.Visible = false;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.lblCurDataPos);
            this.groupBox3.Controls.Add(this.lblCurLapPercent);
            this.groupBox3.Controls.Add(this.label26);
            this.groupBox3.Controls.Add(this.lblGear);
            this.groupBox3.Controls.Add(this.label25);
            this.groupBox3.Controls.Add(this.btSetDataEndPos);
            this.groupBox3.Controls.Add(this.lblSessionTime);
            this.groupBox3.Controls.Add(this.btSetDataStartPos);
            this.groupBox3.Controls.Add(this.lblLongG);
            this.groupBox3.Controls.Add(this.lblLatG);
            this.groupBox3.Controls.Add(this.lblRpm);
            this.groupBox3.Controls.Add(this.lblSpeed);
            this.groupBox3.Controls.Add(this.lblCurLap);
            this.groupBox3.Controls.Add(this.sliderDataViewerPos);
            this.groupBox3.Controls.Add(this.label22);
            this.groupBox3.Controls.Add(this.label20);
            this.groupBox3.Controls.Add(this.label19);
            this.groupBox3.Controls.Add(this.label17);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Controls.Add(this.btGotoSampleEndMarker);
            this.groupBox3.Controls.Add(this.btDataViewerPause);
            this.groupBox3.Controls.Add(this.btDataViewerPlay);
            this.groupBox3.Controls.Add(this.btGotoSampleStartMarker);
            this.groupBox3.Location = new System.Drawing.Point(6, 74);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(270, 238);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            // 
            // lblCurDataPos
            // 
            this.lblCurDataPos.Location = new System.Drawing.Point(114, 163);
            this.lblCurDataPos.Name = "lblCurDataPos";
            this.lblCurDataPos.Size = new System.Drawing.Size(60, 15);
            this.lblCurDataPos.TabIndex = 28;
            this.lblCurDataPos.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblCurLapPercent
            // 
            this.lblCurLapPercent.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblCurLapPercent.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCurLapPercent.Location = new System.Drawing.Point(213, 128);
            this.lblCurLapPercent.Name = "lblCurLapPercent";
            this.lblCurLapPercent.Size = new System.Drawing.Size(53, 15);
            this.lblCurLapPercent.TabIndex = 27;
            this.lblCurLapPercent.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(134, 130);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(73, 13);
            this.label26.TabIndex = 26;
            this.label26.Text = "Current Lap-%";
            // 
            // lblGear
            // 
            this.lblGear.Font = new System.Drawing.Font("Microsoft Sans Serif", 25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGear.Location = new System.Drawing.Point(125, 41);
            this.lblGear.Name = "lblGear";
            this.lblGear.Size = new System.Drawing.Size(40, 40);
            this.lblGear.TabIndex = 25;
            this.lblGear.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label25.Location = new System.Drawing.Point(3, 16);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(93, 17);
            this.label25.TabIndex = 24;
            this.label25.Text = "Session Time";
            // 
            // btSetDataEndPos
            // 
            this.btSetDataEndPos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btSetDataEndPos.Location = new System.Drawing.Point(238, 207);
            this.btSetDataEndPos.Name = "btSetDataEndPos";
            this.btSetDataEndPos.Size = new System.Drawing.Size(25, 25);
            this.btSetDataEndPos.TabIndex = 23;
            this.btSetDataEndPos.Text = "E";
            this.btSetDataEndPos.UseVisualStyleBackColor = true;
            this.btSetDataEndPos.Click += new System.EventHandler(this.btSetDataEndPos_Click);
            // 
            // lblSessionTime
            // 
            this.lblSessionTime.BackColor = System.Drawing.Color.Black;
            this.lblSessionTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSessionTime.ForeColor = System.Drawing.Color.Red;
            this.lblSessionTime.Location = new System.Drawing.Point(106, 12);
            this.lblSessionTime.Name = "lblSessionTime";
            this.lblSessionTime.Size = new System.Drawing.Size(120, 22);
            this.lblSessionTime.TabIndex = 11;
            this.lblSessionTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btSetDataStartPos
            // 
            this.btSetDataStartPos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btSetDataStartPos.Location = new System.Drawing.Point(6, 207);
            this.btSetDataStartPos.Name = "btSetDataStartPos";
            this.btSetDataStartPos.Size = new System.Drawing.Size(25, 25);
            this.btSetDataStartPos.TabIndex = 22;
            this.btSetDataStartPos.Text = "S";
            this.btSetDataStartPos.UseVisualStyleBackColor = true;
            this.btSetDataStartPos.Click += new System.EventHandler(this.btSetDataStartPos_Click);
            // 
            // lblLongG
            // 
            this.lblLongG.BackColor = System.Drawing.Color.White;
            this.lblLongG.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblLongG.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLongG.Location = new System.Drawing.Point(193, 90);
            this.lblLongG.Name = "lblLongG";
            this.lblLongG.Size = new System.Drawing.Size(71, 20);
            this.lblLongG.TabIndex = 21;
            this.lblLongG.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblLatG
            // 
            this.lblLatG.BackColor = System.Drawing.Color.White;
            this.lblLatG.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblLatG.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLatG.Location = new System.Drawing.Point(55, 91);
            this.lblLatG.Name = "lblLatG";
            this.lblLatG.Size = new System.Drawing.Size(71, 20);
            this.lblLatG.TabIndex = 20;
            this.lblLatG.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblRpm
            // 
            this.lblRpm.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRpm.Location = new System.Drawing.Point(208, 41);
            this.lblRpm.Name = "lblRpm";
            this.lblRpm.Size = new System.Drawing.Size(58, 20);
            this.lblRpm.TabIndex = 19;
            this.lblRpm.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblSpeed
            // 
            this.lblSpeed.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.lblSpeed.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSpeed.Location = new System.Drawing.Point(61, 42);
            this.lblSpeed.Name = "lblSpeed";
            this.lblSpeed.Size = new System.Drawing.Size(58, 20);
            this.lblSpeed.TabIndex = 18;
            this.lblSpeed.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblCurLap
            // 
            this.lblCurLap.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblCurLap.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCurLap.Location = new System.Drawing.Point(77, 124);
            this.lblCurLap.Name = "lblCurLap";
            this.lblCurLap.Size = new System.Drawing.Size(25, 25);
            this.lblCurLap.TabIndex = 17;
            this.lblCurLap.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // sliderDataViewerPos
            // 
            this.sliderDataViewerPos.AutoSize = false;
            this.sliderDataViewerPos.Location = new System.Drawing.Point(6, 181);
            this.sliderDataViewerPos.Name = "sliderDataViewerPos";
            this.sliderDataViewerPos.Size = new System.Drawing.Size(258, 20);
            this.sliderDataViewerPos.TabIndex = 10;
            this.sliderDataViewerPos.TickStyle = System.Windows.Forms.TickStyle.None;
            this.sliderDataViewerPos.ValueChanged += new System.EventHandler(this.sliderDataViewerPos_ValueChanged);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(6, 130);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(65, 13);
            this.label22.TabIndex = 16;
            this.label22.Text = "Current Lap:";
            this.label22.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label20.Location = new System.Drawing.Point(134, 92);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(55, 17);
            this.label20.TabIndex = 15;
            this.label20.Text = "Long G";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label19.Location = new System.Drawing.Point(6, 92);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(43, 17);
            this.label19.TabIndex = 14;
            this.label19.Text = "Lat G";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(164, 43);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(38, 17);
            this.label17.TabIndex = 13;
            this.label17.Text = "RPM";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(6, 43);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(49, 17);
            this.label15.TabIndex = 12;
            this.label15.Text = "Speed";
            // 
            // btGotoSampleEndMarker
            // 
            this.btGotoSampleEndMarker.Image = global::VideoDashboard.Properties.Resources.lastframe;
            this.btGotoSampleEndMarker.Location = new System.Drawing.Point(160, 207);
            this.btGotoSampleEndMarker.Name = "btGotoSampleEndMarker";
            this.btGotoSampleEndMarker.Size = new System.Drawing.Size(25, 25);
            this.btGotoSampleEndMarker.TabIndex = 9;
            this.btGotoSampleEndMarker.UseVisualStyleBackColor = true;
            this.btGotoSampleEndMarker.Click += new System.EventHandler(this.btGotoSampleEndMarker_Click);
            // 
            // btDataViewerPause
            // 
            this.btDataViewerPause.Image = global::VideoDashboard.Properties.Resources.pause;
            this.btDataViewerPause.Location = new System.Drawing.Point(137, 207);
            this.btDataViewerPause.Name = "btDataViewerPause";
            this.btDataViewerPause.Size = new System.Drawing.Size(25, 25);
            this.btDataViewerPause.TabIndex = 8;
            this.btDataViewerPause.UseVisualStyleBackColor = true;
            this.btDataViewerPause.Click += new System.EventHandler(this.btDataViewerPause_Click);
            // 
            // btDataViewerPlay
            // 
            this.btDataViewerPlay.Image = global::VideoDashboard.Properties.Resources.play;
            this.btDataViewerPlay.Location = new System.Drawing.Point(114, 207);
            this.btDataViewerPlay.Name = "btDataViewerPlay";
            this.btDataViewerPlay.Size = new System.Drawing.Size(25, 25);
            this.btDataViewerPlay.TabIndex = 7;
            this.btDataViewerPlay.UseVisualStyleBackColor = true;
            this.btDataViewerPlay.Click += new System.EventHandler(this.btDataViewerPlay_Click);
            // 
            // btGotoSampleStartMarker
            // 
            this.btGotoSampleStartMarker.Image = global::VideoDashboard.Properties.Resources.firstframe;
            this.btGotoSampleStartMarker.Location = new System.Drawing.Point(91, 207);
            this.btGotoSampleStartMarker.Name = "btGotoSampleStartMarker";
            this.btGotoSampleStartMarker.Size = new System.Drawing.Size(25, 25);
            this.btGotoSampleStartMarker.TabIndex = 6;
            this.btGotoSampleStartMarker.UseVisualStyleBackColor = true;
            this.btGotoSampleStartMarker.Click += new System.EventHandler(this.btGotoSampleStartMarker_Click);
            // 
            // btOpenDataLogFile
            // 
            this.btOpenDataLogFile.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.btOpenDataLogFile.Location = new System.Drawing.Point(201, 45);
            this.btOpenDataLogFile.Name = "btOpenDataLogFile";
            this.btOpenDataLogFile.Size = new System.Drawing.Size(75, 25);
            this.btOpenDataLogFile.TabIndex = 1;
            this.btOpenDataLogFile.Text = "Open";
            this.btOpenDataLogFile.UseVisualStyleBackColor = true;
            this.btOpenDataLogFile.Click += new System.EventHandler(this.btOpenDataLogFile_Click);
            // 
            // tbDataLogFile
            // 
            this.tbDataLogFile.Location = new System.Drawing.Point(6, 19);
            this.tbDataLogFile.Name = "tbDataLogFile";
            this.tbDataLogFile.Size = new System.Drawing.Size(270, 20);
            this.tbDataLogFile.TabIndex = 0;
            // 
            // m_videoPlayer
            // 
            this.m_videoPlayer.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.m_videoPlayer.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.m_videoPlayer.CurrentVideoPos = ((long)(0));
            this.m_videoPlayer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_videoPlayer.Location = new System.Drawing.Point(306, 129);
            this.m_videoPlayer.Margin = new System.Windows.Forms.Padding(0);
            this.m_videoPlayer.Name = "m_videoPlayer";
            this.m_videoPlayer.Size = new System.Drawing.Size(457, 350);
            this.m_videoPlayer.TabIndex = 2;
            // 
            // pnlOverlayConfig
            // 
            this.pnlOverlayConfig.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.pnlOverlayConfig.BackColor = System.Drawing.SystemColors.Control;
            this.pnlOverlayConfig.Controls.Add(this.groupBox4);
            this.pnlOverlayConfig.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlOverlayConfig.Location = new System.Drawing.Point(764, 129);
            this.pnlOverlayConfig.Margin = new System.Windows.Forms.Padding(0);
            this.pnlOverlayConfig.Name = "pnlOverlayConfig";
            this.pnlOverlayConfig.Size = new System.Drawing.Size(323, 350);
            this.pnlOverlayConfig.TabIndex = 3;
            // 
            // groupBox4
            // 
            this.groupBox4.ContextMenuStrip = this.OverlayConfigCtxMenu;
            this.groupBox4.Controls.Add(this.groupBox9);
            this.groupBox4.Controls.Add(this.groupBox8);
            this.groupBox4.Controls.Add(this.groupBox5);
            this.groupBox4.Enabled = false;
            this.groupBox4.Location = new System.Drawing.Point(2, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(316, 318);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Overlay Config";
            // 
            // OverlayConfigCtxMenu
            // 
            this.OverlayConfigCtxMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.resetOverlaySettings});
            this.OverlayConfigCtxMenu.Name = "contextMenuStrip1";
            this.OverlayConfigCtxMenu.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.OverlayConfigCtxMenu.Size = new System.Drawing.Size(103, 26);
            // 
            // resetOverlaySettings
            // 
            this.resetOverlaySettings.Name = "resetOverlaySettings";
            this.resetOverlaySettings.Size = new System.Drawing.Size(102, 22);
            this.resetOverlaySettings.Text = "Reset";
            this.resetOverlaySettings.Click += new System.EventHandler(this.resetOverlaySettings_Click);
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.label12);
            this.groupBox9.Controls.Add(this.label11);
            this.groupBox9.Controls.Add(this.label10);
            this.groupBox9.Controls.Add(this.sliderOverlayScale);
            this.groupBox9.Location = new System.Drawing.Point(41, 229);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(244, 74);
            this.groupBox9.TabIndex = 3;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Size Scale";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(109, 57);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(33, 13);
            this.label12.TabIndex = 8;
            this.label12.Text = "100%";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(206, 57);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(33, 13);
            this.label11.TabIndex = 7;
            this.label11.Text = "150%";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 57);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(27, 13);
            this.label10.TabIndex = 6;
            this.label10.Text = "50%";
            // 
            // sliderOverlayScale
            // 
            this.sliderOverlayScale.AutoSize = false;
            this.sliderOverlayScale.Location = new System.Drawing.Point(6, 19);
            this.sliderOverlayScale.Maximum = 150;
            this.sliderOverlayScale.Minimum = 50;
            this.sliderOverlayScale.Name = "sliderOverlayScale";
            this.sliderOverlayScale.Size = new System.Drawing.Size(232, 35);
            this.sliderOverlayScale.TabIndex = 5;
            this.sliderOverlayScale.TickFrequency = 50;
            this.sliderOverlayScale.Value = 100;
            this.sliderOverlayScale.Scroll += new System.EventHandler(this.sliderOverlayScale_Scroll);
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.label13);
            this.groupBox8.Controls.Add(this.label9);
            this.groupBox8.Controls.Add(this.label8);
            this.groupBox8.Controls.Add(this.sliderTransparency);
            this.groupBox8.Location = new System.Drawing.Point(41, 147);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(244, 76);
            this.groupBox8.TabIndex = 2;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Opacity";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(109, 57);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(27, 13);
            this.label13.TabIndex = 7;
            this.label13.Text = "50%";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(205, 57);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(33, 13);
            this.label9.TabIndex = 6;
            this.label9.Text = "100%";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 57);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(21, 13);
            this.label8.TabIndex = 5;
            this.label8.Text = "0%";
            // 
            // sliderTransparency
            // 
            this.sliderTransparency.AutoSize = false;
            this.sliderTransparency.Location = new System.Drawing.Point(6, 19);
            this.sliderTransparency.Maximum = 100;
            this.sliderTransparency.Minimum = 1;
            this.sliderTransparency.Name = "sliderTransparency";
            this.sliderTransparency.Size = new System.Drawing.Size(232, 35);
            this.sliderTransparency.TabIndex = 4;
            this.sliderTransparency.TickFrequency = 50;
            this.sliderTransparency.Value = 100;
            this.sliderTransparency.Scroll += new System.EventHandler(this.sliderTransparency_Scroll);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.lblPosY);
            this.groupBox5.Controls.Add(this.lblPosX);
            this.groupBox5.Controls.Add(this.label2);
            this.groupBox5.Controls.Add(this.label1);
            this.groupBox5.Controls.Add(this.sliderOverlayPosY);
            this.groupBox5.Controls.Add(this.sliderOverlayPosX);
            this.groupBox5.Location = new System.Drawing.Point(41, 45);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(244, 95);
            this.groupBox5.TabIndex = 1;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Position";
            // 
            // lblPosY
            // 
            this.lblPosY.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblPosY.Location = new System.Drawing.Point(169, 64);
            this.lblPosY.Name = "lblPosY";
            this.lblPosY.Size = new System.Drawing.Size(45, 15);
            this.lblPosY.TabIndex = 7;
            // 
            // lblPosX
            // 
            this.lblPosX.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblPosX.Location = new System.Drawing.Point(169, 19);
            this.lblPosX.Name = "lblPosX";
            this.lblPosX.Size = new System.Drawing.Size(45, 15);
            this.lblPosX.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(18, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(15, 25);
            this.label2.TabIndex = 5;
            this.label2.Text = "Y";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(18, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(15, 25);
            this.label1.TabIndex = 4;
            this.label1.Text = "X";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // sliderOverlayPosY
            // 
            this.sliderOverlayPosY.AutoSize = false;
            this.sliderOverlayPosY.Location = new System.Drawing.Point(39, 64);
            this.sliderOverlayPosY.Maximum = 10000;
            this.sliderOverlayPosY.Name = "sliderOverlayPosY";
            this.sliderOverlayPosY.Size = new System.Drawing.Size(124, 25);
            this.sliderOverlayPosY.TabIndex = 2;
            this.sliderOverlayPosY.TickStyle = System.Windows.Forms.TickStyle.None;
            this.sliderOverlayPosY.Scroll += new System.EventHandler(this.sliderOverlayPosY_Scroll);
            // 
            // sliderOverlayPosX
            // 
            this.sliderOverlayPosX.AutoSize = false;
            this.sliderOverlayPosX.Location = new System.Drawing.Point(39, 19);
            this.sliderOverlayPosX.Maximum = 10000;
            this.sliderOverlayPosX.Name = "sliderOverlayPosX";
            this.sliderOverlayPosX.Size = new System.Drawing.Size(124, 25);
            this.sliderOverlayPosX.TabIndex = 3;
            this.sliderOverlayPosX.TickStyle = System.Windows.Forms.TickStyle.None;
            this.sliderOverlayPosX.Scroll += new System.EventHandler(this.sliderOverlayPosX_Scroll);
            // 
            // pnlLapInfo
            // 
            this.pnlLapInfo.BackColor = System.Drawing.SystemColors.Control;
            this.pnlLapInfo.Controls.Add(this.groupBox6);
            this.pnlLapInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlLapInfo.Location = new System.Drawing.Point(1, 480);
            this.pnlLapInfo.Margin = new System.Windows.Forms.Padding(0);
            this.pnlLapInfo.Name = "pnlLapInfo";
            this.pnlLapInfo.Size = new System.Drawing.Size(304, 132);
            this.pnlLapInfo.TabIndex = 4;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.lblCurLapTime);
            this.groupBox6.Controls.Add(this.label24);
            this.groupBox6.Controls.Add(this.btNextLap);
            this.groupBox6.Controls.Add(this.btPrevLap);
            this.groupBox6.Controls.Add(this.btSectionMarkers);
            this.groupBox6.Controls.Add(this.btLapTimingDetail);
            this.groupBox6.Controls.Add(this.lblFastestLapTime);
            this.groupBox6.Controls.Add(this.label6);
            this.groupBox6.Controls.Add(this.lblTotalLaps);
            this.groupBox6.Controls.Add(this.label5);
            this.groupBox6.Controls.Add(this.lblCurLapIdx);
            this.groupBox6.Controls.Add(this.label4);
            this.groupBox6.Controls.Add(this.lblFastestLap);
            this.groupBox6.Controls.Add(this.label3);
            this.groupBox6.Location = new System.Drawing.Point(3, 3);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(298, 117);
            this.groupBox6.TabIndex = 1;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Lap Info";
            // 
            // lblCurLapTime
            // 
            this.lblCurLapTime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblCurLapTime.Location = new System.Drawing.Point(184, 57);
            this.lblCurLapTime.Name = "lblCurLapTime";
            this.lblCurLapTime.Size = new System.Drawing.Size(60, 15);
            this.lblCurLapTime.TabIndex = 16;
            this.lblCurLapTime.Text = "00:00.000";
            this.lblCurLapTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(128, 57);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(50, 13);
            this.label24.TabIndex = 15;
            this.label24.Text = "Lap time:";
            // 
            // btNextLap
            // 
            this.btNextLap.Location = new System.Drawing.Point(272, 52);
            this.btNextLap.Name = "btNextLap";
            this.btNextLap.Size = new System.Drawing.Size(20, 24);
            this.btNextLap.TabIndex = 14;
            this.btNextLap.Text = ">";
            this.btNextLap.UseVisualStyleBackColor = true;
            this.btNextLap.Click += new System.EventHandler(this.btNextLap_Click);
            // 
            // btPrevLap
            // 
            this.btPrevLap.Location = new System.Drawing.Point(250, 52);
            this.btPrevLap.Name = "btPrevLap";
            this.btPrevLap.Size = new System.Drawing.Size(20, 24);
            this.btPrevLap.TabIndex = 13;
            this.btPrevLap.Text = "<";
            this.btPrevLap.UseVisualStyleBackColor = true;
            this.btPrevLap.Click += new System.EventHandler(this.btPrevLap_Click);
            // 
            // btSectionMarkers
            // 
            this.btSectionMarkers.Location = new System.Drawing.Point(172, 86);
            this.btSectionMarkers.Name = "btSectionMarkers";
            this.btSectionMarkers.Size = new System.Drawing.Size(120, 25);
            this.btSectionMarkers.TabIndex = 12;
            this.btSectionMarkers.Text = "Section Markers";
            this.btSectionMarkers.UseVisualStyleBackColor = true;
            this.btSectionMarkers.Visible = false;
            // 
            // btLapTimingDetail
            // 
            this.btLapTimingDetail.Location = new System.Drawing.Point(6, 86);
            this.btLapTimingDetail.Name = "btLapTimingDetail";
            this.btLapTimingDetail.Size = new System.Drawing.Size(65, 25);
            this.btLapTimingDetail.TabIndex = 11;
            this.btLapTimingDetail.Text = "Laps Info";
            this.btLapTimingDetail.UseVisualStyleBackColor = true;
            this.btLapTimingDetail.Click += new System.EventHandler(this.btLapTimingDetail_Click);
            // 
            // lblFastestLapTime
            // 
            this.lblFastestLapTime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblFastestLapTime.Location = new System.Drawing.Point(232, 26);
            this.lblFastestLapTime.Name = "lblFastestLapTime";
            this.lblFastestLapTime.Size = new System.Drawing.Size(60, 15);
            this.lblFastestLapTime.TabIndex = 7;
            this.lblFastestLapTime.Text = "00:00.000";
            this.lblFastestLapTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(176, 26);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(50, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "Lap time:";
            // 
            // lblTotalLaps
            // 
            this.lblTotalLaps.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblTotalLaps.Location = new System.Drawing.Point(92, 57);
            this.lblTotalLaps.Name = "lblTotalLaps";
            this.lblTotalLaps.Size = new System.Drawing.Size(25, 15);
            this.lblTotalLaps.TabIndex = 5;
            this.lblTotalLaps.Text = "00";
            this.lblTotalLaps.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(74, 57);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(12, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "/";
            // 
            // lblCurLapIdx
            // 
            this.lblCurLapIdx.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblCurLapIdx.Location = new System.Drawing.Point(43, 57);
            this.lblCurLapIdx.Name = "lblCurLapIdx";
            this.lblCurLapIdx.Size = new System.Drawing.Size(25, 15);
            this.lblCurLapIdx.TabIndex = 3;
            this.lblCurLapIdx.Text = "00";
            this.lblCurLapIdx.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 57);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Lap:";
            // 
            // lblFastestLap
            // 
            this.lblFastestLap.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblFastestLap.Location = new System.Drawing.Point(131, 26);
            this.lblFastestLap.Name = "lblFastestLap";
            this.lblFastestLap.Size = new System.Drawing.Size(21, 15);
            this.lblFastestLap.TabIndex = 1;
            this.lblFastestLap.Text = "00";
            this.lblFastestLap.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 26);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(116, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Fastest Lap in Session:";
            // 
            // pnlOverlay
            // 
            this.pnlOverlay.BackColor = System.Drawing.SystemColors.Control;
            this.pnlOverlay.Controls.Add(this.groupBox7);
            this.pnlOverlay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlOverlay.Location = new System.Drawing.Point(764, 1);
            this.pnlOverlay.Margin = new System.Windows.Forms.Padding(0);
            this.pnlOverlay.Name = "pnlOverlay";
            this.pnlOverlay.Size = new System.Drawing.Size(323, 127);
            this.pnlOverlay.TabIndex = 5;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.chkBxEnableOverlayPreview);
            this.groupBox7.Controls.Add(this.cbOverlayList);
            this.groupBox7.Enabled = false;
            this.groupBox7.Location = new System.Drawing.Point(3, 3);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(309, 121);
            this.groupBox7.TabIndex = 0;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Overlay";
            // 
            // chkBxEnableOverlayPreview
            // 
            this.chkBxEnableOverlayPreview.AutoSize = true;
            this.chkBxEnableOverlayPreview.Location = new System.Drawing.Point(6, 46);
            this.chkBxEnableOverlayPreview.Name = "chkBxEnableOverlayPreview";
            this.chkBxEnableOverlayPreview.Size = new System.Drawing.Size(92, 17);
            this.chkBxEnableOverlayPreview.TabIndex = 1;
            this.chkBxEnableOverlayPreview.Text = "Show Overlay";
            this.chkBxEnableOverlayPreview.UseVisualStyleBackColor = true;
            this.chkBxEnableOverlayPreview.CheckedChanged += new System.EventHandler(this.chkBxEnableOverlayPreview_CheckedChanged);
            // 
            // cbOverlayList
            // 
            this.cbOverlayList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbOverlayList.FormattingEnabled = true;
            this.cbOverlayList.Location = new System.Drawing.Point(6, 19);
            this.cbOverlayList.Name = "cbOverlayList";
            this.cbOverlayList.Size = new System.Drawing.Size(297, 21);
            this.cbOverlayList.TabIndex = 0;
            this.cbOverlayList.SelectedIndexChanged += new System.EventHandler(this.cbOverlayList_SelectedIndexChanged);
            // 
            // pnlProject
            // 
            this.pnlProject.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.pnlProject.BackColor = System.Drawing.SystemColors.Control;
            this.pnlProject.Controls.Add(this.groupBox10);
            this.pnlProject.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlProject.Location = new System.Drawing.Point(306, 1);
            this.pnlProject.Margin = new System.Windows.Forms.Padding(0);
            this.pnlProject.Name = "pnlProject";
            this.pnlProject.Size = new System.Drawing.Size(457, 127);
            this.pnlProject.TabIndex = 6;
            // 
            // groupBox10
            // 
            this.groupBox10.Controls.Add(this.groupBox12);
            this.groupBox10.Controls.Add(this.lblProjectName);
            this.groupBox10.Location = new System.Drawing.Point(3, 3);
            this.groupBox10.Name = "groupBox10";
            this.groupBox10.Size = new System.Drawing.Size(451, 145);
            this.groupBox10.TabIndex = 0;
            this.groupBox10.TabStop = false;
            this.groupBox10.Text = "Project";
            // 
            // groupBox12
            // 
            this.groupBox12.Controls.Add(this.lblProjectNotes);
            this.groupBox12.Location = new System.Drawing.Point(6, 42);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(439, 79);
            this.groupBox12.TabIndex = 1;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Notes";
            // 
            // lblProjectNotes
            // 
            this.lblProjectNotes.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblProjectNotes.Location = new System.Drawing.Point(6, 16);
            this.lblProjectNotes.Name = "lblProjectNotes";
            this.lblProjectNotes.Size = new System.Drawing.Size(427, 60);
            this.lblProjectNotes.TabIndex = 0;
            this.lblProjectNotes.DoubleClick += new System.EventHandler(this.lblProjectNotes_DoubleClick);
            // 
            // lblProjectName
            // 
            this.lblProjectName.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblProjectName.Location = new System.Drawing.Point(12, 19);
            this.lblProjectName.Name = "lblProjectName";
            this.lblProjectName.Size = new System.Drawing.Size(427, 20);
            this.lblProjectName.TabIndex = 0;
            this.lblProjectName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblProjectName.DoubleClick += new System.EventHandler(this.lblProjectName_DoubleClick);
            // 
            // pnlVideoDataSync
            // 
            this.pnlVideoDataSync.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.pnlVideoDataSync.BackColor = System.Drawing.SystemColors.Control;
            this.pnlVideoDataSync.Controls.Add(this.groupBox11);
            this.pnlVideoDataSync.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlVideoDataSync.Location = new System.Drawing.Point(306, 480);
            this.pnlVideoDataSync.Margin = new System.Windows.Forms.Padding(0);
            this.pnlVideoDataSync.Name = "pnlVideoDataSync";
            this.pnlVideoDataSync.Size = new System.Drawing.Size(457, 132);
            this.pnlVideoDataSync.TabIndex = 7;
            // 
            // groupBox11
            // 
            this.groupBox11.Controls.Add(this.lblOutVidDataLength);
            this.groupBox11.Controls.Add(this.label30);
            this.groupBox11.Controls.Add(this.lblOutVidLength);
            this.groupBox11.Controls.Add(this.label28);
            this.groupBox11.Controls.Add(this.label21);
            this.groupBox11.Controls.Add(this.lblSyncEndMarkerSample);
            this.groupBox11.Controls.Add(this.lblSyncStartMarkerSample);
            this.groupBox11.Controls.Add(this.label7);
            this.groupBox11.Controls.Add(this.lblSyncEndMarkerFrame);
            this.groupBox11.Controls.Add(this.label16);
            this.groupBox11.Controls.Add(this.lblSyncStartMarkerFrame);
            this.groupBox11.Controls.Add(this.label14);
            this.groupBox11.Location = new System.Drawing.Point(3, 3);
            this.groupBox11.Name = "groupBox11";
            this.groupBox11.Size = new System.Drawing.Size(451, 75);
            this.groupBox11.TabIndex = 0;
            this.groupBox11.TabStop = false;
            this.groupBox11.Text = "Video - Data synchronization";
            // 
            // lblOutVidDataLength
            // 
            this.lblOutVidDataLength.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblOutVidDataLength.Location = new System.Drawing.Point(304, 50);
            this.lblOutVidDataLength.Name = "lblOutVidDataLength";
            this.lblOutVidDataLength.Size = new System.Drawing.Size(50, 15);
            this.lblOutVidDataLength.TabIndex = 11;
            this.lblOutVidDataLength.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(229, 52);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(69, 13);
            this.label30.TabIndex = 10;
            this.label30.Text = "Data Length:";
            // 
            // lblOutVidLength
            // 
            this.lblOutVidLength.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblOutVidLength.Location = new System.Drawing.Point(153, 51);
            this.lblOutVidLength.Name = "lblOutVidLength";
            this.lblOutVidLength.Size = new System.Drawing.Size(50, 15);
            this.lblOutVidLength.TabIndex = 9;
            this.lblOutVidLength.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(74, 52);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(73, 13);
            this.label28.TabIndex = 8;
            this.label28.Text = "Video Length:";
            // 
            // label21
            // 
            this.label21.Location = new System.Drawing.Point(344, 27);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(14, 14);
            this.label21.TabIndex = 7;
            this.label21.Text = "--";
            // 
            // lblSyncEndMarkerSample
            // 
            this.lblSyncEndMarkerSample.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblSyncEndMarkerSample.Location = new System.Drawing.Point(364, 26);
            this.lblSyncEndMarkerSample.Name = "lblSyncEndMarkerSample";
            this.lblSyncEndMarkerSample.Size = new System.Drawing.Size(41, 15);
            this.lblSyncEndMarkerSample.TabIndex = 6;
            this.lblSyncEndMarkerSample.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblSyncStartMarkerSample
            // 
            this.lblSyncStartMarkerSample.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblSyncStartMarkerSample.Location = new System.Drawing.Point(144, 26);
            this.lblSyncStartMarkerSample.Name = "lblSyncStartMarkerSample";
            this.lblSyncStartMarkerSample.Size = new System.Drawing.Size(41, 15);
            this.lblSyncStartMarkerSample.TabIndex = 5;
            this.lblSyncStartMarkerSample.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label7
            // 
            this.label7.Location = new System.Drawing.Point(124, 27);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 14);
            this.label7.TabIndex = 4;
            this.label7.Text = "--";
            // 
            // lblSyncEndMarkerFrame
            // 
            this.lblSyncEndMarkerFrame.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblSyncEndMarkerFrame.Location = new System.Drawing.Point(297, 26);
            this.lblSyncEndMarkerFrame.Name = "lblSyncEndMarkerFrame";
            this.lblSyncEndMarkerFrame.Size = new System.Drawing.Size(41, 15);
            this.lblSyncEndMarkerFrame.TabIndex = 3;
            this.lblSyncEndMarkerFrame.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(229, 26);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(62, 13);
            this.label16.TabIndex = 2;
            this.label16.Text = "End Marker";
            // 
            // lblSyncStartMarkerFrame
            // 
            this.lblSyncStartMarkerFrame.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblSyncStartMarkerFrame.Location = new System.Drawing.Point(77, 26);
            this.lblSyncStartMarkerFrame.Name = "lblSyncStartMarkerFrame";
            this.lblSyncStartMarkerFrame.Size = new System.Drawing.Size(41, 15);
            this.lblSyncStartMarkerFrame.TabIndex = 1;
            this.lblSyncStartMarkerFrame.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(6, 26);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(65, 13);
            this.label14.TabIndex = 0;
            this.label14.Text = "Start Marker";
            // 
            // pnlProgress
            // 
            this.pnlProgress.BackColor = System.Drawing.SystemColors.Control;
            this.pnlProgress.Controls.Add(this.grpBoxProgress);
            this.pnlProgress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlProgress.Location = new System.Drawing.Point(764, 480);
            this.pnlProgress.Margin = new System.Windows.Forms.Padding(0);
            this.pnlProgress.Name = "pnlProgress";
            this.pnlProgress.Size = new System.Drawing.Size(323, 132);
            this.pnlProgress.TabIndex = 8;
            // 
            // grpBoxProgress
            // 
            this.grpBoxProgress.Controls.Add(this.btCancelRenderer);
            this.grpBoxProgress.Controls.Add(this.lblProgressPercent);
            this.grpBoxProgress.Controls.Add(this.progressBar1);
            this.grpBoxProgress.Controls.Add(this.lblProgressFrames);
            this.grpBoxProgress.Controls.Add(this.label27);
            this.grpBoxProgress.Controls.Add(this.lblEstimatedTime);
            this.grpBoxProgress.Controls.Add(this.label18);
            this.grpBoxProgress.Controls.Add(this.label23);
            this.grpBoxProgress.Controls.Add(this.lblElapsedTime);
            this.grpBoxProgress.Location = new System.Drawing.Point(3, 5);
            this.grpBoxProgress.Name = "grpBoxProgress";
            this.grpBoxProgress.Size = new System.Drawing.Size(316, 123);
            this.grpBoxProgress.TabIndex = 1;
            this.grpBoxProgress.TabStop = false;
            // 
            // btCancelRenderer
            // 
            this.btCancelRenderer.Location = new System.Drawing.Point(228, 82);
            this.btCancelRenderer.Name = "btCancelRenderer";
            this.btCancelRenderer.Size = new System.Drawing.Size(75, 23);
            this.btCancelRenderer.TabIndex = 12;
            this.btCancelRenderer.Text = "Cancel";
            this.btCancelRenderer.UseVisualStyleBackColor = true;
            this.btCancelRenderer.Click += new System.EventHandler(this.btCancelRenderer_Click);
            // 
            // lblProgressPercent
            // 
            this.lblProgressPercent.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblProgressPercent.Location = new System.Drawing.Point(37, 85);
            this.lblProgressPercent.Name = "lblProgressPercent";
            this.lblProgressPercent.Size = new System.Drawing.Size(55, 20);
            this.lblProgressPercent.TabIndex = 16;
            this.lblProgressPercent.Text = "99%";
            this.lblProgressPercent.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(98, 85);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(100, 20);
            this.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar1.TabIndex = 15;
            // 
            // lblProgressFrames
            // 
            this.lblProgressFrames.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblProgressFrames.Location = new System.Drawing.Point(98, 13);
            this.lblProgressFrames.Name = "lblProgressFrames";
            this.lblProgressFrames.Size = new System.Drawing.Size(85, 15);
            this.lblProgressFrames.TabIndex = 14;
            this.lblProgressFrames.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(10, 15);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(66, 13);
            this.label27.TabIndex = 13;
            this.label27.Text = "# of Frames:";
            // 
            // lblEstimatedTime
            // 
            this.lblEstimatedTime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblEstimatedTime.Location = new System.Drawing.Point(98, 59);
            this.lblEstimatedTime.Name = "lblEstimatedTime";
            this.lblEstimatedTime.Size = new System.Drawing.Size(85, 15);
            this.lblEstimatedTime.TabIndex = 11;
            this.lblEstimatedTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(10, 37);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(74, 13);
            this.label18.TabIndex = 8;
            this.label18.Text = "Elapsed Time:";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(10, 60);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(82, 13);
            this.label23.TabIndex = 10;
            this.label23.Text = "Estimated Time:";
            // 
            // lblElapsedTime
            // 
            this.lblElapsedTime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblElapsedTime.Location = new System.Drawing.Point(98, 36);
            this.lblElapsedTime.Name = "lblElapsedTime";
            this.lblElapsedTime.Size = new System.Drawing.Size(85, 15);
            this.lblElapsedTime.TabIndex = 9;
            this.lblElapsedTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // dataViewerTickTimer
            // 
            this.dataViewerTickTimer.Interval = 16;
            this.dataViewerTickTimer.Tick += new System.EventHandler(this.dataViewerTickTimer_Tick);
            // 
            // timer1
            // 
            this.timer1.Interval = 55;
            this.timer1.Tick += new System.EventHandler(this.ONVideoProgress_tick);
            // 
            // frmProject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.ClientSize = new System.Drawing.Size(1088, 613);
            this.Controls.Add(this.MainPanelTable);
            this.MaximizeBox = false;
            this.Name = "frmProject";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "frmProject";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmProject_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmProject_FormClosed);
            this.Load += new System.EventHandler(this.frmProject_Load);
            this.Shown += new System.EventHandler(this.frmProject_Shown);
            this.MainPanelTable.ResumeLayout(false);
            this.pnlInputVideoFile.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.pnlDataLogger.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderDataViewerPos)).EndInit();
            this.pnlOverlayConfig.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.OverlayConfigCtxMenu.ResumeLayout(false);
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderOverlayScale)).EndInit();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sliderTransparency)).EndInit();
            this.groupBox5.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.sliderOverlayPosY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sliderOverlayPosX)).EndInit();
            this.pnlLapInfo.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.pnlOverlay.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.pnlProject.ResumeLayout(false);
            this.groupBox10.ResumeLayout(false);
            this.groupBox12.ResumeLayout(false);
            this.pnlVideoDataSync.ResumeLayout(false);
            this.groupBox11.ResumeLayout(false);
            this.groupBox11.PerformLayout();
            this.pnlProgress.ResumeLayout(false);
            this.grpBoxProgress.ResumeLayout(false);
            this.grpBoxProgress.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel MainPanelTable;
        private System.Windows.Forms.Panel pnlInputVideoFile;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btOpenVideoFile;
        private System.Windows.Forms.TextBox tbVideoFileName;
        private System.Windows.Forms.Panel pnlDataLogger;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btOpenDataLogFile;
        private System.Windows.Forms.TextBox tbDataLogFile;
        private videoPlayer m_videoPlayer;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Panel pnlOverlayConfig;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar sliderOverlayPosY;
        private System.Windows.Forms.TrackBar sliderOverlayPosX;
        private System.Windows.Forms.Panel pnlLapInfo;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblFastestLap;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lblTotalLaps;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lblCurLapIdx;
        private System.Windows.Forms.Label lblFastestLapTime;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btLapTimingDetail;
        private System.Windows.Forms.Button btSectionMarkers;
        private System.Windows.Forms.Panel pnlOverlay;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.ComboBox cbOverlayList;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.TrackBar sliderTransparency;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TrackBar sliderOverlayScale;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Button btDetailedSessionInfo;
        private System.Windows.Forms.Panel pnlProject;
        private System.Windows.Forms.GroupBox groupBox10;
        private System.Windows.Forms.Panel pnlVideoDataSync;
        private System.Windows.Forms.GroupBox groupBox11;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label lblSyncStartMarkerFrame;
        private System.Windows.Forms.Label lblSyncEndMarkerFrame;
        private System.Windows.Forms.Button btGotoSampleEndMarker;
        private System.Windows.Forms.Button btDataViewerPause;
        private System.Windows.Forms.Button btDataViewerPlay;
        private System.Windows.Forms.Button btGotoSampleStartMarker;
        private System.Windows.Forms.TrackBar sliderDataViewerPos;
        private System.Windows.Forms.Label lblProjectName;
        private System.Windows.Forms.Timer dataViewerTickTimer;
        private System.Windows.Forms.Label lblSessionTime;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Button btPrevLap;
        private System.Windows.Forms.Button btNextLap;
        private System.Windows.Forms.Label lblCurLap;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label lblCurLapTime;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label lblSpeed;
        private System.Windows.Forms.Label lblLongG;
        private System.Windows.Forms.Label lblLatG;
        private System.Windows.Forms.Label lblRpm;
        private System.Windows.Forms.CheckBox chkBxEnableOverlayPreview;
        private System.Windows.Forms.Label lblSyncStartMarkerSample;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label lblSyncEndMarkerSample;
        private System.Windows.Forms.ContextMenuStrip OverlayConfigCtxMenu;
        private System.Windows.Forms.ToolStripMenuItem resetOverlaySettings;
        private System.Windows.Forms.Button btSetDataEndPos;
        private System.Windows.Forms.Button btSetDataStartPos;
        private System.Windows.Forms.GroupBox grpBoxProgress;
        private System.Windows.Forms.Label lblElapsedTime;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Button btCancelRenderer;
        private System.Windows.Forms.Label lblEstimatedTime;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label lblGear;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label lblCurLapPercent;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.GroupBox groupBox12;
        private System.Windows.Forms.Label lblProjectNotes;
        private System.Windows.Forms.Label lblPosY;
        private System.Windows.Forms.Label lblPosX;
        private System.Windows.Forms.Panel pnlProgress;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Label lblProgressFrames;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label lblProgressPercent;
        private System.Windows.Forms.Label lblOutVidLength;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label lblOutVidDataLength;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Button btShowInVideoInfo;
        private System.Windows.Forms.Label lblCurDataPos;
        private System.Windows.Forms.Timer timer1;
    }
}