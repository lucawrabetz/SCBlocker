path1 = "oct24.csv"
path2 = "table2.txt"


with open(path1, "r") as fi, open(path2, "w") as fo:
    for line in fi: 
        clean_line = line.strip().replace(",", "&").replace("0.0001", "-") + "\\" + "\\"
        fo.write(clean_line +"\n")



