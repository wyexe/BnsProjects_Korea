namespace BnsConsole
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.dgvAccount = new System.Windows.Forms.DataGridView();
            this.Column6 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column5 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.删除帐号ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.停止挂机ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.恢复挂机ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.冻结游戏ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.恢复窗口ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.停止窗口排序ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.恢复窗口排序ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.单帐号收邮件删号ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.单帐号收邮件停止挂机ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.所有帐号停止挂机ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.查找日志ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bugReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.恶作剧开始ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.开始恶作剧ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.停止恶作剧ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.控制台设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.挂机设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.卡号设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.帐号设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.刷新ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.开始挂机ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.一键收邮件ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.收邮件停止挂机ToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.恢复优化ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lbPrompt = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.lbWorkCardCount = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lbDumpCount = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lbConsoleDLL = new System.Windows.Forms.Label();
            this.Lable5 = new System.Windows.Forms.Label();
            this.lbBnsProjectDLL = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.dgvAccount)).BeginInit();
            this.contextMenuStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dgvAccount
            // 
            this.dgvAccount.AllowUserToAddRows = false;
            this.dgvAccount.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvAccount.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column6,
            this.Column1,
            this.Column2,
            this.Column3,
            this.Column4,
            this.Column5});
            this.dgvAccount.ContextMenuStrip = this.contextMenuStrip1;
            this.dgvAccount.Location = new System.Drawing.Point(1, 21);
            this.dgvAccount.Name = "dgvAccount";
            this.dgvAccount.RowHeadersVisible = false;
            this.dgvAccount.RowTemplate.Height = 23;
            this.dgvAccount.Size = new System.Drawing.Size(834, 933);
            this.dgvAccount.TabIndex = 0;
            // 
            // Column6
            // 
            this.Column6.HeaderText = "序号";
            this.Column6.Name = "Column6";
            this.Column6.Width = 60;
            // 
            // Column1
            // 
            this.Column1.HeaderText = "帐号";
            this.Column1.Name = "Column1";
            this.Column1.Width = 150;
            // 
            // Column2
            // 
            this.Column2.HeaderText = "状态";
            this.Column2.Name = "Column2";
            this.Column2.Width = 170;
            // 
            // Column3
            // 
            this.Column3.HeaderText = "日志";
            this.Column3.Name = "Column3";
            this.Column3.Width = 180;
            // 
            // Column4
            // 
            this.Column4.HeaderText = "在线时长";
            this.Column4.Name = "Column4";
            // 
            // Column5
            // 
            this.Column5.HeaderText = "上次重登原因";
            this.Column5.Name = "Column5";
            this.Column5.Width = 160;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.删除帐号ToolStripMenuItem,
            this.停止挂机ToolStripMenuItem,
            this.恢复挂机ToolStripMenuItem,
            this.冻结游戏ToolStripMenuItem,
            this.恢复窗口ToolStripMenuItem,
            this.停止窗口排序ToolStripMenuItem,
            this.恢复窗口排序ToolStripMenuItem,
            this.单帐号收邮件删号ToolStripMenuItem,
            this.单帐号收邮件停止挂机ToolStripMenuItem,
            this.所有帐号停止挂机ToolStripMenuItem,
            this.查找日志ToolStripMenuItem,
            this.bugReportToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(191, 268);
            // 
            // 删除帐号ToolStripMenuItem
            // 
            this.删除帐号ToolStripMenuItem.Name = "删除帐号ToolStripMenuItem";
            this.删除帐号ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.删除帐号ToolStripMenuItem.Text = "删除帐号";
            this.删除帐号ToolStripMenuItem.Click += new System.EventHandler(this.删除帐号ToolStripMenuItem_Click);
            // 
            // 停止挂机ToolStripMenuItem
            // 
            this.停止挂机ToolStripMenuItem.Name = "停止挂机ToolStripMenuItem";
            this.停止挂机ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.停止挂机ToolStripMenuItem.Text = "停止挂机";
            this.停止挂机ToolStripMenuItem.Click += new System.EventHandler(this.停止挂机ToolStripMenuItem_Click);
            // 
            // 恢复挂机ToolStripMenuItem
            // 
            this.恢复挂机ToolStripMenuItem.Name = "恢复挂机ToolStripMenuItem";
            this.恢复挂机ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.恢复挂机ToolStripMenuItem.Text = "开始挂机";
            this.恢复挂机ToolStripMenuItem.Click += new System.EventHandler(this.恢复挂机ToolStripMenuItem_Click);
            // 
            // 冻结游戏ToolStripMenuItem
            // 
            this.冻结游戏ToolStripMenuItem.Name = "冻结游戏ToolStripMenuItem";
            this.冻结游戏ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.冻结游戏ToolStripMenuItem.Text = "暂停挂机";
            this.冻结游戏ToolStripMenuItem.Click += new System.EventHandler(this.冻结游戏ToolStripMenuItem_Click);
            // 
            // 恢复窗口ToolStripMenuItem
            // 
            this.恢复窗口ToolStripMenuItem.Name = "恢复窗口ToolStripMenuItem";
            this.恢复窗口ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.恢复窗口ToolStripMenuItem.Text = "恢复窗口";
            this.恢复窗口ToolStripMenuItem.Click += new System.EventHandler(this.恢复窗口ToolStripMenuItem_Click);
            // 
            // 停止窗口排序ToolStripMenuItem
            // 
            this.停止窗口排序ToolStripMenuItem.Name = "停止窗口排序ToolStripMenuItem";
            this.停止窗口排序ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.停止窗口排序ToolStripMenuItem.Text = "停止窗口排序";
            this.停止窗口排序ToolStripMenuItem.Click += new System.EventHandler(this.停止窗口排序ToolStripMenuItem_Click);
            // 
            // 恢复窗口排序ToolStripMenuItem
            // 
            this.恢复窗口排序ToolStripMenuItem.Name = "恢复窗口排序ToolStripMenuItem";
            this.恢复窗口排序ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.恢复窗口排序ToolStripMenuItem.Text = "恢复窗口排序";
            this.恢复窗口排序ToolStripMenuItem.Click += new System.EventHandler(this.恢复窗口排序ToolStripMenuItem_Click);
            // 
            // 单帐号收邮件删号ToolStripMenuItem
            // 
            this.单帐号收邮件删号ToolStripMenuItem.Name = "单帐号收邮件删号ToolStripMenuItem";
            this.单帐号收邮件删号ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.单帐号收邮件删号ToolStripMenuItem.Text = "单帐号收邮件删号";
            this.单帐号收邮件删号ToolStripMenuItem.Click += new System.EventHandler(this.单帐号收邮件删号ToolStripMenuItem_Click);
            // 
            // 单帐号收邮件停止挂机ToolStripMenuItem
            // 
            this.单帐号收邮件停止挂机ToolStripMenuItem.Name = "单帐号收邮件停止挂机ToolStripMenuItem";
            this.单帐号收邮件停止挂机ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.单帐号收邮件停止挂机ToolStripMenuItem.Text = "单帐号收邮件停止挂机";
            this.单帐号收邮件停止挂机ToolStripMenuItem.Click += new System.EventHandler(this.单帐号收邮件停止挂机ToolStripMenuItem_Click);
            // 
            // 所有帐号停止挂机ToolStripMenuItem
            // 
            this.所有帐号停止挂机ToolStripMenuItem.Name = "所有帐号停止挂机ToolStripMenuItem";
            this.所有帐号停止挂机ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.所有帐号停止挂机ToolStripMenuItem.Text = "所有帐号停止挂机";
            this.所有帐号停止挂机ToolStripMenuItem.Click += new System.EventHandler(this.所有帐号停止挂机ToolStripMenuItem_Click);
            // 
            // 查找日志ToolStripMenuItem
            // 
            this.查找日志ToolStripMenuItem.Name = "查找日志ToolStripMenuItem";
            this.查找日志ToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.查找日志ToolStripMenuItem.Text = "查找日志";
            this.查找日志ToolStripMenuItem.Click += new System.EventHandler(this.查找日志ToolStripMenuItem_Click);
            // 
            // bugReportToolStripMenuItem
            // 
            this.bugReportToolStripMenuItem.Name = "bugReportToolStripMenuItem";
            this.bugReportToolStripMenuItem.Size = new System.Drawing.Size(190, 22);
            this.bugReportToolStripMenuItem.Text = "BugReport";
            this.bugReportToolStripMenuItem.Click += new System.EventHandler(this.bugReportToolStripMenuItem_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.恶作剧开始ToolStripMenuItem,
            this.设置ToolStripMenuItem,
            this.刷新ToolStripMenuItem,
            this.开始挂机ToolStripMenuItem,
            this.一键收邮件ToolStripMenuItem,
            this.收邮件停止挂机ToolStripMenuItem1,
            this.恢复优化ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(837, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 恶作剧开始ToolStripMenuItem
            // 
            this.恶作剧开始ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.开始恶作剧ToolStripMenuItem,
            this.停止恶作剧ToolStripMenuItem});
            this.恶作剧开始ToolStripMenuItem.Name = "恶作剧开始ToolStripMenuItem";
            this.恶作剧开始ToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.恶作剧开始ToolStripMenuItem.Text = "挂机";
            // 
            // 开始恶作剧ToolStripMenuItem
            // 
            this.开始恶作剧ToolStripMenuItem.Name = "开始恶作剧ToolStripMenuItem";
            this.开始恶作剧ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.开始恶作剧ToolStripMenuItem.Text = "开始挂机";
            this.开始恶作剧ToolStripMenuItem.Click += new System.EventHandler(this.开始恶作剧ToolStripMenuItem_Click);
            // 
            // 停止恶作剧ToolStripMenuItem
            // 
            this.停止恶作剧ToolStripMenuItem.Name = "停止恶作剧ToolStripMenuItem";
            this.停止恶作剧ToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.停止恶作剧ToolStripMenuItem.Text = "停止挂机";
            this.停止恶作剧ToolStripMenuItem.Click += new System.EventHandler(this.停止恶作剧ToolStripMenuItem_Click);
            // 
            // 设置ToolStripMenuItem
            // 
            this.设置ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.控制台设置ToolStripMenuItem,
            this.挂机设置ToolStripMenuItem,
            this.卡号设置ToolStripMenuItem,
            this.帐号设置ToolStripMenuItem});
            this.设置ToolStripMenuItem.Name = "设置ToolStripMenuItem";
            this.设置ToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.设置ToolStripMenuItem.Text = "设置";
            // 
            // 控制台设置ToolStripMenuItem
            // 
            this.控制台设置ToolStripMenuItem.Name = "控制台设置ToolStripMenuItem";
            this.控制台设置ToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.控制台设置ToolStripMenuItem.Text = "控制台设置";
            this.控制台设置ToolStripMenuItem.Click += new System.EventHandler(this.控制台设置ToolStripMenuItem_Click);
            // 
            // 挂机设置ToolStripMenuItem
            // 
            this.挂机设置ToolStripMenuItem.Name = "挂机设置ToolStripMenuItem";
            this.挂机设置ToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.挂机设置ToolStripMenuItem.Text = "挂机设置";
            this.挂机设置ToolStripMenuItem.Click += new System.EventHandler(this.挂机设置ToolStripMenuItem_Click);
            // 
            // 卡号设置ToolStripMenuItem
            // 
            this.卡号设置ToolStripMenuItem.Name = "卡号设置ToolStripMenuItem";
            this.卡号设置ToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.卡号设置ToolStripMenuItem.Text = "卡号设置";
            this.卡号设置ToolStripMenuItem.Click += new System.EventHandler(this.卡号设置ToolStripMenuItem_Click);
            // 
            // 帐号设置ToolStripMenuItem
            // 
            this.帐号设置ToolStripMenuItem.Name = "帐号设置ToolStripMenuItem";
            this.帐号设置ToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.帐号设置ToolStripMenuItem.Text = "帐号设置";
            this.帐号设置ToolStripMenuItem.Click += new System.EventHandler(this.帐号设置ToolStripMenuItem_Click);
            // 
            // 刷新ToolStripMenuItem
            // 
            this.刷新ToolStripMenuItem.Name = "刷新ToolStripMenuItem";
            this.刷新ToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.刷新ToolStripMenuItem.Text = "刷新";
            this.刷新ToolStripMenuItem.Click += new System.EventHandler(this.刷新ToolStripMenuItem_Click);
            // 
            // 开始挂机ToolStripMenuItem
            // 
            this.开始挂机ToolStripMenuItem.Name = "开始挂机ToolStripMenuItem";
            this.开始挂机ToolStripMenuItem.Size = new System.Drawing.Size(65, 20);
            this.开始挂机ToolStripMenuItem.Text = "开始挂机";
            this.开始挂机ToolStripMenuItem.Click += new System.EventHandler(this.开始挂机ToolStripMenuItem_Click);
            // 
            // 一键收邮件ToolStripMenuItem
            // 
            this.一键收邮件ToolStripMenuItem.Name = "一键收邮件ToolStripMenuItem";
            this.一键收邮件ToolStripMenuItem.Size = new System.Drawing.Size(77, 20);
            this.一键收邮件ToolStripMenuItem.Text = "收邮件删号";
            this.一键收邮件ToolStripMenuItem.Click += new System.EventHandler(this.一键收邮件ToolStripMenuItem_Click);
            // 
            // 收邮件停止挂机ToolStripMenuItem1
            // 
            this.收邮件停止挂机ToolStripMenuItem1.Name = "收邮件停止挂机ToolStripMenuItem1";
            this.收邮件停止挂机ToolStripMenuItem1.Size = new System.Drawing.Size(101, 20);
            this.收邮件停止挂机ToolStripMenuItem1.Text = "收邮件停止挂机";
            this.收邮件停止挂机ToolStripMenuItem1.Click += new System.EventHandler(this.收邮件停止挂机ToolStripMenuItem1_Click);
            // 
            // 恢复优化ToolStripMenuItem
            // 
            this.恢复优化ToolStripMenuItem.Name = "恢复优化ToolStripMenuItem";
            this.恢复优化ToolStripMenuItem.Size = new System.Drawing.Size(65, 20);
            this.恢复优化ToolStripMenuItem.Text = "恢复优化";
            this.恢复优化ToolStripMenuItem.Click += new System.EventHandler(this.恢复优化ToolStripMenuItem_Click);
            // 
            // lbPrompt
            // 
            this.lbPrompt.ForeColor = System.Drawing.Color.Red;
            this.lbPrompt.Location = new System.Drawing.Point(11, 959);
            this.lbPrompt.Name = "lbPrompt";
            this.lbPrompt.Size = new System.Drawing.Size(77, 12);
            this.lbPrompt.TabIndex = 2;
            this.lbPrompt.Text = "木有提示……";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Red;
            this.label1.Location = new System.Drawing.Point(687, 958);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "卡号有效数量:";
            // 
            // lbWorkCardCount
            // 
            this.lbWorkCardCount.AutoSize = true;
            this.lbWorkCardCount.ForeColor = System.Drawing.Color.Red;
            this.lbWorkCardCount.Location = new System.Drawing.Point(769, 957);
            this.lbWorkCardCount.Name = "lbWorkCardCount";
            this.lbWorkCardCount.Size = new System.Drawing.Size(11, 12);
            this.lbWorkCardCount.TabIndex = 4;
            this.lbWorkCardCount.Text = "0";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.Red;
            this.label2.Location = new System.Drawing.Point(687, 978);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(107, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "Dump错误日志数量:";
            // 
            // lbDumpCount
            // 
            this.lbDumpCount.AutoSize = true;
            this.lbDumpCount.ForeColor = System.Drawing.Color.Red;
            this.lbDumpCount.Location = new System.Drawing.Point(790, 976);
            this.lbDumpCount.Name = "lbDumpCount";
            this.lbDumpCount.Size = new System.Drawing.Size(11, 12);
            this.lbDumpCount.TabIndex = 6;
            this.lbDumpCount.Text = "0";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.SystemColors.Control;
            this.label3.ForeColor = System.Drawing.Color.Red;
            this.label3.Location = new System.Drawing.Point(12, 979);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(119, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "ConsoleDLL编译时间:";
            // 
            // lbConsoleDLL
            // 
            this.lbConsoleDLL.AutoSize = true;
            this.lbConsoleDLL.ForeColor = System.Drawing.Color.Red;
            this.lbConsoleDLL.Location = new System.Drawing.Point(137, 979);
            this.lbConsoleDLL.Name = "lbConsoleDLL";
            this.lbConsoleDLL.Size = new System.Drawing.Size(77, 12);
            this.lbConsoleDLL.TabIndex = 8;
            this.lbConsoleDLL.Text = "201701010101";
            // 
            // Lable5
            // 
            this.Lable5.AutoSize = true;
            this.Lable5.ForeColor = System.Drawing.Color.Red;
            this.Lable5.Location = new System.Drawing.Point(251, 978);
            this.Lable5.Name = "Lable5";
            this.Lable5.Size = new System.Drawing.Size(137, 12);
            this.Lable5.TabIndex = 9;
            this.Lable5.Text = "BnsProjectDLL编译时间:";
            // 
            // lbBnsProjectDLL
            // 
            this.lbBnsProjectDLL.AutoSize = true;
            this.lbBnsProjectDLL.ForeColor = System.Drawing.Color.Red;
            this.lbBnsProjectDLL.Location = new System.Drawing.Point(391, 978);
            this.lbBnsProjectDLL.Name = "lbBnsProjectDLL";
            this.lbBnsProjectDLL.Size = new System.Drawing.Size(77, 12);
            this.lbBnsProjectDLL.TabIndex = 10;
            this.lbBnsProjectDLL.Text = "201701010101";
            this.lbBnsProjectDLL.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(837, 1000);
            this.Controls.Add(this.lbBnsProjectDLL);
            this.Controls.Add(this.Lable5);
            this.Controls.Add(this.lbConsoleDLL);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lbDumpCount);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lbWorkCardCount);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lbPrompt);
            this.Controls.Add(this.dgvAccount);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "54Bns";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dgvAccount)).EndInit();
            this.contextMenuStrip1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dgvAccount;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 恶作剧开始ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 开始恶作剧ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 停止恶作剧ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 控制台设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 挂机设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 卡号设置ToolStripMenuItem;
        private System.Windows.Forms.Label lbPrompt;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lbWorkCardCount;
        private System.Windows.Forms.ToolStripMenuItem 帐号设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 刷新ToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 删除帐号ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 停止挂机ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 恢复挂机ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 恢复窗口ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 开始挂机ToolStripMenuItem;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lbDumpCount;
        private System.Windows.Forms.ToolStripMenuItem 冻结游戏ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 停止窗口排序ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 恢复窗口排序ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 一键收邮件ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 收邮件停止挂机ToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem 单帐号收邮件删号ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 单帐号收邮件停止挂机ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 所有帐号停止挂机ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 恢复优化ToolStripMenuItem;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column6;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column2;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column4;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lbConsoleDLL;
        private System.Windows.Forms.Label Lable5;
        private System.Windows.Forms.Label lbBnsProjectDLL;
        private System.Windows.Forms.ToolStripMenuItem 查找日志ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bugReportToolStripMenuItem;
    }
}

