namespace LicenseManager
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
            this.mainTabControl = new System.Windows.Forms.TabControl();
            this.tpLicensing = new System.Windows.Forms.TabPage();
            this.tpCustomers = new System.Windows.Forms.TabPage();
            this.dataGridViewCustomers = new System.Windows.Forms.DataGridView();
            this.tpBeta = new System.Windows.Forms.TabPage();
            this.btNewBetaLicense = new System.Windows.Forms.Button();
            this.dataGridViewBetaTesters = new System.Windows.Forms.DataGridView();
            this.btUpdateTable = new System.Windows.Forms.Button();
            this.btRefreshTable = new System.Windows.Forms.Button();
            this.btAddNewTester = new System.Windows.Forms.Button();
            this.mainTabControl.SuspendLayout();
            this.tpCustomers.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewCustomers)).BeginInit();
            this.tpBeta.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewBetaTesters)).BeginInit();
            this.SuspendLayout();
            // 
            // mainTabControl
            // 
            this.mainTabControl.Controls.Add(this.tpLicensing);
            this.mainTabControl.Controls.Add(this.tpCustomers);
            this.mainTabControl.Controls.Add(this.tpBeta);
            this.mainTabControl.Dock = System.Windows.Forms.DockStyle.Top;
            this.mainTabControl.Location = new System.Drawing.Point(0, 0);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(877, 520);
            this.mainTabControl.TabIndex = 1;
            // 
            // tpLicensing
            // 
            this.tpLicensing.Location = new System.Drawing.Point(4, 22);
            this.tpLicensing.Name = "tpLicensing";
            this.tpLicensing.Padding = new System.Windows.Forms.Padding(3);
            this.tpLicensing.Size = new System.Drawing.Size(869, 494);
            this.tpLicensing.TabIndex = 0;
            this.tpLicensing.Text = "Licensing";
            this.tpLicensing.UseVisualStyleBackColor = true;
            // 
            // tpCustomers
            // 
            this.tpCustomers.Controls.Add(this.dataGridViewCustomers);
            this.tpCustomers.Location = new System.Drawing.Point(4, 22);
            this.tpCustomers.Name = "tpCustomers";
            this.tpCustomers.Padding = new System.Windows.Forms.Padding(3);
            this.tpCustomers.Size = new System.Drawing.Size(869, 494);
            this.tpCustomers.TabIndex = 1;
            this.tpCustomers.Text = "Customers";
            this.tpCustomers.UseVisualStyleBackColor = true;
            // 
            // dataGridViewCustomers
            // 
            this.dataGridViewCustomers.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewCustomers.Dock = System.Windows.Forms.DockStyle.Top;
            this.dataGridViewCustomers.Location = new System.Drawing.Point(3, 3);
            this.dataGridViewCustomers.Name = "dataGridViewCustomers";
            this.dataGridViewCustomers.Size = new System.Drawing.Size(863, 485);
            this.dataGridViewCustomers.TabIndex = 0;
            // 
            // tpBeta
            // 
            this.tpBeta.Controls.Add(this.btAddNewTester);
            this.tpBeta.Controls.Add(this.btNewBetaLicense);
            this.tpBeta.Controls.Add(this.dataGridViewBetaTesters);
            this.tpBeta.Location = new System.Drawing.Point(4, 22);
            this.tpBeta.Name = "tpBeta";
            this.tpBeta.Padding = new System.Windows.Forms.Padding(3);
            this.tpBeta.Size = new System.Drawing.Size(869, 494);
            this.tpBeta.TabIndex = 2;
            this.tpBeta.Text = "BETA";
            this.tpBeta.UseVisualStyleBackColor = true;
            // 
            // btNewBetaLicense
            // 
            this.btNewBetaLicense.Location = new System.Drawing.Point(738, 465);
            this.btNewBetaLicense.Name = "btNewBetaLicense";
            this.btNewBetaLicense.Size = new System.Drawing.Size(125, 23);
            this.btNewBetaLicense.TabIndex = 1;
            this.btNewBetaLicense.Text = "New BETA License";
            this.btNewBetaLicense.UseVisualStyleBackColor = true;
            // 
            // dataGridViewBetaTesters
            // 
            this.dataGridViewBetaTesters.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewBetaTesters.Dock = System.Windows.Forms.DockStyle.Top;
            this.dataGridViewBetaTesters.Location = new System.Drawing.Point(3, 3);
            this.dataGridViewBetaTesters.Name = "dataGridViewBetaTesters";
            this.dataGridViewBetaTesters.Size = new System.Drawing.Size(863, 456);
            this.dataGridViewBetaTesters.TabIndex = 0;
            this.dataGridViewBetaTesters.UserDeletedRow += new System.Windows.Forms.DataGridViewRowEventHandler(this.dataGridViewBetaTesters_UserDeletedRow);
            this.dataGridViewBetaTesters.UserDeletingRow += new System.Windows.Forms.DataGridViewRowCancelEventHandler(this.dataGridViewBetaTesters_UserDeletingRow);
            // 
            // btUpdateTable
            // 
            this.btUpdateTable.Location = new System.Drawing.Point(85, 526);
            this.btUpdateTable.Name = "btUpdateTable";
            this.btUpdateTable.Size = new System.Drawing.Size(75, 23);
            this.btUpdateTable.TabIndex = 3;
            this.btUpdateTable.Text = "Update";
            this.btUpdateTable.UseVisualStyleBackColor = true;
            this.btUpdateTable.Click += new System.EventHandler(this.btUpdateTable_Click);
            // 
            // btRefreshTable
            // 
            this.btRefreshTable.Location = new System.Drawing.Point(4, 526);
            this.btRefreshTable.Name = "btRefreshTable";
            this.btRefreshTable.Size = new System.Drawing.Size(75, 23);
            this.btRefreshTable.TabIndex = 2;
            this.btRefreshTable.Text = "Refresh";
            this.btRefreshTable.UseVisualStyleBackColor = true;
            this.btRefreshTable.Click += new System.EventHandler(this.btRefreshTable_Click);
            // 
            // btAddNewTester
            // 
            this.btAddNewTester.Location = new System.Drawing.Point(607, 465);
            this.btAddNewTester.Name = "btAddNewTester";
            this.btAddNewTester.Size = new System.Drawing.Size(125, 23);
            this.btAddNewTester.TabIndex = 2;
            this.btAddNewTester.Text = "Add Tester";
            this.btAddNewTester.UseVisualStyleBackColor = true;
            this.btAddNewTester.Click += new System.EventHandler(this.btAddNewTester_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(877, 665);
            this.Controls.Add(this.btUpdateTable);
            this.Controls.Add(this.btRefreshTable);
            this.Controls.Add(this.mainTabControl);
            this.Name = "frmMain";
            this.Text = "VideoDashboard License Manager";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmMain_FormClosing);
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.mainTabControl.ResumeLayout(false);
            this.tpCustomers.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewCustomers)).EndInit();
            this.tpBeta.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewBetaTesters)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage tpLicensing;
        private System.Windows.Forms.TabPage tpCustomers;
        private System.Windows.Forms.TabPage tpBeta;
        private System.Windows.Forms.DataGridView dataGridViewBetaTesters;
        private System.Windows.Forms.Button btNewBetaLicense;
        private System.Windows.Forms.Button btUpdateTable;
        private System.Windows.Forms.Button btRefreshTable;
        private System.Windows.Forms.DataGridView dataGridViewCustomers;
        private System.Windows.Forms.Button btAddNewTester;
    }
}

