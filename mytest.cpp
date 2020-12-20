#include <iostream>

#include "AVLTree.h"

using namespace std;

int8_t compare(int a, int b) {
    return a - b;
}

int main() {
    cout << "Hello World" << endl;
    AVLTree<int> tree(compare);

    tree.clear();
    tree.insert(8);
    tree.insert(9);

    if (tree.contains(8))
        cout << 8 << endl;

    if (tree.contains(9))
        cout << 9 << endl;

}

/*
int main(){
    // wordTreeTest first
    // Create and print tree.
    SplayTree<string> tree = SplayTree<string>();

    cout << "------------" << endl;
    tree.insert("b");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("c");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("d");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("a");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("a");
    tree.prettyDump();

    // Now add a bunch
    for (string c = "a"; c[0] <= 'z' ; c[0]++) {
        tree.insert(c);
        tree.prettyDump();
        cout << "-------------------------------------" << endl;
    }

    // bookTreeTest second
    BookTree aTree;
    aTree.loadData("Entire_king_James_bible_data.txt");
    string title = "king james bible";
    cout << "tinsmith: " << aTree.findFrequency(title,"tinsmith") << endl;
    cout << "of      : " << aTree.findFrequency(title,"of") << endl;
    cout << "king    : " << aTree.findFrequency(title,"king") << endl;
    cout << "james   : " << aTree.findFrequency(title,"james") << endl;
    cout << "bible   : " << aTree.findFrequency(title,"bible") << endl;
    cout << "true    : " << aTree.findFrequency(title,"true") << endl;
    cout << "false   : " << aTree.findFrequency(title,"false") << endl;
    cout << "heaven  : " << aTree.findFrequency(title,"heaven") << endl;
    cout << "hell    : " << aTree.findFrequency(title,"hell") << endl;
    cout << "death   : " << aTree.findFrequency(title,"death") << endl;
    cout << "life    : " << aTree.findFrequency(title,"life") << endl;
    cout << "yes     : " << aTree.findFrequency(title,"yes") << endl;
    cout << "no      : " << aTree.findFrequency(title,"no") << endl;
    cout << aTree.getTextTreeHeight(title);
    cout << endl << endl;
    //aTree.dumpTitle("king james bible");
    aTree.dump(true);
    cout << endl;

    // SplayTree Test
    BookTree bTree;
    bTree.loadData("data.txt");
    cout << "Lookup 'the wonderful wizard of oz': " << bTree.getTextTreeHeight("the wonderful wizard of oz") << endl;
    cout << "Lookup 'the wonderful wizard of oz': " << bTree.getTextTreeHeight("the wonderful wizard of oz") << endl;
    cout << "Lookup 'a tale of two cities'      : " << bTree.getTextTreeHeight("a tale of two cities") << endl;
    cout << "Lookup 'the wonderful wizard of oz': " << bTree.getTextTreeHeight("the wonderful wizard of oz") << endl;
    cout << endl;

    return 0;
}*/