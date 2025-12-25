def wordGame(row1, row2, row3):
    m1 = m2 = m3 = 0
    f = ''
    d = {}
    for word in row1:
        if word not in d:
            d[word] = 1
        else:
            d[word] += 1
    for word in row2:
        if word not in d:
            d[word] = 1
        else:
            d[word] += 1
    for word in row3:
        if word not in d:
            d[word] = 1
        else:
            d[word] += 1
    
    for word in row1:
        if d[word] == 1:
            m1+=3
        elif d[word] == 2:
            m1+=1
    for word in row2:
        if d[word] == 1:
            m2+=3
        elif d[word] == 2:
            m2+=1
    for word in row3:
        if d[word] == 1:
            m3+=3
        elif d[word] == 2:
            m3+=1
    f = str(m1)+" "+str(m2)+" "+str(m3)
    return f
    
# Driver code
t = int(input())
for _ in range (t):
    n = int(input())
    r1 = list(map(str, input().split(' ')))
    r2 = list(map(str, input().split(' ')))
    r3 = list(map(str, input().split(' ')))
    print(wordGame(r1, r2, r3))