from gudhi import RipsComplex

"""This file is part of the Gudhi Library. The Gudhi library
   (Geometric Understanding in Higher Dimensions) is a generic C++
   library for computational topology.

   Author(s):       Vincent Rouvreau

   Copyright (C) 2016 INRIA

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

__author__ = "Vincent Rouvreau"
__copyright__ = "Copyright (C) 2016 INRIA"
__license__ = "GPL v3"


def test_empty_rips():
    rips_complex = RipsComplex()
    assert rips_complex.__is_defined() == True
    assert rips_complex.__is_persistence_defined() == False

def test_rips():
    point_list = [[0, 0], [1, 0], [0, 1], [1, 1]]
    rips_complex = RipsComplex(points=point_list, max_dimension=1,
                               max_edge_length=42)
    assert rips_complex.__is_defined() == True
    assert rips_complex.__is_persistence_defined() == False

    assert rips_complex.num_simplices() == 10
    assert rips_complex.num_vertices() == 4

    assert rips_complex.get_filtered_tree() == \
           [([0], 0.0), ([1], 0.0), ([2], 0.0), ([3], 0.0),
            ([0, 1], 1.0), ([0, 2], 1.0), ([1, 3], 1.0),
            ([2, 3], 1.0), ([1, 2], 1.4142135623730951),
            ([0, 3], 1.4142135623730951)]
    assert rips_complex.get_star_tree([0]) == \
           [([0], 0.0), ([0, 1], 1.0), ([0, 2], 1.0),
            ([0, 3], 1.4142135623730951)]
    assert rips_complex.get_coface_tree([0], 1) == \
           [([0, 1], 1.0), ([0, 2], 1.0),
            ([0, 3], 1.4142135623730951)]

def test_filtered_rips():
    point_list = [[0, 0], [1, 0], [0, 1], [1, 1]]
    filtered_rips = RipsComplex(points=point_list, max_dimension=1,
                                      max_edge_length=1.0)
    assert filtered_rips.__is_defined() == True
    assert filtered_rips.__is_persistence_defined() == False

    assert filtered_rips.num_simplices() == 8
    assert filtered_rips.num_vertices() == 4
