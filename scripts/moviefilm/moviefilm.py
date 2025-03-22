#!/bin/python

# generates a funny moviefilm from a list of frames
from PIL import Image
import sys
import os

xoff = int(sys.argv[1])
yoff = int(sys.argv[2])

width = int(sys.argv[3])
height = int(sys.argv[4])

frames = os.listdir("./frames")

filmstrip = Image.open("./filmstrip.png").convert('RGBA')
strips = []
for frame in frames:
    stripper = Image.open("./filmstrip.png").convert('RGBA')
    im = Image.open("./frames/"+frame).convert('RGBA').resize((width,height))
    px = im.load()
    filmimg = stripper.load()
    for x in range(0,width):
        for y in range(0,height):
            filmimg[xoff+x,yoff+y] = px[x,y]
    strips.append(filmimg)

finalimage = Image.new('RGBA', (len(strips)*filmstrip.width, filmstrip.height), color=(0, 0, 0,0))
finalpx = finalimage.load()
offset = 0;
for strip in strips:
    for x in range(0,filmstrip.width):
        for y in range(0,filmstrip.height):
            finalpx[offset+x,y] = strip[x,y]
    offset += filmstrip.width
finalimage.save("test.png")


