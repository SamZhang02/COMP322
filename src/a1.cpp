// Name: Sam Zhang
// Student ID: 261072449
// COMP 322

#include <iostream>
#include <fstream>
#include <vector>

//Question 1
bool word_diff(std::string word1, std::string word2)
{
    return (word1 == word2);
}

//Question 2
bool classical_file_diff(std::string file1, std::string file2)
{
    std::string content1;
    std::string content2;
    std::ifstream fobj1(file1);
    std::ifstream fobj2(file2);

    std::string line;
    // get content from both files and store them in approriate variables
    while (std::getline(fobj1, line)) 
    {
        content1 += line + '\n';
    }
    while (std::getline(fobj2, line))
    {
        content2 += line + '\n';
    }
    // compare the two variables char by char (==)
    return word_diff(content1, content2);
}

//Question 3
std::size_t hash_it(std::string input)
{
   return std::hash<std::string>()(input);
}

//Question 4
bool enhanced_file_diff(std::string file1, std::string file2)
{
    std::string content1;
    std::string content2;
    std::ifstream fobj1(file1);
    std::ifstream fobj2(file2);

    std::string line;
    // get conent from both files and store them in approriate variables
    while (std::getline(fobj1, line)) 
    {
        content1 += line + '\n';
    }
    while (std::getline(fobj2, line))
    {
        content2 += line + '\n';
    }
    // compare the hash value of both variables
    return (hash_it(content1) == hash_it(content2));
    
}
//Question 5

// function to get the base name of a filename.
std::string getname(std::string file)
{
    std::string name;
    for (int i = file.length() - 1; i > -1; i--)
    {
        if (file[i] == '/') break;
        name = file[i] + name;
    }
    return name;
}

void mismatched_lines_helper(std::string file1_name, std::string file2_name, std::ifstream &fobj1, std::ifstream &fobj2)
{
    std::string file1_line; 
    std::string file2_line; 
    //base case: stop if both fiels are done reading 
    if (fobj1.eof() && fobj2.eof()) return ;

    //if one or more files still have lines left, keep reading
    else if (fobj1.eof())
    {
        std::getline(fobj2, file2_line);
        // if we get an empty line \n, the line comparator won't pick it up, since the getline() method strips the delimiter for me.
        // therefore added a special case here.
        std::cout << file1_name + ": " << "(Out of content.)" << std::endl;
        std::cout << file2_name + ": " << file2_line << std::endl;
    } 

    else if (fobj2.eof()) 
    {
        std::getline(fobj1, file1_line);
        // as above
        std::cout << file1_name + ": " << file1_line << std::endl;
        std::cout << file2_name + ": " << "(Out of content.)" << std::endl;
    }

    else
    {
        std::getline(fobj1, file1_line);
        std::getline(fobj2, file2_line);

        //compare the two lines via hashing, and display them if they are different
        if (!(hash_it(file1_line) == hash_it(file2_line)))
        {
            std::cout << file1_name + ": " << file1_line << std::endl;
            std::cout << file2_name + ": " << file2_line << std::endl;
        }
    }
    mismatched_lines_helper(file1_name, file2_name, fobj1, fobj2);
}

void list_mismatched_lines(std::string file1, std::string file2)
{
    std::ifstream fobj1(file1);
    std::ifstream fobj2(file2);
    std::string file1_name = getname(file1);
    std::string file2_name = getname(file2);

    mismatched_lines_helper(file1_name, file2_name, fobj1, fobj2);

    fobj1.close();
    fobj2.close();
}

//Question 6

// function to split a line into a vector of words
std::vector<std::string> get_words(std::string line)
{
    std::vector<std::string> words;
    if (line == "") return words;
    std::string word;
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ' ')
        {
            // To only split words by single spaces, remove the next line.
            if (word == "") continue;
            words.push_back(word);
            word = "";
        }
        else
        {
            word += line[i];
        }
    }
    if (word != "") words.push_back(word);
    return words;
}

