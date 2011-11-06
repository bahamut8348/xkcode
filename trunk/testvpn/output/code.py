
import json

def enc( obj ):
	return json.dumps( obj )

def dec( buf ):
	return json.loads( buf )
