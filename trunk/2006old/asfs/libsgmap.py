from libsg import SG
import math

sg = None

import sqlite3 

class MapInfo:
	
	def __init__(self):
		self.conn = sqlite3.connect("map.sqlite")
		self.cursor = self.conn.cursor()
	
	def insert(self, type, pos, x, y, distance ):
		row = self.getinfo(pos)
		if row :
			if row[0]!= type:
				print "update", pos, type
				self.cursor.execute("update npc set type = %d where pos=%d" % (type, pos) )
			return
		self.cursor.execute("insert into npc ( type, pos, x,y, distance, status ) values (%d,%d,%d,%d,%f,%d)" % (type, pos, x,y, distance, 0 ))
		self.conn.commit()
	
	def getinfo(self, pos ):
		self.cursor.execute("select type,distance from  npc where pos = %d " % pos )
		row = self.cursor.fetchone()
		return row
	
	def getbest(self, t ):
		self.cursor.execute("select type, pos , distance from npc where type in %s order by distance limit 10" % ( str(t) ) )
		rows = self.cursor.fetchall()
		return rows
	
	def getbestforme(self, t , p ):
		
		self.cursor.execute("select pos, x,y, type from npc where type in %s" % ( str(t) )  )
		rows = self.cursor.fetchall()
		rows2 = map( (lambda x : ( x[3], x[0], math.sqrt((x[1]-p[0])**2 + (x[2]-p[1])**2) )) , rows )
		print "type", t, "count:", len( rows2 )
		rows2.sort( key =lambda x: x[2] )
		return rows2

	
	def remove( self, pos ):
		self.cursor.execute("delete from npc where pos = %d " % pos )
		self.conn.commit()
	
	def findall(self, sg ):
		n = 70
		for x  in range(-n, n, 9 ):
			for y in range( -n, n, 8 ):
				npcs = sg.lookup_xy( x, y)['npc_tent']
				for npc in npcs:
					a,b = sg.calc_xy(npc[2])
					self.insert( npc[1], npc[2], a, b, math.sqrt(a**2 + b**2) )
					print npc, a, b ,"distans:" ,math.sqrt(a**2 + b**2)

import os,time
def getmapinfo(s = None):
	sg = SG()
	fn = "lasttime"
	print "start get map info "
	if not os.path.exists(fn): 
		open(fn,"wb").write("0")
	lasttime = float(open(fn).read())
	if time.time() - lasttime > 600:
		m = MapInfo()
		print m.findall( sg )
	
		open(fn,"wb").write(str(time.time()))
	print "end get map info."
if __name__ == "__main__":
	getmapinfo()
	m = MapInfo()
	m.getbestforme( (1,2), (0,1) )