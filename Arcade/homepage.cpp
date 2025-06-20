// #include "homepage.hpp"
// std::string getText(std:: string s){
//     std::string text;
//     std:: cout << "Enter " << s << ":" << std::endl;
//     std:: cin >> text;
//     return text;
// }
// bool homepage(){
//     MySQLConnector db("localhost", "root", "l9hfG&7a", "mydatabase");
//     if (!db.connect()) {
//         return 1;
//     }
//     int exit = 0;
//     while(true){
//         std::cout<<"Choose what you wish to do(1/2/3):" << std::endl;
//         std::cout << "1. Login (Existing User)" << std::endl;
//         std::cout << "2. Signup (New User)" << std::endl;
//         std::cout << "3. Exit" << std::endl;
//         int choice = 0;
//         std::cin >> choice;
//         if(choice == 1){
//             while(true){
//                 if(login(db)){
//                     std::cout << "Logged in successfully" << std::endl;
//                     exit = 1;
//                     break;
//                 }
//                 else{
//                     std::cout << "Invalid Username/Password." << std::endl;
//                     std::cout << "Choose what you wish to do(1/2):" << std:: endl;
//                     std::cout << "1. Try Again" << std:: endl;
//                     std::cout << "2. Back to Home" << std::endl;
//                     int retry = 0;
//                     std::cin >> retry;
//                     if(retry == 1){
//                         continue;
//                     }
//                     else if(retry == 2){
//                         break;
//                     }
//                 }
//             }
//         }
//         else if(choice == 2){
//             while(true){
//                 if(signup(db)){
//                     std::cout << "Signed up successfully" << std::endl;
//                     exit = 1;
//                     break;
//                 }
//                 else{
//                     std::cout << "Username already exists" << std::endl;
//                     std::cout << "Choose what you wish to do(1/2):" << std:: endl;
//                     std::cout << "1. Try Again" << std:: endl;
//                     std::cout << "2. Back to Home" << std::endl;
//                     int retry = 0;
//                     std::cin >> retry;
//                     if(retry == 1){
//                         continue;
//                     }
//                     else if(retry == 2){
//                         break;
//                     }
//                 }
//             }
//         }
//         else if(choice == 3){
//             std::cout << "Exiting" << std::endl;
//             exit = 1;
//             break;
//         }
//         else{
//             std:: cout << "Invalid Input. Please try again." << std::endl;
//         }
//         if(exit == 1){
//             break;
//         }
//     }
//     return true;
// }
// bool login(MySQLConnector& db){
//     std:: string username = getText("Username");
//     std:: string password = getText("Password");
// 	char query[256];
// 	sprintf(query, "SELECT * FROM user_details WHERE username = '%s';", username.c_str());
//     MYSQL_RES* res = db.executeQuery(query);
//     if (!res){
//         return 1;
//     }

//     MYSQL_ROW row = mysql_fetch_row(res);
//     mysql_free_result(res);
//     if(row!= nullptr && row[1] == password){
//             return true;
//     }
//     else{
//             return false;
//     }
// }

// bool signup(MySQLConnector& db){
//     std:: string username = getText("Username");
//     std:: string password = getText("Password");
//     char query[256];
//     sprintf(query, "SELECT * FROM user_details WHERE username = '%s';", username.c_str());
//     MYSQL_RES* res = db.executeQuery(query);

//     MYSQL_ROW row = mysql_fetch_row(res);
//     mysql_free_result(res);
//     if(row!= nullptr){
//             return false;
//     }
//     else{
//         sprintf(query, "INSERT INTO user_details VALUES('%s','%s');", username.c_str());
//         res = db.executeQuery(query);

//         return true;
//     }   
// }

#include "homepage.hpp"

// Homepage::Homepage(const char* server, const char* user, const char* password, const char* database)
//     : db(server, user, password, database) {
//     db.connect();
// }
Homepage::Homepage(MySQLConnector& database):db(database){
    db.connect();
}



