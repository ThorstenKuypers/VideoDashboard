namespace Activatar.SampleApp
{
    partial class KeyGenerator
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(KeyGenerator));
            this.optValid = new System.Windows.Forms.RadioButton();
            this.opt30Days = new System.Windows.Forms.RadioButton();
            this.optExpired = new System.Windows.Forms.RadioButton();
            this.optDiffName = new System.Windows.Forms.RadioButton();
            this.optInvalid = new System.Windows.Forms.RadioButton();
            this.cmdCopyAndExit = new System.Windows.Forms.Button();
            this.txtKey = new System.Windows.Forms.TextBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // optValid
            // 
            this.optValid.AutoSize = true;
            this.optValid.Location = new System.Drawing.Point(12, 78);
            this.optValid.Name = "optValid";
            this.optValid.Size = new System.Drawing.Size(162, 17);
            this.optValid.TabIndex = 0;
            this.optValid.Text = "Generate a valid product key";
            this.optValid.UseVisualStyleBackColor = true;
            this.optValid.CheckedChanged += new System.EventHandler(this.OptionChanged);
            // 
            // opt30Days
            // 
            this.opt30Days.AutoSize = true;
            this.opt30Days.Location = new System.Drawing.Point(12, 101);
            this.opt30Days.Name = "opt30Days";
            this.opt30Days.Size = new System.Drawing.Size(196, 17);
            this.opt30Days.TabIndex = 1;
            this.opt30Days.Text = "Generate a 30 days trial product key";
            this.opt30Days.UseVisualStyleBackColor = true;
            this.opt30Days.CheckedChanged += new System.EventHandler(this.OptionChanged);
            // 
            // optExpired
            // 
            this.optExpired.AutoSize = true;
            this.optExpired.Location = new System.Drawing.Point(12, 124);
            this.optExpired.Name = "optExpired";
            this.optExpired.Size = new System.Drawing.Size(180, 17);
            this.optExpired.TabIndex = 2;
            this.optExpired.Text = "Generate an expired product key";
            this.optExpired.UseVisualStyleBackColor = true;
            this.optExpired.CheckedChanged += new System.EventHandler(this.OptionChanged);
            // 
            // optDiffName
            // 
            this.optDiffName.AutoSize = true;
            this.optDiffName.Location = new System.Drawing.Point(12, 147);
            this.optDiffName.Name = "optDiffName";
            this.optDiffName.Size = new System.Drawing.Size(270, 17);
            this.optDiffName.TabIndex = 3;
            this.optDiffName.Text = "Generate a product key for a different product name";
            this.optDiffName.UseVisualStyleBackColor = true;
            this.optDiffName.CheckedChanged += new System.EventHandler(this.OptionChanged);
            // 
            // optInvalid
            // 
            this.optInvalid.AutoSize = true;
            this.optInvalid.Checked = true;
            this.optInvalid.Location = new System.Drawing.Point(12, 170);
            this.optInvalid.Name = "optInvalid";
            this.optInvalid.Size = new System.Drawing.Size(176, 17);
            this.optInvalid.TabIndex = 4;
            this.optInvalid.TabStop = true;
            this.optInvalid.Text = "Generate an invalid product key";
            this.optInvalid.UseVisualStyleBackColor = true;
            this.optInvalid.CheckedChanged += new System.EventHandler(this.OptionChanged);
            // 
            // cmdCopyAndExit
            // 
            this.cmdCopyAndExit.Location = new System.Drawing.Point(104, 249);
            this.cmdCopyAndExit.Name = "cmdCopyAndExit";
            this.cmdCopyAndExit.Size = new System.Drawing.Size(178, 23);
            this.cmdCopyAndExit.TabIndex = 6;
            this.cmdCopyAndExit.Text = "Copy to Clipboard and Exit";
            this.cmdCopyAndExit.UseVisualStyleBackColor = true;
            this.cmdCopyAndExit.Click += new System.EventHandler(this.OnCopyAndExitClick);
            // 
            // txtKey
            // 
            this.txtKey.BackColor = System.Drawing.Color.White;
            this.txtKey.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtKey.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtKey.Location = new System.Drawing.Point(53, 211);
            this.txtKey.Name = "txtKey";
            this.txtKey.ReadOnly = true;
            this.txtKey.Size = new System.Drawing.Size(299, 16);
            this.txtKey.TabIndex = 7;
            this.txtKey.Text = "00000-00000-00000-00000-00000-00000";
            this.txtKey.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(403, 64);
            this.pictureBox1.TabIndex = 5;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox2.Image")));
            this.pictureBox2.Location = new System.Drawing.Point(18, 204);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(358, 30);
            this.pictureBox2.TabIndex = 8;
            this.pictureBox2.TabStop = false;
            // 
            // KeyGenerator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(394, 285);
            this.Controls.Add(this.txtKey);
            this.Controls.Add(this.cmdCopyAndExit);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.optInvalid);
            this.Controls.Add(this.optDiffName);
            this.Controls.Add(this.optExpired);
            this.Controls.Add(this.opt30Days);
            this.Controls.Add(this.optValid);
            this.Controls.Add(this.pictureBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "KeyGenerator";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Product Key Generator";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RadioButton optValid;
        private System.Windows.Forms.RadioButton opt30Days;
        private System.Windows.Forms.RadioButton optExpired;
        private System.Windows.Forms.RadioButton optDiffName;
        private System.Windows.Forms.RadioButton optInvalid;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button cmdCopyAndExit;
        private System.Windows.Forms.TextBox txtKey;
        private System.Windows.Forms.PictureBox pictureBox2;
    }
}
