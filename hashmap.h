/*hashmap.h*/

//
// Nishant Chudasama
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06 Part 1: Hashmap
// Date: 04/10/2020 
//
// References: original non-collision code written by Prof. Hummel (UIC)
// 
// hashmap implements a hash table that deals with collisions using linear probing; 
// The class provides the underlying hash table along with insert() and search() functions;
// the user of this class must provide a Hash(key, N) function that 
// hashes the given key and returns an integer in the range 0..N-1, 
// inclusive, if the key is valid.  If the key is invalid, the Hash() 
// function must return a negative value.
//
// The hash function is provided in the calls to insert and search.
// Pass the name of your hash function as the 3rd parameter.  Example:
//
//   int myHash(string key, int N)
//   { ... }
//
//   int N = 10000;
//   hashmap<string, double>  hmap(N);  
//
//   if (hmap.insert("deep-dish pizza", 19.99, myHash))
//     cout << "inserted" << endl;
//   else
//     cout << "not inserted?!" << endl;
// 

#pragma once

#include <iostream>
#include <functional>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
    struct HashEntry
    {
        bool   ContainsData;  // false => empty, true => contains data
        KeyT   Key;           // (key, value)
        ValueT Value;

        HashEntry()
        {
            ContainsData = false;
        }
    };

    HashEntry* HT;  // hash table array of structs
    int        N;   // capacity of hash table (# of locations or array size)

    // Helper function to update values for a given index in the hash table
    void _insert(HashEntry& bucket, KeyT key, ValueT value) {
        bucket.Key = key;
        bucket.Value = value;
        bucket.ContainsData = true;  // array location now contains data
    }

public:
    //
    // constructor: N is the size of the underlying hash table
    //
    hashmap(int N)
    {
        this->N = N;
        this->HT = new HashEntry[N];
    }

    //
    // copy constructor
    //
    hashmap(const hashmap& other)
    {
        this->N = other.N;
        this->HT = new HashEntry[N];

        // Copy over values
        for (int i = 0; i < N; ++i)
        {
            this->HT[i].ContainsData = other.HT[i].ContainsData;
            this->HT[i].Key = other.HT[i].Key;
            this->HT[i].Value = other.HT[i].Value;
        }
    }

    //
    // destructor
    //
    virtual ~hashmap()
    {
        delete[] this->HT;
    }

    //
    // getN
    //
    // Returns the capacity of the hash table, i.e. the value N that
    // was passed to the constructor.
    //
    int getN() const
    {
        return this->N;
    }

    //
    // insert
    //
    // Inserts the given (key, value) pair into the hash table, overwriting
    // the previous value if already inserted.  If the insert was successful
    // then true is returned, otherwise false is returned (meaning the key
    // was not hashed successfully, e.g. due to improper formatting).
    //
    // NOTE: the caller must provide a function Hash(key, N) that returns
    // an integer in the the range 0..N-1, inclusive, if the key is valid.
    // If the key is invalid, the hash function must return a negative
    // integer. The hash function is provided by simply calling insert
    // with the name of your hash function.  Example:
    //
    //   int myHash(string key, int N)
    //   { ... }
    //
    //   int N = 10000;
    //   hashmap<string, double>  hmap(N);  
    //
    //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
    //     cout << "inserted" << endl;
    //   else
    //     cout << "not inserted?!" << endl;
    //
    bool insert(KeyT key, ValueT value, function<int(KeyT, int)> Hash)
    {
        //
        // Call the provided hash function with the key, and N, the size
        // of our hash table:
        //
        int index = Hash(key, N);

        //
        // insert:
        //
        if (index < 0 || index >= N)  // invalid key or hash function:
        {
            return false;
        }
        else  // valid array index, store into hash table:
        {
            // Insert into correct location	
            if (!HT[index].ContainsData) {
                _insert(HT[index], key, value);
                return true;
            }

            // Keep probing until empty location found:
            int bucketsProbed = 0;
            while ((HT[index].ContainsData) && (bucketsProbed < N)) {
                // If key exists, overwrite values:
                if (HT[index].Key == key) {
                    _insert(HT[index], key, value); // Insert data into the HashTable
                    return true;
                }

                // Advance to next bucket
                index = (index + 1) % N;
                bucketsProbed++;
            }

            // Empty location found:
            if (bucketsProbed < N) {
                _insert(HT[index], key, value);   // Insert data into the HashTable
                return true;
            }

            // If get here, no location was empty	  
            return false;
        }

    }

    //
    // search
    //
    // Searches the hash table for the matching key, and if found then
    // (1) the value is returned via the reference parameter and (2) true
    // is returned.  If the key could not be found, or if the hash 
    // function returned an invalid value (e.g. due to an improperly 
    // formatted key), then false is returned.
    //
    // NOTE: the caller must provide a function Hash(key, N) that returns
    // an integer in the the range 0..N-1, inclusive, if the key is valid.
    // If the key is invalid, the hash function must return a negative
    // integer. The hash function is provided by simply calling search
    // with the name of your hash function.  Example:
    //
    //   int myHash(string key, int N)
    //   { ... }
    //
    //   int N = 10000;
    //   hashmap<string, double>  hmap(N);
    //   ...
    //   double price;
    //   if (hmap.search("deep-dish pizza", price, myHash))
    //     cout << "deep-dish piazza costs " << price << endl;
    //   else
    //     cout << "not found?!" << endl;
    //
    bool search(KeyT key, ValueT& value, function<int(KeyT, int)> Hash) const
    {
        //
        // Call the provided hash function with the key, and N, the size
        // of our hash table:
        //
        int index = Hash(key, N);

        //
        // search:
        //
        if (index < 0 || index >= N)  // invalid key or hash function:
        {
            return false;
        }

        // Key found at correct location
        if (HT[index].ContainsData && HT[index].Key == key) {
            value = HT[index].Value;
            return true;
        }

        // Keep probing till value found:
        int bucketsProbed = 0;

        while ((HT[index].ContainsData) && (bucketsProbed < N+1))
        {
            // Wrap around if at end of array
            index = (index + 1) % N;
            bucketsProbed++;

            // Value found
            if (HT[index].Key == key) {
                value = HT[index].Value;
                return true;
            }
        }

        // If get here, value not found
        return false;
    }

    //
    // operator "="
    // Clears "this" hashmap and makes a copy of the "other" hashmap
    //
    hashmap& operator= (const hashmap& other)
    {
        delete[] this->HT;

        this->N = other.N;
        this->HT = new HashEntry[N];

        for (int i = 0; i < N; ++i)
        {
            this->HT[i].ContainsData = other.HT[i].ContainsData;
            this->HT[i].Key = other.HT[i].Key;
            this->HT[i].Value = other.HT[i].Value;
        }
    }

    vector<KeyT> getKeys()
    {
        vector<KeyT> keys;

        for (int i = 0; i < N; ++i) {
            if (HT[i].ContainsData) {
                keys.push_back(HT[i].Key);
            }
        }

        return keys;
    }

};
