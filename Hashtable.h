//
//  SeperateChain.h
//
//
//  Created by Viktoriya Lazarova on 13.05.16 г..
//
//

#ifndef Hashtable_h
#define Hashtable_h

#include<iostream>
#include "Container.h"
using namespace std;

class HashtableEmptyException : public ContainerException {
public:
    virtual const char* what() const noexcept override {
        return "Hashtable : empty ";
    }
};


template <typename E, size_t N=25>
class Hashtable : public Container<E> {
    size_t tsize;
    size_t numberElem;
    size_t maxFactor;
    void add_(const E& e);
    void remove_(const E& e);
    size_t hash(const E& e) const {return hashValue(e)% tsize;}
    void resize(size_t newSize);
    void quicksort(E*, int, int) const;
    E w() const;
    size_t y() const;
    
 public:
    struct Element {
        E value;
        Element* nextElement;
        
        Element(E value) {
            this->value=value;
            this->nextElement = nullptr;
        }
    };

    Element** table;
    
    Hashtable() {
        tsize = N;
        numberElem = 0;
        maxFactor = 2;
        table = new Element*[tsize];
        for(size_t i=0; i<tsize; ++i) {
            table[i] = nullptr;
        }
    }
    ~Hashtable() {
        for(size_t i = 0; i < tsize; i++) {
            while(table[i]!=nullptr) {
                Element* ptr = table[i]->nextElement;
                delete table[i];
                table[i] = ptr;
            }
        
        }
        delete[] table;
        
    }
    Hashtable(std::initializer_list<E> list) : Hashtable() {for(auto e: list) add_(e);}

    using Container<E>::add;
    virtual void add(const E e[], size_t len) override;
    using Container<E>::remove;
    virtual void remove(const E e[], size_t len) override;
   
    virtual bool member(const E& e) const override;
    virtual size_t size() const override {return numberElem;}
    
    virtual std::ostream& print(std::ostream& o) const override;
    
    virtual size_t apply(std::function<void(const E&)> f, Order order = dontcare) const override;

    virtual E min() const override;
    virtual E max() const override;
    E ldeleted;
    bool deleted = false;
    E untergr;
    E obergr;
    size_t counter = 0;
};

template<typename E, size_t N>
void Hashtable<E,N>::add_(const E& e) {
    size_t position = hash(e);
    if(table[position] == nullptr) {
        table[position] = new Element(e);
        numberElem++;
        untergr = obergr;
        obergr = e;
        counter++;
        
    }
    else {
        Element* newElem;
        newElem = new Element(e);
        newElem->nextElement = table[position];
        table[position] = newElem;
        numberElem++;
        untergr = obergr;
        obergr = e;
        counter++;

    }
}

template<typename E, size_t N>
void Hashtable<E,N>::add(const E e[], size_t len) {
    if(numberElem + len > tsize * maxFactor){
        size_t newtsize = tsize;
        while(numberElem+len > newtsize * maxFactor){
            newtsize *= 2;
        }
        resize(newtsize);
    }
    for(size_t i=0; i<len; ++i) {
        if(!member(e[i])){ //return;
            add_(e[i]);
        }
        continue;
    }
}


template<typename E, size_t N>
bool Hashtable<E,N>::member(const E& e) const {
    size_t position = hash(e);
    Element* pointer = table[position];
    while (pointer != nullptr) {
        if(pointer ->value == e) {
            return true;
        }
        pointer = pointer->nextElement;
    }
    return false;
}
template<typename E, size_t N>
void Hashtable<E,N>::resize(size_t newSize) {
    Element** oldtable = table;
    size_t oldtsize = tsize;
    table = new Element*[newSize];
    for(size_t i=0; i<newSize; ++i) {
        table[i] = nullptr;
    }
    tsize = newSize;
    numberElem = 0;
    Element* helptr = nullptr;
    for (size_t i=0; i<oldtsize; ++i) {
        helptr= oldtable[i];
        while(helptr!=nullptr) {
            add_(helptr->value);
            helptr=helptr->nextElement;
        }
    }
    for(size_t i = 0; i< oldtsize; i++) {
        helptr = oldtable[i];
        while (oldtable[i]) {
            helptr = oldtable[i]->nextElement;
            delete oldtable[i];
            oldtable[i] = helptr;
            
        }
    }
    delete[] oldtable;
    helptr = nullptr;
}

template<typename E, size_t N>
std::ostream& Hashtable<E,N>::print(std::ostream& o) const {
    //o << tsize << endl;;
    //o << N;
    Element* pointer;
    if(tsize == 0) throw HashtableEmptyException();
    
    else {
        
        for (size_t i=0; i<tsize; i++) {
            pointer=table[i];
            while (pointer!= nullptr) {
                
                o << "[" << i << "] ";
                o << pointer ->value << " --> ";
                pointer = pointer ->nextElement;
            }
            o << "\n";
        }
        o << "This is w() test: " << w() << endl;
        size_t anzgrenze = y();
        o << "This is y() test: " << anzgrenze << endl;
        //o << "The highestlow: " << highestlow << endl;
        o << "The last deleted: " << ldeleted;
    }
    return o;
}

