using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace VideoDashboard
{
    public partial class videoTrimSlider : UserControl
    {
        public videoTrimSlider()
        {
            InitializeComponent();
        }

        private void DrawSlider(Graphics g)
        {
            Pen pen = new Pen(sliderColor, 1.0f);

            Point ss = new Point(5, 5);
            Point se = new Point(this.Size.Width - 5, 5);
            g.DrawLine(pen, ss, se);

            Brush br = new SolidBrush(sliderColor);
            g.FillEllipse(br, sliderStartCap);
            g.FillEllipse(br, sliderEndCap);

        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            this.sliderColor = Color.Black;
            this.activeColor = Color.Yellow;

            this.sliderGrabSize = new Size(8, 8);
            this.sliderStartCap = new Rectangle(5 - (sliderGrabSize.Width / 2), 5 - (sliderGrabSize.Height / 2), sliderGrabSize.Width, sliderGrabSize.Height);
            this.sliderEndCap = new Rectangle((this.ClientRectangle.Width - sliderGrabSize.Width - 1), 5 - (sliderGrabSize.Height / 2), sliderGrabSize.Width, sliderGrabSize.Height);
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            DrawSlider(e.Graphics);
        }

        protected override void OnMouseHover(EventArgs e)
        {
            base.OnMouseHover(e);

        }
        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            Point mp = new Point(MousePosition.X, MousePosition.Y);
            Point lp = PointToClient(mp);
            this.curMousePos = lp.X;
            Graphics g = Graphics.FromHwnd(this.Handle);

            if ((lp.X >= sliderStartCap.X) && (lp.X <= (sliderStartCap.X + sliderStartCap.Width)))
            {
                g.FillEllipse(new SolidBrush(activeColor), sliderStartCap.X + 2, sliderStartCap.Y + 2, 4, 4);
            }
            else
            {
                g.FillEllipse(new SolidBrush(sliderColor), sliderStartCap);
            }

            if (e.Button == MouseButtons.Left)
            {
                sliderStartCap.X = (lp.X - (sliderStartCap.Width / 2));
            }

            this.Invalidate();
        }
        protected override void OnMouseLeave(EventArgs e)
        {
            //base.OnMouseLeave(e);
            //mouseOverStartGrab = false;
            //this.Invalidate();
        }
        public override Color BackColor
        {
            get
            {
                return base.BackColor;
            }
            set
            {
                base.BackColor = value;
            }
        }

        public Color ActiveColor
        {
            get
            {
                return activeColor;
            }
            set
            {
                activeColor = value;
            }
        }
        private Color sliderColor;
        public Color activeColor;
        private Size sliderGrabSize;
        private Rectangle sliderStartCap;
        private Rectangle sliderEndCap;
        //private bool mouseOverStartGrab;
        //private bool mouseOverEndGrab;
        //private bool startCapDrag;
        private int curMousePos;
    }
}
