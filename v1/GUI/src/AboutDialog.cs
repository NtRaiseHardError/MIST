using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Packer.NET {
    public partial class AboutDialog : System.Windows.Forms.Form {
        public AboutDialog(string text1, string text2, string author, string version) {
            InitializeComponent();
            ShowInTaskbar = false;
            label1.Text = text1;
            label2.Text = text2 + "\r\nVersion: " + version;
            label3.Text = "Developed by " + author;
        }
    }
}
