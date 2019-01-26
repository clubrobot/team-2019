#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import igraph
from common.geogebra import Geogebra
from threading import Thread, Event
from time import sleep, time
import math


def intersect(A, B):
    (xA, yA), (xA2, yA2) = A
    (xB, yB), (xB2, yB2) = B
    dxA, dyA = xA2 - xA, yA2 - yA
    dxB, dyB = xB2 - xB, yB2 - yB
    det = dyA * dxB - dxA * dyB
    if det != 0:
        tA = (-dyB * (xB - xA) + dxB * (yB - yA)) / det
        tB = (-dyA * (xB - xA) + dxA * (yB - yA)) / det
        return tA > 0 and tA < 1 and tB > 0 and tB < 1


class Obstacle:
    def __init__(self, roadmap, shape):
        self.roadmap = roadmap
        self.shape = shape
        self.theta = 0
        self.position = (-1000, -1000)
        self.edges = list()

    def get_shape(self):
        return [(math.cos(self.theta) * a - math.sin(self.theta) * b + self.position[0],
                 math.sin(self.theta) * a + math.cos(self.theta) * b + self.position[1]) for (a, b) in self.shape]

    def set_position(self, x, y, theta=0):
        self.position = (x, y)
        self.theta = 0
        self.roadmap.reset_obstacle(self)
        self.roadmap.cut_obstacle(self)

    def disable(self):
        self.roadmap.reset_obstacle(self)


class ObstacleTemp(Obstacle):
    def __init__(self, roadmap, shape, timeout=1):
        Obstacle.__init__(self, roadmap, shape)
        self.thread = Thread(target=self._temp_disable)
        self.event = Event()
        self.timeout = timeout

    def set_position(self, x, y, theta=0, blocking=False):
        if self.thread.is_alive():
            return False
        self.position = (x, y)
        self.theta = theta
        self.roadmap.reset_obstacle(self)
        self.roadmap.cut_obstacle(self)
        self.event.set()
        if self.thread.is_alive():
            self.thread.join()
        self.event.clear()
        self.thread = Thread(target=self._temp_disable)
        self.thread.start()
        return True

    def _temp_disable(self):
        start_time = time()
        while time() - start_time < self.timeout and not self.event.is_set():
            sleep(0.1)
        self.disable()


