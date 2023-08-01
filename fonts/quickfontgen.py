import sys
from string import ascii_lowercase
from string import ascii_uppercase

numbers = [7,7,7,7,7,7,7,7,
           7,7,7,7,7,7,6,6,
           7,7,5,7,8,6,9,8,
           7,7,7,7,6,7,7,7,
           11,7,7,7,7,7,6,7,
           7,6,7,7,3,6,7,3,
           11,7,7,7,7,6,6,5,
           7,7,11,7,7,7,7,7,
           7,7,10,6,7,10,6,6,
        ]
def makeword(word, posx, posy, charw):
    print("""   <letter> 
        <posx>""" + str(posx) + """</posx>
        <posy>""" + str(posy) + """</posy>
        <char>""" + str(word) + """</char>
        <charwid>""" + str(charw) + """</charwid>
    </letter>\n""")


rows = 100
maxnumber = 0
posx = 0
posy = 0
char = 0
lowercase = False


print("Knuxfont's font generator")
print("PLEASE KNOW: this does NOT generate complete fonts")
print("i literally only made this to do a few things, but mainly just interpret VWF font styles")
print("in fact, if you really dug this far back in the source tree to find this")
print("(i mean, i plan on removing this file, but i might not,")
print("also the game IS gonna be open-sourced after release so idk, maybe?)")
with open('incompletefont.xml', 'w') as f:
    sys.stdout = f
    print("<letters>\n")
    while maxnumber < 10:
        makeword(maxnumber, posx % rows, posy,8)
        maxnumber += 1
        posx += 1
        posy = int( posx / rows)
        char+=1
    for word in ascii_uppercase:
        makeword(word, posx % rows, posy,8)
        posx += 1
        posy = int( posx / rows)
        char+=1
    if lowercase:
        for word in ascii_lowercase:
            makeword(word, posx % rows, posy,8)
            posx += 1
            posy = int( posx / rows)
            char+=1

