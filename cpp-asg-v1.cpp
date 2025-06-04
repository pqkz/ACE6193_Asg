#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

//MAXIMUM LIMITS
const int MAX_FLASHCARDS = 20;
const int MAX_USERS = 10;

class Flashcard //load, save flashcard
{
    
    public:
    string question;
    string answer;
    string hint;
    int difficultyScore; // (1 = Easy, 2 = Moderate, 3 = Hard);
    int userScore;

    //public:
    Flashcard() //default constructor;
    {
        question = "";
        answer = "";
        hint = "";
        difficultyScore = 0;
        userScore =0;
    }

    Flashcard(string a_question, string a_ans, string a_hint, int a_diffscore, int a_userscore)
    {
        question = a_question;
        answer = a_ans;
        hint = a_hint;
        difficultyScore = a_diffscore;
        userScore = a_userscore;
    }

    void display() const{ //when reviewing flashcard
        cout <<"Q: "<<question<<"\n";
        cout <<"(Difficulty score: "<<difficultyScore<<")\n";
    }

    bool checkAnswer(const string &userAnswer )
    {
        return userAnswer == answer;
    }

    int getUserScore() const{
        return userScore;
    }

    string getHint() const{
        return hint;
    }

    //void save(ofstream& out)const { //savefile

    //}

    //void load(ifstream& in) //load file
    //{

    //}
    



};

class FlashcardManager //add flashcards,manage cards 
{
    public:
    Flashcard flashcards[MAX_FLASHCARDS];
    int count;

    FlashcardManager():count(0){}

    void addFlashcard(string question, string answer, string hint,int difficultyScore, int userScore)
    {
        if(count <MAX_FLASHCARDS)
        {
            flashcards[count] = Flashcard(question,answer,hint,difficultyScore,userScore);
            count ++;
            cout <<"Flashcard added successfully!"<<endl;
        }

        else
        {
            cout <<"Maximum number of flashcards reached."<<endl;
        }
    }

    void displayFlashcards()
    {
        if(count == 0)
        {
            cout <<"No flashcards to display."<<endl;
            return;
        }

        else
        {
            cout <<endl;
            cout <<"Total flashcards = "<<count<<endl;

            for(int i=0;i<count;i++)
            {
                cout <<"Question: "<<flashcards[i].question<<endl;
                cout <<"Answer: "<<flashcards[i].answer<<endl;
                cout <<endl;
            }
        }
    }

    void reviewFlashcards() {
        if (count == 0) {
            cout << "No flashcards available to review.\n";
            return;
        }

        string userAnswer;
        for (int i = 0; i < count; ++i) {
            cout << "\nFlashcard " << (i + 1) << ":\n";
            flashcards[i].display();

            cout << "Your Answer: ";
            getline(cin, userAnswer);

            if (flashcards[i].checkAnswer(userAnswer)) {
                cout << "Correct!\n";
            } else {
                cout << "Incorrect.\n";
                cout << "Hint: " << flashcards[i].getHint() << "\n";
                cout << "Try again: ";
                getline(cin, userAnswer);

                if (flashcards[i].checkAnswer(userAnswer)) {
                    cout << "Correct on second try.\n";
                } else {
                    cout << "Still incorrect. Correct answer: " << flashcards[i].answer << "\n";
                }
            }
        }
    }
};

class Menu
{
public: 

    static void display()
    {
        cout <<"\n == Digital Flash Card Menu == \n";
        cout<<"Enter Option: "<<endl;
        cout <<"1. Add Flash Card: "<<endl;
        cout <<"2. Review Flash Card: "<<endl;
        cout <<"3. Save Cards: "<<endl;
        cout <<"4. Load Cards: "<<endl;
        cout <<"5. Exit: "<<endl;
    }
};

class User//keeps track of user progress
{
    private:
    string name;
    FlashcardManager manager;
    string filename;

    public:
    User(const string &file = "flashcards_data.dat"):filename(file){}
    
    void addNewFlashcard()
    {
    cin.ignore(); // clear newline before taking input
    string question, answer, hint;
    int difficultyScore;

    cout << "Enter Question: ";
    getline(cin, question);
    cout << "Enter Answer: ";
    getline(cin, answer);
    cout << "Enter Hint: ";
    getline(cin, hint);
    cout << "Enter Difficulty (1=Easy, 2=Moderate, 3=Hard): ";
    cin >> difficultyScore;

    manager.addFlashcard(question, answer, hint, difficultyScore, 0);
        
    }

    void loadData()
    {
        ifstream rf(filename, ios::in|ios::binary);
        if(!rf)
        {
            cout <<"Cannot open file for reading!"<<endl;
            return;
        }

        rf.read((char*)&manager,sizeof(FlashcardManager));
        rf.close();
        cout <<"\nLoading all user data from file done"<<endl;
    }

    void saveData()
    {
        ofstream wf(filename, ios::out|ios::binary);
        if(!wf)
        {
            cout <<"Cannot open file for writing!"<<endl;
            return;
        }

        wf.write((char *)&manager,sizeof(FlashcardManager));
        wf.close();
        cout <<"Saving all data into file done"<<endl;
    }

    void startSession()
    {
        int choice = 0;
        while (choice != 5) {
            cout << "\n===== Digital Flashcard Menu =====" << endl;
            cout << "1. Display all flashcards" << endl;
            cout << "2. Add new flashcard" << endl;
            cout << "3. Review flashcards" << endl;
            cout << "4. Save flashcards to file" << endl;
            cout << "5. Load flashcards from file" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;
            system("CLS");

            switch (choice) {
            case 1:
                displayData();
                break;
            case 2:
                addNewFlashcard();
                break;
            case 3:
                manager.reviewFlashcards();
                break;
            case 4:
                saveData();
                break;
            case 5:
                loadData();
                break;
            case 6:
                cout << "Exiting program." << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }        
        }
    }
};
int main()
{

}
