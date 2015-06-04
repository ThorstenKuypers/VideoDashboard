namespace Activatar.SampleApp
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.picLogo = new System.Windows.Forms.PictureBox();
            this.lblLogo = new System.Windows.Forms.Label();
            this.pnl1 = new System.Windows.Forms.Panel();
            this.lblError = new System.Windows.Forms.Label();
            this.lblMessage = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox = new System.Windows.Forms.GroupBox();
            this.lnkActivate = new System.Windows.Forms.LinkLabel();
            this.txtProductKey = new System.Windows.Forms.TextBox();
            this.lnkGenerate = new System.Windows.Forms.LinkLabel();
            this.pnl2 = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.cmdNext = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdExit = new System.Windows.Forms.Button();
            this.cmdTryAgain = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.picLogo)).BeginInit();
            this.pnl1.SuspendLayout();
            this.groupBox.SuspendLayout();
            this.pnl2.SuspendLayout();
            this.SuspendLayout();
            // 
            // picLogo
            // 
            this.picLogo.Image = ((System.Drawing.Image)(resources.GetObject("picLogo.Image")));
            this.picLogo.Location = new System.Drawing.Point(0, 0);
            this.picLogo.Margin = new System.Windows.Forms.Padding(0);
            this.picLogo.Name = "picLogo";
            this.picLogo.Size = new System.Drawing.Size(502, 86);
            this.picLogo.TabIndex = 0;
            this.picLogo.TabStop = false;
            // 
            // lblLogo
            // 
            this.lblLogo.AutoSize = true;
            this.lblLogo.BackColor = System.Drawing.Color.Transparent;
            this.lblLogo.Location = new System.Drawing.Point(334, 43);
            this.lblLogo.Name = "lblLogo";
            this.lblLogo.Size = new System.Drawing.Size(152, 13);
            this.lblLogo.TabIndex = 1;
            this.lblLogo.Text = "Product Key Activation System";
            // 
            // pnl1
            // 
            this.pnl1.Controls.Add(this.lblError);
            this.pnl1.Controls.Add(this.lblMessage);
            this.pnl1.Controls.Add(this.label2);
            this.pnl1.Controls.Add(this.groupBox);
            this.pnl1.Controls.Add(this.lnkGenerate);
            this.pnl1.Location = new System.Drawing.Point(10, 90);
            this.pnl1.Name = "pnl1";
            this.pnl1.Size = new System.Drawing.Size(475, 160);
            this.pnl1.TabIndex = 4;
            // 
            // lblError
            // 
            this.lblError.AutoSize = true;
            this.lblError.BackColor = System.Drawing.Color.Transparent;
            this.lblError.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblError.ForeColor = System.Drawing.Color.Red;
            this.lblError.Location = new System.Drawing.Point(5, 19);
            this.lblError.Name = "lblError";
            this.lblError.Size = new System.Drawing.Size(0, 13);
            this.lblError.TabIndex = 8;
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = true;
            this.lblMessage.BackColor = System.Drawing.Color.Transparent;
            this.lblMessage.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMessage.ForeColor = System.Drawing.Color.Red;
            this.lblMessage.Location = new System.Drawing.Point(5, 3);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(176, 13);
            this.lblMessage.TabIndex = 7;
            this.lblMessage.Text = "This product needs activation";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Location = new System.Drawing.Point(5, 131);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(311, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "To get a valid Product Key for this demo, generate one using the";
            // 
            // groupBox
            // 
            this.groupBox.Controls.Add(this.lnkActivate);
            this.groupBox.Controls.Add(this.txtProductKey);
            this.groupBox.Location = new System.Drawing.Point(6, 40);
            this.groupBox.Name = "groupBox";
            this.groupBox.Size = new System.Drawing.Size(460, 81);
            this.groupBox.TabIndex = 5;
            this.groupBox.TabStop = false;
            this.groupBox.Text = "Please, enter the product key";
            // 
            // lnkActivate
            // 
            this.lnkActivate.AutoSize = true;
            this.lnkActivate.Enabled = false;
            this.lnkActivate.Location = new System.Drawing.Point(338, 54);
            this.lnkActivate.Name = "lnkActivate";
            this.lnkActivate.Size = new System.Drawing.Size(86, 13);
            this.lnkActivate.TabIndex = 1;
            this.lnkActivate.TabStop = true;
            this.lnkActivate.Text = "Activate Product";
            this.lnkActivate.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnActivateClick);
            // 
            // txtProductKey
            // 
            this.txtProductKey.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtProductKey.Location = new System.Drawing.Point(31, 26);
            this.txtProductKey.MaxLength = 41;
            this.txtProductKey.Name = "txtProductKey";
            this.txtProductKey.Size = new System.Drawing.Size(396, 25);
            this.txtProductKey.TabIndex = 0;
            this.txtProductKey.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.txtProductKey.TextChanged += new System.EventHandler(this.OnProductKeyTextChanged);
            // 
            // lnkGenerate
            // 
            this.lnkGenerate.AutoSize = true;
            this.lnkGenerate.Location = new System.Drawing.Point(312, 131);
            this.lnkGenerate.Name = "lnkGenerate";
            this.lnkGenerate.Size = new System.Drawing.Size(144, 13);
            this.lnkGenerate.TabIndex = 4;
            this.lnkGenerate.TabStop = true;
            this.lnkGenerate.Text = "Product Key Generation Tool";
            this.lnkGenerate.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnGenerateClick);
            // 
            // pnl2
            // 
            this.pnl2.Controls.Add(this.label5);
            this.pnl2.Controls.Add(this.label4);
            this.pnl2.Location = new System.Drawing.Point(10, 318);
            this.pnl2.Name = "pnl2";
            this.pnl2.Size = new System.Drawing.Size(475, 100);
            this.pnl2.TabIndex = 5;
            this.pnl2.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.Color.Transparent;
            this.label5.Location = new System.Drawing.Point(7, 43);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(282, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "You have successfully activated your copy of this product.";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.Transparent;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.Green;
            this.label4.Location = new System.Drawing.Point(5, 11);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(141, 20);
            this.label4.TabIndex = 6;
            this.label4.Text = "Congratulations!";
            // 
            // cmdNext
            // 
            this.cmdNext.Enabled = false;
            this.cmdNext.Location = new System.Drawing.Point(326, 266);
            this.cmdNext.Name = "cmdNext";
            this.cmdNext.Size = new System.Drawing.Size(75, 23);
            this.cmdNext.TabIndex = 7;
            this.cmdNext.Text = "Next >";
            this.cmdNext.UseVisualStyleBackColor = true;
            this.cmdNext.Click += new System.EventHandler(this.OnNextClick);
            // 
            // cmdCancel
            // 
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Location = new System.Drawing.Point(410, 266);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(75, 23);
            this.cmdCancel.TabIndex = 8;
            this.cmdCancel.Text = "Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.OnCancelClick);
            // 
            // cmdExit
            // 
            this.cmdExit.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdExit.Location = new System.Drawing.Point(411, 266);
            this.cmdExit.Name = "cmdExit";
            this.cmdExit.Size = new System.Drawing.Size(75, 23);
            this.cmdExit.TabIndex = 10;
            this.cmdExit.Text = "Exit";
            this.cmdExit.UseVisualStyleBackColor = true;
            this.cmdExit.Visible = false;
            this.cmdExit.Click += new System.EventHandler(this.OnExitClick);
            // 
            // cmdTryAgain
            // 
            this.cmdTryAgain.Location = new System.Drawing.Point(326, 266);
            this.cmdTryAgain.Name = "cmdTryAgain";
            this.cmdTryAgain.Size = new System.Drawing.Size(75, 23);
            this.cmdTryAgain.TabIndex = 11;
            this.cmdTryAgain.Text = "Try again";
            this.cmdTryAgain.UseVisualStyleBackColor = true;
            this.cmdTryAgain.Visible = false;
            this.cmdTryAgain.Click += new System.EventHandler(this.OnTryAgainClick);
            // 
            // MainForm
            // 
            this.AcceptButton = this.cmdNext;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.CancelButton = this.cmdCancel;
            this.ClientSize = new System.Drawing.Size(499, 302);
            this.ControlBox = false;
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdNext);
            this.Controls.Add(this.pnl2);
            this.Controls.Add(this.pnl1);
            this.Controls.Add(this.lblLogo);
            this.Controls.Add(this.picLogo);
            this.Controls.Add(this.cmdExit);
            this.Controls.Add(this.cmdTryAgain);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Activatar Sample Application";
            ((System.ComponentModel.ISupportInitialize)(this.picLogo)).EndInit();
            this.pnl1.ResumeLayout(false);
            this.pnl1.PerformLayout();
            this.groupBox.ResumeLayout(false);
            this.groupBox.PerformLayout();
            this.pnl2.ResumeLayout(false);
            this.pnl2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox picLogo;
        private System.Windows.Forms.Label lblLogo;
        private System.Windows.Forms.Panel pnl1;
        private System.Windows.Forms.Label lblMessage;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox;
        private System.Windows.Forms.LinkLabel lnkActivate;
        private System.Windows.Forms.TextBox txtProductKey;
        private System.Windows.Forms.LinkLabel lnkGenerate;
        private System.Windows.Forms.Panel pnl2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button cmdNext;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button cmdExit;
        private System.Windows.Forms.Label lblError;
        private System.Windows.Forms.Button cmdTryAgain;
    }
}
