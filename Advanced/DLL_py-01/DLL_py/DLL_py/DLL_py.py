'''
编码：袁春旭
讲解：袁春旭
内容：python调用DLL中的带参函数
'''
from ctypes import *

dll = windll.LoadLibrary('helloworlddef.dll')

dll.SayHello()

dll2 = windll.LoadLibrary('Calculater.dll')

print(dll2.add1(3, 5))