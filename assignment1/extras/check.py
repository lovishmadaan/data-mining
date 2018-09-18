import sys
f1 = open(sys.argv[1], 'r')
f2 = open(sys.argv[2], 'r')

a = []
b =[]
for lines in f1.readlines():
    temp = list(map(int,lines.split()))
    temp.sort()
    a.append(temp)
for lines in f2.readlines():
    temp = list(map(int,lines.split()))
    temp.sort()
    b.append(temp)    
# b = f2.readlines()
a.sort()
b.sort()
print(a == b)