using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Web;


namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            String[] all =  textBox1.Text.Split('\n');
            foreach (string s in all)
            {

            }
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://v216.ku6.com/226/213/wNfpI1IhaQHyGXEOholFTMg.flv");
            request.KeepAlive = false;
            WebResponse res = request.GetResponse();
           string data =  res.ToString();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = "GET /226/213/wNfpI1IhaQHyGXEOholFTMg.flv HTTP/1.1\r\n" +
                "Accept: */*\r\n" +
                "Accept-Language: zh-cn\r\n" + 
                "UA-CPU: x86\r\n" +
                "Accept-Encoding: gzip, deflate\r\n" +
                "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; TencentTraveler ; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n" +
                "Host: v216.ku6.com\r\n" + 
                "Connection: Keep-Alive\r\n" +
                "Cookie: vjuids=3306481d.1178b4d7c3f.0.63eef64dbe1bdc; vjlast=1201485768; message=4722f96e55beebe1be2a4a7dcc715d95\r\n";
        }
    }
}