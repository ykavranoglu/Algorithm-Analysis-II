/*
Title: Gale Shapley Monogamy
Author: ykavranoglu
Date: 13.03.2021
*/

/*  notes to self
    CONSIDER: using this with variables with meaningful names: proposers[i].preferences[j]
*/

#include <iostream>
#include <fstream>
#include <string>

struct Proposer {
    unsigned int* preferences;  //A dynamic array for the appropriate size will be created
    unsigned int proposition_index = 0;
};

struct Node {
    unsigned int value;
    Node* next = NULL;
};

class Int_Queue {   //highest priority elements are closer to head, head gets processed first. (then head changes and always head gets processed)
public:
    Int_Queue();
    void enqueue(unsigned int);
    void dequeue();
    unsigned int read_value();
    bool is_empty();
private:
    Node* head;
    Node* tail;
};

int main(int argc, char* argv[])
{
    std::string file_name_1 = argv[1];
    std::string file_name_2 = argv[2];
    
    std::ifstream file_1;
    file_1.open(file_name_1);
    if (!file_1) {
        std::cerr << "File 1 can't be opened!";
        exit(1);
    }

    std::ifstream file_2;
    file_2.open(file_name_2);
    if (!file_2) {
        std::cerr << "File 2 can't be opened!";
        exit(1);
    }

    unsigned int couple_num;
    file_1 >> couple_num;
    file_2 >> couple_num;   //redundant assignment, to get through the first line
    
    Proposer* proposers = new Proposer[couple_num]; //created a proposers array containing couple_num of elements
    unsigned int** reactors = new unsigned int*[couple_num];  //created a reactors array containing couple_num of elements
    int* reactor_matches = new int[couple_num]; //negative values means don't have a match

    for (unsigned int i = 0; i < couple_num; i++) {
        reactor_matches[i] = -1;    //negative values means don't have a match
    }

    for (unsigned int i = 0; i < couple_num; i++) {
        proposers[i].preferences = new unsigned int[couple_num];    //preferences array for the required space is created
        reactors[i] = new unsigned int[couple_num];
        unsigned int temporary;
        
        for (unsigned int j = 0; j < couple_num; j++) {
            file_1 >> proposers[i].preferences[j];

            file_2 >> temporary;
            reactors[i][temporary] = j; //reactors list is inversed for ease of operations. Example use: if reactors[1][3] < reactors[1][5], then reactor[1] prefers proposer 3 to 5.
        }
    }

    Int_Queue queue;
    for (unsigned int i = 0; i < couple_num; i++) {
        queue.enqueue(i);   //every proposer is queued at the start
    }

    //in this block, "prop_i'th proposer" will be referred to as "proposer" and "prop_i'th proposers i'th preferred reactor" will be referred to as "reactor" in the comments
    while (!queue.is_empty()) { //while queue is not empty
        unsigned int prop_i = queue.read_value();    // prop_i: "proposer index"
        
        unsigned int i; //holds Proposer::proposition_index
        bool has_match = false;
        while (!has_match) {
            i = proposers[prop_i].proposition_index;

            if (reactor_matches[proposers[prop_i].preferences[i]] < 0) { //if reactor doesn't have a match
                reactor_matches[proposers[prop_i].preferences[i]] = prop_i;
                has_match = true;
            }

            else {  //reactor already has a match
                if (reactors[proposers[prop_i].preferences[i]][prop_i] < reactors[proposers[prop_i].preferences[i]][reactor_matches[proposers[prop_i].preferences[i]]]) {   //if reactor likes proposer more than current match
                    queue.enqueue(reactor_matches[proposers[prop_i].preferences[i]]);

                    reactor_matches[proposers[prop_i].preferences[i]] = prop_i;
                    has_match = true;

                }
                
                else {  //if reactor likes current match more than proposer
                    proposers[prop_i].proposition_index++;

                    if (proposers[prop_i].proposition_index == couple_num) {
                        std::cerr << "Error! proposition_index overflew";   //this shouldn't happen
                    }
                }
            }
        }
        
        queue.dequeue();    //removes the most-priority element from queue since we found a match for it
    }

    //Output Block
    for (unsigned int i = 0; i < couple_num; i++) {
        std::cout << "Proposer: " << i << "\tmatched with\tReactor: " << reactor_matches[i] << std::endl;
    }

    //returning memory
    for (unsigned int i = 0; i < couple_num; i++) {
        delete[] proposers[i].preferences;
        delete[] reactors[i];
    }
    delete[] proposers;
    delete[] reactors;
    delete[] reactor_matches;
}

Int_Queue::Int_Queue()
    :head(NULL), tail(NULL)
{
}

void Int_Queue::enqueue(unsigned int new_value)
{
    Node* new_node = new Node();    //memory allocation for new node
    new_node->value = new_value;
    
    if (!head) { //if queue is empty
        head = new_node;
        tail = new_node;
        return;
    }

    tail->next = new_node;
    tail = new_node;
}

void Int_Queue::dequeue()
{
    if (!head) {
        std::cerr << "Queue is empty. Can't dequeue.";
        return;
    }

    if (head == tail) { //if queue only has 1 element
        tail = NULL;
    }

    Node* temporary;
    temporary = head;
    head = head->next;
    delete temporary;
}

unsigned int Int_Queue::read_value()
{
    if (!head) {
        std::cerr << "Queue is empty. Value can't be read.";
        return 0;
    }

    return head->value;
}

bool Int_Queue::is_empty()
{
    if (head) {
        return false;
    }
    
    return true;
}
