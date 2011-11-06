from win32com.client import constants, Dispatch

import os

class excel:
	
	def __init__(self):
		self.xlApp = Dispatch('Excel.Application')
		

	def open( self, path):
		self.path = os.path.realpath(path)
		try:
			self.xlBook = self.xlApp.Workbooks.Open(self.path)
		except Exception, x:
			print x[1], x[2][2]
			pass
	
	def sheet(self, name):
		self.sht = self.xlBook.Worksheets(name)

	def getHeaders(self):
		ret = []
		for e in range( self.sht.Columns.Count ):
			text = self.sht.Cells(1, e+1).Text
			if text=="" : break
			ret.append(text)
		
		return ret
	
	def getText(self, row , col):
		return self.sht.Cells(row, col).Text
	
	def getSize(self):
		row_num = 0
		col_num = 0
		for e in range( self.sht.Columns.Count ):
			text = self.sht.Cells(1, e+1).Text
			if text=="" : 
				col_num = e
				break
		
		for e in range( self.sht.Rows.Count ):
			text = self.sht.Cells(e+1, 1).Text
			if text == "":
				row_num = e
				break
		
		return row_num, col_num
	
	def getAll(self):
		all = []
		row_num, col_num = self.getSize()
		headers = self.getHeaders()
		for row in range(row_num)[1:]:
			rowitems = {}
			for col in range(col_num):
				rowitems[headers[col]] = self.getText(row+1, col+1)
			all.append(rowitems)
			
		
		return all
				
			
			

	def close(self):
		self.xlBook.Close(0)
		del self.xlApp




if __name__ == "__main__":
	xl = excel()
	xl.open("testreg.xls")
	xl.sheet("testcase")
	print xl.getHeaders()
	print xl.getSize()
	print xl.getAll()
	xl.close()

