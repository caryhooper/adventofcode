package main

import (
	"fmt"
	"hoop/parser"
	"log"
	"reflect"
	"strconv"
	"strings"
	"unicode"
)

func scoreMultiply(mul string) int {
	trimmed := strings.TrimLeft(strings.TrimRight(mul, ")"), "mul(")
	f := func(c rune) bool {
		if c == ',' {
			return true
		} else {
			return false
		}
	}
	fields := strings.FieldsFunc(trimmed, f)
	if len(fields) != 2 {
		log.Fatalf("Failed to parse string for two numbers: %s\n", trimmed)
	}
	firstNum, err := strconv.Atoi(fields[0])
	if err != nil {
		log.Fatalf("Cannot parse %s as an integer.\n", fields[0])
	}
	secondNum, err := strconv.Atoi(fields[1])
	if err != nil {
		log.Fatalf("Cannot parse %s as an integer.\n", fields[1])
	}
	return firstNum * secondNum
}

/*
This function peeks forward within the []rune stream and returns the longest string of numbers that it can.
*/
func peekNumbers(index int, line []rune) []rune {
	numArray := make([]rune, 0)
	nextNum := peek(index, line, 1)
	for unicode.IsDigit(nextNum[0]) {
		numArray = append(numArray, nextNum[0])
		index += 1
		nextNum = peek(index, line, 1)
	}
	return numArray
}

/*
This function performs error handling in peeking forward in the []rune stream
*/
func peek(index int, line []rune, length int) []rune {
	if index+length > len(line) {
		return make([]rune, length)
	} else {
		return line[index : index+length]
	}
}

func countMultiplies(line string, flag bool) (int, bool) {
	//look for mul(X,Y), where X and Y are 1-3 digit numbers
	var mulStatements []string = make([]string, 0)
	var pos int = 0
	var mulEnaled bool = flag
	runes := []rune(line)
	for pos < len(runes) {
		startPos := pos
		// var char rune = runes[pos]
		// pos += 1
		// fmt.Printf("Character %c at byte position %d\n", char, pos)
		prefix := peek(pos, runes, 7)
		if reflect.DeepEqual(prefix, []rune{'d', 'o', 'n', '\'', 't', '(', ')'}) {
			mulEnaled = false
			pos += 7
			continue
		}
		prefix = peek(pos, runes, 4)
		if reflect.DeepEqual(prefix, []rune{'d', 'o', '(', ')'}) {
			mulEnaled = true
			pos += 4
			continue
		}

		if !mulEnaled {
			pos += 1
			continue
		}

		// prefix = peek(pos, runes, 4)
		if !reflect.DeepEqual(prefix, []rune{'m', 'u', 'l', '('}) {
			pos += 1
			continue
		} else {
			//prefix mul( exists. Next we check numbers.  Check first num
			pos += 4
			firstNum := peekNumbers(pos, runes)
			pos += len(firstNum)
			if len(firstNum) < 1 || len(firstNum) > 3 {
				continue
			} else {
				//check comma
				comma := peek(pos, runes, 1)
				pos += 1
				if comma[0] != ',' {
					continue
				} else {
					secondNum := peekNumbers(pos, runes)
					pos += len(secondNum)
					if len(secondNum) < 1 || len(secondNum) > 3 {
						continue
					} else {
						//check trailing parentheses )
						parenth := peek(pos, runes, 1)
						pos += 1
						if parenth[0] != ')' {
							continue
						} else {
							//WE DID IT!
							//process mul
							mulRuneArray := runes[startPos:pos]
							mulString := string(mulRuneArray)
							fmt.Printf("Found: %s\n", mulString)
							mulStatements = append(mulStatements, mulString)
						}
					}
				}
			}

		}
	}
	var totalScore int = 0
	for index := range mulStatements {
		totalScore += scoreMultiply(mulStatements[index])
	}
	return totalScore, mulEnaled
}

func main() {
	lines := parser.ReadLinesFromFile("input")
	// var line string = lines[0]
	var count int = 0
	//Errors here when the input was multiple lines.
	//Needed to save/carry forward the do()/don't() from each line to the next
	var flag bool = true
	for index := range len(lines) {
		newCount, newFlag := countMultiplies(lines[index], flag)
		count = count + newCount
		flag = newFlag
	}
	// fmt.Printf("Part 1 Answer: %d\n", count)
	fmt.Printf("Part 2 Answer: %d\n", count)

}
