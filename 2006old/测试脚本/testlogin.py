
from common.common import *





connect()

testcase = login
p_username = "myusername"
p_password = "mypassword"
ExpectResult = 1
desc = "test  login 1"

test( login, globals())

p_username = "no this user"
test( login, globals() )

p_username = "short"
test( login, globals() )




