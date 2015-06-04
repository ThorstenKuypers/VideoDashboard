using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace VideoDashboard
{
    public partial class frmNewProject : Form
    {
        public frmNewProject()
        {
            InitializeComponent();
        }

        public frmNewProject(string curName, string curNotes)
        {
            InitializeComponent();

            m_projectName = curName;
            m_projectNotes = curNotes;
        }

        public string m_projectName;
        public string m_projectNotes;

        private void frmNewProject_Load(object sender, EventArgs e)
        {
            textBox1.Text = m_projectName;
            textBox2.Text = m_projectNotes;
            this.textBox1.Focus();
        }

        private void textBox1_Leave(object sender, EventArgs e)
        {
            this.m_projectName = textBox1.Text;
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
            {
                this.m_projectName = textBox1.Text;
                textBox2.Focus();
            }
        }

        private void textBox2_Leave(object sender, EventArgs e)
        {
            this.m_projectNotes = textBox2.Text;
        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            //if (e.KeyCode == Keys.Return)
            //{
            //    this.m_projectNotes = textBox2.Text;
            //    button1.Focus();
            //}
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
            {
                MessageBox.Show("Please enter at least a project name");

                textBox1.BackColor = Color.Yellow;
                return;
            }

            DialogResult = DialogResult.OK;
        }

        private void textBox1_Enter(object sender, EventArgs e)
        {
            textBox1.BackColor = Color.White;
        }

        
    }
}
