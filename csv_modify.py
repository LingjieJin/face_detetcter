f = open("./orl_faces/at.txt", mode="r+", encoding="utf-8")
f1 = open("./orl_faces/at.csv", mode = "w", encoding = "utf-8")
for line in f:
    if line[29] == '\\':
        l = line.replace('\n','') + ';' + line[28] + '\n'
        f1.writelines(l)
    else:
        l= line.replace('\n','') + ';' + line[28:30] + '\n'
        f1.writelines(l)
     