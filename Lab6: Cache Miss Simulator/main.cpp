#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

uint32_t num_bits_required(uint32_t number) {
    number = number - 1;
    uint32_t count = 0;
    while (number){
      count++;
      number >>= 1;
    }

   return count;
}

bool ispresent(const queue<uint32_t>& q, uint32_t value) {
    queue<uint32_t> temp_queue = q;  // Create a copy of the original queue

    while (!temp_queue.empty()) {
        if (temp_queue.front() == value) {
            return true;  // Value found in the queue
        }
        temp_queue.pop();  // Move to the next element in the queue
    }

    return false;  // Value not found in the queue
}

string int_to_hex(uint32_t value) {
    stringstream ss;
    ss << "0x" << hex << value;
    return ss.str();
}

class FIFO{
    private:

    int size; // Total size of the cache in Bytes
    int block_size; // Size of each cache block in Bytes
    int associativity; // Associativity of the cache
    string replacement_policy; // Replacement policy (FIFO, LRU, RANDOM)
    string writeback_policy; // Write policy (WB or WT)

    int tot_blocks; // total number of blocks in cache
    int num_sets; // total number of sets in cache

    vector<queue<uint32_t>> v;


    public:
    FIFO(int size, int block_size, int associativity, const string& replacement_policy, const string& writeback_policy) {
        this->size = size;
        this->block_size = block_size;
        this->associativity = associativity;
        this->replacement_policy = replacement_policy;
        this->writeback_policy = writeback_policy;
        this->tot_blocks = size/block_size;
        this->num_sets = tot_blocks/associativity;
        v.resize(num_sets, queue<uint32_t>());
    }

    string access(const string& mode, uint32_t address){

        uint32_t offset_bits = num_bits_required(block_size);
        uint32_t index_bits = num_bits_required(num_sets);
        uint32_t index = (address >> offset_bits) & ((1 << index_bits) - 1);
        uint32_t tag = address >> (index_bits + offset_bits);

        string status = "";

        // check for hit
        // cout<<mode<<" "<<endl;
        if(ispresent(v[index], tag)){
            status =  "Hit";
        }
        else if(mode == "W" && writeback_policy == "WT"){
            status = "Miss";
        }
        else{
            // Handle miss
            v[index].push(tag);
            while(v[index].size()>associativity) v[index].pop();
            status =  "Miss";
        }
        string result = "Address: " + int_to_hex(address) + ", Set: " + int_to_hex(index) + ", Tag: " + int_to_hex(tag) + ", " + status;
        return result;
    }
};

class FIFO_FULLY_ASSOCIATIVE{
    private:

    int size; // Total size of the cache in Bytes
    int block_size; // Size of each cache block in Bytes
    string replacement_policy; // Replacement policy (FIFO, LRU, RANDOM)
    string writeback_policy; // Write policy (WB or WT)

    int tot_blocks; // total number of blocks in cache

    queue<uint32_t> v;


    public:
    FIFO_FULLY_ASSOCIATIVE(int size, int block_size, const string& replacement_policy, const string& writeback_policy) {
        this->size = size;
        this->block_size = block_size;
        this->replacement_policy = replacement_policy;
        this->writeback_policy = writeback_policy;
        this->tot_blocks = size/block_size;
        while (!v.empty())
            v.pop();
    }

    string access(const string& mode, uint32_t address){
        
        uint32_t offset_bits = num_bits_required(block_size);
        uint32_t index = 0;
        uint32_t tag = address >> offset_bits;

        string status = "";

        // check for hit
        if(ispresent(v, tag)){
            status =  "Hit";
        }
        else if(mode == "W" && writeback_policy == "WT"){
            status = "Miss";
        }
        else{
            // Handle miss
            v.push(tag);
            while(v.size()>tot_blocks) v.pop();
            status =  "Miss";
        }
        string result = "Address: " + int_to_hex(address) + ", Set: " + int_to_hex(index) + ", Tag: " + int_to_hex(tag) + ", " + status;
        return result;
    }
};

class LRU{
    private:

    int size; // Total size of the cache in Bytes
    int block_size; // Size of each cache block in Bytes
    int associativity; // Associativity of the cache
    string replacement_policy; // Replacement policy (FIFO, LRU, RANDOM)
    string writeback_policy; // Write policy (WB or WT)

    int tot_blocks; // total number of blocks in cache
    int num_sets; // total number of sets in cache

    vector<vector<pair<uint32_t, int>>> v;

    public:
    LRU(int size, int block_size, int associativity, const string& replacement_policy, const string& writeback_policy) {
        this->size = size;
        this->block_size = block_size;
        this->associativity = associativity;
        this->replacement_policy = replacement_policy;
        this->writeback_policy = writeback_policy;
        this->tot_blocks = size/block_size;
        this->num_sets = tot_blocks/associativity;
        v.resize(num_sets, vector<pair<uint32_t, int>>(associativity, {INT32_MAX, -1}));
    }

