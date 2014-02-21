all: capture

capture: capture.cpp
	g++ $< -o $@ -lX11

sender/lollight-sender-mkII: sender/lollight-sender-mkII.go
	(cd sender; go build -o lollight-sender-mkII)

run_fast: capture sender/lollight-sender-mkII
	./capture | GOMAXPROCS=2 ./sender/lollight-sender-mkII -host=$(host)

run_slow: capture
	./capture | ./sender/lollight-sender-mkI $(host)
