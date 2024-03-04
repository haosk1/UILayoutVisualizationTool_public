#coding:utf-8
import sys
import os
import platform

reload(sys)
sys.setdefaultencoding("utf-8")

os.system("wine cmd /c GraphicsDrawableReName.bat")

os.system("exit")
