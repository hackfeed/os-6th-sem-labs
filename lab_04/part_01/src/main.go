package main

import (
	"fmt"
	"os"
)

func main() {
	login := os.Getenv("LOGIN")
	password := os.Getenv("PASS")
	a := 0

	fmt.Println(login, password)
	fmt.Scanf("%v", a)
}
