"""
The MIT License (MIT) 
Copyright (c) 2016 Missouri State University

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

Jan. 21, 2016  Version 1.0

Created by Matthew Revell, for use in Missouri State University CSC 325 Algorithms class.

Currently there is very little error checking and protection. Use at your own risk.

To use this program: create a file to be used as input to this program, with one instruction 
per line. The first line must be the size of the canvas to print to, as well as an optional
background. Then the program reads line by line through the rest of the file.
Each line has one instruction, with parameters sepearated by spaces.

The first line of the input file MUST have
two parameters with an optional third. The first two are the Width and 
Height of the canvas to be drawn on, and the optional third is the background
color.

Invalid lines in the input file will be printed to console with the bad line and the line 
number starting at 1.
	
Documentation for the input file to this program: See readme.txt or use the File menu
when this program executes.

Documentation for Tkinter, a GUI for Python 
http://infohost.nmt.edu/tcc/help/pubs/tkinter/web/index.html

Sample run: python3  BearPlot.py   inputFile.dat
                     ^ This script   ^ File to process

Sample input file for this program:
*******Start of sample input file*********************************************
700 700 white
coordinate_plane
triangle 20 20 40 40 10 40
COmMent these words will be ignored
line_dotted 40 40 100 100 green
ovAl 500 200 600 400 purple

circle 300 300 50 orange
*******End of sample input file*********************************************


"""


try:
    # Python 2
    from Tkinter import *
except ImportError:
    #Python 3
    from tkinter import *
import sys
import time

VERSION = "0.4"   
# Version 0.4, Feb. 25, 2016 Ken Vollmar. Add DOTRECT and HITKEY.


# Switch statement in python without dictionaries
class switch(object):
    def __init__(self, value):
        self.value = value
        self.fall = False

    def __iter__(self):
        """ Return the match method once, then stop """
        yield self.match
        raise StopIteration

    def match(self, *args):
        """Indicate whether or not to enter a case suite"""
        if self.fall or not args:
            return True
        elif self.value in args:
            self.fall = True
            return True
        else:
            return False

def create_canvas(width=500, height=500, bg='white'):
    w = int(width) + 5
    h = int(width) + 5
    return Canvas(root, width=w, height=h, bg=bg)

def draw_coordinate_plane():
    i = c.create_line(0, HEIGHT_2, WIDTH, HEIGHT_2)
    t = c.create_line(WIDTH_2, 0, WIDTH_2, HEIGHT)

def draw_positive_coordinate_plane():
    i = c.create_line(2, 2, 2, HEIGHT)
    i = c.create_line(2, HEIGHT, WIDTH, HEIGHT)

# This progress bar is a work in progress, currently it 'grows'
# as the progress bar fills...
# TODO: Keep progress bar the same size throughout
def progress(total, current):
    bar_len = 30
    filled_len = int(round(bar_len * current / float(total)))

    percents = round(100.0 * current / float(total), 1)
    return '[' + '=' * filled_len + '_' * (bar_len - filled_len) + ']'

# Open the file and read it into content
try:
	with open(sys.argv[1]) as f:
		content = f.readlines()
except FileNotFoundError:
	sys.exit('Could not find file ' + sys.argv[1])


if content:
    line = content[0].split()
    root = Tk()
    root.title('PyGraphics v' + VERSION + '\t' + sys.argv[1])
    c = create_canvas(width=line[0], height=line[1], bg=line[2])
    c.grid(row=0, column=1, columnspan=4)
else:
	sys.exit('File was empty!')


WIDTH = int(content[0].split()[0])
HEIGHT = int(content[0].split()[1])
WIDTH_2 = int(WIDTH / 2)
HEIGHT_2 = int(HEIGHT / 2)
TOTAL_LINES = len(content)

total_label = Label(root, text="Total lines: " + str(TOTAL_LINES))
total_label.grid(row=1, column=1)


