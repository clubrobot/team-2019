#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
This library is free software from Club robot Insa Rennes sources; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
"""

from xml.etree import ElementTree
import re
from io import BytesIO
from zipfile import ZipFile
import os

import math


class AlreadyExistsError(Exception): pass


class LabelAlreadyUse(Warning): pass


class Geogebra():
    FIXED = "true"
    FREE = "false"
    DOT_ONLY = ["false", "true"]
    LINE_ONLY = ["false", "true"]
    ALL_LABEL = ["true", "true"]

    class Point(tuple):
        pass

    class Line(tuple):
        pass

    class Circle(tuple):
        pass

    class Ellipse(tuple):
        pass

    class Segment(tuple):
        pass

    class Vector(tuple):
        pass

    class PolyLine(list):
        pass

    class Polygon(list):
        pass

    class Angle(float):
        pass

    class Numeric(float):
        pass

    def __init__(self, file):
        # TODO ajouter l'option d'ouverture
        self.file = file
        self.zipfile = ZipFile(file, mode='r')
        xmlfile = self.zipfile.open('geogebra.xml')
        self.loadxml = ElementTree.parse(xmlfile)
        self.root = self.loadxml.getroot()
        self.construction = self.root.find("./construction")

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()

    def close(self):
        self.zipfile.close()
        stream = BytesIO()
        self.loadxml.write(stream, encoding='utf-8', xml_declaration=True)
        # self._clean_ggb()
        self.zipfile = ZipFile('roadmap.ggb', mode='a')
        self.zipfile.writestr('geogebra.xml', stream.getvalue().decode('utf8'))
        self.zipfile.close()

    def set_polygon(self, label, *args, visibility=LINE_ONLY, color=(153, 51, 0), **kwargs):
        self._check_label(label)
        for element in args:
            if not isinstance(self.get(element), Geogebra.Point):
                raise TypeError

        segment_list = list()
        for k in range(len(args)):
            segment_list.append("{}{}".format(args[k - 1], args[k]))
            self.set_segment(segment_list[-1], args[k - 1], args[k])

        type = "Polygon"
        param = []
        attrib = \
            {
                "input": [],
                "output": [("a0", label)]

            }

        for i in range(len(segment_list)):
            attrib["output"].append(("a{}".format(i + 1), segment_list[i]))

        i = 0
        for element in args:
            attrib["input"].append(("a{}".format(i), element))
            i += 1
        self.set_command(type, param, attrib)

        type = "polygon"
        param = [("label", label)]
        attrib = \
            {
                "lineStyle": [("thickness", "2"), ("type", "0"), ("typeHidden", "1")],
                "show": [("object", visibility[1]), ("label", visibility[0])],
                "objColor": [("r", str(color[0])), ("g", str(color[1])), ("b", str(color[2])), ("alpha", "0.1")],
                "layer": [("val", "0")],
                "labelMode": [("val", "0"), ],
                "isShape": [("val", "false")],

            }
        self.set_element(type, param, attrib)

    def set_polyline(self, label, *args, visibility=LINE_ONLY, color=(0, 0, 0), **kwargs):
        self._check_label(label)
        for element in args:
            if not isinstance(self.get(element), Geogebra.Point):
                raise TypeError
        type = "PolyLine"
        param = []
        attrib = \
            {
                "input": [],
                "output": [("a0", label)]

            }
        i = 0
        for element in args:
            attrib["input"].append(("a{}".format(i), element))
            i += 1
        self.set_command(type, param, attrib)
        type = "polyline"
        param = [("label", label)]
        attrib = \
            {
                "lineStyle": [("thickness", "2"), ("type", "0"), ("typeHidden", "1")],
                "show": [("object", visibility[1]), ("label", visibility[0])],
                "objColor": [("r", str(color[0])), ("g", str(color[1])), ("b", str(color[2]))],
                "layer": [("val", "0")],
                "labelMode": [("val", "0")]

            }

        self.set_element(type, param, attrib)

    def set_line(self, label, label_1, label_2):
        self._check_label(label)
        if not isinstance(self.get(label_1), Geogebra.Point) and not isinstance(self.get(label_2), Geogebra.Point):
            raise TypeError
        type = "line"
        param = []
        attrib = \
            {
                "input": [("a0", label_1), ("a1", label_2)],
                "output": [("a0", label)]
            }
        self.set_command(type, param, attrib)

    def set_segment(self, label, label_1, label_2):
        self._check_label(label)
        if not isinstance(self.get(label_1), Geogebra.Point) and not isinstance(self.get(label_2), Geogebra.Point):
            raise TypeError
        type = "Segment"
        param = []
        attrib = \
            {
                "input": [("a0", label_1), ("a1", label_2)],
                "output": [("a0", label)]
            }
        self.set_command(type, param, attrib)

    def set_point(self, label, x, y, z=1.0, color=(128.0, 128.0, 128.0), fixed=FREE, visibility=DOT_ONLY, **kwargs):
        self._check_label(label)
        type = "point"
        param = [("type", "point"), ("label", label)]
        attrib = \
            {
                "show": [("object", visibility[1]), ("label", visibility[0])],
                "objColor": [("r", str(color[0])), ("g", str(color[1])), ("b", str(color[2])), ("alpha", "0.0")],
                "layer": [("val", "0")],
                "labelMode": [("val", "0")],
                "fixed": [("val", fixed)],
                "animation": [("speed", "1"), ("type", "1"), ("playing", "false")],
                "isShape": [("val", "false")],
                "coords": [("x", str(x)), ("y", str(y)), ("z", str(z))],
                "pointSize": [("val", "5")],
                "pointStyle": [("val", "0")]
            }

        self.set_element(type, param, attrib)

    def set_command(self, type, param, attrib):
        command = ElementTree.SubElement(self.construction, "command")
        command.set("name", type)
        for setting in param:
            command.set(setting[0], setting[1])
        for parameter in attrib:
            param = ElementTree.SubElement(command, parameter)
            for setting in attrib[parameter]:
                param.set(setting[0], setting[1])

    def set_element(self, type, param, attrib):
        #  type de l'element et param une liste de tupple
        element = ElementTree.SubElement(self.construction, "element")
        element.set("type", type)
        for setting in param:
            element.set(setting[0], setting[1])
        for parameter in attrib:
            param = ElementTree.SubElement(element, parameter)
            for setting in attrib[parameter]:
                param.set(setting[0], setting[1])

    def remove(self, label):
        for x in self.construction.getchildren():
            try:
                if x.tag in "element":
                    if x.attrib['label'] == label:
                        self.construction.remove(x)
                elif x.tag in "command":
                    for parameter in x.getiterator(tag="output"):
                        if parameter.attrib["a0"] == label:
                            self.construction.remove(x)
            except:
                pass

    def _clean_ggb(self):
        # TODO faire le fait qu'il supprime pas tout d'un coup
        try:
            os.system("zip {} -d geogebra.xml".format(self.file))
        except:
            pass
    
    def get(self, element):
        if type(element) is str:
            label = element
            element = self.root.find("./construction/element[@label='{}']".format(element))
        if element is None:
            raise KeyError(label)
        if element.attrib['type'] == 'point':
            return self._parse_point(element)
        elif element.attrib['type'] == 'line':
            return self._parse_line(element)
        elif element.attrib['type'] == 'conic':
            return self._parse_conic(element)
        elif element.attrib['type'] == 'segment':
            return self._parse_segment(element)
        elif element.attrib['type'] == 'vector':
            return self._parse_vector(element)
        elif element.attrib['type'] == 'polyline':
            return self._parse_polyline(element)
        elif element.attrib['type'] == 'polygon':
            return self._parse_polygon(element)
        elif element.attrib['type'] == 'angle':
            return self._parse_angle(element)
        elif element.attrib['type'] == 'numeric':
            return self._parse_numeric(element)
        else:
            raise NotImplementedError("'{}' elements currently not handled".format(element.attrib['type']))

    def getall(self, pattern):
        elements = self.root.findall('./construction/element[@label]')
        all_labels = [element.get('label') for element in elements]
        labels = [label for label in all_labels if re.match(pattern, label)]
        labels = sorted(labels)

        parse_by_element = ["point", "line", "conic", "angle", "numeric"]
        parse_by_command = ["Segment", "Vector", "Polyline", "Polygon"]

        roadmap = []
        for label in labels:
            element = self.root.find("./construction/element[@label='{}']".format(label))
            if element.attrib['type'] in parse_by_element:
                figure = self.get(element)
                roadmap += [figure]

        #commands = self.root.findall('./construction/command')
        #for command in commands:
        #    if command.attrib["name"] in parse_by_command:
        #        figure = self._parse_segment_from_command(command)
        #        roadmap += [figure]
        return roadmap

    def _check_label(self, label):
        try:
            self.get(label)
        except:
            pass
        else:
            raise AlreadyExistsError('Label already use !')

    def _parse_point(self, element):
        coords = element.find('coords')
        x = float(coords.get('x'))
        y = float(coords.get('y'))
        z = float(coords.get('z'))
        return Geogebra.Point((x / z, y / z))

    def _parse_line(self, element):
        coords = element.find('coords')
        a = float(coords.get('x'))
        b = float(coords.get('y'))
        c = float(coords.get('z'))
        try:
            x, y = -c / a, 0
        except ZeroDivisionError:
            x, y = 0, -c / b
        return Geogebra.Line((x, y, math.pi - math.atan2(a, b)))

    def _parse_conic(self, element):
        matrix = element.find('matrix').attrib
        A = float(matrix.get('A0'))
        C = float(matrix.get('A1'))
        F = -float(matrix.get('A2'))
        B = 2 * float(matrix.get('A3'))
        D = 2 * float(matrix.get('A4'))
        E = 2 * float(matrix.get('A5'))
        if A * C - (B / 2) ** 2 < 0:
            raise ValueError('not an ellipse')
        if A == C and B == 0:
            xc = -D / (2 * A)
            yc = -E / (2 * C)
            radius = math.sqrt(xc ** 2 + yc ** 2 + F)
            return Geogebra.Circle((xc, yc, radius))
        else:
            raise NotImplementedError('ellipses currently not handled')

    def _parse_segment(self, element):
        label = element.attrib['label']
        command = self.root.find("./construction/command[@name='Segment']/output[@a0='{}']/..".format(label))

        if command is not None:
            return self._parse_segment_from_command(command)
        for command in self.root.findall("./construction/command[@name='Polygon']"):
            if label not in command.find('output').attrib.values():
                continue
            output = command.find('output')
            polygon = self.get(output.get('a0'))
            i = [output.get(key) for key in sorted(output.attrib.keys())].index(label) - 1
            return Geogebra.Segment((polygon[i], polygon[(i + 1) % len(polygon)]))

        raise ValueError("inexistant 'Segment' command")

    def _parse_segment_from_command(self, command):
        input = command.find('input')
        a0 = self.get(input.get('a0'))
        a1 = self.get(input.get('a1'))
        return Geogebra.Segment([a0, a1])

    def _parse_vector(self, element):
        label = element.attrib['label']
        command = self.root.find("./construction/command[@name='Vector']/output[@a0='{}']/..".format(label))
        if command is None:
            raise ValueError("inexistant 'Vector' command")
        return self._parse_vector_from_command(command)

    def _parse_vector_from_command(self, command):
        input = command.find('input')
        a0 = self.get(input.get('a0'))
        a1 = self.get(input.get('a1'))
        return Geogebra.Vector([a0, a1])

    def _parse_polyline(self, element):
        label = element.attrib['label']
        command = self.root.find("./construction/command[@name='PolyLine']/output[@a0='{}']/..".format(label))
        if command is None:
            raise ValueError("inexistant 'PolyLine' command")
        return self._parse_polyline_from_command(command)

    def _parse_polyline_from_command(self, command):
        input = command.find('input')
        labels = [input.get(key) for key in sorted(input.attrib.keys())]
        return Geogebra.PolyLine([self.get(label) for label in labels])

    def _parse_polygon(self, element):
        label = element.attrib['label']
        command = self.root.find("./construction/command[@name='Polygon']/output[@a0='{}']/..".format(label))
        if command is None:
            raise ValueError("inexistant 'Polygon' command")
        return self._parse_polygon_from_command(command)

    def _parse_polygon_from_command(self, command):
        input = command.find('input')
        try:
            numvertices = int(input.get('a2'))
            output = command.find('output')
            labels = [input.get('a0'), input.get('a1')] + [output.get(key) for key in
                                                           sorted(output.attrib.keys())[2 - numvertices:]]
        except (TypeError, ValueError):
            labels = [input.get(key) for key in sorted(input.attrib.keys())]
        return Geogebra.Polygon([self.get(label) for label in labels])

    def _parse_angle(self, element):
        value = element.find('value')
        val = float(value.get('val'))
        return Geogebra.Angle(val)

    def _parse_numeric(self, element):
        value = element.find('value')
        val = float(value.get('val'))
        return Geogebra.Numeric(val)
