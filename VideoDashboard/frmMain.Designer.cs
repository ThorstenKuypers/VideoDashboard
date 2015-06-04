namespace VideoDashboard
{
    partial class frmMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.btNewProject = new System.Windows.Forms.ToolStripButton();
            this.btOpenProject = new System.Windows.Forms.ToolStripButton();
            this.btSaveProject = new System.Windows.Forms.ToolStripDropDownButton();
            this.menuSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.menuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.btCloseProject = new System.Windows.Forms.ToolStripButton();
            this.btRenderToPreview = new System.Windows.Forms.ToolStripButton();
            this.btRenderToFile = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.btTools = new System.Windows.Forms.ToolStripSplitButton();
            this.showChannelsListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btOptions = new System.Windows.Forms.ToolStripButton();
            this.toolStripSplitBtHelp = new System.Windows.Forms.ToolStripSplitButton();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 36);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(6, 36);
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.GripMargin = new System.Windows.Forms.Padding(2, 5, 2, 2);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btNewProject,
            this.btOpenProject,
            this.btSaveProject,
            this.btCloseProject,
            this.toolStripSeparator8,
            this.btRenderToPreview,
            this.btRenderToFile,
            this.toolStripSeparator1,
            this.btTools,
            this.btOptions,
            this.toolStripSeparator9,
            this.toolStripSplitBtHelp});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1243, 36);
            this.toolStrip1.Stretch = true;
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // btNewProject
            // 
            this.btNewProject.AutoSize = false;
            this.btNewProject.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btNewProject.Image = ((System.Drawing.Image)(resources.GetObject("btNewProject.Image")));
            this.btNewProject.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btNewProject.Margin = new System.Windows.Forms.Padding(0);
            this.btNewProject.Name = "btNewProject";
            this.btNewProject.Size = new System.Drawing.Size(32, 32);
            this.btNewProject.Text = "toolStripButton1";
            this.btNewProject.ToolTipText = "Create new project";
            this.btNewProject.Click += new System.EventHandler(this.btNewProject_Click);
            // 
            // btOpenProject
            // 
            this.btOpenProject.AutoSize = false;
            this.btOpenProject.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btOpenProject.Image = ((System.Drawing.Image)(resources.GetObject("btOpenProject.Image")));
            this.btOpenProject.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btOpenProject.Name = "btOpenProject";
            this.btOpenProject.Size = new System.Drawing.Size(32, 32);
            this.btOpenProject.Text = "toolStripButton2";
            this.btOpenProject.ToolTipText = "Open existing project";
            this.btOpenProject.Click += new System.EventHandler(this.btOpenProject_Click);
            // 
            // btSaveProject
            // 
            this.btSaveProject.AutoSize = false;
            this.btSaveProject.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btSaveProject.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuSaveAs,
            this.menuSave});
            this.btSaveProject.Image = ((System.Drawing.Image)(resources.GetObject("btSaveProject.Image")));
            this.btSaveProject.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btSaveProject.Name = "btSaveProject";
            this.btSaveProject.Size = new System.Drawing.Size(32, 32);
            this.btSaveProject.Text = "toolStripDropDownButton1";
            this.btSaveProject.ToolTipText = "Save current active project to file";
            // 
            // menuSaveAs
            // 
            this.menuSaveAs.Name = "menuSaveAs";
            this.menuSaveAs.Size = new System.Drawing.Size(111, 22);
            this.menuSaveAs.Text = "Save&As";
            this.menuSaveAs.Click += new System.EventHandler(this.menuSaveAs_Click);
            // 
            // menuSave
            // 
            this.menuSave.Name = "menuSave";
            this.menuSave.Size = new System.Drawing.Size(111, 22);
            this.menuSave.Text = "&Save";
            this.menuSave.Click += new System.EventHandler(this.menuSave_Click);
            // 
            // btCloseProject
            // 
            this.btCloseProject.AutoSize = false;
            this.btCloseProject.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btCloseProject.Image = ((System.Drawing.Image)(resources.GetObject("btCloseProject.Image")));
            this.btCloseProject.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btCloseProject.Name = "btCloseProject";
            this.btCloseProject.Size = new System.Drawing.Size(32, 32);
            this.btCloseProject.Text = "toolStripButton4";
            this.btCloseProject.ToolTipText = "Close current project";
            this.btCloseProject.Click += new System.EventHandler(this.btCloseProject_Click);
            // 
            // btRenderToPreview
            // 
            this.btRenderToPreview.AutoSize = false;
            this.btRenderToPreview.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btRenderToPreview.Image = ((System.Drawing.Image)(resources.GetObject("btRenderToPreview.Image")));
            this.btRenderToPreview.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btRenderToPreview.Name = "btRenderToPreview";
            this.btRenderToPreview.Size = new System.Drawing.Size(32, 32);
            this.btRenderToPreview.Text = "toolStripButton2";
            this.btRenderToPreview.ToolTipText = "Render preview of current project";
            this.btRenderToPreview.Click += new System.EventHandler(this.btRenderToPreview_Click);
            // 
            // btRenderToFile
            // 
            this.btRenderToFile.AutoSize = false;
            this.btRenderToFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btRenderToFile.Image = ((System.Drawing.Image)(resources.GetObject("btRenderToFile.Image")));
            this.btRenderToFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btRenderToFile.Name = "btRenderToFile";
            this.btRenderToFile.Size = new System.Drawing.Size(32, 32);
            this.btRenderToFile.Text = "btRenderToFile";
            this.btRenderToFile.ToolTipText = "Render project to file";
            this.btRenderToFile.Click += new System.EventHandler(this.btRenderToFile_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 36);
            // 
            // btTools
            // 
            this.btTools.AutoSize = false;
            this.btTools.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btTools.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showChannelsListToolStripMenuItem});
            this.btTools.Image = ((System.Drawing.Image)(resources.GetObject("btTools.Image")));
            this.btTools.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btTools.Name = "btTools";
            this.btTools.Size = new System.Drawing.Size(32, 32);
            this.btTools.Text = "toolStripButton5";
            this.btTools.ToolTipText = "Open Overlay Editor";
            this.btTools.Click += new System.EventHandler(this.btTools_Click);
            // 
            // showChannelsListToolStripMenuItem
            // 
            this.showChannelsListToolStripMenuItem.Enabled = false;
            this.showChannelsListToolStripMenuItem.Name = "showChannelsListToolStripMenuItem";
            this.showChannelsListToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.showChannelsListToolStripMenuItem.Text = "Show Channels List";
            this.showChannelsListToolStripMenuItem.Click += new System.EventHandler(this.showChannelsListToolStripMenuItem_Click);
            // 
            // btOptions
            // 
            this.btOptions.AutoSize = false;
            this.btOptions.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btOptions.Image = ((System.Drawing.Image)(resources.GetObject("btOptions.Image")));
            this.btOptions.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btOptions.Name = "btOptions";
            this.btOptions.Size = new System.Drawing.Size(32, 32);
            this.btOptions.Text = "toolStripButton6";
            this.btOptions.ToolTipText = "Settings";
            this.btOptions.Click += new System.EventHandler(this.btOptions_Click);
            // 
            // toolStripSplitBtHelp
            // 
            this.toolStripSplitBtHelp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitBtHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.helpToolStripMenuItem,
            this.toolStripSeparator5,
            this.aboutToolStripMenuItem});
            this.toolStripSplitBtHelp.Image = global::VideoDashboard.Properties.Resources.help;
            this.toolStripSplitBtHelp.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitBtHelp.Name = "toolStripSplitBtHelp";
            this.toolStripSplitBtHelp.Size = new System.Drawing.Size(32, 33);
            this.toolStripSplitBtHelp.Text = "toolStripSplitButton1";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(104, 6);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.ClientSize = new System.Drawing.Size(1243, 689);
            this.Controls.Add(this.toolStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.Name = "frmMain";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "VideoDashboard";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.frmMain_FormClosed);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.MdiChildActivate += new System.EventHandler(this.frmMain_MdiChildActivate);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripButton btNewProject;
        private System.Windows.Forms.ToolStripButton btOpenProject;
        private System.Windows.Forms.ToolStripButton btCloseProject;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripButton btOptions;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitBtHelp;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton btRenderToPreview;
        private System.Windows.Forms.ToolStripButton btRenderToFile;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripDropDownButton btSaveProject;
        private System.Windows.Forms.ToolStripMenuItem menuSaveAs;
        private System.Windows.Forms.ToolStripMenuItem menuSave;
        private System.Windows.Forms.ToolStripSplitButton btTools;
        private System.Windows.Forms.ToolStripMenuItem showChannelsListToolStripMenuItem;

    }
}

