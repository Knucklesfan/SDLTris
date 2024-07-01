# simple quick and dirty endianess swapper, 
# but may contain more if I decide to expand this format 
# (i.e, turning all the values into working with 640x480
#  instead of interpolating)

with open("09e1b8_anim.bin",'rb') as f:
    content = bytearray(f.read())
numberofvalues = content[1]

temp = content[0]
content[0] = content[1]
content[1] = temp

for i in range(1,numberofvalues+1):
    temp = content[i*2]
    content[i*2] = content[i*2+1]
    content[i*2+1] = temp
with open("09e1b8_anim.ksta",'wb') as f:
    f.write(content)
