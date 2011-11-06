using System;
using System.Runtime.InteropServices;





public abstract  class Base
{
	abstract public int foo();
	abstract public int get(int c);
}






class  N
{
	
	[DllImport("m.dll", EntryPoint="CreateA")]
	static extern int  CreateA();

	static unsafe void  Main(string[] s)
	{

		
		int *pa;
		//IntPtr pnt = Marshal.AllocHGlobal(1024);
		IntPtr pnt = (IntPtr)CreateA();
		System.Console.WriteLine(pnt.ToString());
		Marshal.PtrToStructure(pnt, typeof(Base));
		//System.Console.WriteLine(&p);
		System.Console.WriteLine(CreateA());
		return;
	}
}