    string access(const string& mode, uint32_t address){

        uint32_t offset_bits = num_bits_required(block_size);
        uint32_t index_bits = num_bits_required(num_sets);
        uint32_t index = (address >> offset_bits) & ((1 << index_bits) - 1);
        uint32_t tag = address >> (index_bits + offset_bits);

        bool ishit = false;

        int key = -1;
        for(int i=0; i<associativity; i++){
            key = max(key, v[index][i].second);
        }

        // check for hit
        for(int i=0; i<associativity; i++){
            if(v[index][i].first == tag && v[index][i].second != -1){
                v[index][i].second = key + 1;
                ishit = true;
                break;
            }
        }
        string status = "";
        if(ishit){
            // it is hit
            status = "Hit";
        }
        else if(mode == "W" && writeback_policy == "WT"){
            // do nothing
            status = "Miss";
        }
        else{
            // Handle miss
            // insert {tag, 0}
            int k = -1;
            int temp = INT32_MAX;
            for(int i=0; i<associativity; i++){
                if(v[index][i].second == -1){
                    k = i;
                    break;
                }
                if(v[index][i].second < temp){
                    temp = v[index][i].second;
                    k=i;
                }
            }
            v[index][k] = {tag, key+1};
            status = "Miss";
        }
        string result = "Address: " + int_to_hex(address) + ", Set: " + int_to_hex(index) + ", Tag: " + int_to_hex(tag) + ", " + status;
        return result;
    }
};

class LRU_FULLY_ASSOCIATIVE{
    private:

    int size; // Total size of the cache in Bytes
    int block_size; // Size of each cache block in Bytes
    string replacement_policy; // Replacement policy (FIFO, LRU, RANDOM)
    string writeback_policy; // Write policy (WB or WT)

    int tot_blocks; // total number of blocks in cache

    vector<pair<uint32_t, int>> v;

    public:
    LRU_FULLY_ASSOCIATIVE(int size, int block_size, const string& replacement_policy, const string& writeback_policy) {
        this->size = size;
        this->block_size = block_size;
        this->replacement_policy = replacement_policy;
        this->writeback_policy = writeback_policy;
        this->tot_blocks = size/block_size;
        v.resize(tot_blocks, {INT32_MAX, -1});
    }

    string access(const string& mode, uint32_t address){

        uint32_t offset_bits = num_bits_required(block_size);
        uint32_t index = 0;
        uint32_t tag = address >> offset_bits;

        bool ishit = false;

        int key = -1;
        for(int i=0; i<tot_blocks; i++){
            key = max(key, v[i].second);
        }

        // check for hit
        for(int i=0; i<tot_blocks; i++){
            if(v[i].first == tag && v[i].second != -1){
                v[i].second = key + 1;
                ishit = true;
                break;
            }
        }
        string status = "";
        if(ishit){
            // it is hit
            status = "Hit";
        }
        else if(mode == "W" && writeback_policy == "WT"){
            // do nothing
            status = "Miss";
        }
        else{
            // Handle miss
            // insert {tag, 0}
            int k = -1;
            int temp = INT32_MAX;
            for(int i=0; i<tot_blocks; i++){
                if(v[i].second == -1){
                    k = i;
                    break;
                }
                if(v[i].second < temp){
                    temp = v[i].second;
                    k=i;
                }
            }
            v[k] = {tag, key+1};
            status = "Miss";
        }
        string result = "Address: " + int_to_hex(address) + ", Set: " + int_to_hex(index) + ", Tag: " + int_to_hex(tag) + ", " + status;
        return result;
    }
};

class RANDOM{
    private:

    int size; // Total size of the cache in Bytes
    int block_size; // Size of each cache block in Bytes
    int associativity; // Associativity of the cache
    string replacement_policy; // Replacement policy (FIFO, LRU, RANDOM)
    string writeback_policy; // Write policy (WB or WT)

    int tot_blocks; // total number of blocks in cache
    int num_sets; // total number of sets in cache

    vector<vector<uint32_t>> v;

    public:
    RANDOM(int size, int block_size, int associativity, const string& replacement_policy, const string& writeback_policy) {
        this->size = size;
        this->block_size = block_size;
        this->associativity = associativity;
        this->replacement_policy = replacement_policy;
        this->writeback_policy = writeback_policy;
        this->tot_blocks = size/block_size;
        this->num_sets = tot_blocks/associativity;
        v.resize(num_sets, vector<uint32_t>());
    }

