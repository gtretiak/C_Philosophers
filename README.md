# C_Philosophers
Simulating the classic dining philosophers problem using threads and mutexes to explore concurrent programming concepts and prevent deadlock scenarios.

## 📖 Project Overview

This project simulates philosophers sitting around a round table, alternating between eating, thinking, and sleeping. Each philosopher needs two forks to eat spaghetti, but there are only as many forks as philosophers, creating a classic synchronization problem.

### The Challenge
- Philosophers must eat to survive (avoid starvation)
- Each philosopher needs 2 forks to eat
- Forks are shared resources between adjacent philosophers
- Philosophers cannot communicate with each other
- The simulation must avoid deadlocks and data races

<!--## 🏗️ Project Structure

```
philosophers/
├── philo/                  # Mandatory part (threads + mutexes)
│   ├── Makefile
│   ├── *.h
│   └── *.c
└── philo_bonus/           # Bonus part (processes + semaphores)
    ├── Makefile
    ├── *.h
    └── *.c
```
-->
## 🚀 Usage

### Compilation
```bash
# Mandatory part
cd philo && make
### Execution
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Parameters
- `number_of_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Time in ms before a philosopher dies of starvation
- `time_to_eat`: Time in ms a philosopher spends eating
- `time_to_sleep`: Time in ms a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat`: Optional simulation end condition

### Example
```bash
./philo 5 800 200 200    # 5 philosophers, 800ms to die, 200ms to eat/sleep
./philo 4 410 200 200 3  # Stop after each philosopher eats 3 times
```

## 📊 Program Output

The program outputs timestamped state changes:
```
142 1 has taken a fork
142 1 has taken a fork
142 1 is eating
342 1 is sleeping
542 1 is thinking
```

Output format:
- `timestamp_in_ms X has taken a fork`
- `timestamp_in_ms X is eating`
- `timestamp_in_ms X is sleeping`
- `timestamp_in_ms X is thinking`
- `timestamp_in_ms X died`

## 🔧 Implementation Details

### Mandatory Part (philo)
- **Threading**: Each philosopher runs as a separate thread
- **Synchronization**: Mutexes protect fork states
- **Fork Management**: One fork between each pair of philosophers
- **Edge Case**: Single philosopher has access to only one fork
<!--
### Bonus Part (philo_bonus)
- **Processes**: Each philosopher runs as a separate process
- **Synchronization**: Semaphores manage fork availability
- **Fork Pool**: All forks available in the middle of the table
- **IPC**: Inter-process communication via semaphores
-->
## 🛠️ Technical Requirements

### Allowed Functions (Mandatory)
- `memset`, `printf`, `malloc`, `free`, `write`
- `usleep`, `gettimeofday`
- `pthread_create`, `pthread_detach`, `pthread_join`
- `pthread_mutex_init`, `pthread_mutex_destroy`
- `pthread_mutex_lock`, `pthread_mutex_unlock`
<!--
### Allowed Functions (Bonus)
- All mandatory functions plus:
- `fork`, `kill`, `exit`, `waitpid`
- `sem_open`, `sem_close`, `sem_post`, `sem_wait`, `sem_unlink`
-->
### Constraints
- No global variables allowed
- No data races permitted
- Death messages must appear within 10ms of actual death
- Proper memory management (no leaks)
- Thread-safe implementation

## 🎯 Key Challenges

1. **Deadlock Prevention**: Avoiding scenarios where all philosophers wait for forks
2. **Starvation Prevention**: Ensuring all philosophers get chances to eat
3. **Race Condition Handling**: Protecting shared resources with proper synchronization
4. **Timing Precision**: Accurate death detection and state logging
5. **Resource Management**: Proper cleanup of threads/processes and memory

## 🏆 Learning Objectives

- **Concurrent Programming**: Understanding threads and processes
- **Synchronization Primitives**: Mastering mutexes and semaphores
- **Deadlock/Starvation**: Recognizing and preventing common concurrency issues
- **System Programming**: Working with POSIX threading and IPC
- **Debugging**: Identifying race conditions and timing issues

## ⚠️ Common Pitfalls

- **Incorrect mutex usage**: Leading to deadlocks or data races
- **Poor timing logic**: Inaccurate death detection
- **Resource leaks**: Not properly cleaning up threads/mutexes
- **Edge cases**: Single philosopher scenario
- **Message ordering**: Overlapping output from multiple threads

## 🔍 Testing

Test with various scenarios:
- Different philosopher counts (1, 2, 3, 5, 100+)
- Edge cases (very short/long times)
- Stress testing (many philosophers, tight timing)
- Death scenarios (insufficient time to eat)

---
