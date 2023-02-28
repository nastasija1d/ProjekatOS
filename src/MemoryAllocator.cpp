#include "../h/MemoryAllocator.hpp"

Node* MemoryAllocator::free = nullptr;
Node* MemoryAllocator::busy = nullptr;

void MemoryAllocator::initialize() {
    // inicijalizacija memorije na pocetku -> ceo dostupan prosotr ce biti jedan jedini cvor u listi slobodnih
    free = (Node*)HEAP_START_ADDR;
    free->next = nullptr;
    free->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - MEM_BLOCK_SIZE);
    busy = nullptr;
}

void* MemoryAllocator::alloc(size_t size)
{
    size = size * MEM_BLOCK_SIZE;
    Node *prev = nullptr;
    for(Node *temp = free; temp != nullptr ; prev = temp, temp = temp->next) {
        if(temp->size >= size){
            if (temp->size - size > 0) {
                Node *newNode = (Node *) ((char *) temp + size + sizeof(Node));
                if (prev) {
                    prev->next = newNode;
                }else {
                    free = newNode;
                }
                newNode->next = temp->next;
                newNode->size = temp->size - size - sizeof(Node);
            }else{
                if(prev){
                    prev->next = temp->next;
                }else {
                    free = temp->next;
                }
            }
            temp->size = size;
            temp->next = nullptr;

//ubacivanje u listu zauzetih

            if(busy == nullptr)
            {
                busy = temp;
                busy->next = nullptr;
                busy->size = size;
                return (void*)((char*)temp + sizeof(Node));
            }
            Node *p = busy;
            if((char*)temp < (char*)p)
            {
                busy = temp;
                busy->next = p;
                busy->size = temp->size;
                return (void*)((char*)temp + sizeof(Node));
            }
            if(busy->next == nullptr){
                Node *pom = busy;
                if((char*)pom > (char*)temp){
                    busy = temp;
                    busy->next = pom;
                    busy->size = temp->size;
                    pom->next = nullptr;
                }else{
                    pom->next = temp;
                    temp->next = nullptr;
                    temp->size = size;
                }
            }else{
                while(p->next != nullptr && (char*)p->next < (char*)temp)
                    p = p->next;
                temp->next = p->next;
                temp->size = size;
                p->next = temp;
            }
            return (void*)((char*)temp + sizeof(Node));
        }
    }
    return nullptr;
}

int MemoryAllocator::dealloc(void *ptr)
{
    if(ptr == nullptr || busy == nullptr) return -1;
    char *adr = ((char*)ptr - sizeof(Node));
    if (adr < (char*)busy) return -1;

    Node *temp = busy, *prev = nullptr;

    if (adr == (char*)busy){
        busy = busy->next;
    }else{
        while (temp->next != nullptr && adr != (char*)temp){
            prev = temp;
            temp = temp->next;
        }
        if(prev) prev->next = temp->next;
    }

//ubacivanje u listu slobodnih

    Node *newNode = temp;
    newNode->size = temp->size;
    newNode->next = nullptr;

    if(free) {
        Node *t = free;
        Node *p = nullptr;
        if ((char *) newNode < (char *) free) //ako treba da se ubaci pre prvog, postaje prvi
        {
            free = newNode;
            free->next = t;
            free->size = newNode->size;
            if ((char *) t == (char *) free + free->size + sizeof(Node)) {
                free->size += t->size + sizeof(Node);
                free->next = t->next;
            }
            return 0;
        }
        if(free->next == nullptr){
            free->next = newNode;
            return 0;
        }
        for (p = free; p->next; p = p->next) {
            if (newNode < p->next) {
                if ((char *) p + p->size + sizeof(Node) == (char *) newNode) {
                    p->size = p->size + newNode->size + sizeof(Node);
                    t = p->next;
                    if ((char *) t == (char *) p + p->size + sizeof(Node)) {
                        p->size = p->size + t->size + sizeof(Node);
                        p->next = t->next;
                    }
                    return 0;
                }
                t = p->next;
                p->next = newNode;
                newNode->next = t;
                if ((char *) t == (char *) newNode + newNode->size + sizeof(Node)) {
                    newNode->size = newNode->size + t->size + sizeof(Node);
                    newNode->next = t->next;
                }
                return 0;
            }
            t = p;
        }
        p->next = newNode;
    }else
    {
        free = newNode;
        free->size = newNode->size;
        free->next = nullptr;
    }
    return 0;
}

/*
void MemoryAllocator::print(){
    printString("\nLista slobodnih\n");
    Node* t = free;
    while(t){
        printInt((uint64)t % 1000000);
        printString(" s:");
        printInt((int)t->size);
        printString(", ");
        t = t->next;
    }
    printString("\nLista zauzetih\n");
    t = busy;
    while(t){
        printInt((uint64)t % 1000000);
        printString(" s:");
        printInt((int)t->size);
        printString(", ");
        t = t->next;
    }
    printString("\n");
}
 */