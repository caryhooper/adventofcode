package main

import (
	"fmt"
	"hoop/parser"
	"math"
)

type safeCheck func([]int) bool

// https://stackoverflow.com/questions/37334119/how-to-delete-an-element-from-a-slice-in-golang
func remove(slice []int, s int) []int {
	firstSlice := slice[:s]
	lastSlice := slice[s+1:]
	return append(firstSlice, lastSlice...)
}

func isSafeTolerance(row []int) bool {
	if len(row) == 0 {
		return false
	}
	if isSafe(row) {
		return true
	} else {
		for index := range len(row) {
			oldRow := make([]int, len(row))
			for i := range len(row) {
				oldRow[i] = row[i]
			}
			newRow := remove(oldRow, index)
			if isSafe(newRow) {
				return true
			}
		}
		return false
	}
}

func isSafe(row []int) bool {
	if len(row) == 0 {
		return false
	}
	fmt.Println(row)
	// var isSafe bool = true
	var isFirstComparison bool = true
	var isIncreasing bool
	//apply two rules
	//	The levels are either all increasing or all decreasing.
	//	Any two adjacent levels differ by at least one and at most three.
	for index := range len(row) {
		if index+1 < len(row) {
			thisVal := row[index]
			nextVal := row[index+1]

			diff := int(math.Round((math.Abs(float64(thisVal) - float64(nextVal)))))
			if diff == 0 || diff > 3 {
				return false
			}

			if isFirstComparison {
				isFirstComparison = false
				//initialize increasing/decreasing
				if thisVal > nextVal {
					isIncreasing = false
				} else if nextVal > thisVal {
					isIncreasing = true
				} else {
					//equal in size.  We should never get here.
					return false
				}
			} else {
				//hold true with strictly increasing/decreasing o/w return false
				if isIncreasing {
					if thisVal >= nextVal {
						return false
					}
				} else {
					if thisVal <= nextVal {
						return false
					}
				}
			}
		}
	}
	return true
}

func countSafe(rows [][]int, safeFunc safeCheck) int {
	var safeCount int = 0
	for index := range len(rows) {
		if safeFunc(rows[index]) {
			safeCount += 1
		}
	}
	return safeCount
}

func main() {
	lines := parser.ReadLinesFromFile("input")
	rows := parser.SplitLinesIntoRows(lines, 1000)
	partOneAnswer := countSafe(rows, isSafe)
	fmt.Printf("Part 1 Answer: %d\n", partOneAnswer)
	partTwoAnswer := countSafe(rows, isSafeTolerance)
	fmt.Printf("Part 2 Answer: %d\n", partTwoAnswer)
}
