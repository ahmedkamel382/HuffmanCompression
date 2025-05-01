// Ahmed Khalid Mohamed 231005397
// Ahmed Hisham Mohsen 231014862
// Gannah Walid Farouk 221005859
// Muhammed Moatasem Mahmoud 221005418

#include <iostream>
#include <fstream>
#include <queue>

using namespace std;


struct Node {
    char data;
    int frequency;
    Node* next;
};

class PriorityQueue {
public:

    Node* header = NULL;
    // Function to enqueue a node with specified data and frequency
    bool enqueue(char data, int frequency) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL)
            return false;

        newNode->data = data;
        newNode->frequency = frequency;
        newNode->next = NULL;

        // If the list is empty or the frequency of the new node is less than or equal to the frequency of the first node
        if (header == NULL || frequency <= header->frequency) {
            // Insert the new node at the beginning of the list
            newNode->next = header;
            header = newNode;
        }
        // If the frequency of the new node is greater than the frequency of the first node
        else {
            // Traverse list to find the correct position to insert the new node based on its frequency
            Node* current = header;
            while (current->next != NULL && current->next->frequency < frequency)
                current = current->next;
            // Insert the new node after the current node
            newNode->next = current->next;
            current->next = newNode;
        }

        return true;
    }

    // Function to dequeue a node from the beginning of the list
    bool dequeue(char* data, int* frequency) {
        if (header == NULL)
            return false;

        // Retrieve the data and frequency of the first node
        *data = header->data;
        *frequency = header->frequency;

        // Remove the first node from the list and free its memory
        Node* temp = header;
        header = header->next;
        free(temp);

        return true;
    }
};

// Function to calculate the frequency of characters in the specified file and enqueue them into the priority queue
void calculateFrequency(const string& filename, PriorityQueue& pq) {

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    char c;
    int frequency[256] = { 0 };

    // Read characters from file
    while (file.get(c)) {
        frequency[char(c)]++; // Increment the frequency of the character
    }

    file.close();

    // Enqueue characters into the priority queue
    for (int i = 0; i < 256; i++) {
        if (frequency[i] != 0) {
            pq.enqueue(char(i), frequency[i]);
        }
    }
}
// Node structure for Huffman tree
struct TreeNode {
    char data;
    int frequency;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char d, int freq, TreeNode* lft = NULL, TreeNode* rgt = NULL) : data(d), frequency(freq), left(lft), right(rgt) {} //Constructor
};


class huffmanTree {
    TreeNode* root = NULL;
    bool isSingleNodeTree = false;

    /* // FOR DEBUGGING //

    // Function to print Huffman tree recursively
    void printTreeRecursive(TreeNode* node, string prefix = "", bool isLeft = false) {
        if (node != NULL) {
            cout << prefix;
            cout << (isLeft ? "|--" : "|--");
            cout << node->data << "(" << node->frequency << ")" << endl;

            // Recursive call for left child with prefix indicating it's a left child
            printTreeRecursive(node->left, prefix + (isLeft ? "|   " : "    "), true);
            // Recursive call for right child with prefix indicating it's a right child
            printTreeRecursive(node->right, prefix + (isLeft ? "    " : "|   "), false);
        }
    }*/

public:
    string huffmanCodes[256]; // Array to store Huffman codes for characters


