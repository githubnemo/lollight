package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"time"
)

type color struct {
	R, G, B uint8
}

var (
	input     chan color
	flag_Host = flag.String("host", "", "<host[:port]> of pidioder")
)

func send(c color) {
	url := fmt.Sprintf("http://%s/do?action=set&r=%d&g=%d&b=%d", *flag_Host, c.R, c.G, c.B)
	_, err := http.Get(url)

	if err != nil {
		log.Fatal(err)
	}
}

func stepChannel(cur, target uint8) uint8 {
	switch {
	case cur < target:
		return cur + 1
	case cur > target:
		return cur - 1
	default:
		return cur
	}
}

func fadeStep(cur, target color) color {
	return color{
		stepChannel(cur.R, target.R),
		stepChannel(cur.G, target.G),
		stepChannel(cur.B, target.B),
	}
}

func fader() {
	var current color
	var last color

	for {
		select {
		case c := <-input:
			current = c
		default:
			last = fadeStep(last, current)
			send(last)
			time.Sleep(10 * time.Millisecond)
		}
	}
}

func main() {
	flag.Parse()

	if *flag_Host == "" {
		log.Fatal("No PiDioder address given.")
	}

	input = make(chan color)

	go fader()

	for {
		var r, g, b int

		_, err := fmt.Scanf("%d %d %d", &r, &g, &b)

		if err != nil {
			log.Fatal(err)
		}

		input <- color{uint8(r), uint8(g), uint8(b)}
	}
}
