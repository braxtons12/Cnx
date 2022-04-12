#
#   This file is part of m.css.
#
#   Copyright © 2017, 2018, 2019, 2020, 2021, 2022
#             Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

from pygments.style import Style
from pygments.token import Keyword, Name, Comment, String, Error, \
        Literal, Number, Operator, Other, Punctuation, Text, Generic, \
        Whitespace

class DarkStyle(Style):
    background_color = None
    highlight_color = '#34424d'
    default_style = ""

    styles = {
            # C++
            Comment:                '#686e78',
            Comment.Preproc:        '#c67ada',
            Comment.PreprocFile:    '#83a76e',
            Keyword:                '#c67ada',
            Name:                   '#9daaaa',
            Name.Class:             '#dbba75',
            Name.Builtin:           '#dbba75',
            Name.Function:          '#61afef',
            Name.Namespace:         '#00997b',
            String:                 '#83a76e',
            String.Char:            '#83a76e',
            String.Escape:          '#83a76e', # like char
            String.Interpol:        '#83a76e', # like comment
            Number:                 '#d29767',
            Operator:               '#9daaaa',
            Punctuation:            "#9daaaa",

            # CMake
            Name.Builtin:           '#dbba75',
            Name.Variable:          '#9daaaa',

            # reST, HTML
            Name.Tag:               'bold #dcdcdc',
            Name.Attribute:         'bold #dcdcdc',
            Name.Class:             'bold #dcdcdc',
            Operator.Word:          'bold #dcdcdc',
            Generic.Heading:        'bold #ffffff',
            Generic.Emph:           'italic #e6e6e6',
            Generic.Strong:         'bold #e6e6e6',

            # Diffs
            Generic.Subheading:     '#5b9dd9',
            Generic.Inserted:       '#3bd267',
            Generic.Deleted:        '#cd3431'
            }
