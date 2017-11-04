namespace Packer.NET {
    partial class Form {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.targetFileTextBox = new System.Windows.Forms.TextBox();
            this.targetFileButton = new System.Windows.Forms.Button();
            this.outputFileTextBox = new System.Windows.Forms.TextBox();
            this.targetFile = new System.Windows.Forms.Label();
            this.outputFile = new System.Windows.Forms.Label();
            this.outputFileButton = new System.Windows.Forms.Button();
            this.packButton = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.authorLabel = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(280, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.Visible = false;
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.optionsToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
            this.optionsToolStripMenuItem.Text = "Options";
            this.optionsToolStripMenuItem.Visible = false;
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(113, 6);
            this.toolStripMenuItem1.Visible = false;
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // targetFileTextBox
            // 
            this.targetFileTextBox.AllowDrop = true;
            this.targetFileTextBox.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.targetFileTextBox.Enabled = false;
            this.targetFileTextBox.Location = new System.Drawing.Point(12, 25);
            this.targetFileTextBox.Name = "targetFileTextBox";
            this.targetFileTextBox.ReadOnly = true;
            this.targetFileTextBox.Size = new System.Drawing.Size(190, 20);
            this.targetFileTextBox.TabIndex = 1;
            this.targetFileTextBox.DragDrop += new System.Windows.Forms.DragEventHandler(this.targetFileTextBox_DragDrop);
            this.targetFileTextBox.DragOver += new System.Windows.Forms.DragEventHandler(this.targetFileTextBox_DragOver);
            // 
            // targetFileButton
            // 
            this.targetFileButton.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.targetFileButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.targetFileButton.Location = new System.Drawing.Point(220, 26);
            this.targetFileButton.Name = "targetFileButton";
            this.targetFileButton.Size = new System.Drawing.Size(51, 19);
            this.targetFileButton.TabIndex = 2;
            this.targetFileButton.Text = "...";
            this.targetFileButton.UseVisualStyleBackColor = true;
            this.targetFileButton.Click += new System.EventHandler(this.targetFileButton_Click);
            // 
            // outputFileTextBox
            // 
            this.outputFileTextBox.AllowDrop = true;
            this.outputFileTextBox.Enabled = false;
            this.outputFileTextBox.Location = new System.Drawing.Point(12, 64);
            this.outputFileTextBox.Name = "outputFileTextBox";
            this.outputFileTextBox.ReadOnly = true;
            this.outputFileTextBox.Size = new System.Drawing.Size(190, 20);
            this.outputFileTextBox.TabIndex = 3;
            // 
            // targetFile
            // 
            this.targetFile.AutoSize = true;
            this.targetFile.BackColor = System.Drawing.Color.Transparent;
            this.targetFile.Location = new System.Drawing.Point(12, 9);
            this.targetFile.Name = "targetFile";
            this.targetFile.Size = new System.Drawing.Size(73, 13);
            this.targetFile.TabIndex = 4;
            this.targetFile.Text = "Target file";
            // 
            // outputFile
            // 
            this.outputFile.AutoSize = true;
            this.outputFile.BackColor = System.Drawing.Color.Transparent;
            this.outputFile.Location = new System.Drawing.Point(12, 48);
            this.outputFile.Name = "outputFile";
            this.outputFile.Size = new System.Drawing.Size(73, 13);
            this.outputFile.TabIndex = 5;
            this.outputFile.Text = "Output file";
            // 
            // outputFileButton
            // 
            this.outputFileButton.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.outputFileButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.outputFileButton.Location = new System.Drawing.Point(220, 63);
            this.outputFileButton.Name = "outputFileButton";
            this.outputFileButton.Size = new System.Drawing.Size(51, 20);
            this.outputFileButton.TabIndex = 6;
            this.outputFileButton.Text = "...";
            this.outputFileButton.UseVisualStyleBackColor = true;
            this.outputFileButton.Click += new System.EventHandler(this.outputFileButton_Click);
            // 
            // packButton
            // 
            this.packButton.Font = new System.Drawing.Font("Impact", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.packButton.ForeColor = System.Drawing.Color.Black;
            this.packButton.Location = new System.Drawing.Point(196, 90);
            this.packButton.Name = "packButton";
            this.packButton.Size = new System.Drawing.Size(75, 28);
            this.packButton.TabIndex = 7;
            this.packButton.Text = "P A C K";
            this.packButton.UseVisualStyleBackColor = true;
            this.packButton.Click += new System.EventHandler(this.packButton_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(12, 90);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(178, 28);
            this.progressBar1.TabIndex = 8;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 136);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(280, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "Ready.";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripStatusLabel1.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripStatusLabel1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(43, 17);
            this.toolStripStatusLabel1.Text = "Ready.";
            // 
            // authorLabel
            // 
            this.authorLabel.AutoSize = true;
            this.authorLabel.BackColor = System.Drawing.Color.Transparent;
            this.authorLabel.Location = new System.Drawing.Point(12, 121);
            this.authorLabel.Name = "authorLabel";
            this.authorLabel.Size = new System.Drawing.Size(43, 13);
            this.authorLabel.TabIndex = 10;
            this.authorLabel.Text = "label1";
            // 
            // Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::Packer.NET.Resource1.gray_haze_background;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(280, 158);
            this.Controls.Add(this.authorLabel);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.packButton);
            this.Controls.Add(this.outputFileButton);
            this.Controls.Add(this.outputFile);
            this.Controls.Add(this.targetFile);
            this.Controls.Add(this.outputFileTextBox);
            this.Controls.Add(this.targetFileButton);
            this.Controls.Add(this.targetFileTextBox);
            this.Controls.Add(this.menuStrip1);
            this.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ForeColor = System.Drawing.Color.GhostWhite;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "Form";
            this.Text = "M.I.S.T.";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.TextBox targetFileTextBox;
        private System.Windows.Forms.Button targetFileButton;
        private System.Windows.Forms.TextBox outputFileTextBox;
        private System.Windows.Forms.Label targetFile;
        private System.Windows.Forms.Label outputFile;
        private System.Windows.Forms.Button outputFileButton;
        private System.Windows.Forms.Button packButton;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.Label authorLabel;
    }
}

