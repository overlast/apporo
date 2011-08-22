#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
from apporo_python import Apporo

app = Apporo(sys.argv[1]);
ret = app.retrieve(sys.argv[2])
for item in ret:
    print item
app.delete()