    string access(const string& mode, uint32_t address){

        uint32_t offset_bits = num_bits_required(block_size);
        uint32_t index_bits = num_bits_required(num_sets);
        uint32_t index = (address >> offset_bits) & ((1 << index_bits) - 1);
        uint32_t tag = address >> (index_bits + offset_bits);

        bool ishit = false;

        // check for hit
        for(int i=0; i<v[index].size(); i++){
            if(v[index][i]==tag){
                ishit = true;
                break;
            }
        }
        string status = "";
        if(ishit){
            status = "Hit";
        }
        else if(mode == "W" && writeback_policy == "WT"){
            status = "Miss";
        }
        else{
            // Handle miss
            if(v[index].size() < associativity){
                v[index].push_back(tag);
            }
            else {
                srand(time(0));
                int k = rand() % associativity;
                v[index][k] = tag;
            }
            status = "Miss";
        }
        string result = "Address: " + int_to_hex(address) + ", Set: " + int_to_hex(index) + ", Tag: " + int_to_hex(tag) + ", " + status;
        return result;
    }
};

class RANDOM_FULLY_ASSOCIATIVE{
    private:

    int size; // Total size of the cache in Bytes
    int block_size; // Size of each cache block in Bytes
    string replacement_policy; // Replacement policy (FIFO, LRU, RANDOM)
    string writeback_policy; // Write policy (WB or WT)

    int tot_blocks; // total number of blocks in cache

    vector<uint32_t> v;

    public:
    RANDOM_FULLY_ASSOCIATIVE(int size, int block_size, const string& replacement_policy, const string& writeback_policy) {
        this->size = size;
        this->block_size = block_size;
        this->replacement_policy = replacement_policy;
        this->writeback_policy = writeback_policy;
        this->tot_blocks = size/block_size;
    }

    string access(const string& mode, uint32_t address){

        uint32_t offset_bits = num_bits_required(block_size);
        uint32_t index = 0;
        uint32_t tag = address >> offset_bits;

        bool ishit = false;

        // check for hit
        for(int i=0; i<v.size(); i++){
            if(v[i]==tag){
                ishit = true;
                break;
            }
        }
        string status = "";
        if(ishit){
            status = "Hit";
        }
        else if(mode == "W" && writeback_policy == "WT"){
            status = "Miss";
        }
        else{
            // Handle miss
            if(v.size() < tot_blocks){
                v.push_back(tag);
            }
            else {
                srand(time(0));
                int k = rand() % tot_blocks;
                v[k] = tag;
            }
            status = "Miss";
        }
        string result = "Address: " + int_to_hex(address) + ", Set: " + int_to_hex(index) + ", Tag: " + int_to_hex(tag) + ", " + status;
        return result;
    }
};

int main() {
    // Read cache configuration from the file
    ifstream config_file("cache.config");
    if (!config_file.is_open()) {
        cerr << "Error: Unable to open configuration file." << endl;
        return 1;
    }

    int size, block_size, associativity;
    string replacement_policy, writeback_policy;

    config_file >> size >> block_size >> associativity >> replacement_policy >> writeback_policy;
    config_file.close();


    // Read access sequence from the file
    ifstream access_file("cache.access");
    if (!access_file.is_open()) {
        cerr << "Error: Unable to open access file." << endl;
        return 1;
    }

    string mode;
    uint32_t address;

    if(associativity == 0){
        if(replacement_policy == "LRU"){
            LRU_FULLY_ASSOCIATIVE cache(size, block_size, replacement_policy, writeback_policy);
            // Process each access
            while (access_file >> mode >> hex >> address) {
                mode.pop_back();
                string result = cache.access(mode, address);
                cout<< result << endl;
            }
        }
        else if(replacement_policy == "FIFO"){
            FIFO_FULLY_ASSOCIATIVE cache(size, block_size, replacement_policy, writeback_policy);
            // Process each access
            while (access_file >> mode >> hex >> address) {
                mode.pop_back();
                string result = cache.access(mode, address);
                cout<< result << endl;
            }
        }
        if(replacement_policy == "RANDOM"){
            RANDOM_FULLY_ASSOCIATIVE cache(size, block_size, replacement_policy, writeback_policy);
            // Process each access
            while (access_file >> mode >> hex >> address) {
                mode.pop_back();
                string result = cache.access(mode, address);
                cout<< result << endl;
            }
        }
    }
    else {
        if(replacement_policy == "LRU"){
            LRU cache(size, block_size, associativity, replacement_policy, writeback_policy);
            // Process each access
            while (access_file >> mode >> hex >> address) {
                mode.pop_back();
                string result = cache.access(mode, address);
                cout<< result << endl;
            }
        }
        else if(replacement_policy == "FIFO"){
            FIFO cache(size, block_size, associativity, replacement_policy, writeback_policy);
            // Process each access
            while (access_file >> mode >> hex >> address) {
                mode.pop_back();
                string result = cache.access(mode, address);
                cout<< result << endl;
            }
        }
        if(replacement_policy == "RANDOM"){
            RANDOM cache(size, block_size, associativity, replacement_policy, writeback_policy);
            // Process each access
            while (access_file >> mode >> hex >> address) {
                mode.pop_back();
                string result = cache.access(mode, address);
                cout<< result << endl;
            }
        }
    }

    access_file.close();

    return 0;
}