import os
import cmd

def read():
    pass

def write(var):
    pass

class g(cmd.Cmd):
    prompt = ">>"
    intro = "welcome to gui \n \n1. Read config\n2. Write to config\n3. quit"

    def preloop(self):
        if (os.path.isfile("text.txt") == True):
            print("Config file found")
        else:
            print("no config found creating file")
            #f = open("test.txt", "x")
    def do_quit(self, line):
        """3"""
        return True
    def do_Opt_1(self, line):
        """1"""
        print("option 1")
    def do_Opt_2(self, line):
        """2"""
        print("option 2")
      
    

def main():
    pass
    


if __name__ == "__main__":
    g().cmdloop()