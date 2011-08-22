#!/usr/bin/env python
#-*- coding:utf-8 -*-
import apporo

class Apporo(object):
    def __init__(self, path):
        self.app = apporo.new(path)

    def retrieve(self, query):
        return apporo.retrieve(self.app, query)

    def delete(self):
        apporo.delete(self.app)
        self.app = None


