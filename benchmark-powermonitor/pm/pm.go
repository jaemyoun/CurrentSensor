package main

import (
	"fmt"

	"bufio"
	"os"

	"strings"

	"github.com/tarm/serial"
)

func openSerial(serialName string) *serial.Port {
	c := &serial.Config{Name: "/dev/" + serialName, Baud: 115200}
	s, err := serial.OpenPort(c)
	if err != nil {
		fmt.Println("ERROR: cannot open the serial port of which name is", c.Name, ":", err)
		os.Exit(-1)
	}
	return s
}

func makeLogDir(logPath string) string {
	if err := os.MkdirAll(logPath, 0775); err != nil {
		fmt.Println("ERROR: cannot create the log directory.")
		os.Exit(-1)
	}

	if logPath[len(logPath)-1] != '/' {
		logPath = logPath + "/"
	}

	return logPath
}

func readSerial(s *serial.Port) []byte {
	buf := make([]byte, 128)
	n, err := s.Read(buf)
	if err != nil {
		fmt.Println("ERROR: cannot real serial port.", err)
		os.Exit(-1)
	}

	// fmt.Printf("%s", string(buf[:n]))
	return buf[:n]
}

func main() {

	if len(os.Args) != 3 {
		fmt.Println("ERROR: invalid arguments. i.e. $./pm [deviceNames:ttyACM0,ttyACM1] [logPath]")
		os.Exit(-1)
	}

	logPath := makeLogDir(os.Args[2])

	serialNames := strings.Split(os.Args[1], ",")
	for _, serialName := range serialNames {

		go func(serialName string) {
			serial := openSerial(serialName)

			fd, err := os.OpenFile(logPath+serialName, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0664)
			if err != nil {
				fmt.Println("ERROR: cannot open log file.", err)
				os.Exit(-1)
			}

			for {
				received := readSerial(serial)
				fd.Write(received)
			}
		}(serialName)
	}

	exit := make(chan bool)
	go func() {
		bio := bufio.NewReader(os.Stdin)
		bio.ReadByte()
		exit <- true
	}()

	select {
	case <-exit:
		return
	}
}
