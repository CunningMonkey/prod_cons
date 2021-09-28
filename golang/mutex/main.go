package main

import (
	"fmt"
	"sync"
)

type Queue struct {
	datas  []int
	mutex  sync.Mutex
	length int
}

func MakeQueue(length int) Queue {
	return Queue{
		datas:  []int{},
		length: length,
	}
}

func (q *Queue) Put(data int) {
	for {
		q.mutex.Lock()
		if len(q.datas) < q.length {
			q.datas = append(q.datas, data)
			fmt.Printf("Put: %d\n", data)
			q.mutex.Unlock()
			break
		}
		q.mutex.Unlock()
	}
}

func (q *Queue) Get() int {
	for {
		q.mutex.Lock()
		if len(q.datas) != 0 {
			q.mutex.Unlock()
			res := q.datas[0]
			if len(q.datas) == 1 {
				q.datas = []int{}
			} else {
				q.datas = q.datas[1:]
			}
			return res
		}
		q.mutex.Unlock()
	}
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
