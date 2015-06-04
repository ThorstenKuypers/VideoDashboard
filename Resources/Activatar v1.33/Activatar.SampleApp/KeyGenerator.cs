using System;
using System.Windows.Forms;

namespace Activatar.SampleApp
{
    public partial class KeyGenerator : Form
    {
        ProductKeyCheater _keyCheater;

        bool _generated = false;

        #region KeyGenerator constructor
        public KeyGenerator()
        {
            InitializeComponent();
            _keyCheater = new ProductKeyCheater();
        }
        #endregion

        public bool OpenForm(IWin32Window owner)
        {
            ShowDialog(owner);
            return _generated;
        }

        private void OptionChanged(object sender, EventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            RadioButton option = sender as RadioButton;
            if (option.Checked)
            {
                switch (option.Name)
                {
                    case "optValid":
                        txtKey.Text = _keyCheater.GenerateKey(1, 1, 0);
                        break;
                    case "opt30Days":
                        txtKey.Text = _keyCheater.GenerateKey(1, 1, 30);
                        break;
                    case "optExpired":
                        txtKey.Text = _keyCheater.GenerateKey(1, 1, -15);
                        break;
                    case "optDiffName":
                        txtKey.Text = _keyCheater.GenerateKey(2, 1, 0);
                        break;
                    case "optInvalid":
                        string key = _keyCheater.GenerateKey(1, 1, 0);
                        key = key.Replace('0', '9');
                        key = key.Replace('1', '8');
                        key = key.Replace('2', '7');
                        key = key.Replace('3', '6');
                        key = key.Replace('4', '5');
                        txtKey.Text = key;
                        break;
                    default:
                        break;
                }
                this.Cursor = Cursors.Default;
            }
        }

        private void OnCopyAndExitClick(object sender, EventArgs e)
        {
            Clipboard.SetData(DataFormats.Text, txtKey.Text);
            _generated = true;
            Close();
        }
    }
}
