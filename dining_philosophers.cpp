// https://leetcode.com/problems/the-dining-philosophers/

class DiningPhilosophers {
public:
    std::vector<bool> forks;
    std::mutex m;
    std::condition_variable cv;
    DiningPhilosophers():forks(5, true) {}

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) 
    {
        int left = philosopher;
        int right = (philosopher + 4) % 5;
        {
            std::unique_lock<mutex> ul(m);    
            cv.wait(ul, [this, left, right] {
                return forks[left] && forks[right];
            });
            forks[left] = false;
            forks[right] = false;
        }
        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();
        {
            unique_lock<mutex> ul(m);
            forks[left] = true;
            forks[right] = true;
        }
        cv.notify_all();
    }
};