    // Function to generate Huffman codes
    void generateCodes(TreeNode* node, string code) {

        if (node->left == NULL && node->right == NULL) {
            huffmanCodes[node->data] = code.empty() ? "1" : code;
            return;
        }
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

    // Comparison for priority queue
    struct CompareNodes {
        bool operator()(TreeNode* left, TreeNode* right) {
            return left->frequency > right->frequency;
        }
    };


    // Function to build the Huffman tree
    void buildTree(int frequencies[256]) {
        priority_queue<TreeNode*, vector<TreeNode*>, CompareNodes> pq;

        // Create leaf nodes and add them to the priority queue
        for (int i = 0; i < 256; i++) {
            if (frequencies[i] > 0) {
                pq.push(new TreeNode(char(i), frequencies[i]));
            }
        }

        if (pq.size() == 1) {
            isSingleNodeTree = true;
        }
        else {
            isSingleNodeTree = false;
        }
        // Build the Huffman tree
        while (pq.size() > 1) {
            TreeNode* left = pq.top();
            pq.pop();
            TreeNode* right = pq.top();
            pq.pop();

            if (left->frequency > right->frequency) {
                swap(left, right);
            }

            TreeNode* parent = new TreeNode('\0', left->frequency + right->frequency, left, right);
            pq.push(parent);
        }

        // Set the root of the Huffman tree
        root = pq.top();
        pq.pop();

        // Generate Huffman codes
        generateCodes(root, "");
    }


    // Function to print Huffman codes
    void printHuffmanCodes() {
        cout << "Huffman Codes:" << endl;
        for (int i = 0; i < 256; i++) {
            if (!huffmanCodes[i].empty()) {
                cout << char(i) << ": " << huffmanCodes[i] << endl;
            }
        }
    }

    // Function to compress input file and generate compressed file
    void compressFile(string& inputFile, string& compressedFile) {
        ofstream outFile(compressedFile);
        if (!outFile.is_open()) {
            cout << "Error opening output file: " << compressedFile << endl;
            return;
        }

        ifstream inFile(inputFile);
        if (!inFile.is_open()) {
            cout << "Error opening input file: " << inputFile << endl;
            outFile.close();
            return;
        }

        char c;
        string buffer;
        string fullHuffmanCode;  // String to display concatenated Huffman codes
        int charCount = 0;
        while (inFile.get(c)) {
            // Concatenate the Huffman code for the current character to the buffer
            buffer += huffmanCodes[char(c)];
            /*
             // FOR DEBUGGING //

            fullHuffmanCode += huffmanCodes[char(c)]; // Display the concatenated Huffman Codes
            */
            charCount++;
            while (buffer.length() >= 8) {
                // Extract the first 8 bits from the buffer
                string eightBits = buffer.substr(0, 8);

                // Convert binary string to integer
                char asciiChar = 0;
                for (int i = 0; i < 8; i++) {
                    asciiChar = (asciiChar << 1) | (eightBits[i] - '0');
                }

                // Write the ASCII character to the output file
                outFile.put(asciiChar);

                // Remove the processed bits from the buffer
                buffer.erase(0, 8);
            }
        }

        // Write remaining bits
        if (!buffer.empty()) {
            // Add zeros to make up a full byte
            while (buffer.length() < 8) {
                buffer += '0';
            }

            // Convert binary string to integer
            char asciiChar = 0;
            for (int i = 0; i < 8; i++) {
                asciiChar = (asciiChar << 1) | (buffer[i] - '0');
            }

            // Write the ASCII character to the output file
            outFile.put(asciiChar);
        }
        // Write the character count at the end of the file to help with decompression (in case of one node)
        outFile.write(reinterpret_cast<const char*>(&charCount), sizeof(charCount));
        cout << "File compressed successfully." << endl;

        /*
        // FOR DEBUGGING //
        // Print the concatenated Huffman codes
        cout << "Concatenated Huffman Codes: " << fullHuffmanCode << endl;*/

        inFile.close();
        outFile.close();
    }
    /*
              // FOR DEBUGGING //

             // Function to print the Huffman tree structure
            void printTree() {
                printTreeRecursive(root);
            }*/

            // Function to decompress file and generate decompressed file
    void decompressFile(string& compressedFile, string& outputFile) {
        ifstream inFile(compressedFile, ios::binary);
        if (!inFile.is_open()) {
            cout << "Error opening compressed file: " << compressedFile << endl;
            return;
        }

        ofstream outFile(outputFile);
        if (!outFile.is_open()) {
            cout << "Error opening output file: " << outputFile << endl;
            inFile.close();
            return;
        }

        // Read the file until the end to get the character count (in case of one node)
        inFile.seekg(-static_cast<int>(sizeof(int)), ios::end);
        int charCount;
        inFile.read(reinterpret_cast<char*>(&charCount), sizeof(charCount));
        inFile.seekg(0, ios::beg);

        // Special case for single character file. Write the character repeatedly to the output file
        if (isSingleNodeTree) {
            char data = root->data;
            for (int i = 0; i < charCount; i++) {
                outFile.put(data);
            }
        }
        else {
            // Traverse Huffman tree based on the bits read from the compressed file
            TreeNode* currentNode = root;
            int count = 0;
            char bit;
            while (inFile.get(bit) && count < charCount) {
                for (int i = 7; i >= 0 && count < charCount; i--) {
                    if (currentNode == NULL) {
                        inFile.close();
                        outFile.close();
                        return;
                    }
                    if (bit & (1 << i)) {
                        currentNode = currentNode->right;
                    }
                    else {
                        currentNode = currentNode->left;
                    }
                    // If a leaf node is reached, write the character to the output file
                    if (currentNode != NULL && currentNode->left == NULL && currentNode->right == NULL) {
                        outFile.put(currentNode->data);
                        // Reset the current node to the root for the next character
                        currentNode = root;
                        count++;
                    }
                }
            }
        }

        cout << "File decompressed successfully." << endl;
        inFile.close();
        outFile.close();
    }
};


int main() {
    string inputFile;
    string compressedFile = "compressed.txt";
    string decompressedFile = "decompressed.txt";

    cout << "Enter file name (Make sure to include the proper file extension): " << endl;
    cin >> inputFile;
    cout << endl;
    ifstream fileCheck(inputFile);
    while (!fileCheck.is_open()) {
        cout << "File not found. Please enter a valid file name: ";
        cin >> inputFile;
        cout << endl;
        fileCheck.open(inputFile);
    }

    PriorityQueue pq;
    calculateFrequency(inputFile, pq);

    int frequencies[256] = { 0 };

    char data;
    int freq;
    while (pq.header != NULL) {
        pq.dequeue(&data, &freq);
        frequencies[data] = freq;
        cout << "Data: " << data << ", ASCII Value: " << int(data) << " , Frequency: " << freq << endl;
    }

    // Build Huffman tree
    huffmanTree hTree;
    hTree.buildTree(frequencies);

    // FOR DEBUGGING //
  // hTree.printTree();

    hTree.printHuffmanCodes();

    hTree.compressFile(inputFile, compressedFile);
    hTree.decompressFile(compressedFile, decompressedFile);
    return 0;
}