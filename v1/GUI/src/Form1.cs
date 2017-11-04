using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Management;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace Packer.NET {
    public partial class Form : System.Windows.Forms.Form {
        private static string version = "1.0";
        private static string name = "M.I.S.T.\r\nMinimalistic Intervention\r\nand Signature Transformation";
        private static string name2 = "M.I.S.T.";
        private static string author = "dtm";
        private AboutDialog aboutDialog;

        private string targetFilePath;
        private string outputFilePath;
        
        [DllImport("packer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
        public static extern int PackFile([MarshalAs(UnmanagedType.LPWStr)] string targetFile, [MarshalAs(UnmanagedType.LPWStr)] string outputFile);

        public Form() {
            InitializeComponent();
            this.Text = this.Text + " v" + version;
            authorLabel.Text = "Developed by " + author;
            aboutDialog = new AboutDialog(name2, name, author, version);
        }

        #region MenuStrip

        private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e) {
            aboutDialog.ShowDialog();
        }

        #endregion

        #region Interface

        private void targetFileTextBox_DragDrop(object sender, DragEventArgs e) {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            if (files != null && files.Length != 0)
                targetFileTextBox.Text = files[0];
        }

        private void targetFileTextBox_DragOver(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy;
            else
                e.Effect = DragDropEffects.None;
        }

        private void targetFileButton_Click(object sender, EventArgs e) {
            OpenFileDialog file = new OpenFileDialog();
            if (file.ShowDialog() == DialogResult.OK) {
                targetFilePath = file.FileName;
                targetFileTextBox.Text = file.FileName;
            }

            file.Dispose();
        }

        private void outputFileButton_Click(object sender, EventArgs e) {
            SaveFileDialog file = new SaveFileDialog();
            file.DefaultExt = "exe";
            file.Filter = "Executable files (*.exe)|*.exe|All files (*.*)|*.*";
            if (file.ShowDialog() == DialogResult.OK) {
                outputFilePath = file.FileName;
                outputFileTextBox.Text = file.FileName;
            }

            file.Dispose();
        }

        private void packButton_Click(object sender, EventArgs e) {
            progressBar1.Value = 0;
            if (!File.Exists(targetFilePath) || !Directory.Exists(Path.GetDirectoryName(outputFilePath))) {
                toolStripStatusLabel1.Text = "Please specify the required parameters.";
                return;
            }

            toolStripStatusLabel1.Text = "Preparing to pack " + System.IO.Path.GetFileName(targetFilePath);

            int success = -1;
            try {
                 success = PackFile(targetFilePath, outputFilePath);
            } catch (BadImageFormatException ex) {
                MessageBox.Show("\"Packer.dll\" is corrupt.", "Packer Module Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                progressBar1.Value = 0;
                toolStripStatusLabel1.Text = "Operation failed: " + "Corrupt module.";
                return;
            }

            if (success == 0) {
                progressBar1.Value = 100;
                toolStripStatusLabel1.Text = "Operation successfully completed.";
                return;
            }

            progressBar1.Value = 0;
            toolStripStatusLabel1.Text = "Operation failed: " + success;
        }

        #endregion
    }
}
