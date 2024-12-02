package main

import (
	"fmt"
	"hoop/parser"
	"log"
	"math"
	"sort"
)

func calculateDistance(col1 []int, col2 []int) int {
	if len(col1) != len(col2) {
		log.Fatal("Columns are of different length.\n")
	}

	var totalDistance float64 = 0
	var diff float64 = 0

	for index := range len(col1) {
		diff = math.Abs(float64(col1[index] - col2[index]))
		totalDistance += diff
	}

	return int(math.Round(totalDistance))
}

func calculateSimilarityScore(col1 []int, col2 []int) int {
	//Inputs col1 and col2 must be sorted.
	var leftIndex int = 0
	var rightIndex int = 0
	var leftValue int = 0
	var leftCount int = 0
	var rightCount int = 0
	//Note: similarity score is calculated by taking the number of times that X occurs in col2 for every
	//X in col1
	var similarityScore int = 0

	for leftIndex < len(col1) {

		leftValue = col1[leftIndex]
		leftCount = 1

		//Count same numbers on left + increment left
		for leftIndex+1 < len(col1) && leftValue == col1[leftIndex+1] {
			leftIndex += 1
			leftValue = col1[leftIndex]
			leftCount += 1
		}

		//Count numbers on right + increment right
		// rightValue := col2[rightIndex]
		for rightIndex+1 < len(col2) && leftValue > col2[rightIndex] {
			rightIndex += 1
		}

		for rightIndex < len(col2) && leftValue == col2[rightIndex] {
			rightCount += 1
			rightIndex += 1
			// rightValue = col2[rightIndex]
		}

		score := leftValue * leftCount * rightCount
		similarityScore += score

		//reset
		rightCount = 0
		leftIndex += 1
	}
	return similarityScore

}

func main() {
	fmt.Println("hello world")
	lines := parser.ReadLinesFromFile("input")

	for index := range len(lines) {
		fmt.Println(lines[index])
	}
	var numColumns int = 2
	var columns [][]int = parser.SplitLinesIntoColumns(lines, numColumns)

	var col1 []int = columns[0]
	var col2 []int = columns[1]

	sort.Ints(col1)
	sort.Ints(col2)
	fmt.Println(col1)
	fmt.Println(col2)

	var answer int = calculateDistance(col1, col2)
	fmt.Printf("Part 1 Answer: %d\n", answer)

	answer = calculateSimilarityScore(col1, col2)
	fmt.Printf("Part 2 Answer: %d\n", answer)

}
