

from libsg import SG, tostr

cs = [116399,125463,145742,57747,63829, 164347,121356]

sg = SG()

"""
[[2058022, 1, 0, '', 1462, '\xe5\x80\xad\xe5\x9b\xbd\xe9\x83\xa8\xe9\x98\x9f', 294, -553, [[470166, '\xe5\xae\x8b\xe5\xb8\xad\xe9\xa6\xa5']], 8000, 0, 2000, 0,1299, 0], [2060196, 12, 0, '', 0, '\xe7\xbb\xbf\xe6\xb4\xb2', 214, -587, [[460058, '\xe5\x8f\xb8\xe9\xa9\xac\xe8\x97\xa9']], 0, 0, 0, 0, 2707, 0]]

"""

def test():
	info = sg.get_battle_info( cs[0] )
	for bt in  info['battle']:
		print bt
	
	info = sg.get_mili_info( cs[0] )
	for gt in info['goto']:
		print gt
		print tostr( gt[5] ), gt[-2], sg.stand_mid( gt[6], gt[7]) , sg.tomyxy(gt[6], gt[7])
	
	
	for gt in info['come']:
		pass

test()
#print sg.get_current_wars( cs[0] )

