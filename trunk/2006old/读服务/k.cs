using System;
using System.Diagnostics;
              
class MySample{

    public static void Main(){

        EventLog myLog = new EventLog();
        myLog.Log = "Application";                      
        foreach(EventLogEntry entry in myLog.Entries){
			if(!entry.EntryType.ToString().Equals("Information"))
				Console.WriteLine("\nEntry: " + entry.Message);
        } 
		
		myLog.Log = "System";                      
        foreach(EventLogEntry entry in myLog.Entries){
			if(!entry.EntryType.ToString().Equals("Information"))
				Console.WriteLine("\nEntry: " + entry.Message);
        }  

		
    }       
}
