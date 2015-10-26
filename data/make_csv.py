import random

def make_simple_csv():
    cols = 3
    rows = 3
    f = open("pycsv.csv", "w")
    for i in range(rows):
        string = ",".join('"' + str(3*i+x) + '"' for x in range(cols)) + "\n"
        f.write(string)
    f.close();

def make_csv():
    cols = 10
    rows = 10
    f = open("pycsv.csv", "w")
    for i in range(rows):
        string = ",".join('"' + str(random.randrange(1000)) + '"' for x in range(cols)) + "\n"
        f.write(string)
    f.close();

make_simple_csv()
