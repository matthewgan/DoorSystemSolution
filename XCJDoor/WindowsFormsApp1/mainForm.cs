using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using WindowsFormsApp1;

namespace XCJDoorSystem
{
    public partial class mainForm: Form
    {
        public mainForm()
        {
            InitializeComponent();
        }


        private void toolStripLabel1_Click(object sender, EventArgs e)
        {

        }

        //action after the form loaded
        private void mainForm_Load(object sender, EventArgs e)
        {
            //get defualt COM port selected (often the last one)
            this.comSelect.Items.AddRange(SerialPort.GetPortNames());
            this.comSelect.SelectedIndex = this.comSelect.Items.Count - 1;

            //set default btn state
            this.startConnectBtn.Enabled = true;
            this.stopConnectBtn.Enabled = false;

            this.nameBox.Enabled = false;
            this.IDBox.Enabled = false;
            this.editBtn.Enabled = false;
            this.submitBtn.Enabled = false;

            

            this.cardStateLab.Text = "DO NOT move the card while processing";
        }

        


        private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.DisposeSerialPort();
        }

        void port_DataRecived(object sender, SerialDataReceivedEventArgs e)
        {
            this.RefreshLogBox();
        }

        private SerialPort port = null;
        private void InitialSerialPort()
        {
            try
            {
                string portName = this.comSelect.SelectedItem.ToString();
                port = new SerialPort(portName, 9600);
                port.Encoding = Encoding.ASCII;
                port.DataReceived += port_DataRecived;
                port.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("初始化串口发生错误：" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void DisposeSerialPort()
        {
            if (port != null)
            {
                try
                {
                    if (port.IsOpen)
                    {
                        port.WriteLine("0#");
                        this.cardStateLab.Text = "Please scan a card to continue";
                        port.Close();
                    }
                    port.Dispose();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("关闭串口发生错误：" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }

        private string ReadSerialData()
        {
            string value = "";
            try
            {
                if (port != null && port.BytesToRead > 0)
                {
                    value = port.ReadLine();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("读取串口数据发生错误：" + ex.Message, "提示信息", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

            return value;
        }

        private void RefreshLogBox()
        {
            string value = this.ReadSerialData();
            Action<string> setValueAction = text => this.logBox.Text += text += "\r\n";

            if (this.logBox.InvokeRequired)
            {
                this.logBox.Invoke(setValueAction, value);
            }
            else
            {
                setValueAction(value);
            }

            if (value == "Card Found\r")
            {
                this.cardStateLab.Invoke(new MethodInvoker(delegate ()
                {
                    this.cardStateLab.Text = "Found a valid card.";
                }));
            }

            if (value.StartsWith("UID"))
            {
                this.uidInfoLab.Invoke(new MethodInvoker(delegate ()
                {
                    this.uidInfoLab.Text = value.Substring(4);
                }));
            }

            if (value.StartsWith("TYPE:"))
            {
                this.cardTypeLab.Invoke(new MethodInvoker(delegate ()
                {
                    this.cardTypeLab.Text = value.Substring(5);
                }));
            }

            if (value == "New factory card\r")
            {
                const string message =
                       "This is a new card. Click YES to make the XCJ card. (The card will be formated and encrypted)";
                const string caption = "Make Card";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.YesNo,
                                             MessageBoxIcon.Question);

               
                this.cardStateLab.Text = "This is a new card.";
                
                // If the no button was pressed ...
                if (result == DialogResult.Yes)
                {
                    if (port != null && port.IsOpen)
                    {
                        port.WriteLine("-1#");//Send command to Arduino
                    }
                    this.cardStateLab.Text = "Format card success. Re-scan to edit Info.";
                }
                else if (result == DialogResult.No)
                {
                    if (port != null && port.IsOpen)
                    {
                        port.WriteLine("-2#");//Send command to Arduino
                        this.cardStateLab.Invoke(new MethodInvoker(delegate ()
                        {
                            this.cardStateLab.Text = "Please scan a card to continue";
                        }));
                    }
                }
            }

            if (value.StartsWith("STAFF_NAME="))
            {
                this.nameBox.Invoke(new MethodInvoker(delegate ()
                {
                    this.nameBox.Text = value.Substring(11);
                }));
                this.cardStateLab.Invoke(new MethodInvoker(delegate ()
                {
                    this.editBtn.Enabled = true;
                }));
            }

            if (value.StartsWith("STAFF_ID="))
            {
                this.IDBox.Invoke(new MethodInvoker(delegate ()
                {
                    this.IDBox.Text = value.Substring(9);
                }));
            }

            if (value.StartsWith("CARD REMOVED"))
            {
                const string message =
                       "Card has been moved, please try again";
                const string caption = "Warning";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.OK,
                                             MessageBoxIcon.Information);

                this.cardStateLab.Invoke(new MethodInvoker(delegate ()
                {
                    this.cardStateLab.Text = "Please scan a card to continue";
                }));
            }

            if (value.StartsWith("UNKONW CARD!"))
            {
                const string message =
                       "Unknow Card, please try another one.";
                const string caption = "Warning";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.OK,
                                             MessageBoxIcon.Information);
 
                this.cardStateLab.Invoke(new MethodInvoker(delegate ()
                {
                    this.cardStateLab.Text = "Please scan a card to continue";
                }));

            }
        }

        //init the serial port after the button clicked
        private void startConnectBtn_Click(object sender, EventArgs e)
        {
            this.InitialSerialPort();
            this.startConnectBtn.Enabled = false;
            this.stopConnectBtn.Enabled = true;
        }

        //close the serial port after the button clicked
        private void stopConnectBtn_Click(object sender, EventArgs e)
        {
            this.DisposeSerialPort();
            this.stopConnectBtn.Enabled = false;
            this.startConnectBtn.Enabled = true;
        }


        private void clearLogBtn_Click(object sender, EventArgs e)
        {
            this.logBox.Text = "";
        }

        private void submitBtn_Click(object sender, EventArgs e)
        {
            if (port != null && port.IsOpen)
            {
                port.WriteLine("#"+nameBox.Text + "&");//Send command to Arduino
                port.WriteLine("-" + IDBox.Text + "+");//Send command to Arduino

            }
            SqlBridge myCon = new SqlBridge();
            if (myCon.Insert(nameBox.Text, IDBox.Text,0))
            {
                const string message =
                       "Process done. Data saved to database successfull";
                const string caption = "Congratulation";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.OK,
                                             MessageBoxIcon.Information);

            }
            else
            {
                const string message =
                       "Process failed. Please try again";
                const string caption = "Sorry";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.OK,
                                             MessageBoxIcon.Information);
            }
            this.cardStateLab.Text = "ALL DONE.";
            this.nameBox.Enabled = false;
            this.IDBox.Enabled = false;
            this.submitBtn.Enabled = false;
            this.editBtn.Enabled = false;
        }

        private void editBtn_Click(object sender, EventArgs e)
        {
            this.nameBox.Enabled = true;
            this.IDBox.Enabled = true;
            this.editBtn.Enabled = false;
            this.submitBtn.Enabled = true;
            if (port != null && port.IsOpen)
            {
                port.WriteLine("?START WRITE#");//Send command to Arduino

            }
            this.cardStateLab.Text = "Edit the name and ID for the card.";
        }

        private void changeCard_Click(object sender, EventArgs e)
        {
            this.nameBox.Enabled = false;
            this.IDBox.Enabled = false;
            this.submitBtn.Enabled = false;
            this.editBtn.Enabled = false;
            if (port != null && port.IsOpen)
            {
                port.WriteLine("?RESET#");//Send command to Arduino

            }
            this.cardStateLab.Text = "Please scan a card to continue";
        }
    }
}
