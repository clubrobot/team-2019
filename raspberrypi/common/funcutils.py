import threading, time, signal

from datetime import timedelta

# Job class to create thread when function are called
class Job(threading.Thread):
    def __init__(self, execute, *args, **kwargs):
        threading.Thread.__init__(self)
        self.daemon = False
        self.stopped = threading.Event()
        self.execute = execute
        self.args = args
        self.kwargs = kwargs
        
    def stop(self):
        self.stopped.set()
        self.join()
    
    def run(self):
        self.execute(*self.args, **self.kwargs)
        self.stopped.set()

    def __bool__(self):
        return self.stopped.is_set()

# Func class to bind existing func to it's threading homolog
class ThreadMethod():
    def __init__(self, func):
        self.func       = func
        self.job        = None

    def __call__(self, *args, **kwargs):
        self.args   = args
        self.kwargs = kwargs
        self.job    = Job(self.process)
        self.job.stopped.clear()
        self.job.start()

    def process(self):
        print('Process')
        self.func(*self.args, **self.kwargs)

    def abort(self):
        self.job.stop()
    
    def end(self):
        return self.job