// function that checks for word differences between two lines (given as vectors of words).
void check_words_diff(std::string file1_name, std::string file2_name, std::vector<std::string> &line1_words, std::vector<std::string> &line2_words, int line_num, int i)
{
    std::string word1; 
    std::string word2; 
    //base case: if all words have been checked from both lines, return
    if (i >= line1_words.size() && i >= line2_words.size()) return;

    // if one or more line have words left, recursively check for differences
    // if only one line left, recursively print out the rest while printing out (Out of content) for the empty line.
    if (i >= line1_words.size()) 
    {
        word2 = line2_words[i];
        std::cout << file1_name + ": (Out of content.) " << " (line " << line_num << ")" << std::endl;
        std::cout << file2_name + ": " << word2 << " (line " << line_num << ")" << std::endl;
    }
    else if (i >= line2_words.size()) 
    {
        word1 = line1_words[i];
        std::cout << file1_name + ": " << word1 << " (line " << line_num << ")" << std::endl;
        std::cout << file1_name + ": (Out of content.) " << " (line " << line_num << ")" << std::endl;
    }
    else
    {
        word1 = line1_words[i];
        word2 = line2_words[i];
        if (hash_it(word1) != hash_it(word2))
        {
            std::cout << file1_name + ": " << word1 << " (line " << line_num << ")" << std::endl;
            std::cout << file2_name + ": " << word2 << " (line " << line_num << ")" << std::endl;
        }
    }

    check_words_diff(file1_name, file2_name,line1_words, line2_words, line_num, i+1);
}

void mismatched_words_helper(std::string file1_name, std::string file2_name, std::ifstream &fobj1, std::ifstream &fobj2, int n)
{
    std::string file1_line;
    std::string file2_line;
    bool file1_empty = false;
    bool file2_empty = false;
    
    // start by getting line via data stream, to ensure always being on the same line in both files.

    //base case: both files are done reading
    if (fobj1.eof() && fobj2.eof())
    {
        return ;
    }

    //if one or more file still have words left, keep reading
    if (fobj1.eof())
    {
        file1_empty = true;
        file1_line = "";
        getline(fobj2, file2_line);
        // if we get an empty line \n, the word comparator won't pick it up, since the getline() method strips the delimiter for me.
        // therefore added a special case here.
        if (file2_line == "")
        {
            std::cout << file1_name + ": (Out of content.) " << "(line " << n << ")" << std::endl;
            std::cout << file2_name + ": " << file2_line << "(line " << n << ")" << std::endl;
        }
    }

    else if (fobj2.eof())
    {
        file2_empty = true;
        file1_line = "";
        file2_line = "";
        getline(fobj1,file1_line);
        // as above
        if (file1_line == "")
        {
            std::cout << file1_name + ": " << file1_line << "(line " << n << ")" << std::endl;
            std::cout << file2_name + ": (Out of content.) " << "(line " << n << ")" << std::endl;
        }
    }
    else
    {
        getline(fobj1,file1_line);
        getline(fobj2,file2_line);
    }


    // we only check for word by word differences for 3 cases for efficiency:
    // the hash value from line 1 is different from the hash from line 2
    // file1 is empty (the line is guaranteed to be different)
    // file2 is empty (the line is guaranteed to be different)
    if (file1_empty || file2_empty || hash_it(file1_line) != hash_it(file2_line))
    {
        // split the line into an array
        std::vector<std::string> line1_words = get_words(file1_line);
        std::vector<std::string> line2_words = get_words(file2_line);
        check_words_diff(file1_name, file2_name, line1_words, line2_words, n, 0);
    }
    mismatched_words_helper(file1_name, file2_name, fobj1, fobj2,n+1);
}

void list_mismatched_words(std::string file1, std::string file2)
{
    // The function by defaults consider any number of space to be the same, i.e.
    // "Hello World" is equivalent to "Hello      World" in terms of word matching.
    // Modifications to get_words() can be made to only separate words by a single space.

    std::ifstream fobj1(file1);
    std::ifstream fobj2(file2);

    //get basenames of the two files
    std::string file1_name = getname(file1);
    std::string file2_name = getname(file2);

    mismatched_words_helper(file1_name, file2_name, fobj1, fobj2, 1);

    fobj1.close();
    fobj2.close();
}

int main()
{
    // Q1
    std::string str1 = "Hello World";
    std::string str2 = "hEllO World";
    std::string str3 = "World";
    std::string str4 = "Hello World";
    bool result = word_diff(str1, str2); // False
    result = word_diff(str1, str3); // False
    result = word_diff(str1, str4); // True
    // Q2
    std::string file1 = "./txt_folder/file1.txt";
    std::string file2 = "./txt_folder/file2.txt";
    result = classical_file_diff(file1, file2); // False
    // Q3
    std::string mystr = "I love this assignment";
    std::size_t h1 = hash_it (mystr);
    // Q4
    result = enhanced_file_diff(file1, file2); // False
    // Q5
    list_mismatched_lines(file1, file2); // This should print to the screen the mismatched lines
    // Q6
    list_mismatched_words(file1, file2); // This should print to the screen the mismatched words
    return 0;
}
