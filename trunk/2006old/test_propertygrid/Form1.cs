using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;
using System.Diagnostics;



namespace test_propertygrid
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        class MyObject : ICustomTypeDescriptor
        {
            public object mObject; 
            
            public AttributeCollection GetAttributes()
             {
                 return TypeDescriptor.GetAttributes(mObject);
             }

            public string GetClassName()
            {
                return TypeDescriptor.GetClassName(mObject);
            }

            public string GetComponentName()
            {
                return TypeDescriptor.GetComponentName(mObject);
            }
            public TypeConverter GetConverter()
            {
                return TypeDescriptor.GetConverter(mObject);
            }
            public EventDescriptor GetDefaultEvent()
            {
                return TypeDescriptor.GetDefaultEvent(mObject);
            }
            public PropertyDescriptor GetDefaultProperty()
            {
                return TypeDescriptor.GetDefaultProperty(mObject);
            }
            public object GetEditor(Type editorBaseType)
            {
                return TypeDescriptor.GetEditor(mObject, editorBaseType);
            }
            public EventDescriptorCollection GetEvents(Attribute[] attributes)
            {
                return TypeDescriptor.GetEvents(mObject, attributes);
            }
            public EventDescriptorCollection GetEvents()
            {
                return TypeDescriptor.GetEvents(mObject);
            }

            public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
            {
                return TypeDescriptor.GetProperties(mObject, attributes);
            }
            
            public PropertyDescriptorCollection GetProperties()
            {
                return TypeDescriptor.GetProperties(mObject);
            }

            public object GetPropertyOwner(PropertyDescriptor pd)
            {
                return mObject;
            }
        }

        public class HelpAttribute : Attribute
        {
            public HelpAttribute()
            {
               string s=  this.ToString();
            }
            
        }

        
        class Haha
        {
         
            int a;
            int b;
                        
           
                 
            public int ValueA
            {
                get { return a; }
                set { a = value; }
                
            }


            [Conditional("ZHANG")]
            [Help]
            void test ()
            {

            }
           
        }

        
        void InitGrid( out int a)
        {
            MyObject o = new MyObject();
            o.mObject = new Haha();
            Haha h = new Haha();
            h.ValueA = 1;

            a = 1;
            

            string s = "sdf";

            AttributeCollection acs = TypeDescriptor.GetAttributes( new Haha() );

            grid1.SelectedObject = o;

           

          
            
        }

        int foo<T1>()
        {
            return 1;
        }

       

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'psplayerDataSet.Account' table. You can move, or remove it, as needed.
            this.accountTableAdapter.Fill(this.psplayerDataSet.Account);
            int c = 0;
            InitGrid(out c);
            Debug.WriteLine(c);


            
        }

        private void fillByToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {
                this.accountTableAdapter.FillBy(this.psplayerDataSet.Account);
            }
            catch (System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message);
            }

        }

        

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            try
            {
                this.accountTableAdapter.PD(this.psplayerDataSet.Account);
            }
            catch (System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.Message);
            }

        }
    }
}