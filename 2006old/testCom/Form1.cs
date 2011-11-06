using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using System.Runtime.InteropServices;
using System.Reflection;

namespace testCom
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
            //Type t = Type.GetTypeFromProgID("PythonDemos.Utilities");
            //Object o = Activator.CreateInstance(t);
            //Type.InvokeMember             
            Object o = (object)Interaction.CreateObject("PythonDemos.Utilities", "");
            string ret = (string )Interaction.CallByName(o, "GetDict", CallType.Get, null);
            Array rets = (Array)Interaction.CallByName(o, "SplitString", CallType.Get, new object[] { "1 2 3 4" });
            string s= rets.GetValue(1).ToString();
           

            Type t = Type.GetTypeFromProgID("PythonDemos.Utilities");
            Object o2 = Activator.CreateInstance(t);
           Object ss =  t.InvokeMember("GetDict", BindingFlags.InvokeMethod, null, o2, null);

           s = ss.ToString();

        }
    }
}