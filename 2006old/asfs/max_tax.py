import gdata.spreadsheet.service
import gdata.service
import atom.service
import gdata.spreadsheet
import string
from libsg import SG, tostr
class Data:
	def __init__(self):
		self.username = "xkwanted"
		self.password = "32810443"
		self.client = gdata.spreadsheet.service.SpreadsheetsService()
		self.client.email = self.username
		self.client.password = self.password
		self.client.source = "test xk"
		self.client.ProgrammaticLogin()
	
	def get_list(self):
		feed = self.client.GetSpreadsheetsFeed()
		self._PrintFeed(feed)
	def get_worksheets(self, key ):
		feed = self.client.GetWorksheetsFeed(key)
		print tostr( str( feed) )
	
	def query_test(self):
		query = gdata.spreadsheet.service.CellQuery()
		query['min-col'] = '1'
		query['max-col'] = '5'
		query['min-row'] = '1'
		query['max-row'] = '1'
		feed = self.client.GetCellsFeed("pFH5K-hMXAmr_25slvG3teg", "pFH5K-hMXAmr_25slvG3teg", query=query)
		print tostr( str(feed) )
	
	def init(self):
		id = 19
		#feed = self.client.GetSpreadsheetsFeed()
		#idpart = feed.entry[id].id.text.split('/')
		#print idpart
		self.key = 'pFH5K-hMXAmr_25slvG3teg'
		# first worksheet
		feed = self.client.GetWorksheetsFeed(self.key )
		idpart = feed.entry[0].id.text.split('/')
		self.wskey = idpart[-1]
		return 
		query = gdata.spreadsheet.service.CellQuery()
		query['min-col'] = '1'
		query['max-col'] = '2'
		query['min-row'] = '1'
		query['max-row'] = '1'
		feed = self.client.GetCellsFeed(key, wskey, query=query)
		print feed.entry[1].content.text

		feed = self.client.UpdateCell(row=1, col=3, inputValue=5, key=key, wksht_id=wskey)
		print feed
	def set_value( self, vs ):
		def setvalue(col, text ):
			self.client.UpdateCell(row=10, col=col, inputValue=str(text), key=self.key, wksht_id=self.wskey)
		setvalue( 3, vs[0] )
		setvalue( 5, vs[1] )
		setvalue( 6, vs[2] )
		setvalue( 10, vs[3] )
		setvalue( 11, vs[4] )
		setvalue( 12, vs[5] )
	
	def get_tax( self ):
		query = gdata.spreadsheet.service.CellQuery()
		query['min-col'] = '13'
		query['max-col'] = '14'
		query['min-row'] = '10'
		query['max-row'] = '10'
		feed = self.client.GetCellsFeed(self.key, self.wskey, query=query)
		a =feed.entry[0].content.text
		b = feed.entry[1].content.text
		return a,b

	def _PrintFeed(self, feed):
		for i, entry in enumerate(feed.entry):
			if isinstance(feed, gdata.spreadsheet.SpreadsheetsCellsFeed):
				print '%s %s\n' % ( tostr(entry.title.text), tostr(entry.content.text))
			elif isinstance(feed, gdata.spreadsheet.SpreadsheetsListFeed):
				print '%s %s %s' % (i, tostr(entry.title.text), tostr(entry.content.text))
				# Print this row's value for each column (the custom dictionary is
				# built using the gsx: elements in the entry.)
				print 'Contents:'
				for key in entry.custom: 
					print ' %s: %s' % (key, entry.custom[key].text)
				print '\n',
			else:
				print '%s %s\n' % (i, tostr(entry.title.text))


def main():
	d = Data()

	cids =  [79652,125463,145742,63829, 169578]
	d.init()

	sg = SG()
	print 
	print 
	def check(i):
		v = []
		
		sg.change_city( i )
		info =  sg.get_people_info()
		v.append( info[1] )
		v.append( info[4] )
		v.append( info[3] )
		info = sg.get_fin_info()
		name = tostr( info[1] )
		v.append( -info[4] )
		v.append( -info[5] )
		v.append( -info[-3] )
		d.set_value( v )
		return name,d.get_tax()
	for c in cids:
		ret = check(c)
		print ret[0], ret[1][1], ret[1][0]

	
	

main()