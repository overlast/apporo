#!/usr/bin/env python
#-*- coding:utf-8 -*-
import apporo

class Apporo(object):
    def __init__(self, path):
        self.app = apporo.new(path)

    def approximate_match(self, query):
        return apporo.approximate_match(self.app, query)

    def delete(self):
        apporo.delete(self.app)
        self.app = None


