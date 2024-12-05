package main

import (
	"errors"
	"fmt"
	"hoop/parser"
	"reflect"
	"sort"
)

type WordSquare struct {
	letter rune
	isUsed bool
}

type GridSquare struct {
	x int
	y int
}

type ByValue []GridSquare

func (a ByValue) Len() int      { return len(a) }
func (a ByValue) Swap(i, j int) { a[i], a[j] = a[j], a[i] }
func (a ByValue) Less(i, j int) bool {
	if a[i].x == a[j].x {
		return a[i].y < a[j].y
	} else {
		return a[i].x < a[j].x
	}
}

func retrieveRunes(letterGrids []GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	thisWord := make([]rune, 0)
	//First get all runes in order
	for _, gs := range letterGrids {
		thisWord = append(thisWord, ws[gs.y][gs.x].letter)
	}
	if !reflect.DeepEqual(thisWord, word) {
		return letterGrids, errors.New("word not found")
	} else {
		return letterGrids, nil
	}

}

//Given an array of grid squares, determine if letters match

func searchUp(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(3,3) --> (3,3),(3,2),(3,1),(3,0) // dec y

	keyLen := len(word)
	//bounds check
	if startGrid.y-keyLen+1 < 0 {
		return words, errors.New("out of bounds")
	}
	//grab word and compare
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x,
			y: startGrid.y - index,
		}
		words = append(words, gs)
	}

	foundWords, err := retrieveRunes(words, word, ws)
	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchDown(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(0,0) --> (0,0),(0,1),(0,2),(0,3) //inc y

	keyLen := len(word)
	//bounds check
	if startGrid.y+keyLen > len(ws) {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x,
			y: startGrid.y + index,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchRight(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(0,0) --> (0,0),(1,0),(2,0),(3,0) //inc x

	keyLen := len(word)
	//bounds check
	if startGrid.x+keyLen > len(ws[startGrid.y]) {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x + index,
			y: startGrid.y,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchLeft(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(3,3) --> (3,3),(2,3),(1,3),(0,3) - decrement x

	keyLen := len(word)
	//bounds check
	if startGrid.x-keyLen+1 < 0 {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x - index,
			y: startGrid.y,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchDiagUpRight(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(3,3) --> (3,3),(4,2),(5,1),(6,0) - inc x, dec y

	keyLen := len(word)
	//bounds check
	if startGrid.x+keyLen > len(ws[startGrid.y]) || startGrid.y-keyLen+1 < 0 {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x + index,
			y: startGrid.y - index,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchDiagUpLeft(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(3,3) --> (3,3),(2,2),(1,1),(0,0) //dec x dec y

	keyLen := len(word)
	//bounds check
	if startGrid.x-keyLen+1 < 0 || startGrid.y-keyLen+1 < 0 {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x - index,
			y: startGrid.y - index,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchDiagDownRight(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(3,3) --> (3,3),(4,4),(5,5),(6,6) //inc x inc y

	keyLen := len(word)
	//bounds check
	if startGrid.x+keyLen > len(ws[startGrid.y]) || startGrid.y+keyLen > len(ws) {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x + index,
			y: startGrid.y + index,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}
func searchDiagDownLeft(startGrid GridSquare, word []rune, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(3,3) --> (3,3),(2,4),(1,5),(0,6) // dec x, inc y

	keyLen := len(word)
	//bounds check
	if startGrid.x-keyLen+1 < 0 || startGrid.y+keyLen > len(ws) {
		return words, errors.New("out of bounds")
	}
	//grab word and compare TODO-go to keylen-1 instead?
	for index := range keyLen {
		gs := GridSquare{
			x: startGrid.x - index,
			y: startGrid.y + index,
		}
		words = append(words, gs)
	}
	foundWords, err := retrieveRunes(words, word, ws)

	if err != nil {
		return foundWords, errors.New("word not found")
	}

	sort.Sort(ByValue(foundWords))
	return foundWords, nil
}

func searchXmas(startGrid GridSquare, ws [][]WordSquare) ([]GridSquare, error) {
	words := make([]GridSquare, 0)
	//(1,1) --> (0,0),(1,1),(2,2)  backslash
	if startGrid.x-1 < 0 || startGrid.x+1 >= len(ws[startGrid.y]) || startGrid.y-1 < 0 || startGrid.y+1 >= len(ws) {
		return words, errors.New("out of bounds")
	}

	check1 := string([]rune{ws[startGrid.y-1][startGrid.x-1].letter, ws[startGrid.y][startGrid.x].letter, ws[startGrid.y+1][startGrid.x+1].letter})
	check2 := string([]rune{ws[startGrid.y+1][startGrid.x+1].letter, ws[startGrid.y][startGrid.x].letter, ws[startGrid.y-1][startGrid.x-1].letter})

	//(1,1) --> (0,2),(1,1),(2,0) forwardslash
	check3 := string([]rune{ws[startGrid.y+1][startGrid.x-1].letter, ws[startGrid.y][startGrid.x].letter, ws[startGrid.y-1][startGrid.x+1].letter})
	check4 := string([]rune{ws[startGrid.y-1][startGrid.x+1].letter, ws[startGrid.y][startGrid.x].letter, ws[startGrid.y+1][startGrid.x-1].letter})
	if (check1 == "MAS" || check2 == "MAS") && (check3 == "MAS" || check4 == "MAS") {
		words = append(words, startGrid)
		return words, nil
	} else {
		return words, errors.New("word not found")
	}
}

func addWord(word []GridSquare, words [][]GridSquare) [][]GridSquare {
	var containsWord bool = false
	for index := range len(words) {
		// var t reflect.TestingT
		if reflect.DeepEqual(word, words[index]) {
			containsWord = true
		}
	}
	if !containsWord {
		words = append(words, word)
	}
	return words
}

func searchMas(ws [][]WordSquare) [][]GridSquare {
	words := make([][]GridSquare, 0)
	// runeKey := []rune("MAS")

	for y := range len(ws) {
		for x := range len(ws[y]) {
			gridSquare := GridSquare{
				x: x,
				y: y,
			}
			word, err := searchXmas(gridSquare, ws)
			if err == nil {
				words = addWord(word, words)
			}
		}
	}
	return words
}

func search(key string, ws [][]WordSquare) [][]GridSquare {
	//search left-to-right (0,0 first)
	//  then search top-to-bottom (X,Y)
	words := make([][]GridSquare, 0)
	runeKey := []rune(key)
	for y := range len(ws) {
		for x := range len(ws[y]) {
			// var word []GridSquare = make([]GridSquare,0)
			gridSquare := GridSquare{
				x: x,
				y: y,
			}
			word, err := searchUp(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchDown(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchLeft(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchRight(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchDiagUpLeft(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchDiagUpRight(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchDiagDownLeft(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
			word, err = searchDiagDownRight(gridSquare, runeKey, ws)
			if err == nil {
				words = addWord(word, words)
				ws[y][x].isUsed = true
			}
		}
	}

	return words
}

func convertToWordSquare(grid [][]rune) [][]WordSquare {
	ws := make([][]WordSquare, 0)
	for i := range len(grid) {
		row := make([]WordSquare, len(grid[i]))
		for j := range len(grid[i]) {
			row[j] = WordSquare{
				letter: grid[i][j],
				isUsed: false,
			}
		}
		ws = append(ws, row)
	}
	return ws
}

func main() {

	lines := parser.ReadLinesFromFile("input")
	grid := parser.SplitLinesInto2DArray(lines)
	wordSearch := convertToWordSquare(grid)
	words := search("XMAS", wordSearch)

	partOneAnswer := len(words)
	fmt.Printf("Part 1 Answer: %d\n", partOneAnswer)

	wordsPartTwo := searchMas(wordSearch)
	partTwoAnswer := len(wordsPartTwo)
	fmt.Printf("Part 2 Answer: %d\n", partTwoAnswer)
}
