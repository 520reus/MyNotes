#!/usr/bin/env python
#-*- coding:UTF-8 -*-（添加）

import numpy as np
import pandas as pd
from scipy import io
import os
import matplotlib as mpl
import matplotlib.pyplot as plt

def arrayToList(array,featureName):
    """
    将数组转换成列表
    :param array: 需要转换的数组
    :param featureName:
    :return: 返回一个列表
    """
    list = []
    for i in range(len(array)):
        dict = {}
        temp = array[i]
        for j in range(len(temp)):
            A = featureName + str(j)
            dict[A] = temp[j]
        list.append(dict)
    return list


path = '../data/xd_traj1.mat'
change_path = os.path.splitext(path)
data = io.loadmat(path)
arrayX = data['1']
arrayY = data['2']
listX = arrayToList(arrayX,'x')
listY = arrayToList(arrayY,'y')
DataFrameX = pd.DataFrame(listX)
DataFrameY = pd.DataFrame(listY)
sum = pd.concat([DataFrameX,DataFrameY],axis=1)
sum.to_csv(change_path[0]+'1.csv')

