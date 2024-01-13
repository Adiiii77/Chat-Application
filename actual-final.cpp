#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

class Message {
public:
    Message(const string& sender, const string& content) : sender(sender), content(content) {}
    virtual ~Message() {}

    virtual void display() const = 0;
    virtual string getType() const = 0;

    string getSender() const { return sender; }
    string getContent() const { return content; }

private:
    string sender;
    string content;
};

class TextMessage : public Message {
public:
    TextMessage(const string& sender, const string& content) : Message(sender, content) {}

    void display() const override {
        cout << "[" << getSender() << "]: " << getContent() << endl;
    }

    string getType() const override {
        return "Text";
    }
};

class FancyTextMessage : public TextMessage {
public:
    FancyTextMessage(const string& sender, const string& content, const string& style)
            : TextMessage(sender, content), style(style) {}

    void display() const override {
        cout << "[" << getSender() << "]: " << "[" << style << "] " << getContent() << endl;
    }

    string getType() const override {
        return "FancyText";
    }

private:
    string style;
};



class StyleManager {
public:
    static string selectStyle() {
        cout << "Select a style style:\n";
        cout << "1. Bold\n";
        cout << "2. Italic\n";
        cout << "3. Underline\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                return "\033[1m";
            case 2:
                return "\033[3m";
            case 3:
                return "\033[4m";
            default:
                return "\033[0m" ;
        }
    }
};

class User {
public:
    User(const string& username, const string& password) : username(username), password(password) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }

private:
    string username;
    string password;
};

class UserManager {
public:
    static bool authenticateUser(const string& username, const string& password) {
        for (const auto& user : users) {
            if (user.getUsername() == username && user.getPassword() == password) {
                return true;
            }
        }
        return false;
    }

    static vector<User> users;
};

vector<User> UserManager::users = {
        {"user1", "pass1"},
        {"user2", "pass2"},
        {"user3", "pass3"},
        {"user4", "pass4"},
        {"user5", "pass5"},
        {"user6", "pass6"},
        {"user7", "pass7"},
        {"user8", "pass8"},
        {"user9", "pass9"},
        {"user10", "pass10"}
};

class Chat {
public:
    map<string, vector<Message*>> messages;
    map<string, vector<string>> userConversations; // Map to store user and their conversation files

    void addMessage(Message* message, const string& recipient) {
        string fileName = getConversationFileName(message->getSender(), recipient);
        saveMessageToFile(fileName, message);

        // Update userConversations map
        userConversations[message->getSender()].push_back(fileName);
        userConversations[recipient].push_back(fileName);
    }

void viewPreviousConversations(const string& username) const {
    vector<string> potentialPartners;

    for (const User& user : UserManager::users) {
        if (user.getUsername() != username) {
            potentialPartners.push_back(user.getUsername());
        }
    }

    if (potentialPartners.empty()) {
        cout << "No potential conversation partners found.\n";
        return;
    }

    cout << "Select a user to view conversations:\n";
    int index = 1;
    for (const auto& partner : potentialPartners) {
        cout << index << " - " << partner << endl;
        ++index;
    }

    int choice;
    cout << "Enter your choice (or 0 to go back): ";
    cin >> choice;

    if (choice >= 1 && choice <= index - 1) {
        string selectedUser = potentialPartners[choice - 1];
        string fileName = getConversationFileName(username, selectedUser);
        displayConversation(fileName);
    } else {
        cout << "Invalid choice or going back.\n";
    }
}


static vector<string> getUserList(const string& username, const map<string, vector<Message*>>& messages) {
    vector<string> users;

    for (const auto& entry : messages) {
        size_t separatorIndex = entry.first.find('_');
        string user1 = entry.first.substr(0, separatorIndex);
        string user2 = entry.first.substr(separatorIndex + 1);
        if (user1 == username) {
            users.push_back(user2);
        } else if (user2 == username) {
            users.push_back(user1);
        }
    }

    sort(users.begin(), users.end());
    users.erase(unique(users.begin(), users.end()), users.end());

    return users;
}


private:
    static string getConversationFileName(const string& sender, const string& recipient) {
        // Ensure consistent order of usernames in the file name
        return (sender < recipient) ? (sender + "_" + recipient + ".txt") : (recipient + "_" + sender + ".txt");
    }


    static void saveMessageToFile(const string& fileName, Message* message) {
        ofstream file(fileName, ios::app);  // Open the file in append mode

        if (file.is_open()) {
            time_t now = time(nullptr);
            tm* localTime = localtime(&now);
            file << message->getSender() << "(time- " << put_time(localTime, "%Y-%m-%d %H:%M:%S") << "):\"" << message->getContent() << "\"\n";
            file.close();
        } else {
            cerr << "Unable to open file for writing." << endl;
        }
    }


    static void displayConversation(const string& fileName) {
        ifstream file(fileName);

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        } else {
            cerr << "Unable to open file for reading." << endl;
        }
    }
};


int main() {
    using namespace std;

    srand(static_cast<unsigned>(time(nullptr)));

    string username, password;

    int c;
    cout<<"HELLO! WELCOME TO CHIT-CHAT!"<<endl;

    cout<<"\nDo you wish to create an account?\nNo - 0\nYes - 1"<<endl;
    cin>>c;

    if (c==1){
        cout<<"Enter preferred username: ";
        cin>>username;
        cout<<"Enter preferred password: ";
        cin>>password;
        UserManager::users.push_back({username,password});
        cout<<"\nINFO LOGGED :)";
    }
    else if(c!=0){
        cout<<"INVALID CHOICE :(";
    }
    cout<<"\nLOGIN TO CHIT-CHAT :)"<<endl;
    while (true) {
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            if (UserManager::authenticateUser(username, password)) {
                cout << "Authentication successful!\nWelcome, " << username << "!" << endl;
                break;
            } else {
                cout << "Authentication failed. Please try again.\n" << endl;
            }
        }



    Chat chat;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Send Text Message\n";
        cout << "2. View Previous Conversations\n";
        cout << "3. Exit\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string content;
                cout << "Enter message content: ";
                cin.ignore();  // Clear the input buffer
                getline(cin, content);

                string recipient;
                cout << "Enter the username of the recipient: ";
                cin >> recipient;
                bool found =false;
                for (const User& u_single : UserManager::users) {
                    if (u_single.getUsername() == recipient) {
                        found = true;
                        break;
                    }
                }
                if (found){
                    cout << "Do you want to send a fancy text message? (y/n): ";
                    char fancyChoice;
                    cin >> fancyChoice;

                    if (fancyChoice == 'y' || fancyChoice == 'Y') {
                        string style = StyleManager::selectStyle();
                        FancyTextMessage fancyMessage(username, content, style);
                        chat.addMessage(&fancyMessage, recipient);
                    } else {
                        TextMessage textMessage(username, content);
                        chat.addMessage(&textMessage, recipient);
                    }

                }
                else{
                    cout<<recipient<<" is not in our database, Sorry :(";
                }

                break;
            }
            case 2:
                chat.viewPreviousConversations(username);
                break;
            case 3:
                cout << "Exiting the Chat Application. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}