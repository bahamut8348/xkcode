from functools import wraps

class HijackReturn(Exception):
    def __init__(self, val):
        self.val = val

def hijack(f):
    @wraps(f)
    def hijacked(*args, **kwargs):
        try:
            return f(*args, **kwargs)
        except HijackReturn, e:
            return e.val
    return hijacked

@hijack
def foo(x):
    x = x + 1
    hijacker(x)
    return x * 2

def hijacker(x):
    if x == 21:
        print "[Oh no, it's going to be 42!]"
        raise HijackReturn(41)