def main():
    # Start counting line numbers at 2 to help with troubleshooting and because we
    # we skip the first line.
    line_number = 2
    for l in content[1:]:
        line = l.split()
        # This try is for a blank line
        try:
            for case in switch(line[0].lower()):

                if case('line'):
                    try:
                        fg=line[5]
                    except:
                        fg='black'
                    i = c.create_line(line[1], line[2], line[3], line[4], fill=fg)
                    break
                
                if case('line_solid'):
                    try:
                        fg=line[5]
                    except:
                        fg='black'
                    w = 3.0
                    i = c.create_line(line[1], line[2], line[3], line[4], fill=fg, width=w)
                    break

                if case('line_dotted'):
                    try:
                        fg=line[5]
                    except:
                        fg='black'
                    w = 3.0
                    i = c.create_line(line[1], line[2], line[3], line[4], fill=fg, width=w, dash=(4,4))
                    break

                if case('point'):
                    try:
                        fg=line[3]
                    except:
                        fg='black'
                    i = c.create_oval(line[1], line[2], line[1], line[2], fill=fg)
                    break

                if case('sleep'):
                    try:
                        tm = line[1]
                    except:
                        tm = 2
                    time.sleep(float(tm))
                    break

                # KV experimental
                if case('dotrect'):  # USAGE:  dotrect topLeftX topLeftY bottomRightX bottomRightY (color)
                    try:
                        fg=line[5]
                    except:
                        fg=None
                    i = c.create_rectangle(line[1], line[2], line[3], line[4], fill=fg, dash=(4,4))
                    break

                # KV experimental
				# Pause indefinitely while waiting for user to press ENTER key
				# ?? Running BearPlot causes system to lose focus into the graphics window.
				# User must first click in DOS window to regain focus, then ENTER.
                if case('presskey'):  # USAGE:  presskey 
                    try:
                        input("Press ENTER key to continue.")
                    except (SyntaxError, EOFError) as e:  # ?? two potential technical issues upon user input
                        pass
                    break
					
                if case('oval'): # USAGE:   oval pointx1 pointy1 pointx2 pointy2 (color)
                    try:
                        fg=line[5]
                    except:
                        fg=None
                    i = c.create_oval(line[1], line[2], line[3], line[4], fill=fg)
                    break

                if case('triangle'):
                    try:
                        fg=line[7]
                    except:
                        fg='black'
                    i = c.create_line(line[1], line[2], line[3], line[4], fill=fg)
                    i = c.create_line(line[3], line[4], line[5], line[6], fill=fg)
                    i = c.create_line(line[1], line[2], line[5], line[6], fill=fg)
                    break

                if case('rectangle'):  # USAGE:  rectangle topLeftX topLeftY bottomRightX bottomRightY (color)
                    try:
                        fg=line[5]
                    except:
                        fg=None
                    i = c.create_rectangle(line[1], line[2], line[3], line[4], fill=fg)
                    break

                if case('text'):  # USAGE:     text pointx pointy fontsize Hello World!
                    #i = c.create_text(line[1], line[2], text=line[3:])
                    i = c.create_text(line[1], line[2], font=('Helvetica', line[3]), text=line[4:])
                    break

                if case('comment'):
                    break

                if case('coordinate_plane'):
                    draw_coordinate_plane()
                    break

                if case('positive_coordinate_plane'):
                    draw_positive_coordinate_plane()
                    break

                # Default case, if the run reaches here the user entered a
                # bad command.
                if case():
                    print("Unknown command on LINE:", line_number, "\n",
                            ' '.join(str(e) for e in line), 
                            "="*40, "\n")
        
        # Empty line
        except IndexError:
            pass

        # Print out a little information about the exception
        except:
            print("ERR: On LINE: ", line_number,
                    "  ", sys.exc_info()[0], 
                    "\n", ' '.join(str(e) for e in line),
                    "\n", "="*40, "\n") 


        total_label.config(text="Total lines: " + str(TOTAL_LINES) + 
                "\t Current line: " + str(line_number) + "\t" +
                str(progress(TOTAL_LINES, line_number)))
        c.update()
        line_number += 1


def reset():
    c.delete('all')
    main()

def about():
    top = Toplevel()
    top.minsize(width=200, height=100)
    top.title("About")
    msg = Message(top, text="Python graphics script. Version " + VERSION + "\nWritten for use in\nDr. Vollmar's CSC 325 class.")
    msg.pack()
    button = Button(top, text="OK", command=top.destroy)
    button.pack()

def help_popup():
    top = Toplevel()
    top.title("Help")
    top.minsize(width=int(root.winfo_screenwidth()*1/3), 
            height=int(root.winfo_screenheight()*2/4))
    T = Text(top, height=60)
    T.pack(fill=BOTH)
    with open('readme.txt') as f:
        for line in f.readlines():
            T.insert(END, line)

def peek():
    top = Toplevel()
    top.title("Peek at\t" + sys.argv[1])
    T = Text(top)
    T.pack(fill=BOTH)
    line_n = 1
    for l in content:
        T.insert(END, str(line_n) + ": " + l)
        line_n += 1

# Menubar
menubar = Menu(root)
filemenu = Menu(menubar, tearoff=0)
menubar.add_cascade(label="File", menu=filemenu)
filemenu.add_command(label="Restart", command=reset)
filemenu.add_command(label="Peek file", command=peek)
filemenu.add_separator()
filemenu.add_command(label="Help", command=help_popup)
filemenu.add_command(label="About", command=about)
filemenu.add_separator()
filemenu.add_command(label="Exit", command=root.quit)
root.config(menu=menubar)

main()
root.mainloop()
