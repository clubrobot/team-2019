from math import exp, log
import matplotlib.pyplot as plt
import numpy as np




class Funct:
    def __init__(self, arguments, kwargs):
        if not isinstance(kwargs, dict):
            raise AttributeError("not a dict given !")
        for arg in arguments:
            if kwargs.get(arg) is None:
                raise AttributeError("{} parameter not given !".format(arg))
            object.__setattr__(self, arg, kwargs.get(arg))
            
    def __call__(self, vect, distance):
        if distance<=0:
            return self.saturate(vect)
        return [vect[0]*self.apply(distance), vect[1]*self.apply(distance)]

    def saturate(self, vect):
        return [vect[0]*self.get_max(), vect[1]*self.get_max()]

    def get_max(self):
        raise NotImplementedError()

    # permet d'appliquer la fonction sachant que x>0
    def apply(self, x):
        raise NotImplementedError()


class Exp(Funct):
    args = ["alpha", "beta"]
    def __init__(self, **kwargs):
        Funct.__init__(self, Exp.args, kwargs)

    def apply(self, scalaire):
        return exp(-scalaire*self.alpha)*self.beta

    def get_max(self):
        return self.beta


class Log(Funct):
    args = ["alpha", "beta", "ceta"]
    def __init__(self, **kwargs):
        Funct.__init__(self, Log.args, kwargs)

    def apply(self, scalaire):
        if(-scalaire*self.beta+self.alpha)<=0:
            return 0
        return max(0,log(-1*scalaire*self.beta+self.alpha)+self.ceta)

    def get_max(self):
        return self.apply(0)


class Lin(Funct):
    args = ["a", "b"]

    def __init__(self, **kwargs):
        Funct.__init__(self, Lin.args, kwargs)
    
    def apply(self, scalaire):
        return max(0, scalaire*-1*self.a + self.b)

    def get_max(self):
        return self.b


funct_list = {"lin" : Lin, "exp" : Exp, "log": Log}

if __name__ == "__main__":
    a = Log(alpha=7, beta=0.3, ceta=2)
    t = np.arange(-1.0, 20.0, 0.01)
    s = np.zeros_like(t, dtype=np.float32)
    for i in range(t.shape[0]):
        s[i] = a([1,1], t[i])[0]
    
    fig, ax = plt.subplots()
    ax.plot(t, s)

    ax.grid()
    plt.show()
