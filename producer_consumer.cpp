class ProducerConsumer 
{
public:
    ProducerConsumer(int capacity) 
    {
        this->max_size = capacity;
        this->cur_size = 0;
    }
    
    void enqueue(int element) 
    {
        std::unique_lock<std::mutex> ul1(m);
        this->cv.wait(ul1, [this] {
            return (this->cur_size < this->max_size) ? true : false;
        });
        q.push(element);
        this->cur_size++;
        this->cv.notify_one();
    }
    
    int dequeue() 
    {
        std::unique_lock<std::mutex> ul2(m);
        this->cv.wait(ul2, [this] {
            return (this->cur_size > 0) ? true : false;
        });
        auto ans = q.front();
        q.pop();
        this->cur_size--;
        cv.notify_one();
        return ans;
    }
    
    int size() 
    {
        std::lock_guard<std::mutex> lg(m);
        return this->cur_size;   
    }
private:
    std::mutex m;
    std::condition_variable cv;
    int cur_size;
    int max_size;
    queue<int> q;
};