class RoadMap:
    LEFT = 1
    RIGHT = -1

    def __init__(self, vertices=list(), edges=set()):
        self.graph = igraph.Graph()
        self.graph.add_vertices(len(vertices))
        self.graph.vs['coords'] = vertices
        self.graph.add_edges(edges)
        self.reset_edges()

    def reset_edges(self):
        for edge in self.graph.es:
            vertex = self.graph.vs[edge.target]['coords']
            edge['weight'] = self.get_vertex_distance(edge.source, vertex)
            edge['cuted_by'] = list()

    def cut_edges(self, cutline):
        for edge in self.graph.es:
            source = self.graph.vs[edge.source]['coords']
            target = self.graph.vs[edge.target]['coords']
            if intersect((source, target), cutline):
                edge['weight'] = math.inf

    def temp_cut_edges(self, cutline, time=1):
        cuted_edges = list()
        for edge in self.graph.es:
            source = self.graph.vs[edge.source]['coords']
            target = self.graph.vs[edge.target]['coords']
            if intersect((source, target), cutline):
                edge['weight'] = math.inf
                edge['cuted_by'].append("temp_cut")
                cuted_edges.append(edge)
        Thread(target=lambda: self._remove_cut_edges(cuted_edges, time)).start()

    def _remove_cut_edges(self, cuted_edges, time):
        sleep(time)
        for edge in cuted_edges:
            try:
                edge['cuted_by'].remove('temp_cut')
            except AttributeError:
                pass
            if len(edge['cuted_by']) == 0:
                vertex = self.graph.vs[edge.target]['coords']
                edge['weight'] = self.get_vertex_distance(edge.source, vertex)

    def create_obstacle(self, shape):
        return Obstacle(self, shape)

    def create_temp_obstacle(self, shape, timeout=1):
        return ObstacleTemp(self, shape, timeout)

    def cut_obstacle(self, obstacle):
        obstacle_shape = obstacle.get_shape()
        for edge in self.graph.es:
            source = self.graph.vs[edge.source]['coords']
            target = self.graph.vs[edge.target]['coords']
            for i in range(len(obstacle_shape)):
                cutline = (obstacle_shape[i], obstacle_shape[(i + 1) % len(obstacle_shape)])
                if intersect((source, target), cutline):
                    edge['weight'] = math.inf
                    if not obstacle in edge['cuted_by']:
                        edge['cuted_by'].append(obstacle)
                        obstacle.edges.append(edge)

    def reset_obstacle(self, obstacle):
        for edge in obstacle.edges:
            try:
                edge['cuted_by'].remove(obstacle)
            except (AttributeError, ValueError):
                pass

            if len(edge['cuted_by']) == 0:
                vertex = self.graph.vs[edge.target]['coords']
                edge['weight'] = self.get_vertex_distance(edge.source, vertex)
        obstacle.edges = list()

    def get_vertex_distance(self, vid, vertex):
        x0, y0 = self.graph.vs[vid]['coords']
        x1, y1 = vertex
        return math.hypot(x1 - x0, y1 - y0)

    def get_closest_vertex(self, vertex):
        def key(v): return self.get_vertex_distance(v.index, vertex)

        return min(self.graph.vs, key=key).index

    def add_vertex(self, vertex):
        closest = self.get_closest_vertex(vertex)
        neighbors = set()
        for edge in self.graph.es[self.graph.incident(closest)]:
            if edge['weight'] < math.inf:
                neighbors.add(edge.source)
                neighbors.add(edge.target)
        self.graph.add_vertices(1)
        vid = self.graph.vcount() - 1
        self.graph.vs[vid]['coords'] = vertex
        for target in neighbors:
            self.graph.add_edge(vid, target, weight=self.get_vertex_distance(target, vertex),cuted_by=list())
        return vid

    def best_side(self, x, y, theta):
        result = 0
        for (a, b) in self.graph.vs['coords']:
            a -= x
            b -= y
            result += math.copysign(1, -math.sin(theta) * a + math.cos(theta) * b)
        return math.copysign(1, result)

    def get_shortest_path(self, source, target):
        v = self.add_vertex(source)
        to = self.add_vertex(target)
        indexes = [v]
        for eid in self.graph.get_shortest_paths(v, to, weights='weight', output='epath')[0]:
            edge = self.graph.es[eid]
            if edge['weight'] == math.inf:
                raise RuntimeError('path not found')
            if indexes[-1] != edge.source:
                indexes.append(edge.source)
            else:
                indexes.append(edge.target)
        path = [self.graph.vs[vid]['coords'] for vid in indexes]
        self.graph.delete_vertices([v, to])
        return path

    @staticmethod  # Factory function
    def load(geogebra, pattern='roadmap_{\s*\d+\s*,\s*\d+\s*}'):
        # `geogebra` may be a GeoGebra file or a .ggb filename
        if isinstance(geogebra, str):
            geogebra = Geogebra(geogebra)
            geogebra.close()

        # Parse the geogebra file to get a list of segments which match the `pattern` regular expression
        begin = time()
        segments = geogebra.getall(pattern)  # Takes a while
        print("\tPARSING : ", time() - begin)

        # Deduce the graph vertices from the previous list of segmenets
        vertices = set()
        for segment in segments:
            vertices.add(segment[0])
            vertices.add(segment[1])

        # Convert the above set in a list so that vertices are now indexed
        vertices = list(vertices)

        # Compute the graph edges using the above vertices list
        edges = set()
        for segment in segments:
            i = vertices.index(segment[0])
            j = vertices.index(segment[1])
            edges.add((min(i, j), max(i, j)))

        # Give the hand to the RoadMap constructor
        roadmap = RoadMap(vertices, edges)
        return roadmap
