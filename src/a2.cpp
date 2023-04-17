// Assignment 2
// Name: Sam Zhang
// Student ID: 261072449
// Class: COMP 322
// Due Date: 2023-03-10

#include <iostream>
#include <fstream>
#include <sstream>

int hashIt(std::string content);

// Node class: Each node contains a version number, a hash value for comparison, 
// and the content of the file at that version stored as a string.
class Node
{
    private:
        std::string content;
        int version;
        int hashValue;

    public:
        Node* next;
        Node* prev;

        Node(std::string content, int version)
        {
            this->content = content;
            this->version = version;
            this->next = nullptr;
            this->prev = nullptr;
            this->hashValue = hashIt(content);
        }

        std::string getContent()
        {
            return this->content;
        }

        int getVersion()
        {
            return this->version;
        }

        int getHashValue()
        {
            return this->hashValue;
        }

        void printVersion()
        {
            std::cout << "Version number: " << this->version << std::endl;
            std::cout << "Hash Value: " << this->hashValue << std::endl;
            std::cout << "Content: " << this->content << std::endl;
        }
};

// A linked list data structure that takes in version nodes.
// Each node contains the information of the file at that version.
// The linked list is instantiated with the filepath of the file being tracked.
class LinkedList
{
    private:
        int numberOfVersions;   
        std::string path;

    public:
        Node* head;
        Node* tail;
        LinkedList(std::string filePath)
        {
            this->path = filePath;
            this->numberOfVersions = 0;
            this->head = nullptr;
            this->tail = nullptr;
        }

        std::string getFilePath()
        {
            return this->path;
        }

