import random

TESTS = 3000

def XOR_maker():
    output_txt = ""
    output_txt += "topology: 2 4 1\n"
    for i in range(TESTS):
        x = random.randint(0,1)
        y = random.randint(0,1)
        output_txt += "in: %d.0 %d.0\n" % (x,y)
        
        output_txt += "out: %d.0z\n" % (x^y)
    with open("test/XOR.txt", "w") as f:
        f.write(output_txt)
    print("Done")
def NAND_maker():
    output_txt = ""
    output_txt += "topology: 2 4 1\n"
    for i in range(TESTS):
        x = random.randint(0,1)
        y = random.randint(0,1)
        output_txt += "in: %d.0 %d.0\n" % (x,y)
        
        output_txt += "out: %d.0z\n" % (not(x and y))
    with open("test/NAND.txt", "w") as f:
        f.write(output_txt)
    print("Done")
def MUX_maker():
    output_txt = ""
    output_txt += "topology: 3 4 1\n"
    for i in range(TESTS):
        x = random.randint(0,1)
        y = random.randint(0,1)
        z = random.randint(0,1)
        output_txt += "in: %d.0 %d.0 %d.0\n" % (x,y,z)
        
        output_txt += "out: %d.0z\n" % (x and y or not z)
    with open("test/MUX.txt", "w") as f:
        f.write(output_txt)
    print("Done")
def DEMUX_maker():
    output_txt = ""
    output_txt += "topology: 2 4 2\n"
    for i in range(TESTS):
        x = random.randint(0,1)
        y = random.randint(0,1)
        output_txt += "in: %d.0 %d.0\n" % (x,y)
        
        output_txt += "out: %d.0 %d.0z\n" % (x and y, not x and y)
    with open("test/DEMUX.txt", "w") as f:
        f.write(output_txt)
    print("Done")
def ADDER_maker():
    output_txt = ""
    output_txt += "topology: 2 4 1\n"
    for i in range(TESTS):
        x = random.randint(0,1)
        y = random.randint(0,1)
        output_txt += "in: %d.0 %d.0\n" % (x,y)
        
        output_txt += "out: %d.0z\n" % (x ^ y)
    with open("test/ADDER.txt", "w") as f:
        f.write(output_txt)
    print("Done")
def SUBTRACTOR_maker():
    output_txt = ""
    output_txt += "topology: 2 4 1\n"
    for i in range(TESTS):
        x = random.randint(0,1)
        y = random.randint(0,1)
        output_txt += "in: %d.0 %d.0\n" % (x,y)
        
        output_txt += "out: %d.0z\n" % (x ^ y)
    with open("test/SUBTRACTOR.txt", "w") as f:
        f.write(output_txt)
    print("Done")
def FlipFlopMaker():
    output_txt = ""
    output_txt += "topology: 2 2 2 1\n"
    for i in range(TESTS):
        x = random.randint(0, 1)
        y = random.randint(0, 1)

        state = random.randint(0, 1)

        output_txt += "in: %d.0 %d.0\n" % (x, y)

        if state == 0:
            next_state = x
        else:
            next_state = y

        output_txt += "out: %d.0\n" % next_state

    with open("test/FlipFlop.txt", "w") as f:
        f.write(output_txt)
    print("Done")




if __name__ == "__main__":
    XOR_maker()
    NAND_maker()
    MUX_maker()
    DEMUX_maker()
    ADDER_maker()
    SUBTRACTOR_maker()
    FlipFlopMaker()
    
