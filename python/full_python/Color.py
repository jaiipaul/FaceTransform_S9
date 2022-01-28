#!/usr/bin/python
#-*- coding: utf-8 -*-
"""
 .. module:: Color

 Colors for terminal

 --> currently only tested on Windows

 .. moduleauthor:: CTT - 24/02/2020
"""

class Color:
    """
        color style for VT100 terminal
    """
    END = '\033[0m'  # Default color
    BOLD = '\033[1m'    # Bold or light
    UNDERLINE = '\033[4m'
    BLINK = '\033[5m'
    NOBOLD = '\033[22m'    # No bold or dark
    NOUNDERLINE = '\033[24m'
    NOBLINK = '\033[25m'
    # Normal (dark) foreground color set
    BLACK = '\033[30m'
    DARKRED = '\033[31m'
    DARKGREEN = '\033[32m'
    DARKYELLOW = '\033[33m'
    DARKBLUE = '\033[34m'
    DARKPURPLE = '\033[35m'
    DARKCYAN = '\033[36m'
    DARKWHITE = '\033[37m'
    FGDEFAULT = '\033[39m'
    # Normal (dark) background color set
    BGBLACK = '\033[40m'
    BGDARKRED = '\033[41m'
    BGDARKGREEN = '\033[42m'
    BGDARKYELLOW = '\033[43m'
    BGDARKBLUE = '\033[44m'
    BGDARKPURPLE = '\033[45m'
    BGDARKCYAN = '\033[46m'
    BGDARKWHITE = '\033[47m'
    BGDEFAULT = '\033[49m'
    # Light foreground color set
    DARKGREY = '\033[90m'
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    PURPLE = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'
    # Light background color set
    BGDARKGREY = '\033[100m'
    BGRED = '\033[101m'
    BGGREEN = '\033[102m'
    BGYELLOW = '\033[103m'
    BGBLUE = '\033[104m'
    BGPURPLE = '\033[105m'
    BGCYAN = '\033[106m'
    BGWHITE = '\033[107m'
    NONE = ''
