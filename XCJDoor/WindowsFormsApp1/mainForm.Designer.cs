namespace XCJDoorSystem
{
    partial class mainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainForm));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.comSelect = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.startConnectBtn = new System.Windows.Forms.ToolStripButton();
            this.stopConnectBtn = new System.Windows.Forms.ToolStripButton();
            this.clearLogBtn = new System.Windows.Forms.ToolStripButton();
            this.logBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cardStateLab = new System.Windows.Forms.Label();
            this.cardInfo = new System.Windows.Forms.GroupBox();
            this.changeCard = new System.Windows.Forms.Button();
            this.editBtn = new System.Windows.Forms.Button();
            this.submitBtn = new System.Windows.Forms.Button();
            this.IDBox = new System.Windows.Forms.TextBox();
            this.nameBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.cardTypeLab = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.uidInfoLab = new System.Windows.Forms.Label();
            this.uidLab = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.toolStrip1.SuspendLayout();
            this.cardInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.comSelect,
            this.toolStripSeparator1,
            this.startConnectBtn,
            this.stopConnectBtn,
            this.clearLogBtn});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(473, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(38, 22);
            this.toolStripLabel1.Text = "COM";
            this.toolStripLabel1.Click += new System.EventHandler(this.toolStripLabel1_Click);
            // 
            // comSelect
            // 
            this.comSelect.BackColor = System.Drawing.SystemColors.Window;
            this.comSelect.Name = "comSelect";
            this.comSelect.Size = new System.Drawing.Size(121, 25);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // startConnectBtn
            // 
            this.startConnectBtn.ForeColor = System.Drawing.SystemColors.ControlText;
            this.startConnectBtn.Image = ((System.Drawing.Image)(resources.GetObject("startConnectBtn.Image")));
            this.startConnectBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.startConnectBtn.Name = "startConnectBtn";
            this.startConnectBtn.Size = new System.Drawing.Size(106, 22);
            this.startConnectBtn.Text = "Start Connect";
            this.startConnectBtn.Click += new System.EventHandler(this.startConnectBtn_Click);
            // 
            // stopConnectBtn
            // 
            this.stopConnectBtn.Image = ((System.Drawing.Image)(resources.GetObject("stopConnectBtn.Image")));
            this.stopConnectBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.stopConnectBtn.Name = "stopConnectBtn";
            this.stopConnectBtn.Size = new System.Drawing.Size(106, 22);
            this.stopConnectBtn.Text = "Stop Connect";
            this.stopConnectBtn.Click += new System.EventHandler(this.stopConnectBtn_Click);
            // 
            // clearLogBtn
            // 
            this.clearLogBtn.Image = ((System.Drawing.Image)(resources.GetObject("clearLogBtn.Image")));
            this.clearLogBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.clearLogBtn.Name = "clearLogBtn";
            this.clearLogBtn.Size = new System.Drawing.Size(84, 22);
            this.clearLogBtn.Text = "Clear Log";
            this.clearLogBtn.Click += new System.EventHandler(this.clearLogBtn_Click);
            // 
            // logBox
            // 
            this.logBox.Location = new System.Drawing.Point(12, 323);
            this.logBox.Multiline = true;
            this.logBox.Name = "logBox";
            this.logBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.logBox.Size = new System.Drawing.Size(451, 144);
            this.logBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label1.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(12, 45);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 21);
            this.label1.TabIndex = 2;
            this.label1.Text = "State:";
            // 
            // cardStateLab
            // 
            this.cardStateLab.AutoSize = true;
            this.cardStateLab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.cardStateLab.Font = new System.Drawing.Font("Tahoma", 10.5F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cardStateLab.Location = new System.Drawing.Point(79, 47);
            this.cardStateLab.Name = "cardStateLab";
            this.cardStateLab.Size = new System.Drawing.Size(42, 17);
            this.cardStateLab.TabIndex = 3;
            this.cardStateLab.Text = "label2";
            // 
            // cardInfo
            // 
            this.cardInfo.Controls.Add(this.changeCard);
            this.cardInfo.Controls.Add(this.editBtn);
            this.cardInfo.Controls.Add(this.submitBtn);
            this.cardInfo.Controls.Add(this.IDBox);
            this.cardInfo.Controls.Add(this.nameBox);
            this.cardInfo.Controls.Add(this.label4);
            this.cardInfo.Controls.Add(this.label3);
            this.cardInfo.Controls.Add(this.cardTypeLab);
            this.cardInfo.Controls.Add(this.label2);
            this.cardInfo.Controls.Add(this.uidInfoLab);
            this.cardInfo.Controls.Add(this.uidLab);
            this.cardInfo.Font = new System.Drawing.Font("Tahoma", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cardInfo.Location = new System.Drawing.Point(12, 67);
            this.cardInfo.Name = "cardInfo";
            this.cardInfo.Size = new System.Drawing.Size(451, 227);
            this.cardInfo.TabIndex = 4;
            this.cardInfo.TabStop = false;
            // 
            // changeCard
            // 
            this.changeCard.Location = new System.Drawing.Point(292, 23);
            this.changeCard.Name = "changeCard";
            this.changeCard.Size = new System.Drawing.Size(105, 39);
            this.changeCard.TabIndex = 7;
            this.changeCard.Text = "Reset Card";
            this.changeCard.UseVisualStyleBackColor = true;
            this.changeCard.Click += new System.EventHandler(this.changeCard_Click);
            // 
            // editBtn
            // 
            this.editBtn.Location = new System.Drawing.Point(89, 144);
            this.editBtn.Name = "editBtn";
            this.editBtn.Size = new System.Drawing.Size(271, 30);
            this.editBtn.TabIndex = 10;
            this.editBtn.Text = "Edit Info";
            this.editBtn.UseVisualStyleBackColor = true;
            this.editBtn.Click += new System.EventHandler(this.editBtn_Click);
            // 
            // submitBtn
            // 
            this.submitBtn.Location = new System.Drawing.Point(89, 180);
            this.submitBtn.Name = "submitBtn";
            this.submitBtn.Size = new System.Drawing.Size(271, 30);
            this.submitBtn.TabIndex = 9;
            this.submitBtn.Text = "Submit";
            this.submitBtn.UseVisualStyleBackColor = true;
            this.submitBtn.Click += new System.EventHandler(this.submitBtn_Click);
            // 
            // IDBox
            // 
            this.IDBox.Location = new System.Drawing.Point(253, 104);
            this.IDBox.Name = "IDBox";
            this.IDBox.Size = new System.Drawing.Size(175, 24);
            this.IDBox.TabIndex = 7;
            // 
            // nameBox
            // 
            this.nameBox.Location = new System.Drawing.Point(253, 78);
            this.nameBox.Name = "nameBox";
            this.nameBox.Size = new System.Drawing.Size(175, 24);
            this.nameBox.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(11, 107);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(226, 17);
            this.label4.TabIndex = 5;
            this.label4.Text = "Personal ID (Less than 10 number):";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(11, 81);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(227, 17);
            this.label3.TabIndex = 4;
            this.label3.Text = "Person Name (Less than 15 letters):";
            // 
            // cardTypeLab
            // 
            this.cardTypeLab.AutoSize = true;
            this.cardTypeLab.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.cardTypeLab.Location = new System.Drawing.Point(145, 41);
            this.cardTypeLab.Name = "cardTypeLab";
            this.cardTypeLab.Size = new System.Drawing.Size(62, 19);
            this.cardTypeLab.TabIndex = 3;
            this.cardTypeLab.Text = "Mifire M1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(131, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 19);
            this.label2.TabIndex = 2;
            this.label2.Text = "Card Type";
            // 
            // uidInfoLab
            // 
            this.uidInfoLab.AutoSize = true;
            this.uidInfoLab.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.uidInfoLab.Font = new System.Drawing.Font("Tahoma", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.uidInfoLab.Location = new System.Drawing.Point(39, 41);
            this.uidInfoLab.Name = "uidInfoLab";
            this.uidInfoLab.Size = new System.Drawing.Size(59, 19);
            this.uidInfoLab.TabIndex = 1;
            this.uidInfoLab.Text = "NS0100";
            // 
            // uidLab
            // 
            this.uidLab.AutoSize = true;
            this.uidLab.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.uidLab.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.uidLab.Location = new System.Drawing.Point(26, 20);
            this.uidLab.Name = "uidLab";
            this.uidLab.Size = new System.Drawing.Size(83, 19);
            this.uidLab.TabIndex = 0;
            this.uidLab.Text = "Card UID";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 308);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "System Log";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(302, 470);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(161, 12);
            this.label6.TabIndex = 6;
            this.label6.Text = "ALL RIGHT RESERVE BY ROXAS";
            // 
            // mainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 487);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.cardInfo);
            this.Controls.Add(this.cardStateLab);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.logBox);
            this.Controls.Add(this.toolStrip1);
            this.Name = "mainForm";
            this.Text = "新车间门禁管理系统";
            this.Load += new System.EventHandler(this.mainForm_Load);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.cardInfo.ResumeLayout(false);
            this.cardInfo.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripComboBox comSelect;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.TextBox logBox;
        private System.Windows.Forms.ToolStripButton startConnectBtn;
        private System.Windows.Forms.ToolStripButton stopConnectBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label cardStateLab;
        private System.Windows.Forms.GroupBox cardInfo;
        private System.Windows.Forms.Label uidInfoLab;
        private System.Windows.Forms.Label uidLab;
        private System.Windows.Forms.Label cardTypeLab;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolStripButton clearLogBtn;
        private System.Windows.Forms.Button submitBtn;
        private System.Windows.Forms.TextBox IDBox;
        private System.Windows.Forms.TextBox nameBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button editBtn;
        private System.Windows.Forms.Button changeCard;
    }
}