std::string Homepage::getText(std:: string s){
    std::string text;
    std:: cout << "Enter " << s << ":" << std::endl;
    std:: cin >> text;
    return text;
}

std::string Homepage::show(){
    int exit = 0;
    while(true){
        std::cout<<"Choose what you wish to do(1/2/3):" << std::endl;
        std::cout << "1. Login (Existing User)" << std::endl;
        std::cout << "2. Signup (New User)" << std::endl;
        std::cout << "3. Exit" << std::endl;
        int choice = 0;
        std::cin >> choice;
        if(choice == 1){
            while(true){
                if(login()){
                    std::cout << "Logged in successfully" << std::endl;
                    exit = 1;
                    break;
                }
                else{
                    std::cout << "Invalid Username/Password." << std::endl;
                    std::cout << "Choose what you wish to do(1/2):" << std:: endl;
                    std::cout << "1. Try Again" << std:: endl;
                    std::cout << "2. Back to Home" << std::endl;
                    int retry = 0;
                    std::cin >> retry;
                    if(retry == 1){
                        continue;
                    }
                    else if(retry == 2){
                        break;
                    }
                }
            }
        }
        else if(choice == 2){
            while(true){
                if(signup()){
                    std::cout << "Signed up successfully" << std::endl;
                    exit = 1;
                    break;
                }
                else{
                    std::cout << "Username already exists" << std::endl;
                    std::cout << "Choose what you wish to do(1/2):" << std:: endl;
                    std::cout << "1. Try Again" << std:: endl;
                    std::cout << "2. Back to Home" << std::endl;
                    int retry = 0;
                    std::cin >> retry;
                    if(retry == 1){
                        continue;
                    }
                    else if(retry == 2){
                        break;
                    }
                }
            }
        }
        else if(choice == 3){
            std::cout << "Exiting" << std::endl;
            exit = 1;
            break;
        }
        else{
            std:: cout << "Invalid Input. Please try again." << std::endl;
        }
        if(exit == 1){
            break;
        }
    }
    return currentuser;
}

bool Homepage::login(){
    std:: string username = getText("Username");
    std:: string password = getText("Password");
	char query[256];
	sprintf(query, "SELECT * FROM user_details WHERE username = '%s';", username.c_str());
    MYSQL_RES* res = db.executeQuery(query);
    if (!res){
        return false;
    }
    bool authenticated = false;
    MYSQL_ROW row;
    while((row=mysql_fetch_row(res)) != nullptr){
        if(row[1] == password){
            currentuser = username;
            authenticated = true;
            break;
        }
    }
    mysql_free_result(res);
    return authenticated;
}

bool Homepage::signup(){
    std:: string username = getText("Username");
    std:: string password = getText("Password");
    char query[256];
    sprintf(query, "SELECT * FROM user_details WHERE username = '%s';", username.c_str());
    MYSQL_RES* res = db.executeQuery(query);

    MYSQL_ROW row = mysql_fetch_row(res);
    mysql_free_result(res);
    if(row!= nullptr){
            return false;
    }
    else{
        sprintf(query, "INSERT INTO user_details VALUES('%s','%s');", username.c_str(), password.c_str());
        db.executeQuery(query);
        currentuser = username;
        createtables(username);
        return true;
    }

}

void Homepage::createtables(std::string username){
    char query[256];
    //TicTacToe
    sprintf(query, "INSERT INTO tictactoe VALUES('%s','player',0,0,0,0);", username.c_str());
    db.executeQuery(query);
    sprintf(query, "INSERT INTO tictactoe VALUES('%s','computer',0,0,0,0);",username.c_str());
    db.executeQuery(query);
    //Snake
    sprintf(query, "INSERT INTO snake VALUES('%s', 0, 0);", username.c_str());
    db.executeQuery(query);
    //Othello
    sprintf(query, "INSERT INTO othello VALUES('%s','player',0,0,0,0);",username.c_str());
    db.executeQuery(query);
    sprintf(query, "INSERT INTO othello VALUES('%s','computer',0,0,0,0);",username.c_str());
    db.executeQuery(query);
}
