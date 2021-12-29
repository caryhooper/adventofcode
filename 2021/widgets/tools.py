import ast

class SnailfishCalculator:
    def __init__(self):
        pass

    def initialize(self,input):
        self.tree = ast.literal_eval(input)
    #Rules
    #If any pair is nested inside four pairs, the leftmost pair explodes
    #If any number is 10 or greater, the leftmost number splits

class Node:
    def __init__(self,value):
        left,right = value
        if isinstance(left,list):
            self.left = Node(left)
        else:
            self.left = left
        if isinstance(right,list):
            self.right = Node(right)
        else:
            self.right = right