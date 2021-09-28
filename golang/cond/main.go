package main

import (
	"fmt"
	"sync"
)

type Queue struct {
	datas    []int
	length   int
	condProd sync.Cond
	condCons sync.Cond
	lock     *sync.Mutex
}

func MakeQueue(length int) Queue {
	mutex := new(sync.Mutex)
	return Queue{
		datas:    []int{},
		condProd: *sync.NewCond(mutex),
		condCons: *sync.NewCond(mutex),
		lock:     mutex,
		length:   length,
	}
}

func (q *Queue) Put(data int) {
	q.lock.Lock()
	if len(q.datas) >= q.length {
		q.condProd.Wait()
	}
	q.datas = append(q.datas, data)
	fmt.Printf("Put: %d\n", data)
	q.condCons.Signal()
	q.lock.Unlock()
}

func (q *Queue) Get() int {
	q.lock.Lock()
	if len(q.datas) == 0 {
		q.condCons.Wait()
	}
	res := q.datas[0]
	if len(q.datas) == 1 {
		q.datas = []int{}
	} else {
		q.datas = q.datas[1:]
	}
	q.lock.Unlock()
	q.condProd.Signal()
	return res
}

func main() {
	var wait sync.WaitGroup
	queue := MakeQueue(1)
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
