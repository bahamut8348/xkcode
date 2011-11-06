using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;


namespace MakeStrTable
{
    
   
    
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            toolStripComboBox1.Items.Add("Brwose...");
        }

        private void startToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string[] paths = {"error_ap.h", "error_bp.h", "error_cp.h", "error_ip.h", "error_mp.h", "error_np.h", 
            "error_sp.h", "error_wedp.h" };
            string basepath = "E:\\Work\\0429\\PSServer\\Include\\Errors\\";
            CutString cs = new CutString(@"e:\Work\0429\PSServer\Language\Res\ErrorText.txt");
            richTextBox1.AppendText(cs.makeString2());
            foreach( string path in paths )
            {
                //CutString cs = new CutString( basepath + path );
                // richTextBox1.AppendText(cs.makeString());
               
            }
            
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void toolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = toolStripComboBox1.SelectedIndex;
            if( idx == toolStripComboBox1.Items.Count-1 )
            {
                folderdlg1.ShowDialog();
                string selepath = folderdlg1.SelectedPath;
                
            }
        }
    }


    class CutString
    {
        string mPath;
        string[] mFilters = { "" };
        string[] mReplace = { "" };

        public CutString(string path)
        {
            mPath = path;

        }

        public bool checkString(string str)
        {
            foreach (string s in mFilters)
            {
                if( s== "")
                {
                    break;
                }
                Regex r = new Regex(s, RegexOptions.IgnoreCase);
                Match m = r.Match(str);
                if (m.Success)
                {
                    return true;
                }
            }
            return false;
        }

        public string makeString2()
        {
            string ret = "";
            string[] lines = File.ReadAllLines(mPath);
            foreach (string l in lines)
            {
                string line = l.Trim();
                int pos = line.IndexOf(",");
                if (pos > 0)
                {
                    
                    string key = line.Substring(pos).Trim( " \",\t".ToCharArray() );
                    if (checkString(key))
                    {
                        continue;
                    }

                    foreach (string s in mReplace)
                    {
                        if( s=="" )
                        {
                            break;
                        }
                        if (key.Contains(s))
                        {
                            key = key.Replace(s, "");
                        }
                    }

                    key.Trim(" \t\",".ToCharArray());

                    if (checkString(key))
                    {
                        continue;
                    }

                    key.Trim(" \t\",".ToCharArray());

                    if (key != "")
                    {
                        string final;
                        ret += key +"= \"" + key  + "\"\r\n";
                    }
                }
            }


            return ret;
        }

        public string makeString()
        {
            string ret = "";
            string[] lines = File.ReadAllLines(mPath);
            foreach (string l in lines)
            {
                string line = l.Trim();
                int pos = line.LastIndexOf("=");
                if (pos > 0)
                {

                    string key = line.Substring(0, pos).Trim();


                    if (checkString(key))
                    {
                        continue;
                    }

                    foreach (string s in mReplace)
                    {
                        if (key.Contains(s))
                        {
                            key = key.Replace(s, "");
                        }
                    }
                    key = key.Trim();

                    if (checkString(key))
                    {
                        continue;
                    }

                    if (key != "")
                    {
                        string final;
                        ret += key + "\r\n";
                    }
                }
            }


            return ret;
        }

       

    }
}