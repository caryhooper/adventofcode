package main

import (
	"fmt"
	"hoop/parser"
	"strconv"
	"strings"
)

type Rule struct {
	numBefore int
	numAfter  int
}
type RuleSet []Rule

func applyRule(rule Rule, pageNumberArray []int) bool {
	//interate through pageNumberArray until we get to the first number.
	//    if numAfter is found and numBefore is found, numBefore must have been found first.
	// var numBeforeFound bool = false
	var numBeforeFound bool = false
	var numAfterFoundFirst bool = false
	for _, num := range pageNumberArray {
		if num == rule.numBefore {
			numBeforeFound = true
		}
		if num == rule.numAfter && numBeforeFound {
			return true
		}
		if num == rule.numAfter && !numBeforeFound {
			numAfterFoundFirst = true
		}
	}
	if numAfterFoundFirst && numBeforeFound {
		return false
	} else {
		return true
	}
}

func returnMiddle(pageNumberArray []int) int {
	middleIndex := (len(pageNumberArray) - 1) / 2
	return pageNumberArray[middleIndex]
}

func findIndex(needle int, haystack []int) int {
	for index, val := range haystack {
		if val == needle {
			return index
		}
	}
	return 0
}

func correctNumSet(rules RuleSet, numSet []int) []int {
	//input nonCompliant numSet
	rule, isCompliant := isCompliantCheck(rules, numSet)
	//based on the rule, both numbers must be contained in set.  So let's scan for the index of the first number and bring it forward in the array
	for !isCompliant {
		//find index of second number of rule
		indexAfter := findIndex(rule.numAfter, numSet)
		indexBefore := findIndex(rule.numBefore, numSet)
		timesToSwap := indexBefore - indexAfter
		//swap values
		for i := 0; i < timesToSwap; i++ {
			//first swap indexAfter with indexAfter+1
			numSet[indexAfter+i], numSet[indexAfter+1+i] = numSet[indexAfter+1+i], numSet[indexAfter+i]
		}
		rule, isCompliant = isCompliantCheck(rules, numSet)
	}
	return numSet
}

func isCompliantCheck(rules RuleSet, pageNumSet []int) (Rule, bool) {
	// var isCompliant bool = true
	for _, rule := range rules {
		//apply rule against pageNumSet
		if !applyRule(rule, pageNumSet) {
			return rule, false
		}
	}
	return Rule{}, true
}

func main() {
	lines := parser.ReadLinesFromFile("input")
	var index int = 0
	var line string = ""
	//find split
	for index, line = range lines {
		if line == "" {
			break
		}
	}
	ruleLines := lines[0:index]
	pageNumbers := lines[index+1:]
	var rules RuleSet

	//Save initial set of rules in "rules" RuleSet
	for _, line := range ruleLines {
		line = strings.TrimSpace(line)
		fields := strings.Split(line, "|")
		//TODO strconv error handling
		firstNum, _ := strconv.Atoi(fields[0])
		secondNum, _ := strconv.Atoi(fields[1])
		var rule Rule = Rule{numBefore: firstNum, numAfter: secondNum}
		rules = append(rules, rule)
	}

	nonCompliantNumSets := make([][]int, 0)

	//do work
	var sumMiddleNumbers int = 0
	for _, line := range pageNumbers {
		var pageNumSet []int = make([]int, 0)
		line = strings.TrimSpace(line)
		fields := strings.Split(line, ",")
		//Populate pageNumSet with pages (parse string)
		for _, num := range fields {
			//TODO strconv error handling
			nextNum, _ := strconv.Atoi(num)
			pageNumSet = append(pageNumSet, nextNum)
		}
		//Run rules against the pageNumSet

		_, isCompliant := isCompliantCheck(rules, pageNumSet)

		if isCompliant {
			middleValue := returnMiddle(pageNumSet)
			sumMiddleNumbers += middleValue
		} else {
			nonCompliantNumSets = append(nonCompliantNumSets, pageNumSet)
		}
	}

	fmt.Printf("Part 1 Answer: %d\n", sumMiddleNumbers)

	sumMiddleNumbers = 0
	for _, numSet := range nonCompliantNumSets {
		newNumSet := correctNumSet(rules, numSet)
		sumMiddleNumbers += returnMiddle(newNumSet)
	}
	fmt.Printf("Part 2 Answer: %d\n", sumMiddleNumbers)
}
