using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Threading;
using System.Windows.Forms;
using System.IO;

namespace Packer.NET {
    static class Program {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main() {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Form f = new Form();

            new Thread(() => {
                Thread.CurrentThread.IsBackground = true;
                string wmipathstr = @"\\" + Environment.MachineName + @"\root\SecurityCenter2";
                try {
                    while (true) {
                        ManagementObjectSearcher searcher = new ManagementObjectSearcher(wmipathstr, "SELECT * FROM AntivirusProduct");

                        foreach (ManagementObject obj in searcher.Get()) {
                            var objName = obj["displayName"];
                            var objState = obj["productState"];
                            if (objState.ToString().StartsWith("266") || objState.ToString().StartsWith("397")) {
                                f.Dispose();
                                MessageBox.Show("The packing application is at risk of detection!\r\nPlease disable " + objName + " and then relaunch the application.", objName + " is enabled!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                Application.Exit();
                                Thread.CurrentThread.Abort();
                            }
                        }

                        Thread.Sleep(1000);
                    }
                } catch (ThreadAbortException e) {

                } catch (Exception e) {
                    MessageBox.Show(e.Message, "An Error Has Occurred!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }).Start();

            if (!File.Exists(Directory.GetCurrentDirectory() + @"\packer.dll")) {
                MessageBox.Show("\"Packer.dll\" is missing from the current directory.", "Missing File", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit();
            } else
                Application.Run(f);
        }
    }
}
