#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

//MAXIMUM LIMITS
const int MAX_FLASHCARDS = 20;
const int MAX_USERS = 10;

class ScoreManager
{
public:
    int UserScore;
    ScoreManager():UserScore(0){}

    bool Review(int total = 3)const{
        return UserScore<total; // space repetition
    }
    void updateUserScore(bool correct)
    {
        if(correct)
        {
            if(UserScore < 5) //increase score up to a max of 5
            {UserScore++;}
        }
        else if(UserScore > 0) 
        {
            UserScore--;
        }
    }
};

class Flashcard 
{
    
    public:
    string question;
    string answer;
    string hint;
    int difficultyScore; // (1 = Easy, 2 = Moderate, 3 = Hard);
    int userScore;
    ScoreManager scoreManager;

    //public:
    Flashcard() //default constructor;
    {
        question = "";
        answer = "";
        hint = "";
        difficultyScore = 0;
        userScore =0;
    }

    Flashcard(string a_question, string a_ans, string a_hint, int a_diffscore, int a_userscore) //parameterised constructor
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

     void updateUserScore(bool correct)
    {
        scoreManager.updateUserScore(correct);
        userScore = scoreManager.UserScore;  // Sync userScore here
    }

    string getDifficultyLevel() const {
    switch (difficultyScore) {
        case 1: return "Easy";
        case 2: return "Moderate";
        case 3: return "Hard";
        default: return "Unrated";
    }
}


};

class FlashcardManager //add flashcards,manage cards 
{
    public:
    Flashcard flashcards[MAX_FLASHCARDS];
    int count;
    string filename;

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
        //bubble sort implementation (spaced repetition)
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-1;j++)
            {
                if(flashcards[j].getUserScore()>flashcards[j+1].getUserScore())
                {
                    swap(flashcards[j],flashcards[j+1]);
                }
            }
        }
        for (int i = 0; i < count; ++i) {
            cout << "\nFlashcard " << (i + 1) << ":\n";
            flashcards[i].display();

            cout << "Your Answer: ";
            getline(cin, userAnswer);

            if (flashcards[i].checkAnswer(userAnswer)) {
                cout << "Correct!\n";
                flashcards[i].updateUserScore(true);
            } else {
                cout << "Incorrect.\n";
                cout << "Hint: " << flashcards[i].getHint() << "\n";
                cout << "Try again: ";
                getline(cin, userAnswer);

                if (flashcards[i].checkAnswer(userAnswer)) {
                    cout << "Correct on second try.\n";
                    flashcards[i].updateUserScore(true);
                } else {
                    cout << "Still incorrect. Correct answer: " << flashcards[i].answer << "\n";
                    flashcards[i].updateUserScore(false);

                }

            }

            int difficulty;
            cout <<"Rate the difficulty(1=Easy, 2=Moderate, 3=Hard)";
            cin >>difficulty;
            while(difficulty<1||difficulty>3)
            {
                cout << "Invalid input. Please enter 1, 2, or 3: ";
                cin >> difficulty;
            }

            cin.ignore();
            flashcards[i].difficultyScore = difficulty;
        }
    }

    void loadData(const string &filename)
    {
        ifstream rf(filename, ios::in|ios::binary);
        if(!rf)
        {
            cout <<"Cannot open file for reading!"<<endl;
            return;
        }

        rf.read((char*)&count,sizeof(count));
        if (count > MAX_FLASHCARDS)
            count = MAX_FLASHCARDS;

        for (int i = 0; i < count; i++)
        {
            flashcards[i].question = readString(rf);
            flashcards[i].answer = readString(rf);
            flashcards[i].hint = readString(rf);
            rf.read((char *)&flashcards[i].difficultyScore, sizeof(int));
            rf.read((char *)&flashcards[i].userScore, sizeof(int));
        }
        rf.close();
        cout <<"\nLoading all user data from file done"<<endl;
    }

    void saveData(const string &filename)
    {
        ofstream wf(filename, ios::out|ios::binary);
        if(!wf)
        {
            cout <<"Cannot open file for writing!"<<endl;
            return;
        }

        wf.write((char *)&count,sizeof(count));
        for (int i = 0; i < count; i++)
        {
            writeString(wf, flashcards[i].question);
            writeString(wf, flashcards[i].answer);
            writeString(wf, flashcards[i].hint);
            wf.write((char *)&flashcards[i].difficultyScore, sizeof(int));
            wf.write((char *)&flashcards[i].userScore, sizeof(int));
        }
        wf.close();
        cout <<"Saving all data into file done"<<endl;
    }

    private:
     void writeString(ofstream &out, const string &str)
    {
        size_t len = str.size();
        out.write((char *)&len, sizeof(len));
        out.write(str.c_str(), len);
    }

    string readString(ifstream &in)
    {
        size_t len;
        in.read((char *)&len, sizeof(len));
        string str(len, ' ');
        in.read(&str[0], len);
        return str;
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
        cout <<"5. Display All Flash Cards"<<endl;
        cout <<"6. Show User Scores"<<endl;
        cout <<"7. Exit: "<<endl;
    }
};

class User//keeps track of user progress
{
    private:
    string name;
    FlashcardManager manager;
    //ScoreManager scoreManager;
    string filename;

    public:
    User(const string &file = "flashcards_data.dat"):filename(file){}
    
    void addNewUser()
    {
        cout <<"Enter Name: ";
        getline(cin,name);
        
    }


    void saveData()
    {
        manager.saveData(filename);
    }

    void loadData()
    {
        manager.loadData(filename);
    }

    void displayScore() {
        cout << "Showing flashcard scores for user: " << name << "\n\n";

        if (manager.count == 0)
        {
            cout << "No flashcards available.\n";
            return;
        }
        for (int i = 0; i < manager.count; ++i)
        {
            cout << "Flashcard " << (i + 1) << ": " << manager.flashcards[i].question << "\n";
            cout << "Your Score: " << manager.flashcards[i].getUserScore() << "\n\n";
            cout << "Difficulty Level: " << manager.flashcards[i].getDifficultyLevel() << "\n\n";
        }
    }
    void startSession() 
    {
        addNewUser();
        int option;
        do
        {
            Menu::display();
            cout <<"Your choice: ";
            cin>>option;
            cin.ignore();
            switch (option) {
            case 1:
                addFlashcard();
                break;
            case 2:
                manager.reviewFlashcards();
                break;
            case 3:
                manager.saveData(filename);
                break;
            case 4:
                manager.loadData(filename);
                break;
            case 5:
                manager.displayFlashcards();
                break;
            case 6:
                displayScore();
                break;
            case 7:
                cout<< "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
            }        
            
        } while (option!=7);
    }

    private:
    void addFlashcard()
    {
        string q, a, h;
        int diff;

        cout << "Enter question: ";
        getline(cin, q);
        cout << "Enter answer: ";
        getline(cin, a);
        cout << "Enter hint: ";
        getline(cin, h);



        manager.addFlashcard(q, a, h, 0, 0);
    }

};

int main()
{
    User user;
    user.startSession();
    return 0;
}

