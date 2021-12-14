import collections
import itertools

class Manual:
    def __init__(self):
        #Part1 Dict in the form pair: insert_letter
        self.pair_insertions = dict()
        self.polymer_template = ""
        self.step = 0
        self.letter_set = ""
        self.all_pairs_count = collections.Counter()
        self.letters_count = dict()
        #Part2 
        pass

    def parse_rules(self,lines):
        for line in lines:
            line = line.strip()
            if "->" in line:
                pair,insert_letter = line.split(' -> ')
                self.pair_insertions[pair] = insert_letter
            elif line == "":
                pass
            else:
                self.polymer_template = line
            
        #Initialize data structures for part 2
        #Find all possible letters used
        self.letter_set = "".join(set("".join([k+v for k,v in self.pair_insertions ])))
        #Calculate all pairs of possible letters
        all_pairs = ["".join(i) for i in itertools.product(self.letter_set,repeat=2)]
        #Save all as a counter object to count the pairs.  
        self.all_pairs_count = collections.Counter({x:0 for x in all_pairs})
        self.letters_count = {x:0 for x in self.letter_set}

        #Count each letter in seed
        for i in self.polymer_template:
            self.letters_count[i] += 1
        for i in range(0,len(self.polymer_template)-1):
            pair = self.polymer_template[i] + self.polymer_template[i+1]
            self.all_pairs_count[pair] += 1

    def do_steps(self,number_of_steps=100,do_better=False):
        for _ in range(0,number_of_steps):
            if do_better:
                self.do_step_better()
            else:
                self.do_step()
            self.step += 1
            
    def do_step(self):
        #save first letter
        new_template = self.polymer_template[0]
        for i in range(0,len(self.polymer_template)-1):
            pair = self.polymer_template[i:i+2]
            if pair in self.pair_insertions.keys():
                #Insert letter and save the last letter
                new_template += self.pair_insertions[pair] + pair[1]
            else:
                #Save the last letter
                pair[1]
        self.polymer_template = new_template
    
    def do_step_better(self):
        #For each step, we're no longer keeping track of the polymer_template string.
        #Instead, we'll update the all_pairs_count
        if self.step <= 1:
            from pprint import pprint
            pprint(f"Step {self.step} All Pairs: {self.all_pairs_count}")
            pprint(f"Step {self.step} All Letters: {self.letters_count}")
        temp_all_pairs_count = collections.Counter(self.all_pairs_count)
        temp_letters_count = dict(self.letters_count)
        # print(self.all_pairs_count)
        for pair, count in self.all_pairs_count.most_common():
            #Lookup replacement letter & add to all_letters
            replacement = self.pair_insertions[pair]
            temp_letters_count[replacement] += count
            #Update all_pairs_count twice with each pair.
            first_letter,second_letter = pair
            temp_all_pairs_count[first_letter + replacement] += count
            temp_all_pairs_count[replacement + second_letter] += count
            temp_all_pairs_count[pair] -= count

        self.all_pairs_count = temp_all_pairs_count
        self.letters_count = temp_letters_count

    def find_most_and_least_common_elements(self):
        
        counts = collections.Counter(self.polymer_template)
        common  = counts.most_common()
        _,most_common_count = common[0]
        _,least_common_count = common[-1]
        return most_common_count, least_common_count
        