template<typename E,size_t N>
void Hashtable<E,N>::remove_(const E& e){
    size_t position = hash(e);
    Element* removeptr = table[position];
    Element* afterptr = nullptr;
    if(table[position]->value == e) {
        ldeleted = table[position]->value;
        table[position] = removeptr->nextElement;
        delete removeptr;
        numberElem--;
    } else {
        while(removeptr && !(removeptr->value == e)) {
                afterptr = removeptr;
                removeptr = removeptr->nextElement;
        }
        if(afterptr) {
            ldeleted = afterptr->value;
            afterptr->nextElement = removeptr->nextElement;
            delete removeptr;
            numberElem--;
            
        }
    }
    deleted = true;
    
    
}

template <typename E, size_t N>
void Hashtable<E,N>::remove(const E e[], size_t len) {
    if(numberElem==0) return;
    else {
        for(size_t i=0; i<len; i++) {
            if(member(e[i]))
                remove_(e[i]);
            else continue;
        }
    }
        
    
}

template <typename E, size_t N>
E Hashtable<E,N>::min() const {
    if (numberElem == 0) throw HashtableEmptyException();
    else {
        Element* valuep = nullptr;
        for (size_t i=0; i<tsize; i++) {
            if(table[i]!=nullptr){
                Element* ep = table[i];
                if((ep->nextElement==nullptr) && (valuep==nullptr || valuep->value > table[i]->value))
                    valuep = ep;
                else {
                    while(ep!=nullptr) {
                        if(valuep == nullptr || (valuep->value > ep->value)) {
                            valuep = ep;
                        }
                        ep = ep->nextElement;
                    }
                }
            }
        }
        //delete valuep; //changed
       // delete ep;
        return valuep->value;
    }
}

template <typename E, size_t N>
E Hashtable<E,N>::max() const {
    if (numberElem==0) throw HashtableEmptyException();
    else {
        Element* valuep = nullptr;
        for(size_t i=0; i<tsize; i++) {
            if(table[i]!=nullptr) {
                Element* ep = table[i];
                if((ep->nextElement==nullptr) && (valuep==nullptr ||  table[i]->value > valuep->value))
                    valuep = ep;
                else {
                    while(ep!=nullptr) {
                        if(valuep == nullptr || ep->value > valuep->value) {
                            valuep = ep;
                        }
                        ep=ep->nextElement;
                    }
                }
            }
        }
        return valuep->value;
    }
}

template <typename E, size_t N>
size_t Hashtable<E,N>::apply(std::function<void(const E&)> f , Order order) const {
    size_t count=0;
    if(order!=dontcare) {
        E* values = new E[numberElem];
        try {
            size_t position = 0;
            for(size_t i=0; i<tsize; i++) { // all values in the datastructure will be copyed into the values array//
                Element* pointer = table[i];
                while(pointer!=nullptr) {
                    values[position] = pointer->value;
                    position++;
                    pointer=pointer->nextElement;
                }
            }
            quicksort(values,0,numberElem-1);
            if(order == descending) {
                for (size_t i=numberElem; i-- > 0;) {
                    f(values[i]);
                    count++;
                }
            }
            else {
                for(size_t i=0; i<numberElem; ++i) {
                    f(values[i]);
                    count++;
                }
            }
        } catch (...) {}
        delete[] values;
    }
    else {
        Element* pointer = nullptr;
        try {
            for (size_t i =0; i<tsize; i++) {
                if(table[i]!=nullptr) {
                    pointer = table[i];
                    while (pointer != nullptr) {
                        f(pointer->value);
                        count++;
                        pointer=pointer->nextElement;
                    }
                }
            }

        } catch (...) {}
    }
    return count;
}
template <typename E, size_t N>
void Hashtable<E,N>::quicksort(E* values,int l, int r) const {
    if(numberElem == 0) throw HashtableEmptyException();
    else {
        int i,j;
        E pivot;
        if(r>l) {
            pivot = values[r]; i = l-1; j = r;
            for(;;) {
                while(pivot > values[++i] );
                while(values[--j] > pivot) if(j == i) break;
                if(i>=j) break;
                swap(values[i], values[j]);
                
            }
            swap(values[i], values[r]);
            quicksort(values, l, i-1);
            quicksort(values, i+1, r);
        }
    }
}
/*---------------------------------------------------------
 Adhoc Beispiel
 ---------------------------------------------------------*/
template<typename E, size_t N>
E Hashtable<E,N>::w() const {
    if(!deleted) throw HashtableEmptyException();
    E highestlow = min();
    if(highestlow > ldeleted) throw HashtableEmptyException();
    else {
        Element* help = nullptr;
        for(size_t i = 0; i<tsize; i++) {
            help = table[i];
            while(help!=nullptr) {
                if(ldeleted > help->value) {
                    if(help->value > highestlow)
                        highestlow = help->value;
                    help = help->nextElement;
                }
                else {
                    help = help->nextElement;
                }
                
            }
        }
    }
    return highestlow;
    
}
/*---------------------------------------------------------
 Adhoc Beispiel
 ---------------------------------------------------------*/
template<typename E, size_t N>
size_t Hashtable<E,N>::y() const {
    size_t sum = 0;
    if (2 > counter) throw HashtableEmptyException();
    else {
        if(untergr > obergr) {
            E helper = untergr;
            untergr = obergr;
            obergr = helper;
        }
        for(size_t i = 0; i<tsize; i++) {
            Element* help = table[i];
            while(help!=nullptr) {
                if(!(untergr > help->value) && !(help->value > obergr)) {
                    sum++;
                    help = help->nextElement;
                }
                else help = help->nextElement;
            }
        }
    }
    return sum;
    
}

#endif /* Hashtable_h */
