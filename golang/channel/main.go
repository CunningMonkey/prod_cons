package main

import (
	"fmt"
	"sync"
)

type Queue struct {
	datas  chan int
	length int
}

func MakeQueue(length int) Queue {
	return Queue{
		datas:  make(chan int, length),
		length: length,
	}
}

func (q *Queue) Put(data int) {
	q.datas <- data
	fmt.Printf("Put: %d \n", data)
}

func (q *Queue) Get() int {
	return <- q.datas
}

func main() {
	var wait sync.WaitGroup
	queue := MakeQueue(10)
	wait.Add(1)
	go func() {
		for i := 0; i < 100; i++ {
			queue.Put(i)
		}
		wait.Done()
	}()

	wait.Add(1)
	go func() {
		for i := 0; i < 100; i++ {
			fmt.Printf("Get: %d\n", queue.Get())
		}
		wait.Done()
	}()

	// time.Sleep(time.Duration(10) * time.Second()
	wait.Wait()
}
