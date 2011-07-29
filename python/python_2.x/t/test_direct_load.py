#!/usr/bin/env python
#-*- coding:utf-8 -*-
import sys
import apporo

app = apporo.new(sys.argv[1]);
ret = apporo.approximate_match(app, sys.argv[2]);
for item in ret:
    print item
ret = None
res = apporo.delete(app);
res = None
app = None