        // Function to save a version. If the current version is the same as the tail version (compared by hashcode), then refuse to save.
        void add(std::string content)
        {
            int newHashValue = hashIt(content);
            if (this->tail != nullptr && newHashValue == this->tail->getHashValue())
            {
                std::cout << "git322 did not detect any change to your file and will not create a new version." << std::endl;
                return;
            }

            Node* curr = head;
            while (curr != nullptr)
            {
                if (curr->getHashValue() == newHashValue)
                {
                    std::cout << "This version of the file is already in the git log as version " << curr->getVersion() << "; git322 will not create a new version." << std::endl;
                    return;
                }
                curr = curr->next;
            }

            this->numberOfVersions++;
            Node* newNode = new Node(content, this->numberOfVersions);
            if (head == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
        }

        // Function to remove a version. If the version does not exist, print an error message.
        void remove(int version)
        {
            Node* curr = head;
            // Iteratively traverse the linked list, 
            // if the version number matches the input, delete the node.

            while (curr != nullptr)
            {
                if (curr -> getVersion() == version)
                {
                    if (curr == head && curr != tail)
                    {
                        head = curr->next;
                        head->prev = nullptr;
                    }

                    else if (curr != head && curr == tail)
                    {
                        tail = curr->prev;
                        tail->next = nullptr;
                    }

                    else if (curr == head && curr == tail)
                    {
                        head = nullptr;
                        tail = nullptr;
                    }

                    else
                    {
                        curr->prev->next = curr->next;
                    }

                    delete curr;
                    this->numberOfVersions--;
                    return;
                }
                curr = curr->next;
            }
            std::cout << "Please enter a valid version number." << std::endl;
        }

        // Function to load a version into the given file path. 
        // If the version does not exist, print an error message.
        void load(int version)
        {
            Node* curr = head;
            // Iteratively traverse the linked list, 
            // if the version number matches the input, write the content to the file into the path.

            while (curr != nullptr)
            {
                if (curr -> getVersion() == version)
                {
                    std::ofstream file;
                    file.open(this->path);
                    file << curr->getContent();
                    file.close();
                    return;
                }
                curr = curr->next;
            }
            std::cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << std::endl;
        }

        // Function to print all currently stored versions in the linked list. 
        void print()
        {
            Node* curr = head;
            std::cout << "Number of versions: " << this->numberOfVersions << std::endl;
            while (curr != nullptr)
            {
                curr->printVersion(); 
                curr = curr->next;
            }
        }

        // Function that takes in two versions and compare them line by line.
        // If one of the versions do not exist, or if both versions given are the same version,
        // print an error message.

        void compare(int version1, int version2)
        {
            // Input validation: If the two versions are the same, print an error message.
            if (version1 == version2)
            {
                std::cout << "Please enter 2 different version numbers." << std::endl;
                return;
            }

            Node* curr = head;
            Node* version1Node = nullptr;
            Node* version2Node = nullptr;

            // Input validation: If one of the two versions do not exist, print an error message.
            while (curr != nullptr)
            {
                if (curr -> getVersion() == version1) version1Node = curr;
                if (curr -> getVersion() == version2) version2Node = curr;
                curr = curr->next;
            }

            if (version1Node == nullptr || version2Node == nullptr)
            {
                std::cout << "Please enter 2 valid version numbers. If you are not sure please press 'p' to list all valid version numbers." << std::endl;
                return;
            }

            int lineNum = 1;
            std::string line1;
            std::string line2;

            // Convert the content of the two versions into a stringstream to extract contet line by line.
            std::stringstream version1Content(version1Node->getContent());
            std::stringstream version2Content(version2Node->getContent());

            // Read the two strings line by line. 
            // If the two lines are different, print the line number and the two lines.
            // Upon seeing an empty line or reaching the end of the file, 
            // print the approriate message (empty line or out of content).
            while (getline(version1Content,line1) && getline(version2Content,line2))
            {
                if (line1 != line2)
                {
                    if(line1 == "") line1 = "<Empty Line>";
                    if(line2 == "") line2 = "<Empty Line>";

                    std::cout << "Line " << lineNum << ": " << line1 << " <<>> " << line2 << std::endl;
                }
                else std::cout << "Line " << lineNum << ": " << "<Identical>"<< std::endl;
                lineNum++;
            }

            while(getline(version1Content,line1))
            {
                getline(version1Content,line1);
                if(line1 == "") line1 = "<Empty Line>";
                std::cout << "Line " << lineNum << ": " << line1 << " <<>> " << "<Out of Content>" << std::endl;
                lineNum++;
            }

            while(getline(version2Content,line2))
            {
                getline(version2Content,line1);
                if(line2 == "") line2 = "<Empty Line>";
                std::cout << "Line " << lineNum << ": " << "<Out of Content>" << " <<>> " << line2 << std::endl;
                lineNum++;
            }
        }

        void search(std::string keyword)
        {
            Node* curr = head;
            bool found = false;

            // Iteratively traverse the linked list and check the content of every node.
            // If the node contains the keyword, print the version information.
            while (curr != nullptr)
            {
                if (curr->getContent().find(keyword) != std::string::npos)
                {
                    if (!found)
                    {
                        std::cout << "The keyword '" << keyword << "' has been found in the following versions:" << std::endl;
                        found = true;
                    }
                    curr->printVersion();
                }
                curr = curr->next;
            }

            // If the keyword was not found in any version, print the approriate message.
            if (!found) std::cout << "Your keyword '" << keyword << "' was not found in any version." << std::endl;
        }

};

// Helper function to hash a string.
int hashIt(std::string content)
{
    return std::hash<std::string>()(content);
}

// Helper function to extract the content of a file as a string.
std::string getFileContent(std::ifstream &file)
{
    std::string content;
    std::stringstream ss;
    ss << file.rdbuf();
    content = ss.str();

    return content;
}

int main() 
{
    // Declare variables, including file path (currently hardcoded as "./file.txt" as per the assignment instructions)
    std::string path = "./file.txt";
    std::string MESSAGE = "To add the content of your file to version control press 'a'\nTo remove a version press 'r'\nTo load a version press 'l'\nTo print to the screen the detailed list of all versions press 'p'\nTo compare any 2 versions press 'c'\nTo search versions for a keyword press 's'\nTo exit press 'e'";
    std::string answer;

    // Instantiate the linked list and print the instruction message
    LinkedList versions = LinkedList(path);
    std::cout << MESSAGE << std::endl;

    for (;;)
    {
        std::getline(std::cin,answer);

        // Input validation
        if (answer == "") continue;

        if (answer.length() > 1)
        {
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        // Switch statement to handle the user input
        switch (answer[0])
        {
            case 'a':
            {
                std::string content;
                std::ifstream file;

                file.open(path);
                content = getFileContent(file);
                file.close();

                versions.add(content);
                break;
            }

            case 'r':
            {
                int versionNum;
                std::cout << "Enter the number of the version that you want to delete: " << std::endl;
                std::cin >> versionNum;
                versions.remove(versionNum);
                break;
            }

            case 'l':
            {
                int version;
                std::cout << "Which version would you like to load?" << std::endl;
                std::cin >> version;
                versions.load(version);
                break;
            }

            case 'p':
            {
                versions.print();
                break;
            }

            case 'c':
            {
                int version1;
                int version2;
                std::cout << "Please enter the number of the first version to compare: " << std::endl;
                std::cin >> version1;
                std::cout << "Please enter the number of the second version to compare: " << std::endl;
                std::cin >> version2;
                versions.compare(version1, version2);
                break;
            }

            case 's':
            {
                std::string keyword;
                std::cout << "Please enter the keyword that you are looking for: " << std::endl;
                std::cin >> keyword;
                versions.search(keyword);
                break;
            }

            case 'e':
            {
                exit(0);
                break;
            }

            default:
                std::cout << "Invalid input" << std::endl;
                break;
        }
    }
    return 0; 
}