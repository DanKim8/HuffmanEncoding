//
//  main.cpp
//  HuffMan
//
//  Created by Daniel Kim on 10/27/19.
//  Copyright © 2019 Daniel Kim. All rights reserved.
//

#include <iostream>
#include <memory>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

class node
{
public:
    node* left;
    node* right;
    char character;
    int frequency;
    node(char character, int frequency) : character(character), frequency(frequency), left(nullptr), right(nullptr) {}
    node(int frequency, node* left, node* right) : character(0), frequency(frequency), left(left), right(right) {}
};

//node combine_leafs(pair<char, int> n1, pair<char, int> n2){}

//The third parameter for priority_queue is a compare class
class comparesecond
{
public:
    bool operator() (pair<char, int> n1, pair<char, int> n2)
    {
        return n1.second > n2.second;
    }
    bool operator() (node* n1, node* n2)
    {
        return n1->frequency > n2 -> frequency;
    }
};

//Given a map and a finished huffman tree, find the binary of each leaf node
//Make sure string here isn't a reference. Doing so, it will save the string even when recalled in recursion, it will just keep appending top existing binary
void encode(unordered_map<char, string>& unorderedmap, node* root, string binary)
{
    if(root == nullptr)
    {
        return;
    }
    if(root -> left == nullptr && root -> right == nullptr)
    {
        unorderedmap[root->character] = binary;
    }

    //DO NOT USE APPEND
    //We are adding extra things if we do
    encode(unorderedmap, root -> left, binary + "0");
    encode(unorderedmap, root -> right, binary + "1");
}

//
void decode(unordered_map<string, char>& unorderedmap, node* root, const string& encoded)
{
    if (root == nullptr)
    {
        return;
    }
    else if(root -> left == nullptr && root -> right == nullptr)
    {
        return;
    }
    else if(encoded == "1")
    {
        decode(root -> right, encoded);
    }
    else
    {
        decode(root -> left, encoded);
    }
}

void build_huffman(string text, double size)
{
    bool characters[148] = {false};
    int frequency[148] = {0};
    for(int i = 0; i < size; i++)
    {
        if(characters[text[i]] == false)
        {
            characters[text[i]] = true;
        }
        frequency[text[i]]++;
    }

    //Min heap based on the comparison of the second pair
    //priority_queue<pair<char, int>, vector<pair<char, int>>, comparesecond > queue;
    priority_queue<node*, vector<node*>, comparesecond > queue;
    for(int i = 0; i < 148; i++)
    {
        if(characters[i] == true)
        {
            //i is the index of the ascii
//            queue.push(make_pair(char(i), frequency[i]));
            cout <<char(i) << " " << frequency[i] << endl;
            queue.push(new node(char(i), frequency[i]));
        }
    }
    while(queue.size() > 1)
    {
        //Left has to be first as the smaller is the left leaf
        node* left = queue.top();
        queue.pop(); //Removes the top element. In this case, the lowest frequency. Doesn't return anything.
        node* right = queue.top();
        queue.pop();

        /* IF TWO FREQUENCY ARE THE SAME, WE DO NOT KNOW THE ORDER (LEFT OR RIGHT)*/
        /* WE SHOULD MAKE THE LEAST FREQUENT TO THE RIGHT*/

        /*
         A smart algorithm is to then push the sum of the freuency into the priority queue
         The summation of frequency will then sort itself out in the min heap
         The frequency nodes will be connected if they are also one of the lowest frequency
         */
        int sum = left -> frequency + right -> frequency;
        queue.push(new node(sum, left, right));//Since left is the smaller one, it is fine
    }
    //Top now contains a node with the total frequency
    node* root = queue.top();
    //Now transform the string in the tree to the shortened binary
    unordered_map <char, string> unorderedmap;
    //Traverse the tree and store the respective binary into the new tree
    string binary;
    encode(unorderedmap, root, binary);

    unordered_map<char, string>:: iterator itr;
    cout << "\nAll Elements : \n";
    for (itr = unorderedmap.begin(); itr != unorderedmap.end(); itr++)
    {
        // itr works as a pointer to pair<string, double>
        // type itr->first stores the key part  and
        // itr->second stroes the value part
        cout << itr->first << "  " << itr->second << endl;
     }
    string result;
    for(int i = 0; i < size; i++)
    {
        result.append(unorderedmap[text[i]]);
    }
    //cout<< "encoded is " << result << endl;

    string result2;
}

int main()
{
    string str = "abbcccdddd";
    build_huffman(str, str.length());

    return 1;
}

/*
 Algorithm
 Encoding
 1. Garner the characters and their respective frequency done
 2. Create a min heap of frequency done
 3. While there is more than 1 node, combine the 2 nodes of the least frequency
 4. Internal nodes are the sum of the frequency and leaves are the characters
 Decoding
 1.
 */
