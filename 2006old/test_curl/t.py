import pycurl

print pycurl.version


c = pycurl.Curl()
c.setopt(pycurl.URL, "http://163.com")
c.setopt(pycurl.FOLLOWLOCATION, 1)
c.perform()
print c.getinfo(pycurl.HTTP_CODE), c.getinfo(pycurl.EFFECTIVE_URL)

import time
time.sleep(100)