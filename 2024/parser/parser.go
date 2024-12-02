package parser

import (
	"bufio"
	"log"
	"os"
	"strconv"
	"strings"
)

func ReadLinesFromFile(filename string) []string {
	file, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}
	var numLines int = 0
	defer file.Close()

	scanner := bufio.NewScanner(file)

	lines := make([]string, 0, 1024)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
		numLines += 1
	}
	return lines
}

func SplitLinesIntoRows(lines []string, numRows int) [][]int {
	var returnedArrays [][]int = make([][]int, 0)
	for range numRows {
		returnedArrays = append(returnedArrays, make([]int, 0))
	}
	for index := range len(lines) {
		fields := strings.Fields(strings.TrimSpace(lines[index]))
		for i := range len(fields) {
			val, err := strconv.Atoi(fields[i])
			if err != nil {
				log.Fatalf("%s is not a valid integer.\n", fields[i])
			}
			returnedArrays[index] = append(returnedArrays[index], val)
		}

	}
	return returnedArrays
}

func SplitLinesIntoColumns(lines []string, numColumns int) [][]int {
	var returnedArrays [][]int = make([][]int, 0)
	for range numColumns {
		returnedArrays = append(returnedArrays, make([]int, 0))
	}
	for index := range len(lines) {
		fields := strings.Fields(lines[index])
		for i := range len(fields) {
			stringValue, err := strconv.Atoi(fields[i])
			if err != nil {
				log.Fatal(err)
			}
			returnedArrays[i] = append(returnedArrays[i], stringValue)
		}
	}
	return returnedArrays
}
