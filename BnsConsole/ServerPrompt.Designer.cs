namespace BnsConsole
{
    partial class ServerPrompt
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
            this.lbPrompt = new System.Windows.Forms.Label();
            this.lbGetCardMsg = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lbPrompt
            // 
            this.lbPrompt.AutoSize = true;
            this.lbPrompt.Font = new System.Drawing.Font("SimSun", 13F);
            this.lbPrompt.ForeColor = System.Drawing.Color.Red;
            this.lbPrompt.Location = new System.Drawing.Point(36, 34);
            this.lbPrompt.Name = "lbPrompt";
            this.lbPrompt.Size = new System.Drawing.Size(170, 18);
            this.lbPrompt.TabIndex = 0;
            this.lbPrompt.Text = "正在连接服务器……";
            // 
            // lbGetCardMsg
            // 
            this.lbGetCardMsg.AutoSize = true;
            this.lbGetCardMsg.Font = new System.Drawing.Font("SimSun", 13F);
            this.lbGetCardMsg.ForeColor = System.Drawing.Color.Red;
            this.lbGetCardMsg.Location = new System.Drawing.Point(36, 114);
            this.lbGetCardMsg.Name = "lbGetCardMsg";
            this.lbGetCardMsg.Size = new System.Drawing.Size(413, 18);
            this.lbGetCardMsg.TabIndex = 1;
            this.lbGetCardMsg.Text = "正在获取卡号消息……获取成功后自动关闭该窗口!";
            // 
            // ServerPrompt
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(526, 150);
            this.Controls.Add(this.lbGetCardMsg);
            this.Controls.Add(this.lbPrompt);
            this.Name = "ServerPrompt";
            this.Text = "ServerPrompt";
            this.Load += new System.EventHandler(this.ServerPrompt_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbPrompt;
        private System.Windows.Forms.Label lbGetCardMsg;
    }
}