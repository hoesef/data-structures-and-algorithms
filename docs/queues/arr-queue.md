# Array-based queue
A queue is a FIFO (First in, first out) data structure, which, as the name suggests, means that the first element inserted into the queue is the first one removed when a "pop" command is called. The arr-queue::Queue class uses a resizable array of type T as its underlying data container, this is to allow the queue to contain any type without issue.

The Queue class has 2 main functions; **void add(T data)** to insert an element at the end of the queue, and **T pop()** to remove the first element from the queue. There is also a **print()** function to print the content of the queue to the terminal in the same format as a python array.

The Queue class makes use of the *operator new* function in order to allocate new memory to the heap without initializing it, this prevents copying objects unnessecrrally, boosting performance. Additionally, when moving data around in the queue (be it through resize, adding, or poping), move semantics are utlized where possible to steal attributes rather than perform deep coppies. 

<!-- ## Implementation details -->
<!-- The Queue class has 5 attributes:
- m_ptr - this is a pointer to the start of the block of memory that contains the data held in the queue.
- m_size - this is the current size of the queue array
- m_elements - this is the number of elements currently stored in the queue
- m_start - this is the index into the array corresponding to the first element
- m_end - this is the index of the array where the next item should be inserted -->

### Add
Add takes in a pass-by-value copy of the data to be inserted into the array. It first checks that the queue is large enough to store the data, if it is then we use **placement new**  to insert into the appropriate section of the queue. This is just constant time, O(1).

If, however, the queue is currently full, the resize function is called to double the size of the queue. This creates space for the new element to be inserted but also requires copying all other elements of the queue into this larger memory block.

Assuming we have a queue of size $2^k$, and we perform $2^k$ insertions and then one more to force a resize. This is $2^k+1$ additions plus the resizing cost:
$$1 + 2 + ... + 2^{k-1} + 2^k$$
which is $(2 \cdot 2^k) -1$. The overall cost is $3\cdot2^k$, which over $2^k$ operations gives an average cost of approximately 3. O{1}, constant time.

### Pop
Pop returns the element at the start of the queue to the calling function. If the capacity of the queue (m_elements / m_size) is less that or equal to 0.25 then the queue's array is shrunk by 50% to reduce wasted memory. Each time the queue is shrunk, all elements need to be copied over to the new queue location. For a queue with an initial size of N, this can be represented by the series:
$$N/4 + N/8 + N/16 + ...  \space O(N/4) = O(1)$$
