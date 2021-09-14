v1 2/12/20

input requirements: csv file, no header row, nxm adjacency matrix from partition of subsets (n) to elements (m). 
subsets should be the columns, elements should be the rows. 

run: in file_conv.py, change line 58 with (path to instance, n, m, r, path to folder where porta file should be saved, desired name for porta file)
note: path to instance should end with a .csv file, and the desired porta file name should end in .ieq, for example test1.ieq. 

$python3 ./src/file_conv.py
